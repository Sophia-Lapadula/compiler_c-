all: exec

parser.c: parser.y
	bison -d parser.y

scanner.c: scanner.l
	flex scanner.l

exec: parser.c scanner.c
	gcc *.c -o exec

clean:
	rm -f *.o exec parser.c parser.h scanner.c