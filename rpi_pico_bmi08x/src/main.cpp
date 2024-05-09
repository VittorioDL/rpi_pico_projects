/*
 * Copyright (c) 2023 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

// BMI08X------------------------------------------------------------------------
static const struct device *get_bmi088_gyro_device(void)
{
	// const struct device* dev = DEVICE_DT_GET(DT_ALIAS(gyro));
	const struct device* dev = DEVICE_DT_GET_ONE(bosch_bmi08x_gyro);

	if (dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		printk("\nError: no gyro device found.\n");
		return NULL;
	}

	if (!device_is_ready(dev)) {
		printk("\nError: Gyro device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return NULL;
	}

	printk("Found gyro device \"%s\", getting sensor data\n", dev->name);
	return dev;
}

static const struct device *get_bmi088_accel_device(void)
{
	const struct device* dev = DEVICE_DT_GET(DT_ALIAS(accel));

	if (dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		printk("\nError: no accel device found.\n");
		return NULL;
	}

	if (!device_is_ready(dev)) {
		printk("\nError: Accel device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return NULL;
	}
	printk("Found accel device \"%s\", getting sensor data\n", dev->name);
	return dev;
}

int main(void)
{   
	printk("start\n");
	//LED
	static const struct gpio_dt_spec led_dev = GPIO_DT_SPEC_GET(DT_NODELABEL(led), gpios);
    gpio_pin_configure_dt(&led_dev, GPIO_OUTPUT_ACTIVE);

    const struct device *bmi_gyro_dev = get_bmi088_gyro_device();
	if (bmi_gyro_dev == NULL) {
		// printk("bmi_gyro null\n");
		return 0;
	}
    const struct device *bmi_accel_dev = get_bmi088_accel_device();
	if (bmi_accel_dev == NULL) {
		return 0;
	}

    while(1){
        //LED
        printk("nel main\n");
        k_msleep(1000);
		gpio_pin_toggle_dt(&led_dev);

        struct sensor_value accxyz, gyroxyz;

		sensor_sample_fetch(bmi_gyro_dev);
		sensor_channel_get(bmi_gyro_dev, SENSOR_CHAN_GYRO_XYZ, &gyroxyz);

        sensor_sample_fetch(bmi_accel_dev);
		sensor_channel_get(bmi_accel_dev, SENSOR_CHAN_ACCEL_XYZ, &accxyz);
		printk("gyro: %d.%04d", gyroxyz); 
		printk("gyro: %d.%04d; accel: %d.%06d;", gyroxyz, accxyz); 
    }
	return 0;
}
