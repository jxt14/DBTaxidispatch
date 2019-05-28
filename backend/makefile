main.exe:main.o taxidispatch.o
	g++ main.o taxidispatch.o -o main -g
taxidispatch.o:taxidispatch.cpp taxidispatch.h
	g++ taxidispatch.cpp -c
main.o:main.cpp taxidispatch.h
	g++ main.cpp -c
