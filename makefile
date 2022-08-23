CC := clang
CFLAGS := $(shell cat compile_flags.txt)

include ./settings.env

bcm2835: src/bcm2835.c include/bcm2835.h
	@$(CC) \
		src/bcm2835.c \
		$(CFLAGS) \
		-o $@
