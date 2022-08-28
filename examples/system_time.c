#include "bcm2835.h"
#include <stddef.h>

int main(int argc, char **argv) {
	bcm2835 *chip = bcm2835_open();

	if (chip == NULL)
		return 1;

	uint64_t system_time = bcm2835_system_timer_time(chip->system_timer);
	printf("%lu\n", system_time);

	bcm2835_close(chip);

	return 0;
}
