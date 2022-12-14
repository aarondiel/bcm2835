#pragma once
#pragma pack(1)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>

// why does this struct break once i turn it into a bitfield?

typedef struct {
	uint32_t control_status;
	uint32_t counter_lower;
	uint32_t counter_higher;
	uint32_t compare[4];
} bcm2835_timer_peripherals;

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
	uint32_t rising_edge_detect[2];
	uint32_t: 32;
	uint32_t falling_edge_detect[2];
	uint32_t: 32;
	uint32_t high_detect[2];
	uint32_t: 32;
	uint32_t low_detect[2];
	uint32_t: 32;
	uint32_t async_rising_edge_detect[2];
	uint32_t: 32;
	uint32_t async_falling_edge_detect[2];
	uint32_t: 32;
	uint32_t pull_up_down;
	uint32_t pull_up_down_clock[2];
	uint32_t: 32;
} bcm2835_gpio_peripherals;

#pragma pack()

typedef enum {
	BCM2835_GPIO_FUNCTION_INPUT = 0b000,
	BCM2835_GPIO_FUNCTION_OUTPUT = 0b001,
	BCM2835_GPIO_FUNCTION_ALT0 = 0b100,
	BCM2835_GPIO_FUNCTION_ALT1 = 0b101,
	BCM2835_GPIO_FUNCTION_ALT2 = 0b110,
	BCM2835_GPIO_FUNCTION_ALT3 = 0b111,
	BCM2835_GPIO_FUNCTION_ALT4 = 0b011,
	BCM2835_GPIO_FUNCTION_ALT5 = 0b010
} bcm2835_gpio_function;

typedef enum {
	BCM2835_GPIO_EVENT_RISING_EDGE,
	BCM2835_GPIO_EVENT_FALLING_EDGE,
	BCM2835_GPIO_EVENT_LOW,
	BCM2835_GPIO_EVENT_HIGH,
	BCM2835_GPIO_EVENT_ASYNC_RISING_EDGE,
	BCM2835_GPIO_EVENT_ASYNC_FALLING_EDGE
} bcm2835_gpio_event;

typedef enum {
	BCM2835_GPIO_PULL_DISABLE = 0b000,
	BCM2835_GPIO_PULL_DOWN = 0b001,
	BCM2835_GPIO_PULL_UP = 0b010,
} bcm2835_gpio_pull_up_down_state;

typedef struct {
	uint32_t peripheral_base;
	uint32_t peripheral_size;
	uint32_t *peripherals;
	bcm2835_gpio_peripherals *gpio;
	bcm2835_timer_peripherals *timer;
} bcm2835;

bcm2835 *bcm2835_open();
void bcm2835_close(bcm2835 *chip);
void bcm2835_delay(uint64_t millis);
void bcm2835_delay_micro(bcm2835_timer_peripherals *timer, uint64_t micros);

bool bcm2835_gpio_read_line(bcm2835_gpio_peripherals *gpio, uint8_t pin);
bool bcm2835_gpio_event_status(bcm2835_gpio_peripherals *gpio, uint8_t pin);
bool bcm2835_gpio_select_function(
	bcm2835_gpio_peripherals *gpio,
	uint8_t pin,
	bcm2835_gpio_function function
);

bool bcm2835_gpio_write_line(
	bcm2835_gpio_peripherals *gpio,
	uint8_t pin,
	bool state
);

bool bcm2835_gpio_event_detect(
	bcm2835_gpio_peripherals *gpio,
	uint8_t pin,
	bcm2835_gpio_event event,
	bool enable
);

uint64_t bcm2835_timer_time(bcm2835_timer_peripherals *timer);
