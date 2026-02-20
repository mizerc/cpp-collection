all: preclean compile run postclean

compile:
	gcc -o main main.c

run:
	./main

preclean:
	rm -f main

postclean:
	rm -f main

rebuild:
	make clean
	make compile
	make run