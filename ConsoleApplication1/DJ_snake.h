#pragma once
#include <time.h>
#include <fstream>
#include <iostream>

enum Direction {UP, DOWN, LEFT, RIGHT};
enum Player {A, B};
struct SingleNodeSnake {
	int x = 1;
	int y = 1;
	Direction dir = RIGHT;
	SingleNodeSnake* next;
	bool speedup = false;
};
class DJ_snake
{
public:
	DJ_snake *another;

	double duration;

	Player nowplaying;
	bool terminated = false, gg = false;
	int c, maxX, maxY,length = 1;

	SingleNodeSnake* snake = new SingleNodeSnake;
	SingleNodeSnake* tail = snake;

	DJ_snake();
	bool move(int &, int &);
	void new_money(int &, int &);
	void insertAtHead();
	void removeAtTail();
	void control(int,SingleNodeSnake * ,SingleNodeSnake *);
	void start();
private:
	bool got_m = false;
	clock_t clock_start, clock_stop;
};

