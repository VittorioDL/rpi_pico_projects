/*
 * Copyright (c) 2023 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

static const struct device *get_bme280_device(void)
{
	const struct device* dev = DEVICE_DT_GET_ANY(bosch_bme280);

	if (dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		printk("\nError: no device found.\n");
		return NULL;
	}

	if (!device_is_ready(dev)) {
		printk("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return NULL;
	}

	printk("Found device \"%s\", getting sensor data\n", dev->name);
	return dev;
}

//clock-frequency = <I2C_BITRATE_STANDARD>;
int main(void)
{
    //LED
    static const struct gpio_dt_spec led_dev = GPIO_DT_SPEC_GET(DT_NODELABEL(led), gpios);
    gpio_pin_configure_dt(&led_dev, GPIO_OUTPUT_ACTIVE);


    while(1){
        //LED
        printk("nel main\n");
        k_msleep(500);
        gpio_pin_toggle_dt(&led_dev);
        
        //BME
        const struct device *bme_dev = get_bme280_device();
        if (bme_dev == NULL) {
            return 0;
        }
        //BME
        struct sensor_value temp, press, humidity;

		sensor_sample_fetch(bme_dev);
		sensor_channel_get(bme_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		sensor_channel_get(bme_dev, SENSOR_CHAN_PRESS, &press);
		sensor_channel_get(bme_dev, SENSOR_CHAN_HUMIDITY, &humidity);

		printk("temp: %d.%06d; press: %d.%06d; humidity: %d.%06d\n",
		      temp.val1, temp.val2, press.val1, press.val2,
		      humidity.val1, humidity.val2); 
    }
	return 0;
}
