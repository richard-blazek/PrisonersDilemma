#ifndef __IO_H__
#define __IO_H__

#include "player.h"

void io_init(void);
void io_quit(void);
void io_draw(player *players, int count, int rounds);

bool io_loop(void);
int io_mouse_scroll(void);

#endif
