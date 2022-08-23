#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
	uint32_t peripheral_base;
	uint32_t peripheral_size;
} bcm2835;

bcm2835 *bcm2835_open();
void bcm2835_close(bcm2835 *chip);
