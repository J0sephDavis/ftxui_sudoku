#include <vector>
#include <stack>
#include <iostream>
bool solvable(std::vector<std::vector<int>> board) {
	unsigned int column = 0;
	unsigned int row = 0;
	bool solution_found = false;
	struct position {
		unsigned int row;
		unsigned int column;
		unsigned int unit;
	};
	//consider more descriptive variable names for the vectors
	//current values in use by row
	std::vector<std::vector<int>> by_row;
	//current values in use by column
	std::vector<std::vector<int>> by_column;
	//current values in use by unit
	std::vector<std::vector<int>> by_unit;

	std::vector<position> replace_position;
	
	//indexing and identifying for replacement
	for (; row < board.size(); row++) {
		unsigned int unit = 0;
		if (row < 3) unit = 0;
		else if (row < 6) unit = 3;
		else unit = 6;
		for (; column < board[row].size(); column++) {
			if (column < 3) unit += 0;
			else if (column < 6) unit += 1;
			else unit += 2;

			if (board[row][column] == 0) //IF EMPTY POSITION
				replace_position.push_back(position({row,column,unit}));
			else { //IF NON-EMPTY, ADD TO RESTRICTIONS
				by_row[row].push_back(board[row][column]);
				by_column[column].push_back(board[row][column]);
			}
		} //END OF COLUMN UNIT
	}//END OF ROW LOOP
	std::cout << "VALUES IDENTIFIED FOR REPLACEMENT:\n";
	for (const position& value : replace_position) {
		for (
	}

	return solution_found;
}
