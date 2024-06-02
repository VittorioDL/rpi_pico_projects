#include <stdio.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include "drivers/sdcard/sdcard.hpp"

#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/fs/fs.h>
#include <ff.h>

//3v3, CS, MOSI, CLK, MISO, GND
//MISO: RX
//MOSI: TX

int main() {
	drivers::sdcard::SDCard sd;

	sd.init();
	std::string p = "23.3,324.2";
	sd.writeTelemetry("aa");

	return 0;
}
