/*
 * Copyright (c) 2023 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include "drivers/adc/adc.hpp"

int main(void)
{
	printk("start\n");
	drivers::adc::Adc adc;
    while(1){
		// printk("main\n");
		k_msleep(1000);
		double voltage = adc.getVoltage();
		double speed = adc.getPitotSpeed();
		printk("Voltage: %f\n", voltage);
		printk("Pitot: %lf\n", speed);
    }
	return 0;
}


