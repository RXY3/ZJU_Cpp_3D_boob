#ifndef _JUDGE_H_
#define _JUDGE_H_

#include <module/create.h>

void bfsBoomZero(Block &click_pos, std::vector<Block> &frontier);
void boomClear(Block &boom_pos, std::vector<Block> &frontier);
void boomDel(std::vector<Block> &frontier);
bool resultLeft(Block &click_pos, std::vector<Block> &frontier);
bool resultRight(Block &click_pos, std::vector<Block> &frontier);

#endif