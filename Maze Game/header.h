#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define COLS 31 //rowlari string gibi kullanabilmek icin 30 karakter+\0 ihtiyac

typedef struct{
    int x;//row
    int y;//column
}player;

typedef struct {
    int score;
    int sCount;// silver
    int gCount;//gold
    int dCount;//dollar
    int flag;
}scoreboard;

void moveup(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny);
void movedown(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny);
void moveleft(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny);
void moveright(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny);
extern void (*move[4])(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny);

int countrows(const char *name);
char (*allocateMap(int rows))[COLS];
int loadMap(char (*map)[COLS],int rows,char *filename);
void printMap(char (*map)[COLS],int rows);
int findPlayer(player *p,char (*map)[COLS],int rows);

void handleMove(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny);

int hasexit(char (*map)[COLS],int rows);
int hasreachableitem(char (*map)[COLS],int rows,player *p);
int dfs(char (*truthmap)[COLS-1],int x,int y);

void printcontrols(scoreboard *sb,int dif);
void saveScoreToFile(scoreboard *sb,int duration);
void gameend(scoreboard *sb,int duration);
void freeMap(char (*map)[COLS]);

#endif