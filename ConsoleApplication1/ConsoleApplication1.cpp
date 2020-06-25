#include "pch.h"
#include <iostream>
#include <cstdlib>
#include "curses.h"

using namespace std;

bool got_m = false;

enum Direction {
	UP, DOWN, LEFT, RIGHT
};
struct SingleNodeSnake {
	int x = 1;
	int y = 1;
	Direction dir = RIGHT;
	SingleNodeSnake* next = NULL;
};

SingleNodeSnake* snake,* tail;
static int moneyX, moneyY, maxX, maxY;
bool terminated = false;

void new_money(int ,int);
void insertAtHead();
void removeAtTail();

void move() {
	insertAtHead();
	
	if (snake->x==moneyX && snake->y==moneyY) new_money(0,0);
	else removeAtTail();

	if (snake->x > maxX || snake->y > maxY || 
		snake->y < 1 || snake->x < 0) {
		cout << "player A is GG";
		terminated = true;
	}
}
/*
int main() {
	srand(0);
	static int c;

	initscr();
	cbreak();						// Disable buffering, making input available to the program immediately
	timeout(500);					// Wait user's input for at most 1s
	keypad(stdscr, TRUE);			// Enable input from arrow keys
	curs_set(0); 					// Hide cursor
	getmaxyx(stdscr, maxY, maxX);	// Get max X and Y of the terminal
	printw("maxX:%d  maxY:%d", maxX, maxY);

	// Draw the single headed snake
	snake = new SingleNodeSnake;
	tail = snake;
	mvaddch(snake->y, snake->x, '*');

	new_money(maxX,maxY);

	while (!terminated) {
		c = getch();

		switch (c) {
		case KEY_UP:
			if (snake->dir != DOWN) snake->dir = UP;
			break;
		case KEY_DOWN:
			if (snake->dir != UP) snake->dir = DOWN;
			break;
		case KEY_LEFT:
			if (snake->dir != RIGHT) snake->dir = LEFT;
			break;
		case KEY_RIGHT:
			if (snake->dir != LEFT) snake->dir = RIGHT;
			break;
		case 27: // ESC
		case 'q':
			terminated = true;
			break;
		default:
			break;
		}
		move();
	}
	endwin();	// Restores the terminal after Curses activity
}
*/
void new_money(int X,int Y) {
	static int maxX = 0, maxY = 0;
	if (maxX == 0 && maxY == 0) {
		maxX = X;
		maxY = Y;
	}
	moneyX = rand() % maxX + 1;
	moneyY = rand() % maxY + 2;
	mvaddch(moneyY, moneyX, '$');
}
void removeAtTail() {
	SingleNodeSnake* temp = tail;
	mvaddch(tail->y, tail->x, ' ');
	tail = tail->next;
	delete temp;
}
void insertAtHead() {
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
	mvaddch(temp->y, temp->x, '*');
	snake->next = temp;
	snake = temp;
}