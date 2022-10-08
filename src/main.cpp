#include <gpiod.hpp>
#include <string>
#include <unistd.h>

int main() {
	std::string chipName = "gpiochip0";

	gpiod_chip* chip = gpiod_chip_open_by_name(chipName.c_str());
	gpiod_line* line23 = gpiod_chip_get_line(chip, 23);
	gpiod_line_request_output(line23, "blink", 0);

	int state = 0;
	while (true) {
		if (state > 0) {
			state = 0;
		} else {
			state = 1;
		}

		gpiod_line_set_value(line23, state);
		sleep(1);
	}

	return 0;
}