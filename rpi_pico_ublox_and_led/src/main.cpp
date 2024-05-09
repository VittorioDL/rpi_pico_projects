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
#include <drivers/gnss/ublox_neo_m8.h>
#include <ctime>
#include <string>

#define NEO_SERIAL DT_NODELABEL(neom8)

    struct utc_time {
        int32_t hour;
        int32_t min;
        int32_t sec;

        [[nodiscard]] std::string to_string() const {
            std::string time {};
            if ((0 <= hour) && (hour <= 9))
            time.append("0");
            time.append(std::to_string(hour));

            time.append(":");

            if ((0 <= min) && (min <= 9))
            time.append("0");
            time.append(std::to_string(min));

            time.append(":");

            if ((0 <= sec) && (sec <= 9))
            time.append("0");
            time.append(std::to_string(sec));

            return time;
        }

        void tick() {
            tm t {};
            t.tm_year   = 70;
            t.tm_mon    = 1;
            t.tm_mday   = 1;
            t.tm_hour   = hour;
            t.tm_min    = min;
            t.tm_sec    = sec + 1;
            auto new_t  = mktime(&t);
            auto new_tm = *localtime(&new_t);
            hour        = new_tm.tm_hour;
            min         = new_tm.tm_min;
            sec         = new_tm.tm_sec;
        }
    };

//BME
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

	//GPS
	const struct device *gps_dev = DEVICE_DT_GET(NEO_SERIAL);

	if(gps_dev == NULL) {
        printk("\nError: no GPS device found.\n");
        return -1;
    } else {
        if (!device_is_ready(gps_dev)) {
            printk("%s Device not ready", gps_dev->name);
            return -1;
        }
    }
	struct neom8_api* gps_api = (struct neom8_api *) gps_dev->api;

    while(1){
        //LED
        // printk("nel main\n");
        k_msleep(1000);
        gpio_pin_toggle_dt(&led_dev);
        
        //UBLOX
        
		gps_api->send_ubx(gps_dev, 0x01, 0x07, {}, 0); // UBX-NAV-PVT
		int rc = gps_api->fetch_data(gps_dev);
		if (rc) {
			printk("Error %d while reading data", rc);
		}

        struct gpsTime gps_time = gps_api->get_time(gps_dev);
        utc_time time
            = {.hour = gps_time.hour, .min = gps_time.min, .sec = static_cast<int32_t>(gps_time.sec)};
		
		printk("latitude: %d.%04d; longitude: %d.%04d; altitude: %d.%01d; satellites: %d; time: %d:%d:%d\n",
		      gps_api->get_latitude(gps_dev), gps_api->get_longitude(gps_dev), gps_api->get_altitude(gps_dev), gps_api->get_satellites(gps_dev),
			  time.hour, time.min, time.sec); 

        // BME
        const struct device *bme_dev = get_bme280_device();
        if (bme_dev == NULL) {
            return 0;
        }
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
