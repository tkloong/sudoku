sudoku.exe: main.o sudoku.o timer.o
	g++ -o sudoku main.o sudoku.o timer.o -lncursesw

main.o:
	g++ -c main.cpp

sudoku.o:
	g++ -c sudoku.cpp

timer.o:
	g++ -c timer.cpp

clean:
	rm *.o sudoku
