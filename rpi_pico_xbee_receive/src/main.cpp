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


int main(void)
{
	const struct device *xbee_node_dev = DEVICE_DT_GET(XBEE_UART_NODE);
	drivers::xbee::Xbee xbee(xbee_node_dev);

    //TX----------------------------------------------------------
    drivers::xbee::detail::request::transmit_request tx_req;
    tx_req.set_dst_address(0x0013A20041CFED46);
    tx_req.set_frame_id(0);

    //RX------------------------------------------------------------
    xbee.set_rx_callback([this](std::unique_ptr<drivers::xbee::i_response> response)
        { handleCommand(response->get_string()); });

	//LED
	static const struct gpio_dt_spec led_dev = GPIO_DT_SPEC_GET(DT_NODELABEL(led), gpios);
    gpio_pin_configure_dt(&led_dev, GPIO_OUTPUT_ACTIVE);
	
    while(1){
		//LED-------------------------------------------------------------
        gpio_pin_toggle_dt(&led_dev);

        //XBEE-------------------------------------------------------------
        // tx_req.set_payload("temp: "+std::to_string(temp.val1)+"."+std::to_string(temp.val2)+
        //                    " ,press: "+std::to_string(press.val1)+"."+std::to_string(press.val2)+
        //                    " ,humidity: "+std::to_string(humidity.val1)+"."+std::to_string(humidity.val2));
		
        //TX---------------------------------------------------
        tx_req.set_payload("1066,13:14:02.22,1000,F,LAUNCH_WAIT,800.1,100.1,N,N,37.1,%s,121.1,13:14:02.22,700.1,-37.4003,-37.4003\n");
        tx_req.calculate_length();
        tx_req.calculate_checksum();
        xbee.send_request(tx_req);
        
		k_sleep(K_MSEC(1000));
    }
	return 0;
}


