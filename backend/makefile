main.exe:main.o taxidispatch.o
	g++ -O2 main.o taxidispatch.o -o main -g
taxidispatch.o:taxidispatch.cpp taxidispatch.h
	g++ -O2 taxidispatch.cpp -c
main.o:main.cpp taxidispatch.h
	g++ -O2 main.cpp -c
clean:
	rm main taxidispatch.o main.o