#include <cstdlib>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
#include <iostream>
#include <ftxui/component/screen_interactive.hpp>
#include <string>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <vector>
using namespace ftxui;

class sudoku {
	public:
		sudoku(std::function<void()> _quitFunc) : quitFunc(std::move(_quitFunc)) { make_board(); };
		void make_board() {
			Components board_inner;
			int count_row = 0;
			for (auto& row : values) {
				Components tmpRow = {};
				int count_col = 0;
				//for every value in the row, create a focusable component and add it to the constructor(tmp) row
				for (auto& a : row) {
					tmpRow.push_back(Renderer([&](bool focus){
						//get the value in the row as a string
						auto inner = text(std::to_string(a));
						//if focused, display the text with a specific baground color
						if (focus) return inner | bgcolor(Color::DarkMagenta);
						//if not focused, display the text with no specified bgdr color
						else return inner;
						})
						//event wrapper to update the value of the specific entry
						| CatchEvent([&](Event event){
							//if the input character is between the character values of 0 & 9 in ascii (meaning its a text digit)
							if (event.character().at(0) <= '9' && event.character().at(0) > '0') {
								//update the value in the vector to the input character
								values[y][x] = std::stoi(event.character());
								//update row&col of changed value
								solved_row(y);
								solved_col(x);
								//calc blok num and solve
								int blok_col = x/3;
								int blok_row = y/3;
								solved_blok(blok_row*3 + blok_col);
								//quit if complete
								if (done()) quitFunc();
								return true;
							}
							//does return false prevent other function calls? Is it necessary? Whats the hierarchy
							else return false;
						}));
					if (++count_col%3 == 0 and count_col <8) tmpRow.push_back(Renderer([&]{return separator();}));
				}
				//add the cosntructed row to the board, as well as passing the selector for column position by ref
				board_inner.push_back(Container::Horizontal(tmpRow, &x));
				if (++count_row%3 == 0 and count_row<8) board_inner.push_back(Renderer([&]{return separator();}));
			}
			Component analyzer = Renderer([&]{
				return flexbox({
					vbox({
						text("BLOCKS") | bold,
						hbox({
							text(std::to_string(b_scores[0])),
							separatorLight(),
							text(std::to_string(b_scores[1])),
							separatorLight(),
							text(std::to_string(b_scores[2])),
						}),
						separatorLight(),
						hbox({
							text(std::to_string(b_scores[3])),
							separatorLight(),
							text(std::to_string(b_scores[4])),
							separatorLight(),
							text(std::to_string(b_scores[5])),
						}),
						separatorLight(),
						hbox({
							text(std::to_string(b_scores[6])),
							separatorLight(),
							text(std::to_string(b_scores[7])),
							separatorLight(),
							text(std::to_string(b_scores[8])),
						}),
					}) | flex_shrink | borderRounded,
					vbox({
						text("ROWS") | bold,
						text("1:" + std::to_string(r_scores[0])),
						text("2:" + std::to_string(r_scores[1])),
						text("3:" + std::to_string(r_scores[2])),
						text("4:" + std::to_string(r_scores[3])),
						text("5:" + std::to_string(r_scores[4])),
						text("6:" + std::to_string(r_scores[5])),
						text("7:" + std::to_string(r_scores[6])),
						text("8:" + std::to_string(r_scores[7])),
						text("9:" + std::to_string(r_scores[8])),
					}) | borderRounded,
					vbox({
						text("COLUMNS") | bold,
						text("1:" + std::to_string(c_scores[0])),
						text("2:" + std::to_string(c_scores[1])),
						text("3:" + std::to_string(c_scores[2])),
						text("4:" + std::to_string(c_scores[3])),
						text("5:" + std::to_string(c_scores[4])),
						text("6:" + std::to_string(c_scores[5])),
						text("7:" + std::to_string(c_scores[6])),
						text("8:" + std::to_string(c_scores[7])),
						text("9:" + std::to_string(c_scores[8])),
					}) | borderRounded,
				});
			});
			//sets the board component
			this->board = Container::Horizontal({
				Container::Vertical(std::move(board_inner),&y) | border,
				std::move(analyzer)
			});
			//initializes the values.. probably could've just used a for loop
			for (auto iterator = 0; iterator <=8; iterator++) {
				solved_col(iterator);
				solved_row(iterator);
				solved_blok(iterator);
			}
		};
	private:
		//checks if the passed vector contains each digit
		bool checkNine(std::vector<int> values) {
			//vector is assumed to contain only 9 integers
			bool var_1 = false;
			bool var_2 = false;
			bool var_3 = false;
			bool var_4 = false;
			bool var_5 = false;
			bool var_6 = false;
			bool var_7 = false;
			bool var_8 = false;
			bool var_9 = false;
			//
			for (auto& a : values) {
				switch(a) {
					case 1: var_1 = true; break;
					case 2: var_2 = true; break;
					case 3: var_3 = true; break;
					case 4: var_4 = true; break;
					case 5: var_5 = true; break;
					case 6: var_6 = true; break;
					case 7: var_7 = true; break;
					case 8: var_8 = true; break;
					case 9: var_9 = true; break;
					default: break;
				}
			}
			//return the truth table
			return (var_1 and var_2 and var_3 and var_4 and var_5 and var_6 and var_7 and var_8 and var_9);
		}
		bool solved_col(int col) {
			const std::vector<int> col_values = {
				values[0][col],
				values[1][col],
				values[2][col],
				values[3][col],
				values[4][col],
				values[5][col],
				values[6][col],
				values[7][col],
				values[8][col],
			};
			c_scores[col] = checkNine(std::move(col_values));
			return c_scores[col];
		}
		bool solved_row(int row) {
			const std::vector<int> row_values = {
				values[row][0],
				values[row][1],
				values[row][2],
				values[row][3],
				values[row][4],
				values[row][5],
				values[row][6],
				values[row][7],
				values[row][8],
			};
			r_scores[row] = checkNine(std::move(row_values));
			return r_scores[row];
		}
		bool solved_blok(int b) {
			uint8_t off_x;
			uint8_t off_y;
			switch(b) {
				case 0: off_x = 0; off_y = 0; break;
				case 1: off_x = 3; off_y = 0; break;
				case 2: off_x = 6; off_y = 0; break;
				case 3: off_x = 0; off_y = 3; break;
				case 4: off_x = 3; off_y = 3; break;
				case 5: off_x = 6; off_y = 3; break;
				case 6: off_x = 0; off_y = 6; break;
				case 7: off_x = 3; off_y = 6; break;
				case 8: off_x = 6; off_y = 6; break;
				default: std::cout << "DEFAULT\n"; return false;
			}
			//
			const std::vector<int> blok_vals = {
				values[0 + off_y][0 + off_x],
				values[0 + off_y][1 + off_x],
				values[0 + off_y][2 + off_x],
				values[1 + off_y][0 + off_x],
				values[1 + off_y][1 + off_x],
				values[1 + off_y][2 + off_x],
				values[2 + off_y][0 + off_x],
				values[2 + off_y][1 + off_x],
				values[2 + off_y][2 + off_x]
			};
			b_scores[b] =  checkNine(std::move(blok_vals));
			return b_scores[b];
		}
		bool done() {
			bool rows_scored = r_scores[0] + 
				r_scores[1] and 
				r_scores[2] and 
				r_scores[3] and 
				r_scores[4] and 
				r_scores[5] and 
				r_scores[6] and 
				r_scores[7] and 
				r_scores[8]; 
			bool cols_scored = c_scores[0] and 
				c_scores[1] and 
				c_scores[2] and 
				c_scores[3] and 
				c_scores[4] and 
				c_scores[5] and 
				c_scores[6] and 
				c_scores[7] and 
				c_scores[8]; 
			return rows_scored and cols_scored;
		};
		std::vector<std::vector<int>> values = {
			{0,6,8,1,3,5,2,4,7}, //{9,6,8,1,3,5,2,4,7},
			{1,3,7,8,4,2,9,5,6}, //{1,3,7,8,4,2,9,5,6},
			{4,2,5,9,6,7,3,8,1}, //{4,2,5,9,6,7,3,8,1},
			{7,8,2,6,1,3,4,9,5}, //{7,8,2,6,1,3,4,9,5},
			{3,1,4,5,9,8,7,6,2}, //{3,1,4,5,9,8,7,6,2},
			{5,9,6,2,7,4,8,1,3}, //{5,9,6,2,7,4,8,1,3},
			{8,7,9,3,5,1,6,2,4}, //{8,7,9,3,5,1,6,2,4},
			{6,4,1,7,2,9,5,3,8}, //{6,4,1,7,2,9,5,3,8},
			{2,5,3,4,8,6,1,7,9}  //{2,5,3,4,8,6,1,7,9} 
		};
		//column scores, if Column,Row,Block contains all numbers from [1,9] they are marked true, else false.
		std::vector<bool> c_scores = {false,false,false,false,false,false,false,false,false};
		std::vector<bool> r_scores = {false,false,false,false,false,false,false,false,false};
		std::vector<bool> b_scores = {false,false,false,false,false,false,false,false,false};
		//the x,y position of the cursor/focus in the board
		int x = 0;
		int y = 0;
		//
		const std::function<void()> quitFunc;
	public:
		Component board;
};
int main(int argc, char** argv) {
	auto SCREEN = ScreenInteractive::FitComponent();
	std::function<void()> quitFunc = SCREEN.ExitLoopClosure();
	auto game = sudoku(std::move(quitFunc));
	SCREEN.Loop(game.board | border);
	return EXIT_SUCCESS;
}
