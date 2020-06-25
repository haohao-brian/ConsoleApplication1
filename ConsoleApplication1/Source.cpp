#include "DJ_snake.h"
#include <iostream>
#include <cstdlib>
#include "pch.h"
#include "curses.h"
#include <time.h>
#include <fstream>

using namespace std;

int dollarX, dollarY;
void read_build_snake(fstream & , DJ_snake &, char);

int main() {
	// create or read a file
	fstream recorde("recorde.txt", ios::in);
	if (!recorde) {
		cerr << "File could not be open" << endl;
		exit(1);
	}
	
	initscr();
	cbreak();						// Disable buffering, making input available to the program immediately
	timeout(500);					// Wait user's input for at most 1s
	keypad(stdscr, TRUE);			// Enable input from arrow keys
	curs_set(0); 					// Hide cursor

	DJ_snake snakeA;
	mvaddch(0, 0, ' ');
	printw("maxX:%d  maxY:%d", snakeA.maxX, snakeA.maxY);
	snakeA.nowplaying = A;
	DJ_snake snakeB;
	snakeB.nowplaying = B;

	// are they gg?
	int did_they_gg;
	recorde >> did_they_gg;

	// load the recorded money !! Yeah!!!!
	recorde >> dollarX >> dollarY;
	if (dollarX == NULL || dollarY == NULL) {
		dollarX = 50;
		dollarY = snakeA.maxY / 2 - 10;
	}
	mvaddch(dollarY, dollarX, '$');
	
	//load the recorded snakes' length
	recorde >> snakeA.length >> snakeB.length;

	// okay, let's quickly build the snake
	read_build_snake(recorde, snakeA, 'A');
	read_build_snake(recorde, snakeB, 'B');

	// let's make friends yeah!!!!
	snakeB.another = &snakeA;
	snakeA.another = &snakeB;

	if (did_they_gg == 1) {
		snakeA.gg = true;
		snakeA.terminated = true;
		mvaddch(1, snakeA.maxX / 2 - 20, ' ');
		printw("player A is GAME OVER, player B WIN!!!");
	}else if (did_they_gg == 2) {
		snakeB.gg = true;
		snakeB.terminated = true;
		mvaddch(1, snakeA.maxX / 2 - 20, ' ');
		printw("player B is GAME OVER, player A WIN!!!");
	}

	// game started!!! let's go!
	while ((!snakeA.terminated) && (!snakeB.terminated)) {
		int input = getch(); //500ms
		snakeA.control(input, snakeA.snake, snakeB.snake);

		bool did_A_move = false,did_B_move = false;
		did_A_move = snakeA.move(dollarX,dollarY);
		did_B_move = snakeB.move(dollarX,dollarY);

		mvaddch(0, 20,' ');
		printw("duration:%.3f", snakeB.duration);
		if (snakeB.length >= 3) printw("                                                              ");
		else printw("      Player B please use 'a','w',''s','d' to move the snake!!");
	}
	recorde.clear();
	recorde.close();
	
	// game end and recording
	ofstream output("recorde.txt", ios::out);
	if (!output) {
		cerr << "File could not be open" << endl;
		exit(1);
	}
	if (snakeA.gg == true) {
		output << 1 << endl;
	}else if (snakeB.gg == true) {
		output << 2 << endl;
	}else {
		output << 0 << endl;
	}
	output << dollarX << " " << dollarY << endl;
	output << snakeA.length << " " << snakeB.length << endl;
	while (snakeA.tail != NULL) {
		output << snakeA.tail->x << " " << snakeA.tail->y << " "
			<< static_cast<int>(snakeA.tail->dir)	<< " " << 'A' << endl;
		snakeA.tail = snakeA.tail->next;
	}
	while (snakeB.tail != NULL) {
		output << snakeB.tail->x << " " << snakeB.tail->y << " "
			<< static_cast<int>(snakeB.tail->dir)	<< " " << 'B' << endl;
		snakeB.tail = snakeB.tail->next;
	}
	mvaddch(snakeA.maxY/2, snakeA.maxX/2-8, ' ');
	printw("recorde is done!");

	// gg and system await
	mvaddch(snakeA.maxY / 2-1, snakeA.maxX / 2 - 8, ' ');
	printw("press b to start a new game!");
	while (true) {
		mvaddch(snakeA.maxY / 2 - 1, snakeA.maxX / 2 - 25, ' ');
		
		int input = getch(); //500ms
		
		if (input == 'b') {
			mvaddch(snakeA.maxY / 2 - 1, snakeA.maxX / 2 - 25, ' ');
			printw("Recorde Cleaned! Close and Open the game to restart please.");
			mvaddch(snakeA.maxY / 2, snakeA.maxX / 2 - 8, ' ');
			printw("                ");
			fstream del("recorde.txt", ios::out);
			if (!del) {
				cerr << "File could not be open" << endl;
				exit(1);
			}
			del.clear();
			del.close();
		}
		
	}
	//endwin();
	//return 0;
}
void read_build_snake(fstream& read, DJ_snake& DJ,char player) {
	int x, y, d, i = 0;
	char played = NULL;
	read >> x >> y >> d >> played;
	if (played == player) {
		DJ.snake = new SingleNodeSnake;
		DJ.snake->x = x;
		DJ.snake->y = y;
		DJ.snake->dir = static_cast<Direction>(d);
		DJ.snake->next = NULL;
		mvaddch(y, x, player);
		DJ.tail = DJ.snake;
		i++;
	}
	while (read >> x >> y >> d  >> played) {
		if (player == played) {
			SingleNodeSnake *temp = new SingleNodeSnake;
			temp->x = x;
			temp->y = y;
			temp->dir = (Direction)d;
			temp->next = NULL;
			DJ.snake->next = temp; // follow my lead
			DJ.snake = temp;
			i++;
			mvaddch(y, x, player);
		}
		if (i >= DJ.length) {
			break;
		}
	}
	/*
	read.clear();
	read.seekg(0);
	*/
	if (played == NULL) {
		DJ.snake = new SingleNodeSnake;
		DJ.tail = DJ.snake;
		if (player == 'A') {
			mvaddch(DJ.snake->y, DJ.snake->x, player);
		}else if (player == 'B') {
			DJ.snake->y = DJ.maxY - 5;
			mvaddch(DJ.snake->y, DJ.snake->x, player);
		}
	}
}