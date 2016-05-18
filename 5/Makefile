CC=g++
CFLAGS=-Wall -g -std=c++11
#LDFLAGS=-lgslcblas -lgsl -lm

sint: analizadorSintactico.o analizadorLexico.o analizadorSemantico.o main.o
	$(CC) $(CFLAGS) analizadorSintactico.o analizadorLexico.o analizadorSemantico.o main.o
analizadorSintactico.o: analizadorSintactico.cpp analizadorSintactico.h
	$(CC) $(CFLAGS) -c analizadorSintactico.cpp
analizadorLexico.o: analizadorLexico.cpp analizadorLexico.h
	$(CC) $(CFLAGS) -c analizadorLexico.cpp
analizadorSemantico.o: analizadorSemantico.cpp analizadorSemantico.h
	$(CC) $(CFLAGS) -c analizadorSemantico.cpp
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
clean:
	rm *.o
