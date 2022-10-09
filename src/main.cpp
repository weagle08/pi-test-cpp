#include <fcntl.h>
#include <gpiod.hpp>
#include <iostream>
#include <linux/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
	std::string chipName = "gpiochip0";

	gpiod_chip* chip = gpiod_chip_open_by_name(chipName.c_str());
	gpiod_line* line23 = gpiod_chip_get_line(chip, 23);
	gpiod_line_request_output(line23, "blink", 0);

	std::string spiDevicePath = "/dev/spidev0.0";
	uint32_t spiMode = SPI_MODE_0 | SPI_TX_OCTAL | SPI_RX_OCTAL;

	auto fd = open(spiDevicePath.c_str(), O_RDWR);
	int ret = 0;

	ret = ioctl(fd, SPI_IOC_RD_MODE32, &spiMode);
	if (ret == -1) {
		std::cout << "failed to set SPI read mode" << std::endl;
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_WR_MODE32, &spiMode);
	if (ret == -1) {
		std::cout << "failed to set SPI write mode" << std::endl;
		return -1;
	}

	uint32_t speed = 1000;
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1) {
		std::cout << "failed to set SPI write speed" << std::endl;
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1) {
		std::cout << "failed to set SPI read speed" << std::endl;
		return -1;
	}

	uint32_t bits = 8;
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1) {
		std::cout << "failed to set read bits per word" << std::endl;
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1) {
		std::cout << "failed to set write bits per word" << std::endl;
		return -1;
	}

	int state = 0;
	while (true) {
		if (state > 0) {
			state = 0;
		} else {
			state = 1;
		}

		uint8_t txData[] = {};
		uint8_t rxData[5] = {
			0,
		};

		struct spi_ioc_transfer tr = {
			.tx_buf = (unsigned long)txData,
			.rx_buf = (unsigned long)rxData,
			.len = 0, // transmit 0 bits?
			.speed_hz = speed,
			.delay_usecs = 0,
			.bits_per_word = bits,
			.tx_nbits = 8,
			.rx_nbits = 8};

		ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
		if (ret == -1) {
			std::cout << "failed to send/receive" << std::endl;
			ret = 0;
		} else {
			uint8_t* rcv = (uint8_t*)tr.rx_buf;
			std::cout << rcv[0] << std::endl;
		}

		gpiod_line_set_value(line23, state);
		std::cout << "sleeping" << std::endl;
		sleep(1);
	}

	return 0;
}