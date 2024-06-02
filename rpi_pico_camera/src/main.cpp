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
    printk("Ciao mamma sono in tv\n");
	k_msleep(15000);
	printk("Fine trasmissione\n");
	camera.stopRecording();
	
    // while(1){
	// 	//LED-------------------------------------------------------------
    //     printk("nel main\n");
    //     k_msleep(500);
    // }
	return 0;
}


