#ifndef _JUDGE_H_
#define _JUDGE_H_

#include <module/create.h>

void bfsBoomZero(Block &click_pos);
void boomClear(Block &boom_pos);
void boomDel();
bool resultLeft(Block &click_pos);
bool resultRight(Block &click_pos);

#endif