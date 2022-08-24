#include "bcm2835.h"

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

	chip->gpio = (bcm2835_gpio_peripherals *)chip->peripherals + 0x80000;
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
	free(chip);
}

int main(int argc, char **argv) {
	bcm2835 *chip = bcm2835_open();
	bcm2835_close(chip);

	return 0;
}
