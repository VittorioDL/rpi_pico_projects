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
#include <drivers/bmi088/bmi088_acc.h>
#include <drivers/bmi088/bmi088_gyro.h>
#include <ctime>
#include <stdio.h>
#include <string>

#define GYRO_NODE DT_NODELABEL(bmi088_gyr)
constexpr const device *gyro_sensor_dev = DEVICE_DT_GET(GYRO_NODE);

#define ACCELEROMETER_NODE DT_NODELABEL(bmi088_acc)
constexpr const device *accel_sensor_dev = DEVICE_DT_GET(ACCELEROMETER_NODE);

void sensor_readout() {
    struct sensor_value gyro[3];
    sensor_sample_fetch(gyro_sensor_dev);
    sensor_channel_get(gyro_sensor_dev, SENSOR_CHAN_GYRO_XYZ, gyro);
    printk("Gyro x: %f °/s y: %f °/s z: %f °/s",
            gyro[0],
            gyro[1],
            gyro[2]);

    struct sensor_value accel[3];
    sensor_sample_fetch(accel_sensor_dev);
    sensor_channel_get(accel_sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    printk("Accelerometer x: %f m/s2 y: %f m/s2 z: %f m/s2",
            accel[0],
            accel[1],
            accel[2]);
}
    
int main(void)
{   
    while(1){
        //LED
        printk("nel main\n");
        k_msleep(1000);

        sensor_readout();
    
    }
	return 0;
}
