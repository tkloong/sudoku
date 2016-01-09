/*
Name : Tan Kent Loong   CSIE 104 A
Date : March 2011
compile : gcc -o main main.c++ Sudoku.c++ timer.c++ -lncursesw
 */
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <unistd.h>
#include <locale.h>
#include "sudoku.h"
#include "timer.h"
#define  map_Y  2
#define  map_X  2

#define ASCII_ENTER 10

using namespace std;

void initialize(int &height, int &width){
	initscr();
	cbreak();  // disable key buffering
	noecho();  // disable echoing
//	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);  // enable keypad reading
	getmaxyx(stdscr, height, width);  // get screen size
}

char choice(int cursor_y, int cursor_x, int gap, int total, bool go_on){  // Read cursor
	int option = 0;
	int key;
	Timer t;
	for (;;) {
		move(cursor_y, cursor_x);  // Move cursor
		if ((key = toupper(getch())) == ERR) {	
			if (go_on) mvprintw(map_Y+3, map_X+42, "Time : %4d (seconds)", t.now() - Timer::get_start_time());
		}
		else {
			switch (key){
				case KEY_UP:
					option--;
					if (option < 0) option = 0;
					else cursor_y -= gap;  // gap between choices = 4
					break;
				case KEY_DOWN: 
					option++;
					if (option > (total-1)) option = total - 1;
					else cursor_y += gap;  // gap between choices = 4
					break;
				case KEY_RIGHT: 
				case ASCII_ENTER: 
					return option;
				case 'Q':
					// If game is still active, move cursor back to game board
					if (go_on) {
						return 4;
					}
			}
			move(cursor_y, cursor_x);  // Move cursor
			refresh();
		}
	}
}

char homepage(int &height, int &width) {
	attron(A_BOLD);
	attron(A_UNDERLINE);
	mvprintw(5, 40, "Sudoku");
	attrset(A_NORMAL);

	int position = 9;
	mvprintw(position, 40, "Play");  // Position of 'Play'
	mvprintw(position+4, 40, "Quit");  // Position of 'Quit'

	return choice(position, 38, 4, 2, FALSE);
}

int play(int &level){
	Sudoku sudoku(level);
	Timer watch;
	int elapsed_time;
	int key;
	bool hotKey;
	int x=0, y=0; 
	int c_x=0, c_y=0;  //coordinate of the element of sudoku
	watch.start_timer();
	char resume;
	while (TRUE) {
		nodelay(stdscr, TRUE);
		hotKey = TRUE;
		sudoku.printMap(map_Y, map_X);
		sudoku.printQ();
		/* Print Menu */
		mvprintw(map_Y+11, map_X+42, "'P'ause");
		mvprintw(map_Y+13, map_X+42, "'N'ext");
		mvprintw(map_Y+15, map_X+42, "'C'heck answer");
		mvprintw(map_Y+17, map_X+42, "'M'ain");
		mvprintw(map_Y+21, map_X, "Press 'q' to navigate between gameboard and menu");
		sudoku.outputQ();  //Output sudoku question as a file
		move(map_Y+y+1, map_X+x+2);
		refresh();
		if ((key = toupper(getch())) == ERR) {  // User hasn't responded
			elapsed_time = watch.elapsed_time();
			mvprintw(map_Y+3, map_X+42, "Time : %4d (seconds)", elapsed_time);
		}
		else {
			while (hotKey) {
				switch (key) {
					case KEY_LEFT:
					case 'H':
						if (x!=0 && (x==10 || x==22)) x-=1;
						x-=4;
						c_x--;
						break;
					case KEY_RIGHT:
					case 'L':
						if (x!=0 && (x==9 || x==22)) x+=1;
						x+=4;
						c_x++;
						break;
					case KEY_UP:
					case 'K':
						y-=2;
						c_y--;
						break;
					case KEY_DOWN:
					case 'J':
						y+=2;
						c_y++;
						break;
					case '0': case '1':
					case '2': case '3':
					case '4': case '5':
					case '6': case '7':
					case '8': case '9':
						sudoku.modify(c_y, c_x, key-'0');
						break;
					case 'P':
						clear();
						mvprintw(7, 20, "Game paused...");
						mvprintw(10, 14, "Press anykey to continue...");
						nodelay(stdscr, FALSE);
						resume = getch();
						break;
					case 'N':
						mvprintw(map_Y+19, map_X, "			            ");
						sudoku.generation(level);
						watch.start_timer();
						break;
					case 'C':
						if (!sudoku.isCorrect()) {
							mvprintw(map_Y+19, map_X+3, "The answer is correct!");
							key = choice(map_Y+13, map_X+40, 2, 3, FALSE);
							if (key == 0) key = 'N';
							else if (key == 1) key = 'C';
							else key = 'M';
							continue;
						}
						else  mvprintw(map_Y+19, map_X+3, "The answer is wrong!   ");
						break;
					case 'Q':
						if ((key = choice(map_Y+11, map_X+40, 2, 4, TRUE)) == ERR) {
							elapsed_time = watch.elapsed_time();
							mvprintw(map_Y+3, map_X+42, "Time : %5d (seconds)", elapsed_time);
						}
						if (key == 0) key = 'P';
						else if (key == 1) key = 'N';
						else if (key == 2) key = 'C';
						else if (key == 3) key = 'M';
						continue;
					case 'M':
						return 1;
						break;
						//default : hotKey = FALSE;  // User do not want to use hot key
				} // switch (c)
				hotKey = FALSE;  // User do not want to use hot key
				while (x < 0)   {x += 36; c_x = 8;}
				while (x >= 36) {x = 0; c_x = 0;}
				while (y < 0)   {y += 18; c_y = 8;}
				while (y >= 18) {y = 0; c_y = 0;}
			}
		}
	} // while ()
}

int main(){
	int width, height;
	setlocale(LC_ALL,"");
	initialize(height, width);
	int back_to_main = 1;
	int level;
	//float position = 0.4;

	do {
		clear();
		char answer = homepage(height, width);
		switch (answer){
			case 0:  //Play game
				mvprintw(5, 40, "Difficulty:");
				mvprintw(9, 40, "Easy");
				mvprintw(13, 40, "Medium");
				mvprintw(17, 40, "Hard");
				level = choice(9, 38, 4, 3, FALSE);
				switch (level) {  //Choose difficulty
					case 0:
						level = 3;
						break;
					case 1:
						level = 4;
						break;
					case 2:
						level = 5;
						break;
				}
				clear();
				back_to_main = play(level);
				break; 
			case 1:  // Exit
				endwin();
				return 0;  
		}
	} while (back_to_main == 1);
	endwin();
	return 0;  //Exit
}

