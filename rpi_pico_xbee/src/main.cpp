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

    drivers::xbee::detail::request::transmit_request tx_req;

	                       //Indirizzo MAC della GCS        
    tx_req.set_dst_address(0x0013A20041CFED46);
    tx_req.set_frame_id(0);
	
    while(1){
		printk("los pollos hermanos\n");

        //XBEE-------------------------------------------------------------

		//Occhio alla dimensione del payload!
        //tx_req.set_payload("los pollos hermanos\n");
        tx_req.set_payload("2007,13:14:02,34,F,S1,729.4,50.0,P,C,37.6,3.3,101.3,13:14:02,729.4,-40.2370,73.2040,3,59.99,59.91,4.1,CXON\n");
        tx_req.calculate_length();
        tx_req.calculate_checksum();
        xbee.send_request(tx_req);
       
		k_sleep(K_MSEC(1000));
    }
	return 0;
}


