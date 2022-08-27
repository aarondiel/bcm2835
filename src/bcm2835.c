#include "bcm2835.h"

void bcm2835_delay(uint64_t millis) {
	struct timespec sleep_timer = {
		.tv_sec = millis / 1000,
		.tv_nsec = (millis % 1000) * 1000000
	};

	nanosleep(&sleep_timer, NULL);
}

void bcm2835_to_little_endian(uint32_t *input) {
	uint8_t *buffer = (uint8_t *)input;

	*input = (
		buffer[0] << 24 |
		buffer[1] << 16 |
		buffer[2] << 8 |
		buffer[3]
	);
}

bcm2835 *bcm2835_open() {
	bcm2835 *chip = malloc(sizeof(bcm2835));

	FILE *fp = fopen("/proc/device-tree/soc/ranges", "rb");

	if (fp == NULL) {
		free(chip);
		return NULL;
	}

	fseek(fp, 4, SEEK_SET);
	fread(&chip->peripheral_base, sizeof(&chip->peripheral_base), 1, fp);
	bcm2835_to_little_endian(&chip->peripheral_base);

	fseek(fp, 8, SEEK_SET);
	fread(&chip->peripheral_size, sizeof(&chip->peripheral_base), 1, fp);
	bcm2835_to_little_endian(&chip->peripheral_size);

	fclose(fp);

	int fd = open("/dev/mem", O_RDWR);

	if (fp == NULL) {
		free(chip);
		return NULL;
	}

	chip->peripherals = mmap(
		NULL,
		chip->peripheral_size,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		fd,
		chip->peripheral_base
	);

	close(fd);

	if (chip->peripherals == MAP_FAILED) {
		free(chip);
		return NULL;
	}

	chip->gpio = (bcm2835_gpio_peripherals *)(chip->peripherals + 0x80000);
	// chip->pwm = bcm2835_peripherals + 0x83000;
	// chip->clock  = bcm2835_peripherals + 0x40400;
	// chip->pads = bcm2835_peripherals + 0x40000;
	// chip->st = bcm2835_peripherals + 0xc00;
	// chip->aux = bcm2835_peripherals + 0x85400;
	// chip->smi = bcm2835_peripherals + 0x180000;

	// chip->spi = {
	// 	bcm2835_peripherals + 0x81000,
	// 	bcm2835_peripherals + 0x85420
	// } ;

	// chip->i2c = {
	// 	bcm2835_peripherals + 0x81400,
	// 	bcm2835_peripherals + 0x201000
	// };

	return chip;
}

void bcm2835_close(bcm2835 *chip) {
	munmap(chip->peripherals, chip->peripheral_size);
	free(chip);
}

bool bcm2835_gpio_select_function(
	bcm2835_gpio_peripherals *gpio,
	uint8_t pin,
	bcm2835_gpio_function function
) {
	if (pin > 53)
		return 1;

	uint8_t bitshift = (pin % 10) * 3;
	uint32_t bitmask = ~(0b111 << bitshift);
	uint32_t current_value = gpio->function_select[pin / 10] & bitmask;
	uint32_t payload = (function << bitshift) ^ current_value;

	gpio->function_select[pin / 10] ^= payload;

	return 0;
}

bool bcm2835_gpio_clear_line(
	bcm2835_gpio_peripherals *gpio,
	uint8_t pin
) {
	if (pin > 53)
		return 1;

	gpio->clear[pin / 32] |= 1 << (pin % 32);

	return 0;
}

bool bcm2835_gpio_write_line(
	bcm2835_gpio_peripherals *gpio,
	uint8_t pin,
	bool state
) {
	if (pin > 53)
		return 1;

	if (!state)
		return bcm2835_gpio_clear_line(gpio, pin);

	gpio->set[pin / 32] |= 1 << (pin % 32);

	return 0;
}

bool bcm2835_gpio_read_line(bcm2835_gpio_peripherals *gpio, uint8_t pin) {
	if (pin > 53)
		return 1;

	uint32_t bitmask = 1 << (pin % 32);

	return gpio->level[pin / 32] & bitmask;
}
