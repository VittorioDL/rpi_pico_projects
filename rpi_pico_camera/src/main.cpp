/*
 * Copyright (c) 2023 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#include "subsys/camera/camera.hpp"

using namespace payload;

#define CAMERA_NODE	DT_NODELABEL(camera)

int main(void)
{
	//CAMERA
	static const gpio_dt_spec camera_dev = GPIO_DT_SPEC_GET(CAMERA_NODE, gpios);

	subsys::Camera camera(camera_dev);
	camera.startRecording();
    printk("Ciao mamma sono in tv");
	k_msleep(3000);
	camera.stopRecording();

	// //LED
	// static const struct gpio_dt_spec led_dev = GPIO_DT_SPEC_GET(DT_NODELABEL(led), gpios);
    // gpio_pin_configure_dt(&led_dev, GPIO_OUTPUT_ACTIVE);
	
    while(1){
		//LED-------------------------------------------------------------
        printk("nel main\n");
        k_msleep(500);
        // gpio_pin_toggle_dt(&led_dev);
        
		// k_sleep(K_MSEC(100));
    }
	return 0;
}


