#include "snake.h"
#include "cpoint.h"
#include "screen.h"
#include "winsys.h"
#include <time.h>

CSnake::CSnake(CRect r, char _c /*=' '*/) : CFramedWindow(r, _c) {
  srand(time(NULL));
}
void CSnake::draw_menu() {
  int x = geom.topleft.x;
  int y = geom.topleft.y;
  gotoyx(y + 4, x + 4);
  printl("h - toggle help information");
  gotoyx(y + 5, x + 4);
  printl("p - toggle pause/play mode");
  gotoyx(y + 6, x + 4);
  printl("r - restart game");
  gotoyx(y + 7, x + 4);
  printl("arrows - move snake (in play mode) or");
  gotoyx(y + 8, x + 13);
  printl("move window (in pause mode)");
}
void CSnake::init_snake() {
  body.push_back(CPoint(4, 2));
  body.push_back(CPoint(3, 2));
  body.push_back(CPoint(2, 2));
}
void CSnake::init_food() {
  food.x = 20;
  food.y = 10;
}
void CSnake::draw_food() {
  int x = geom.topleft.x;
  int y = geom.topleft.y;
  gotoyx(y + food.y, x + food.x);
  printc('O');
}
void CSnake::new_food() {
  int x, y;
  int check = 0;
  while (1) {
    x = rand() % (geom.size.x - 2) + 1;
    y = rand() % (geom.size.y - 2) + 1;
    for (int i = 0; i < (int)body.size(); i++) {
      if (body[i].x == x && body[i].y == y) {
        check = 1;
        break;
      }
    }
    if (check == 0) {
      food.x = x;
      food.y = y;
      break;
    }
    check = 0;
  }
}
void CSnake::draw_end() {
  gotoyx(geom.topleft.y + 1, geom.topleft.x + 1);
  printl("GAME OVER, result: %d", record);
}
bool CSnake::eating() {
  if (food.x == body[0].x && food.y == body[0].y) {
    record++;
    new_food();
    return true;
  }
  return false;
}
void CSnake::paint_snake() {
  int x = geom.topleft.x;
  int y = geom.topleft.y;
  for (int i = 1; i < (int)body.size(); i++) {
    gotoyx(body[i].y + y, body[i].x + x);
    printc('+');
  }
  gotoyx(body[0].y + y, body[0].x + x);
  printc('*');
}
bool CSnake::move() {
  if(make_move == 0)return true;
  if (pause == 1)
    return true;
  CPoint last = body[body.size() - 1];
  for (int i = (int)body.size() - 1; i > 0; i--) {
    body[i] = body[i - 1];
  }
  if (clicked_key == KEY_UP) {
    body[0] += CPoint(0, -1);
  } else if (clicked_key == KEY_DOWN) {
    body[0] += CPoint(0, 1);
  } else if (clicked_key == KEY_LEFT) {
    body[0] += CPoint(-1, 0);
  } else if (clicked_key == KEY_RIGHT) {
    body[0] += CPoint(1, 0);
  }
  for (int i = 1; i < (int)body.size(); i++) {
    if (body[0].x == body[i].x && body[0].y == body[i].y) {
      alive = 0;
      pause = 1;
      return false;
    }
  }
  if (body[0].x == 0) {
    body[0].x = geom.size.x - 2;
  } else if (body[0].x == geom.size.x - 1) {
    body[0].x = 1;
  } else if (body[0].y == 0) {
    body[0].y = geom.size.y - 2;
  } else if (body[0].y == geom.size.y - 1) {
    body[0].y = 1;
  }
  if (eating() == true) {
    body.push_back(last);
  }
  return true;
}
void CSnake::level_check() {
  if (level == 14)
    return;
  level = (record / 3) + 1;
}
bool CSnake::handleEvent(int key) {
	if(key == ERR){
		make_move = 0;
		counter++;
		if(counter == limit-level){
			counter = 0;
			make_move = 1;
			}
			return true;
		}
  make_move = 1;
  if (key == 'h') {
    if (alive == 0) {
      if (menu == 1)
        menu = 0;
      else
        menu = 1;
      return true;
    }
    if (pause == 1)
      pause = 0;
    else
      pause = 1;
    if (menu == 1)
      menu = 0;
    else
      menu = 1;
    alive = 1;

    return true;
  }
  if (key == 'p') {
    if (pause == 0)
      pause = 1;
    else
      pause = 0;
    return true;
  }
  if (key == 'r') {
    record = 0;
    menu = 1;
    init = 0;
    level = 1;
    pause = 1;
    alive = 1;
    body.clear();
    clicked_key = KEY_RIGHT;
    return true;
  }
  if (key == '\t') {
    pause = 1;
  }
  if (pause == 1)
    return CFramedWindow::handleEvent(key);
  if (pause == 0) {
    if (key == KEY_RIGHT || key == KEY_UP || key == KEY_DOWN ||
        key == KEY_LEFT) {
      if ((key == KEY_UP && clicked_key != KEY_DOWN) ||
          (key == KEY_DOWN && clicked_key != KEY_UP) ||
          (key == KEY_RIGHT && clicked_key != KEY_LEFT) ||
          (key == KEY_LEFT && clicked_key != KEY_RIGHT)) {
        clicked_key = key;
        return true;
      }
      return false;
    }
  }
  return true;
}
void CSnake::paint() {
  CFramedWindow::paint();
  if (menu == 1) {
    if (init == 0) {
      init_snake();
      init_food();
      init = 1;
    }
    draw_menu();
    paint_snake();
    pause = 1;
  }
  gotoyx(geom.topleft.y - 1, geom.topleft.x);
  printl("| Level: %d |", level);
  move();
  if (alive == 0) {
    paint_snake();
    draw_food();
    draw_end();
  } else {
    eating();
    paint_snake();
    draw_food();
  }
  level_check();
}
