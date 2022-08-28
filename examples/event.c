#include "bcm2835.h"

int main(int argc, char **argv) {
	bcm2835 *chip = bcm2835_open();

	if (chip == NULL)
		return 1;

	const uint8_t pin = 17;

	bcm2835_gpio_select_function(chip->gpio, pin, BCM2835_GPIO_FUNCTION_INPUT);
	// bcm2835_gpio_set_pull_up_down(chip->gpio, pin, BCM2835_GPIO_PULL_UP)
	// bcm2835_gpio_event_detect(chip->gpio, pin, BCM2835_GPIO_EVENT_LOW)

	for (uint8_t i = 0; i < 20; i++) {
		// bool event = bcm2835_gpio_event_status(chip->gpio, pin);

		// if (event) {
		// 	bcm2835_gpio_event_clear(chip->gpio, pin);
		// 	printf("event detected!\n");
		// }

		bcm2835_delay(500);
	}

	bcm2835_close(chip);

	return 0;
}
