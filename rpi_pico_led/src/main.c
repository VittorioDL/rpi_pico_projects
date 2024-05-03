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


int main(void)
{
    //LED
    static const struct gpio_dt_spec led_dev = GPIO_DT_SPEC_GET(DT_NODELABEL(led), gpios);
    gpio_pin_configure_dt(&led_dev, GPIO_OUTPUT_ACTIVE);

	static const struct gpio_dt_spec led_dev1 = GPIO_DT_SPEC_GET(DT_NODELABEL(led1), gpios);
    gpio_pin_configure_dt(&led_dev1, GPIO_OUTPUT_ACTIVE);


    while(1){
        //LED
        printk("nel main\n");
        k_msleep(500);
        gpio_pin_toggle_dt(&led_dev);
		gpio_pin_toggle_dt(&led_dev1);
        
        
    }
	return 0;
}
