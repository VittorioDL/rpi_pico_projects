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
#include "drivers/xbee/xbee.hpp"


#define XBEE_UART_NODE DT_CHOSEN(cansat_xbee)

//BME-------------------------------------------------------------
// static const struct device *get_bme280_device(void)
// {
// 	const struct device* dev = DEVICE_DT_GET_ANY(bosch_bme280);

// 	if (dev == NULL) {
// 		/* No such node, or the node does not have status "okay". */
// 		printk("\nError: no device found.\n");
// 		return NULL;
// 	}

// 	if (!device_is_ready(dev)) {
// 		printk("\nError: Device \"%s\" is not ready; "
// 		       "check the driver initialization logs for errors.\n",
// 		       dev->name);
// 		return NULL;
// 	}

// 	printk("Found device \"%s\", getting sensor data\n", dev->name);
// 	return dev;
// }

int main(void)
{
	const struct device *xbee_node_dev = DEVICE_DT_GET(XBEE_UART_NODE);
	drivers::xbee::Xbee xbee(xbee_node_dev);

    drivers::xbee::detail::request::transmit_request tx_req;
    tx_req.set_dst_address(0x0013A20041CFED46);
    tx_req.set_frame_id(0);

	//LED
	static const struct gpio_dt_spec led_dev = GPIO_DT_SPEC_GET(DT_NODELABEL(led), gpios);
    gpio_pin_configure_dt(&led_dev, GPIO_OUTPUT_ACTIVE);
	
    while(1){
		//LED-------------------------------------------------------------
        // printk("nel main\n");
        gpio_pin_toggle_dt(&led_dev);

        //BME--------------------------------------------------------------
        // const struct device *bme_dev = get_bme280_device();
        // if (bme_dev == NULL) {
        //     return 0;
        // }
        // struct sensor_value temp, press, humidity;

		// sensor_sample_fetch(bme_dev);
		// sensor_channel_get(bme_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		// sensor_channel_get(bme_dev, SENSOR_CHAN_PRESS, &press);
		// sensor_channel_get(bme_dev, SENSOR_CHAN_HUMIDITY, &humidity);

		// printk("temp: %d.%06d; press: %d.%06d; humidity: %d.%06d\n",
		//       temp.val1, temp.val2, press.val1, press.val2,
		//       humidity.val1, humidity.val2); 

        //XBEE-------------------------------------------------------------
        // tx_req.set_payload("temp: "+std::to_string(temp.val1)+"."+std::to_string(temp.val2)+
        //                    " ,press: "+std::to_string(press.val1)+"."+std::to_string(press.val2)+
        //                    " ,humidity: "+std::to_string(humidity.val1)+"."+std::to_string(humidity.val2));
		tx_req.set_payload("1066,13:14:02.22,1000,F,LAUNCH_WAIT,800.1,100.1,N,N,37.1,5.0,121.1,13:14:02.22,700.1,-37.4003,-37.4003");
        tx_req.calculate_length();
        tx_req.calculate_checksum();
        xbee.send_request(tx_req);
        
		k_sleep(K_MSEC(1000));
    }
	return 0;
}


