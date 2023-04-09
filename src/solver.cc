#ifndef SUDOKU_BOARD_BACKEND
#define SUDOKU_BOARD_BACKEND
#include <algorithm>
#include <cstddef>
#include <vector>
#include <iostream>
class sudoku_board {
	public:
		sudoku_board(std::vector<std::vector<int>> _board_values)
			: board_values(_board_values)
		{
			for (int r = 0; r < 9; r++) {
				std::vector<bool> temp_row;
				for (int c = 0; c < 9; c++) temp_row.push_back(board_values[r][c] == 0);
				replacement_mask.push_back(temp_row);
			};
	//		for (auto r : replacement_mask) { for (auto c : r) {std::cout << c << ",";} std::cout << "\n";};
		};
		inline void show_board();
		inline static bool replacement_possible(int _row, int _col, int n, std::vector<std::vector<int>> _board);
		inline void solve(unsigned int method = 0) {
			if (method == 0) solver_2();
			else if (method == 1 or method !=0) board_values = solver(board_values);
		}
		inline bool is_valid() {return validator(board_values);};
		inline static bool validator(std::vector<std::vector<int>> _board_values);
		inline std::vector<int> get_column(unsigned int _column);
		inline std::vector<int> get_row(unsigned int _row);
		inline std::vector<int> get_unit(unsigned int _unit);
		inline bool update_cell(unsigned int _row, unsigned int _col, int n);
		std::vector<std::vector<int>> board_values;
	private:
		std::vector<std::vector<bool>> replacement_mask;
		inline static std::vector<std::vector<int>> solver(std::vector<std::vector<int>> _board_values);
		inline static bool checkNine(std::vector<int> _vector);
		inline bool valid_cell(unsigned int _row, unsigned int _col);
		inline bool solver_2();
};
//checks if the space is valid for replacement. (i.e. if the cell is in the replacement mask)
bool sudoku_board::valid_cell(unsigned int _row, unsigned int _col) {
	return replacement_mask[_row][_col];
}
//updates the cell if the specific value is possible
bool sudoku_board::update_cell(unsigned int _row, unsigned int _col, int n){
	if (valid_cell(_row,_col)) { // or replacement_possible(_row, _col, n,board_values))) {
		board_values[_row][_col] = n;
		return true;
	}
	else return false;
};
//Returns the specified column as a vector, from row 0 to 8
std::vector<int> sudoku_board::get_column(unsigned int _column) {
	return(std::vector<int>({
		board_values[0][_column],board_values[1][_column],
		board_values[2][_column],board_values[3][_column],
		board_values[4][_column],board_values[5][_column],
		board_values[6][_column],board_values[7][_column],
		board_values[7][_column],board_values[8][_column]
	}));
}
//Returns the specified row as a vector, from column 0 to 8
std::vector<int> sudoku_board::get_row(unsigned int _row) {
	return(std::vector<int>({
		board_values[_row][0],board_values[_row][1],
		board_values[_row][2],board_values[_row][3],
		board_values[_row][4],board_values[_row][5],
		board_values[_row][6],board_values[_row][7],
		board_values[_row][7],board_values[_row][8]
	}));
}
//returns the specified unit as a vector, left-right then top-down
std::vector<int> sudoku_board::get_unit(unsigned int _unit) {
		uint8_t off_x;
		uint8_t off_y;
		switch(_unit) {
			case 0: off_x = 0; off_y = 0; break;
			case 1: off_x = 3; off_y = 0; break;
			case 2: off_x = 6; off_y = 0; break;
			case 3: off_x = 0; off_y = 3; break;
			case 4: off_x = 3; off_y = 3; break;
			case 5: off_x = 6; off_y = 3; break;
			case 6: off_x = 0; off_y = 6; break;
			case 7: off_x = 3; off_y = 6; break;
			case 8: off_x = 6; off_y = 6; break;
		}
		//
		return(std::vector<int>({
			board_values[0 + off_y][0 + off_x],board_values[0 + off_y][1 + off_x],board_values[0 + off_y][2 + off_x],
			board_values[1 + off_y][0 + off_x],board_values[1 + off_y][1 + off_x],board_values[1 + off_y][2 + off_x],
			board_values[2 + off_y][0 + off_x],board_values[2 + off_y][1 + off_x],board_values[2 + off_y][2 + off_x]
		}));
}
void sudoku_board::show_board() {
	std::cout << "\n";
	for (int row = 0; row < board_values.size(); row++) {
		for (int col = 0; col < board_values[row].size(); col++)
			std::cout << ((board_values[row][col] == 0)?"+":std::to_string(board_values[row][col])) << " ";
		std::cout << "\n";
	}
}
bool sudoku_board::validator(std::vector<std::vector<int>> _board_values) {
	//get_row/get_column/get_unit are not being used because they aren't static.
	//Validate Rows
	for (std::vector<int>& row : _board_values)
		if (checkNine(row) == false) return false;
	//Validate Columns
	for (int c = 0; c < 9; c++) {
		std::vector<int> col_values = {
			_board_values[0][c],_board_values[1][c],
			_board_values[2][c],_board_values[3][c],
			_board_values[4][c],_board_values[5][c],
			_board_values[6][c],_board_values[7][c],
			_board_values[7][c],_board_values[8][c]
		};
		if (checkNine(std::move(col_values)) == false) return false;
	}
	//Validate Units
	for (int unit = 0; unit < 9; unit++) {
		uint8_t off_x;
		uint8_t off_y;
		switch(unit) {
			case 0: off_x = 0; off_y = 0; break;
			case 1: off_x = 3; off_y = 0; break;
			case 2: off_x = 6; off_y = 0; break;
			case 3: off_x = 0; off_y = 3; break;
			case 4: off_x = 3; off_y = 3; break;
			case 5: off_x = 6; off_y = 3; break;
			case 6: off_x = 0; off_y = 6; break;
			case 7: off_x = 3; off_y = 6; break;
			case 8: off_x = 6; off_y = 6; break;
		}
		//
		const std::vector<int> unit_values = {
			_board_values[0 + off_y][0 + off_x],_board_values[0 + off_y][1 + off_x],
			_board_values[0 + off_y][2 + off_x],_board_values[1 + off_y][0 + off_x],
			_board_values[1 + off_y][1 + off_x],_board_values[1 + off_y][2 + off_x],
			_board_values[2 + off_y][0 + off_x],_board_values[2 + off_y][1 + off_x],
			_board_values[2 + off_y][2 + off_x]
		};
		if (checkNine(std::move(unit_values)) == false) return false;
	}
	return true;
}

bool sudoku_board::checkNine(std::vector<int> _vector) {
	bool values[] = {0,0,0,0,0,0,0,0,0};
	for (auto& value : _vector) values[value-1] = 1;
	return (values[0] and values[1]
		and values[2] and values[3]
		and values[4] and values[5]
		and values[6] and values[7]
		and values[8]); 
}
bool sudoku_board::replacement_possible(int _row, int _col, int n, std::vector<std::vector<int>> _board) {
	for (int r = 0; r < 9; r++) {
		if (_board[r][_col] == n) return false;
	}
	for (int c = 0; c < 9; c++) {
		if (_board[_row][c] == n) return false;
	}
	int y_offset = (_row/3)*3;
	int x_offset = (_col/3)*3;
	for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++) {
		if (_board[y_offset+y][x_offset+x] == n) return false;
	}
	//
	return true;
}
//solves the board using recursion, but passes the board along to each call. Likely inefficient
std::vector<std::vector<int>> sudoku_board::solver(std::vector<std::vector<int>> _board_values) {
	if (_board_values.empty()) return _board_values;
	for (int r = 0; r < 9; r++) for (int c = 0; c < 9; c++) {
		if (_board_values[r][c] == 0) {
			for (int n = 1; n < 10; n++) {
				if (replacement_possible(r,c,n,_board_values) == true) {
					_board_values[r][c] = n;
					auto s = solver(_board_values);
					if (s.empty()) _board_values[r][c] = 0;
					else return s;
				}
			} return {}; 
		}
	} 
	return _board_values;
}
//solves the board using recursion directly on the classes board. Likely efficient
bool sudoku_board::solver_2() {
	//If we have found a valid solution, disable backtracking by returning 1(this causes the trees to collapse)
	if (is_valid() == true)	return 1;	
	//iterate over every cell in the board
	for (int r = 0; r<9; r++) for (int c = 0; c < 9 ; c++) {
		if (board_values[r][c] == 0) { //if intended for replacement
			for (int n = 1; n <= 9; n++) { //attempt all values in each replacement position
				board_values[r][c] = n; //attempt w/ n
				auto s = solver_2(); //recursion. Contnues branching
				if(s) return s;
				board_values[r][c] = 0; //reset back to 0 if n did not work
			}
			//this point is reached only after attemting all replacements
		}
		//reached whenever a value has EITHER finished all possibilities, or value was pre-determined
	}
	return 0; //returning a 0 means that backtracking will occur. The last value will be set back to 0 and continue looping from (n,9]
	//reached after all cells have been passsed through, and all replacements have used
}
/*
 * <Proposed 3rd solving technique>
 * Collect the replacement positions into a stack that saves information about them.
 * Tie them, via pointers, to the values in the board.
 * Iterate over the list of pointers and check possibly moves 1,9 in each position and then checking validity of the entire board
 * */

#endif
