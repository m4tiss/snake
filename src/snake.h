#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "screen.h"
#include "cpoint.h"
#include <vector>

class CSnake:public CFramedWindow
{
private:
	int menu = 1;
	int level = 1;
	int init = 0;
	int pause = 1;
	int record = 0;
	int alive = 1;
	int counter = 0;
	int limit = 15;
	int make_move = 0;
	int clicked_key = KEY_RIGHT;
	CPoint food;
	vector <CPoint> body;
	
public:
  CSnake(CRect r, char _c = ' ');
	void paint();
	void draw_menu();
	void paint_snake();
	void init_snake();
	void new_food();
	void init_food();
	void draw_food();
	void draw_end();
	void level_check();
	bool move();
	bool handleEvent(int key);
	bool eating();
};

#endif
