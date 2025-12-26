#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 15
#define MAXWORD 10

typedef struct{
    char word[15];
    int rs,cs,re,ce,found;
}worditem;
typedef struct {
    char map[SIZE][SIZE];
    char smallmap[SIZE][SIZE];
    worditem word[MAXWORD];
    int wordcount;
    int dosya_acildimi;
}puzzle;
void print_puzzle(puzzle p);
puzzle fill_random_map(puzzle p);
puzzle place_single_word(puzzle p,worditem w);
puzzle place_all_words(puzzle p);
puzzle find_word_direction(puzzle p,int i,char guess[SIZE],int r,int c);
puzzle search_in_direction(puzzle p,int i,char guess[SIZE],int r,int c,int dr,int dc);
puzzle uppercase_found_word(puzzle p,int length_of_word,int r,int c,int dr,int dc);
puzzle mark_as_found(puzzle p,int i);
int all_words_found(puzzle p);

#endif