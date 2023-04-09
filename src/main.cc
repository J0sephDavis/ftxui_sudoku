//#include <iostream>
//#include <vector>
#include "solver.cc"
#include <cstdlib>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <string>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

using namespace ftxui;

class sudoku : public sudoku_board {
	public:
		sudoku(std::vector<std::vector<int>> _board, std::function<void()> _quitFunc)
			: quitFunc(std::move(_quitFunc)),
			sudoku_board(std::move(_board))
				{ make_board(); };
		//creates the board component
		void make_board() {
			Components board_inner;
			for (int _row = 0; _row < 9; _row++) {
				Components row_builder = {};
				for (int _col = 0; _col < 9; _col++) {
					//maybe use the replacement_mask to determine if a container entry ought to even be made for a specific value.
					//Instead just making the non-replaceable entries into regular text() that cannot be hovered.
					int* cell_val = &board_values[_row].at(_col);
					row_builder.push_back(Renderer([&, cell_val](bool focus) {
							auto inner = text(std::to_string(*cell_val));
							if (focus) inner |= bgcolor(Color::DarkMagenta);
							return inner;
						})
						| CatchEvent([&, _row, _col](Event event) {
							//if the value is outside our accepted ASCII range. return false
							if (!(event.character().at(0) <= '9' and event.character().at(0) > '0')) return false;
							//set n to the integer represented by the char
							int n = std::stoi(event.character());
							//update the selected cell
							update_cell(_row,_col,n);
							if (done()) quitFunc();
							return true;
						})
					);
					if ((_col+1)%3 == 0 and _col < 8) row_builder.push_back(Renderer([&]{return separator();}));
				}
				board_inner.push_back(Container::Horizontal(row_builder));
				if ((_row+1)%3 == 0 and _row < 8) board_inner.push_back(Renderer([&]{return separator();}));
			}
			//Component that will show validation errors
			//sets the board component
			this->board_component = Container::Horizontal({
				Container::Vertical(std::move(board_inner))
			});
		};
	private:
		bool done() {return is_valid();};
		//
		const std::function<void()> quitFunc;
	public:
		Component board_component;
};
int main(int argc, char** argv) {
	std::vector<std::vector<int>> values = {
		{0,6,8,1,3,5,2,4,7}, //{9,6,8,1,3,5,2,4,7},
		{1,3,7,8,4,2,9,5,6}, //{1,3,7,8,4,2,9,5,6},
		{4,2,5,9,6,7,3,8,1}, //{4,2,5,9,6,7,3,8,1},
		{7,8,2,6,1,3,4,9,5}, //{7,8,2,6,1,3,4,9,5},
		{3,0,4,5,9,0,7,6,2}, //{3,1,4,5,9,8,7,6,2},
		{5,9,6,2,7,4,8,1,3}, //{5,9,6,2,7,4,8,1,3},
		{8,7,9,3,5,1,6,2,4}, //{8,7,9,3,5,1,6,2,4},
		{6,4,1,7,2,9,5,3,8}, //{6,4,1,7,2,9,5,3,8},
		{2,5,3,4,8,6,1,7,9}  //{2,5,3,4,8,6,1,7,9} 
	};
	auto SCREEN = ScreenInteractive::FitComponent();
	std::function<void()> quitFunc = SCREEN.ExitLoopClosure();
	auto game = sudoku(std::move(values),std::move(quitFunc));
	SCREEN.Loop(game.board_component | border);
	return EXIT_SUCCESS;
}
