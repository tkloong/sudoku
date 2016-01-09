#include "sudoku.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>
#include <curses.h>
#include <locale.h>
#include <time.h>
#include <fstream>

using namespace std;

static int num[9];  // number from 1~9

Sudoku::Sudoku(int &level){  // constructor
	generation(level);
}

void Sudoku::initCoordinate(){
	for (int r=0; r<9; r++)
		for (int c=0; c<9; c++){
			questionYX[r][c] = 0;
			coordinateYX[r][c] = 0;
	}
}

void Sudoku::initIsNumLegal(int *num){
	for (int i=0; i<9; i++){
		num[i] = 1;
	}
}

void Sudoku::initNum(int *num){
	for (int i=0; i<9; i++){
		num[i] = i+1;
	}
}

void Sudoku::checkLegalNum(int &row, int &col, int &totalLegalNumber){
	int isNumLegal[9];  // At the beginning, every number is legal
	initNum(num);
	initIsNumLegal(isNumLegal);
	totalLegalNumber = 9;  // There are temporarily 9(all) numbers which can fill into the grid.
	int count;

	if (col != 0){  // Delegitimize the number in the same row.
		count = col-1;
		while (count >= 0){
			isNumLegal[coordinateYX[row][count]-1] = 0;
			count--;
		}
	}

	if (row != 0){  // Delegitimize the number in the same column.
		count = row-1;
		while (count >= 0){
			isNumLegal[coordinateYX[count][col]-1] = 0;
			count--;
		}
	}

	if (row % 3 != 0){  // Delegitimize the numbers in each block If they are not in the first row.
		int colBlock[9]={0,0,0,3,3,3,6,6,6};
		int r = row-1, c = colBlock[col];
		count = 0;
		while (count < (row%3)){  // Determine how many row(s) should I check.
			while(1){
				isNumLegal[coordinateYX[r][c]-1] = 0;
				if ((c+1)%3==0){  // Check if the number has reached block's wall.
					c-=2;
					break;
				}
				else c++;
			}
			r--;
			count++;
		}
	}

	for (int i=0; i<9; i++) 
		if (isNumLegal[i] == 0)
			totalLegalNumber--;  // Calculate total of legal numbers.

	int moveItem = 0, last = 8;
	bool isSwapNum = FALSE;
	if (totalLegalNumber == 0) {  // Check one more time.
		row--; col = 0;  // re-assign the number from the beginning column.
		checkLegalNum(row, col, totalLegalNumber);
	}
	else if (totalLegalNumber != 9) {  // swap num[]  
		for (int i=0; last>=i; ) { 
			if (!isSwapNum && isNumLegal[i]==0) {  // Find the number that is illegal.
				moveItem = i;   // Need to move i-th item. 
				i += 1;
				isSwapNum = TRUE;
			} 
			else if (isSwapNum && isNumLegal[last]==1) {  // Find the legal number from the end of num[].
				isNumLegal[last] = 0;
				isNumLegal[moveItem] = 1;
				swap(num[moveItem], num[last]);
				last -= 1;
				isSwapNum = FALSE;
			}
			else if (isSwapNum) {  // if isNumLegal[last] is illegal, then go to next number.
				last--;
			}
			else i++;
		}
	}
}

void Sudoku::difficulty(int &level){
	/*dig hole*/
	int last;
	int nrand;
	initNum(num);
	for (int row=0; row<9; row++){
		last = 9;
		for (int i=0; i < level; i++) {
			nrand = rand() % last;
			swap(num[nrand], num[last]);
			last--;
			questionYX[row][nrand] = coordinateYX[row][nrand] = 0;
			questionYX[nrand][row] = coordinateYX[nrand][row] = 0;
		}
	}

}

void Sudoku::generation(int &level){   // Generate a new Sudoku
	initCoordinate();  // Initialize coordinate before generation
	srand(time(NULL));
	int totalLegalNumber;
	int nrand;
	for (int row=0; row<9; row++){
		for (int col=0; col<9; col++){
			checkLegalNum(row, col, totalLegalNumber);
			nrand = rand() % totalLegalNumber;
			questionYX[row][col] = coordinateYX[row][col] = num[nrand];
		}
	}
	
	difficulty(level);
}

void Sudoku::printMap(int y, int x){  // print Sudoku map/table
	start_pos_y = y;
	start_pos_x = x;
	mvaddstr(start_pos_y, start_pos_x,   "┌───┬───┬───┬───┬───┬───┬───┬───┬───┐"); 
	mvaddstr(start_pos_y+1, start_pos_x, "│   |   |   │   │   │   │   |   |   │"); 
	mvaddstr(start_pos_y+2, start_pos_x, "├---|---|---│---|---|---│---|---|---┤"); 
	mvaddstr(start_pos_y+3, start_pos_x, "│   |   |   │   |   |   │   |   |   │"); 
	mvaddstr(start_pos_y+4, start_pos_x, "├---|---|---│---|---|---│---|---|---┤"); 
	mvaddstr(start_pos_y+5, start_pos_x, "│   |   |   │   |   |   │   |   |   │"); 
	mvaddstr(start_pos_y+6, start_pos_x, "├───┼───┼───┼───┼───┼───┼───┼───┼───┤"); 
	mvaddstr(start_pos_y+7, start_pos_x, "│   |   |   │   |   |   │   |   |   │"); 
	mvaddstr(start_pos_y+8, start_pos_x, "├---|---|---│---|---|---│---|---|---┤"); 
	mvaddstr(start_pos_y+9, start_pos_x, "│   |   |   │   |   |   │   |   |   │"); 
	mvaddstr(start_pos_y+10, start_pos_x, "├---|---|---│---|---|---│---|---|---┤"); 
	mvaddstr(start_pos_y+11, start_pos_x, "│   |   |   │   |   |   │   |   |   │"); 
	mvaddstr(start_pos_y+12, start_pos_x, "├───┼───┼───┼───┼───┼───┼───┼───┼───┤"); 
	mvaddstr(start_pos_y+13, start_pos_x, "│   |   |   │   |   |   │   |   |   │"); 
	mvaddstr(start_pos_y+14, start_pos_x, "├---|---|---│---|---|---│---|---|---┤"); 
	mvaddstr(start_pos_y+15, start_pos_x, "│   |   |   │   |   |   │   |   |   │"); 
	mvaddstr(start_pos_y+16, start_pos_x, "├---|---|---│---|---|---│---|---|---┤"); 
	mvaddstr(start_pos_y+17, start_pos_x, "│   |   |   │   |   |   │   |   |   │"); 
	mvaddstr(start_pos_y+18, start_pos_x, "└───┴───┴───┴───┴───┴───┴───┴───┴───┘");
}

void Sudoku::printQ(){   // print Sudoku question
	int row = 0, col = 0;
	for (int r=0; r<9; r++){
		if (r==0) row = start_pos_y+1;
		else row += 2;
		for (int c=0; c<9; c++){
			if (c==0) col = start_pos_x+2;
			else col += 4;
			move(row, col);
			if (coordinateYX[r][c] == 0) printw(" ");
			else {
				if(questionYX[r][c] != 0) {
					attron(A_BOLD);
					attron(A_UNDERLINE);
				}
				printw("%d", coordinateYX[r][c]);
				attrset(A_NORMAL);
			}
		}
	}
}

void Sudoku::modify(int &c_y, int &c_x, int num){  // Enable user to type the answer
	if (questionYX[c_y][c_x] == 0)
		coordinateYX[c_y][c_x] = num;
}

bool Sudoku::isCorrect(){  // Check the answer
	int sumHor, sumVer;
	for (int r=0; r<9; r++){
		sumHor = 0;
		sumVer = 0;
		for (int c=0; c<9; c++){
			sumHor += coordinateYX[r][c]; // The sum of every column 
			sumVer += coordinateYX[c][r]; // The sum of every row
		}
		if (sumHor!=45 || sumVer!=45) return 0;  //the sum of every row or column must be 1+2+...+9=45, otherwise the answer is wrong
	}
	return 1;  //The answer is correct
}

void Sudoku::outputQ() {
	ofstream sudokuQ("Sudoku.txt", ios::out);
	
	if (!sudokuQ) {
		cerr << "File could not be opened!" << endl;
		exit(1);
	}

	for (int row=0; row<9; row++) {
		for (int col=0; col<9; col++) {
			sudokuQ << questionYX[row][col];
		}
		sudokuQ << endl;
	}
}
