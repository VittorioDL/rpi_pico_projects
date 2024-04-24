/*
 * Copyright (c) 2023 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include "drivers/xbee/xbee.hpp"


#define XBEE_UART_NODE DT_CHOSEN(cansat_xbee)

//ADC----------------------------------------------------------------------------
#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \ 
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

int main(void)
{
	const struct device *xbee_node_dev = DEVICE_DT_GET(XBEE_UART_NODE);
	drivers::xbee::Xbee xbee(xbee_node_dev);

    drivers::xbee::detail::request::transmit_request tx_req;
    tx_req.set_dst_address(0x0013A20041CFED46);
    tx_req.set_frame_id(0);
    std::string telemetry = "";

	//LED
	static const struct gpio_dt_spec led_dev = GPIO_DT_SPEC_GET(DT_NODELABEL(led), gpios);
    gpio_pin_configure_dt(&led_dev, GPIO_OUTPUT_ACTIVE);

    //ADC--------------------------------------------------------------------------------------
	int err;
	uint32_t count = 0;
	uint16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
	};
    for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!adc_is_ready_dt(&adc_channels[i])) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
			return 0;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("Could not setup channel #%d (%d)\n", i, err);
			return 0;
		}
	}
	
    while(1){
		//LED-------------------------------------------------------------
        gpio_pin_toggle_dt(&led_dev);

        // ADC--------------------------------------------------------------
		printk("ADC reading[%u]:\n", count++);
		int32_t val_mv;
		for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {

			printk("- %s, channel %d: ",
			       adc_channels[i].dev->name,
			       adc_channels[i].channel_id);

			(void)adc_sequence_init_dt(&adc_channels[i], &sequence);

			err = adc_read_dt(&adc_channels[i], &sequence);
			if (err < 0) {
				printk("Could not read (%d)\n", err);
				continue;
			}

			/*
			 * If using differential mode, the 16 bit value
			 * in the ADC sample buffer should be a signed 2's
			 * complement value.
			 */
			if (adc_channels[i].channel_cfg.differential) {
				val_mv = (int32_t)((int16_t)buf);
			} else {
				val_mv = (int32_t)buf;
			}
			printk("%"PRId32, val_mv);
			err = adc_raw_to_millivolts_dt(&adc_channels[i],
						       &val_mv);
			/* conversion to mV may not be supported, skip if not */
			if (err < 0) {
				printk(" (value in mV not available)\n");
			} else {
				printk(" = %"PRId32" mV\n", val_mv);
			}
		}

        //XBEE-------------------------------------------------------------
        // tx_req.set_payload("temp: "+std::to_string(temp.val1)+"."+std::to_string(temp.val2)+
        //                    " ,press: "+std::to_string(press.val1)+"."+std::to_string(press.val2)+
        //                    " ,humidity: "+std::to_string(humidity.val1)+"."+std::to_string(humidity.val2));
        telemetry = "";
        telemetry.append("1066,13:14:02,10,F,LAUNCH_WAIT,8.1,10.1,N,N,7.1,");
        telemetry.append(std::to_string(val_mv));
        telemetry.append(",1.1,13:14:02,7.1,7.3,7.3,10,3.1,3.1,3.1,SP101325\n");
        printk("%s\n",telemetry.c_str());
		tx_req.set_payload(telemetry);
        
        // tx_req.set_payload("1066,13:14:02,10,F,L,8.1,10.1,N,N,37.1,5.0,1.1,13:14:02,7.1,7.4003,7.4003,10,3.01,3.01,3.01,SP101325\n");
        tx_req.calculate_length();
        tx_req.calculate_checksum();
        xbee.send_request(tx_req);
        
		k_sleep(K_MSEC(1000));
    }
	return 0;
}


