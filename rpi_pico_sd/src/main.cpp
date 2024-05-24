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

	// static FATFS fat_fs;
	// static struct fs_mount_t mp = {
	// 	.type = FS_FATFS,
	// 	.fs_data = &fat_fs,
	// };

	// int res;
	// static const char* disk_pdrv = "SD";
	// res = disk_access_init(disk_pdrv);
	// if(res){
	// 	printk("Storage init ERROR\n");
	// }

	// const char* disk_mount_point = "/SD:";
	// mp.mnt_point = disk_mount_point;
	// res = fs_mount(&mp);
	// if(res != FR_OK){
	// 	printk("Error mounting disk\n");
	// }

	// struct fs_file_t filep;
	// const char* filename = "/SD:/flight.txt";

	// fs_file_t_init(&filep);

	// res = fs_open(&filep, filename, FS_O_RDWR | FS_O_CREATE | FS_O_APPEND);
	// if(res){
	// 	printk("Error opening file %s [%i]\n", filename, res);
	// }
	// const char* buf = "los pollos hermanos";
	// res = fs_write(&filep, buf, strlen(buf));
	
	// if(res < 0){
	// 	printk("Error writing to file %s [%i]\n", filename, res);
	// }

	// fs_close(&filep);

	return 0;
}
