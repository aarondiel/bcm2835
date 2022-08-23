CC := clang
CFLAGS := $(shell cat compile_flags.txt)
FILES_TO_COPY := src include makefile settings.env compile_flags.txt
MAKEFLAGS := --no-print-directory

include ./settings.env

bcm2835: src/bcm2835.c include/bcm2835.h
	@$(CC) \
		src/bcm2835.c \
		$(CFLAGS) \
		-o $@

.PHONY: copy-remote
copy-remote:
	@scp \
		-r \
		$(FILES_TO_COPY) \
		$(SSH_TARGET):~/bcm2835 \
		>/dev/null

.PHONY: run-remote
run-remote:
	@make copy-remote
	@ssh \
		-o LogLevel=QUIET \
		$(SSH_TARGET) \
		-t "cd bcm2835 && make && ./bcm2835"
