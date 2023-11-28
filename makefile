all: delete compile run

delete:
	rm -f BnB.exe
compile:
	g++ source/BnB.cpp -O3 -o BnB.exe
run:
	./BnB.exe