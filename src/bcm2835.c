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

	fseek(fp, 4, SEEK_SET);
	fread(&chip->peripheral_base, sizeof(chip->peripheral_base), 1, fp);
	bcm2835_to_little_endian(&chip->peripheral_base);

	fseek(fp, 8, SEEK_SET);
	fread(&chip->peripheral_size, sizeof(chip->peripheral_base), 1, fp);
	bcm2835_to_little_endian(&chip->peripheral_size);

	fclose(fp);

	return chip;
}

void bcm2835_close(bcm2835 *chip) {
	free(chip);
}

int main(int argc, char **argv) {
	bcm2835 *chip = bcm2835_open();

	printf("0x%08x\n", chip->peripheral_base);
	printf("0x%08x\n", chip->peripheral_size);

	bcm2835_close(chip);

	return 0;
}
