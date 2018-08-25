# Makefile for compiling the interpreter and cleaning binaries 
source = main.c \
	util.c \
	queue.c

include = util.h \
	queue.h \
	main.h

bin/verse: $(addprefix src/, $(source)) $(addprefix include/, $(include))
	gcc -Iinclude -o bin/verse $(addprefix src/, $(source))

.PHONY: clean
clean:
	@echo cleaning...
	rm bin/verse