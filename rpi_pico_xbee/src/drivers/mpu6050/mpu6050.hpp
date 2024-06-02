#ifndef PAYLOAD_MPU6050_HPP
#define PAYLOAD_MPU6050_HPP

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>


namespace drivers::mpu6050 {
    class MPU6050Sensor {
        public:
            explicit MPU6050Sensor(const device *_device);
            ~MPU6050Sensor();

            int getAccel();
            int getGyro();

            struct sensor_value accel[3];
            struct sensor_value gyro[3];
        private:
            const device *dev;

            bool ready = false;
    };
}

#endif