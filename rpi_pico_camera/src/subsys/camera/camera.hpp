#include "drivers/gpio/gpio.hpp"

#ifndef CANSAT_CAMERA
#define CANSAT_CAMERA

namespace payload::subsys {
    class Camera : public drivers::gpio::Gpio {
        public:
        explicit Camera(const gpio_dt_spec &dev);
        void               startRecording();
        void               stopRecording();
        [[nodiscard]] bool is_started() const;
        private:
        void                strobe();
        bool                started_ {false};
    };
}

#endif