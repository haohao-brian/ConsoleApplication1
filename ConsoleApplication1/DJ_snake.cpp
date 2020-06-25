#include "DJ_snake.h"
#include <iostream>
#include <cstdlib>
#include "pch.h"
#include "curses.h"
#include <time.h>
#include <fstream>

DJ_snake::DJ_snake() {
	srand(clock());
	initscr();
	cbreak();						// Disable buffering, making input available to the program immediately
	timeout(500);					// Wait user's input for at most 1s
	keypad(stdscr, TRUE);			// Enable input from arrow keys
	curs_set(0); 					// Hide cursor
	getmaxyx(stdscr, maxY, maxX);	// Get max X and Y of the terminal
	//printw("maxX:%d  maxY:%d", maxX, maxY);

	clock_start = clock();
	// build a snake
	/*
	snake = new SingleNodeSnake;
	tail = snake;
	*/
}
void DJ_snake::start() {
	snake = new SingleNodeSnake;
	tail = snake;
	if (nowplaying == A) 
		mvaddch(snake->y, snake->x, 'A');
	else if (nowplaying == B) {
		snake->y = maxY-5;
		mvaddch(snake->y, snake->x, 'B');
	}
	//endwin();	// Restores the terminal after Curses activity
}
void DJ_snake::control(int d,SingleNodeSnake *snakeA, SingleNodeSnake* snakeB) {
	switch (d) {
	case KEY_UP:
		if (snakeA->dir != DOWN) snakeA->dir = UP;
		snakeA->speedup = true;
		snakeB->speedup = false;
		break;
	case KEY_DOWN:
		if (snakeA->dir != UP) snakeA->dir = DOWN;
		snakeA->speedup = true;
		snakeB->speedup = false;
		break;
	case KEY_LEFT:
		if (snakeA->dir != RIGHT) snakeA->dir = LEFT;
		snakeA->speedup = true;
		snakeB->speedup = false;
		break;
	case KEY_RIGHT:
		if (snakeA->dir != LEFT) snakeA->dir = RIGHT;
		snakeA->speedup = true;
		snakeB->speedup = false;
		break;
	case 'w':
		if (snakeB->dir != DOWN) snakeB->dir = UP;
		snakeB->speedup = true;
		snakeA->speedup = false;
		break;
	case 's':
		if (snakeB->dir != UP) snakeB->dir = DOWN;
		snakeB->speedup = true;
		snakeA->speedup = false;
		break;
	case 'a':
		if (snakeB->dir != RIGHT) snakeB->dir = LEFT;
		snakeB->speedup = true;
		snakeA->speedup = false;
		break;
	case 'd':
		if (snakeB->dir != LEFT) snakeB->dir = RIGHT;
		snakeB->speedup = true;
		snakeA->speedup = false;
		break;
	case 27: // ESC
	case 'q':
		terminated = true;
		break;
	default:
		snakeA->speedup = false;
		snakeB->speedup = false;
		break;
	}
}

void DJ_snake::insertAtHead() {
	SingleNodeSnake* temp = new SingleNodeSnake;
	temp->y = snake->y;
	temp->x = snake->x;
	temp->dir = snake->dir;

	switch (snake->dir) {
	case UP:
		temp->y = snake->y - 1;
		break;
	case DOWN:
		temp->y = snake->y + 1;
		break;
	case LEFT:
		temp->x = snake->x - 1;
		break;
	case RIGHT:
		temp->x = snake->x + 1;
		break;
	}

	if (nowplaying == A) {
		mvaddch(temp->y, temp->x, 'A');
	}
	else if (nowplaying == B) {
		mvaddch(temp->y, temp->x, 'B');
	}
	//mvaddch(temp->y, temp->x, '*');
	snake->next = temp;
	snake = temp;
}
bool DJ_snake::move(int& money_X, int& money_Y) {
	clock_stop = clock();
	duration = (double)(clock_stop - clock_start) / CLOCKS_PER_SEC;
	if (snake->speedup == false && duration < 0.5) return false;

	insertAtHead();
	// get money!!..... or not.
	if (snake->x == money_X && snake->y == money_Y) {
		length++;
		new_money(money_X, money_Y);
	}else removeAtTail();
	clock_start = clock();

	//gg because of smashed by enemy
	SingleNodeSnake* check = another->tail;
	while (check != NULL) {
		if (check->x == snake->x && check->y == snake->y) {
			mvaddch(1, maxX / 2 - 20, ' ');
			if (nowplaying == A)printw("player A is GAME OVER, player B WIN!!!");
			if (nowplaying == B)printw("player B is GAME OVER, player A WIN!!!");
			terminated = true;
			gg = true;
			break;
		}
		check = check->next;
	}
	//gg by hitting wall
	if (snake->x > maxX || snake->y > maxY || snake->x < 0 || snake->y < 0) {
		mvaddch(1, maxX / 2-20, ' ');
		if (nowplaying == A)printw("player A is GAME OVER, player B WIN!!!");
		if (nowplaying == B)printw("player B is GAME OVER, player A WIN!!!");
		terminated = true;
		gg = true;
	}
	return false;
}
void DJ_snake::new_money(int &X, int &Y) {
	do {
		X = rand() % maxX + 1;
		Y = rand() % maxY + 2;
	} while (!(X < maxX && Y < maxY));
	mvaddch(Y, X, '$');
}
void DJ_snake::removeAtTail() {
	SingleNodeSnake* temp = tail;

	SingleNodeSnake* check = tail;
	bool exist = false;
	while (check->next!=NULL) {
		check = check->next;
		if (check->x == tail->x && check->y == tail->y) {
			exist = true;
			break;
		}
	}
	if (exist==false) mvaddch(tail->y, tail->x, ' ');

	tail = tail->next;
	delete temp;
}
