#include "bcm2835.h"

int main(int argc, char **argv) {
	bcm2835 *chip = bcm2835_open();

	if (chip == NULL)
		return 1;

	const uint8_t pin = 17;
	bcm2835_gpio_select_function(chip->gpio, pin, BCM2835_GPIO_FUNCTION_INPUT);

	for (uint8_t i = 0; i < 20; i++) {
		bcm2835_gpio_write_line(chip->gpio, pin, 1);
		bcm2835_delay(500);
		bcm2835_gpio_write_line(chip->gpio, pin, 0);
		bcm2835_delay(500);
	}

	bcm2835_close(chip);

	return 0;
}
