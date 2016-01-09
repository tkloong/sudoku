#ifndef SUDOKU_H
#define SUDOKU_H

class Sudoku{
	public:
		Sudoku(int &level);
		void generation (int &level);
		void difficulty (int &level);
		void printMap (int y, int x);
		void printQ (); //print question
		void modify (int &c_y, int &c_x, int num);
		bool isCorrect ();
		void outputQ();
	private:
		int coordinateYX[9][9];
		int questionYX[9][9];
		int start_pos_y;
		int start_pos_x;
		void initCoordinate ();
		void initIsNumLegal (int *num);
		void initNum (int *num);
		void checkLegalNum (int &row, int &col, int &totalLegalNumber);
};

#endif
