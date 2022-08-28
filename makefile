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

bin/system_time: examples/system_time.c build/bcm2835.o
bin/event: examples/event.c build/bcm2835.o
bin/blink: examples/blink.c build/bcm2835.o

bin/blink bin/event bin/system_time:
	@[ -d "bin" ] || mkdir "bin"
	@$(CC) \
		$^ \
		$(CFLAGS) \
		-o $@

.PHONY: copy_remote
copy_remote:
	@scp \
		-r \
		$(FILES_TO_COPY) \
		$(SSH_TARGET):~/bcm2835 \
		>/dev/null

.PHONY: run_remote_blink run_remote_event run_remote_system_time
run_remote_blink run_remote_event run_remote_system_time:
	@make copy_remote
	@$(eval target := bin/$(@:run_remote_%=%))
	@$(SSH) -t "cd bcm2835 && make $(target) && sudo ./$(target)"

.PHONY: test_remote
test_remote:
	@make copy_remote
	@$(SSH) -t "cd bcm2835/test && make bin/test && sudo bin/test"
