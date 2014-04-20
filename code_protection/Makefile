
all: code_protection

code_protection: cuddlebearsCodeProtection.o gensalt.o mtwist.o randistrs.o
	gcc cuddlebearsCodeProtection.o gensalt.o mtwist.o randistrs.o -I/usr/share/doc -lm -o code_protection -lcrypto

cuddlebearsCodeProtection.o: cuddlebearsCodeProtection.c cuddlebearsCodeProtection.h
	gcc -Wall -ansi -pedantic -c cuddlebearsCodeProtection.c

gensalt.o: gensalt.c gensalt.h
	gcc -Wall -ansi -pedantic -c gensalt.c

mtwist.o: mtwist.c mtwist.h
	gcc -Wall -ansi -pedantic -c mtwist.c

randistrs.o: randistrs.c randistrs.h
	gcc -Wall -ansi -pedantic -lm -c randistrs.c

clean:
	rm -rf *o code_protection
