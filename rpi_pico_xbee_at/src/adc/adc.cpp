#include <zephyr/logging/log.h>

#include <cstdio>
#include "adc.hpp"

LOG_MODULE_REGISTER(adc, LOG_LEVEL_DBG);

namespace drivers::adc {
	Adc::Adc() {
		int err;
		int16_t buf;
		struct adc_sequence sequence = {
			.buffer = &buf,
			/* buffer size in bytes, not number of samples */
			.buffer_size = sizeof(buf),
		};

		/* Configure channels individually prior to sampling. */
		for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
			if (!device_is_ready(adc_channels[i].dev)) {
				printk("ADC controller device not ready\n");
				return;
			}

			err = adc_channel_setup_dt(&adc_channels[i]);
			if (err < 0) {
				printk("Could not setup channel #%d (%d)\n", i, err);
				return;
			}
		}
	}

	double Adc::getVoltage() {
		int32_t val_mv;
		uint8_t i = 0;
		printk("- %s, channel %d: ",
				adc_channels[i].dev->name,
				adc_channels[i].channel_id);

		(void)adc_sequence_init_dt(&adc_channels[i], &sequence);

		err = adc_read(adc_channels[i].dev, &sequence);
		if (err < 0) {
			printk("Could not read (%d)\n", err);
		} else {
			printk("%"PRId16, buf);
		}

		/* conversion to mV may not be supported, skip if not */
		val_mv = buf;
		err = adc_raw_to_millivolts_dt(&adc_channels[i],
							&val_mv);
		if (err < 0) {
			printk(" (value in mV not available)\n");
		} else {
			val_mv = val_mv * 4300/3300;
			printk(" = %"PRId32" mV\n", val_mv);
		}

		return round((val_mv/1000.0) * 10)/10;
		return 0;
	}
}