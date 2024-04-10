#include "camera.hpp"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(camera, LOG_LEVEL_DBG);

namespace payload::subsys {
    Camera::Camera(const gpio_dt_spec& dev): drivers::gpio::Gpio(dev) {
        auto ret = configure(GPIO_OUTPUT_ACTIVE);
        if (ret < 0) {
            LOG_ERR("Failed to set %s to GPIO_OUTPUT_ACTIVE", dev_.port->name);
        }
    }

    void Camera::startRecording() {
        if (!started_) {
            strobe();
            started_ = true;
        }
    }
    void Camera::stopRecording() {
        if (started_) {
            strobe();
            started_ = false;
        }
    }

    void Camera::strobe() {
        toggle_pin();
        k_busy_wait(800000);
        toggle_pin();
        k_busy_wait(800000);
        toggle_pin();
    }

    bool Camera::is_started() const {
        return started_;
    }
}