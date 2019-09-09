all: simpleCSVsorter.c merge.c
	make clean;
	gcc -c merge.c;
	gcc -c simpleCSVsorter.c;
	gcc merge.o simpleCSVsorter.o -o simpleCSVsorter;
	make clean-obj;

clean-obj:
	rm -f merge.o
	rm -f simpleCSVsorter.o
clean:
	rm -f simpleCSVsorter
	rm -f merge.o
	rm -f simpleCSVsorter.o
