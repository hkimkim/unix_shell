all: shell.c
	gcc shell.c -w -o shell

shell:
	make all
	./shell .

clean:
	rm shell

.PHONY: all shell
