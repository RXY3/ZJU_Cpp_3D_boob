#ifndef _CREATE_H_
#define _CREATE_H_

extern int SizeByMode[3];
extern int BoomByMode[3];

int ***initFrontier(int mode);
int ***initialClickArr(int mode);
double ***initPosition(double ***block_pos, int mode);
void createBoom(int ***frontier, int mode);
void printFrontier(int ***frontier, int mode);
void destroyFrontier(int ***frontier, int mode);
void destroyClickArr(int ***clickArr, int mode);
void destroyPosition(double ***block_pos, int mode);

#endif
