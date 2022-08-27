include ./settings.env

CC = clang
SSH = ssh -o LogLevel=QUIET $(SSH_TARGET)
CFLAGS = $(shell cat compile_flags.txt)
FILES_TO_COPY = src include examples test makefile settings.env compile_flags.txt
MAKEFLAGS = --no-print-directory

build/bcm2835.o: src/bcm2835.c include/bcm2835.h
	@[ -d "build" ] || mkdir "build"
	@$(CC) \
		$< \
		$(CFLAGS) \
		-c \
		-o $@

bin/blink: examples/blink.c build/bcm2835.o
	@[ -d "bin" ] || mkdir "bin"
	@$(CC) \
		$^ \
		$(CFLAGS) \
		-o $@

.PHONY: copy-remote
copy-remote:
	@scp \
		-r \
		$(FILES_TO_COPY) \
		$(SSH_TARGET):~/bcm2835 \
		>/dev/null

.PHONY: run-remote-blink
run-remote-blink:
	@make copy-remote
	@$(eval target := bin/$(@:run-remote-%=%))
	@$(SSH) -t "cd bcm2835 && make $(target) && sudo ./$(target)"

.PHONY: test-remote
test-remote:
	@make copy-remote
	@$(SSH) -t "cd bcm2835/test && make bin/test && sudo bin/test"
