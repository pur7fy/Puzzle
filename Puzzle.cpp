#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
using namespace bangtal;

#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <stdio.h>
#include <time.h>

ScenePtr scene;
ObjectPtr game_board[16], game_original[16];
ObjectPtr start;

TimerPtr timer1;
float animationTime = 0.05f;
int mixCount = 100;

int blank;

int game_index(ObjectPtr piece) {
	for (int i = 0; i < 16; i++)
		if (game_board[i] == piece) return i;
	return -1;
}

int index_to_x(int index) {
	return 300 + 150 * (index % 4);
}

int index_to_y(int index) {
	return 529 - 150 * (index / 4);
}

void game_move(int index) {
	auto piece = game_board[index];
	game_board[index] = game_board[blank];
	game_board[index]->locate(scene, index_to_x(index), index_to_y(index));
	game_board[blank] = piece;
	game_board[blank]->locate(scene, index_to_x(blank), index_to_y(blank));
	blank = index;
}

bool check_move(int index) {
	if (blank % 4 > 0 && index == blank - 1) return true;
	if (blank % 4 < 3 && index == blank + 1) return true;
	if (blank / 4 > 0 && index == blank - 4) return true;
	if (blank / 4 < 3 && index == blank + 4) return true;

	return false;
}

int random_move() {
	int index = rand() % 16;
	while (!check_move(index)) {
		index = rand() % 16;
	}
	return index;
}

void start_game() {
	mixCount = 100;

	timer1->set(animationTime);
	timer1->start();

	blank = 15;
	game_board[blank]->hide();
	start->hide();

}

bool check_end() {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] != game_original[i]) return false;
	}
	return true;
}

void end_game() {

	game_board[blank]->show();
	start->show();
	showMessage("Completed!!");


}

void init_game() {
	scene = Scene::create("곰돌이 푸 퍼즐", "Images/배경.png");

	char path[20];  //[16]이 아닌 [20]인 이유가 궁금합니다.
	for (int i = 0; i < 16; i++) {
		sprintf(path, "Images/%d.png", i + 1);
		game_board[i] = Object::create(path, scene, index_to_x(i), index_to_y(i));
		game_board[i]->setOnMouseCallback([&](auto piece, auto x, auto y, auto action)->bool {

			int index = game_index(piece);
			if (check_move(index)) {
				game_move(index);

				if (check_end()) {
					end_game();
				}
			}


			return true;
			});
		game_original[i] = game_board[i];
	}

	start = Object::create("Images/start.png", scene, 590, 159);
	start->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
		start_game();
		return true;
		});

	timer1 = Timer::create(animationTime);
	timer1->setOnTimerCallback([&](auto)->bool {

		game_move(random_move());

		mixCount--;
		if (mixCount > 0) {
			timer1->set(animationTime);
			timer1->start();
		}

		return true;
		});


	startGame(scene);
}

int main() {
	srand((unsigned int)time(NULL));

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	init_game();

	return 0;
}