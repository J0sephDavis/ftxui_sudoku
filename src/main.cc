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
			solved_col(0);
			solved_col(1);
			solved_col(2);
			solved_col(3);
			solved_col(4);
			solved_col(5);
			solved_col(6);
			solved_col(7);
			solved_col(8);
			
			solved_row(0);
			solved_row(1);
			solved_row(2);
			solved_row(3);
			solved_row(4);
			solved_row(5);
			solved_row(6);
			solved_row(7);
			solved_row(8);
			
			solved_blok(0);
			solved_blok(1);
			solved_blok(2);
			solved_blok(3);
			solved_blok(4);
			solved_blok(5);
			solved_blok(6);
			solved_blok(7);
			solved_blok(8);
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
//class sudoku {
//	public:
//		sudoku(std::vector<std::vector<int>> state)
//			: _state(std::move(state)){
//				std::cout << "\n_state: {";
//				for (auto& row : _state) {
//					std::cout << "\n{";
//					for (auto& val : row) {
//						std::cout << std::to_string(val) << ",";
//					}
//					std::cout << "},";
//				}
//				std::cout << "}\n";
//
//			};
//		std::vector<std::vector<int>> _state;
//		Component cell(const int x, const int y) {
//			std::cout << "cell(" << x << "," << y <<") | val:";
//			int *val = &_state[y][x];
//			std::cout << *val << "\n";
//			//component class
//			class base : public ComponentBase {
//				public:
//					base(int* value) : val(std::move(value)) {};
//					Element Render() override {
//						const bool active = Active();
//						const bool focused = Focused();
//						if (focused) return text(std::to_string(*val)) | bold;
//						else return text(std::to_string(*val));
//					}
//					bool Focusable() const final { return true; }
//				private:
//					int *val;
//			};
//			//return
//			return Make<base>(std::move(val));
//		};
//		Component blok() {
//			//contains the cells for the blok
//			std::vector<Components> comp;
//			//for each cell, by row then column (y then x)
//			for (std::size_t y = 0; y < 9; y++) {
//				Components tmp_row;
//				for (std::size_t x = 0; x < 9; x++) {
//					//Check is move(...) is needed or effective here.
//					//It may be doing nothing since it is passed as the output of the function
//					tmp_row.emplace_back(std::move(cell(x,y)));
//				}
//				//add each row of cells to the component
//				comp.emplace_back(tmp_row);
//			}
//			//the actual component for each section
//			class base : public ComponentBase {
//				public:
//					base(std::vector<Components> row, std::vector<std::vector<int>> state)
//						: _row(std::move(row)),
//						  _state(std::move(state))
//				{
//					size_t tmp_x = 0;
//					size_t tmp_y = 0;
//					for (auto& row : _state) {
//						for (auto& val : row) {
//							_lockedCells[tmp_y][tmp_x++] = val>0;
//						}
//						tmp_x = 0;
//						tmp_y++;
//					}
//				}
//				private:
//					Element Render() override {
//						Elements OUTER;
//						for (size_t a = 0; a < 9; a++) {
//							Elements INNER;
//							if (a%3 == 0) OUTER.emplace_back(separator() | automerge);
//							for (int b = 0; b < 9; b++) {
//								if (b%3 == 0) INNER.emplace_back(separator() | automerge);
//								const bool selected = (_hPos == b && _vPos == a);
//								INNER.emplace_back(std::move(_row[a][b]->Render())
//										| bgcolor(selected?Color::Magenta:Color::Black));
//							}
//							INNER.emplace_back(separator() | automerge);
//							OUTER.emplace_back(hbox(std::move(INNER)));
//							OUTER.emplace_back(separator() | automerge);
//						}
//						return vbox(std::move(OUTER));
//					}
//					size_t bound(int val) {
//						return std::max(lowerLim,std::min(val,upperLim));
//					}
//					bool right() {
//						if (_hPos == upperLim) return false;
//						_hPos = bound(_hPos+1);
//						return true;
//					}
//					bool left() {
//						if (_hPos == lowerLim) return false;
//						else _hPos = bound(_hPos-1);
//						return true;
//					}
//					bool up() {
//						if (_vPos == lowerLim) return false;
//						_vPos = bound(_vPos-1);
//						return true;
//					}
//					bool down() {
//						if (_vPos == upperLim) return false;
//						_vPos = bound(_vPos+1);
//						return true;
//					}
//					bool characterHandler(Event event) {
//						std::cout << "characterHandler(...): ";
//						int *val = &_state[_vPos][_hPos];
//						char press = event.character().at(0);
//						std:: cout << "(*val: " << *val << ")" << "(press: " << press << ")";
//						//val = the difference between the ascii value of the character
//						//and the ascii value of '0'.
//						//Meaning that '9'-'0' = 9; '4' - '0' = 4, etc.
//						if (press >= '1' && press <= '9')
//							*val = (int)(press - '0');
//						else return false;
//						return true;
//						
//					}
//					bool OnEvent(Event event) override {
//						if (event.is_character()) {
////							if (_lockedCells[_vPos][_hPos] == false)
//								characterHandler(event);
//						}
//						else {
//							if (event == Event::ArrowRight) return right();
//							else if (event == Event::ArrowLeft) return left();
//							else if (event == Event::ArrowUp) return up();
//							else if (event == Event::ArrowDown) return down();
//							else return false;
//						}
//						return true;
//					}
//
//				private:
//					std::vector<Components> _row;
//					std::vector<std::vector<int>> _state;
//					std::vector<std::vector<bool>> _lockedCells;
//					int _hPos = bound(5);
//					int _vPos = bound(5);
//					const int lowerLim = 0;
//					const int upperLim = 8;
//			};
//			return Make<base>(std::move(comp), std::move(_state));
//		}
//};
/*class sudoku : public ComponentBase {
	public:
		sudoku(std::vector<std::vector<int>> state)
			: _state(std::move(state)) {}
		Component cell(const int* value) {
			class base : public ComponentBase {
				public:
					base(const int* value, const bool lock)
						: numRef(std::move(value)),
						IsLocked(std::move(lock)){}
				private:
					const bool IsLocked;
					const int* numRef;
				public:
					Element Render() override {
						//
					}

			};
			const bool _lock = value == 0;
			return Make<base>(std::move(value), _lock);
		}
	private:
		//contains the board values
		std::vector<std::vector<int>> _state;
};*/
