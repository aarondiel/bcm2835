#pragma once
#pragma pack(1)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct {
	uint32_t function_select[6];
	uint32_t: 32;
	uint32_t set[2];
	uint32_t: 32;
	uint32_t clear[2];
	uint32_t: 32;
	uint32_t level[2];
	uint32_t: 32;
	uint32_t event_status[2];
	uint32_t: 32;
	u_int32_t rising_edge_detect_enable[2];
	u_int32_t: 32;
	u_int32_t falling_edge_detect_enable[2];
	u_int32_t: 32;
	u_int32_t high_detect_enable[2];
	u_int32_t: 32;
	u_int32_t low_detect_enable[2];
	u_int32_t: 32;
	u_int32_t async_rising_edge_detect[2];
	u_int32_t: 32;
	u_int32_t async_falling_edge_detect[2];
	u_int32_t: 32;
	u_int32_t pull_up_down_enable;
	u_int32_t pull_up_down_clock[2];
} bcm2835_gpio_peripherals;

#pragma pack()

typedef struct {
	uint32_t peripheral_base;
	uint32_t peripheral_size;
	bcm2835_gpio_peripherals *gpio;
	uint32_t *peripherals;
} bcm2835;

bcm2835 *bcm2835_open();
void bcm2835_close(bcm2835 *chip);
