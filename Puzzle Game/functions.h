#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "header.h"

void print_puzzle(puzzle p){
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            printf("%c ",p.map[i][j]);
        }
        printf("\n");
    }
}
puzzle fill_random_map(puzzle p){
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            p.map[i][j]='a'+(int)(rand()/(RAND_MAX+1.0)*26);
        }
    }
    return p;
}
puzzle place_single_word(puzzle p,worditem w){
    int rowd,cold,i=0;
    //Row direction
    if(w.rs<w.re)
    rowd=1; 
    else if(w.rs==w.re)
    rowd=0;
    else
    rowd=-1;
    //Column direction
    if(w.cs<w.ce)
    cold=1;
    else if(w.cs==w.ce)
    cold=0;
    else
    cold=-1;
    while(((w.rs==w.re)&&(w.cs==w.ce))!=1){
        p.map[w.rs][w.cs]=w.word[i];
        w.rs+=rowd;
        w.cs+=cold;
        i++;
    }
    p.map[w.rs][w.cs]=w.word[i];
    return p;
}
puzzle place_all_words(puzzle p){
    char *place="inputs.txt";
    FILE *fin=fopen(place,"r");
    if(fin==NULL){
        p.dosya_acildimi=0;
        return p;
    }
    for(int i=0;i<MAXWORD;i++){
        fscanf(fin,"%s %d %d %d %d",p.word[i].word,&p.word[i].rs,&p.word[i].cs,&p.word[i].re,&p.word[i].ce);
        p.word[i].found=0;
        p=place_single_word(p,p.word[i]);
        }
    fclose(fin);
    p.dosya_acildimi=1;
    //printf("FCLOSE COMPLETED\n");
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            p.smallmap[i][j]=p.map[i][j];
            //ust uste çakışan kelimelerin çakışmalarındaki harf buyumesi digerini etkilemesin diye kontrol kucuklerde olacak
        }
    }
    return p;
}

puzzle find_word_direction(puzzle p,int kelime_numarasi,char guess[SIZE],int r,int c){
    if(p.smallmap[r][c]==guess[0]){
        int direction[8][2]={{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};
        for(int i=0;i<8;i++){
            if(guess[1]==p.smallmap[r+direction[i][0]][c+direction[i][1]]){
                int dr=direction[i][0];
                int dc=direction[i][1];
                int carpan=strlen(guess)-1;
                int limitr=carpan*dr+r;
                int limitc=carpan*dc+c;
                if(((limitr<0)||(limitr>=SIZE))||((limitc<0)||(limitc>=SIZE))){
                    //Out of limits
                }
                else{
                    p=search_in_direction(p,kelime_numarasi,guess,r,c,dr,dc);
                    if(p.word[kelime_numarasi].found==1)
                    return p;
                }
            }
        }
        return p;
    }
    else{
        printf("Wrong start point!\n");
        return p;
    }
}
puzzle search_in_direction(puzzle p,int kelime_numarasi,char guess[SIZE],int r,int c,int dr,int dc){
    int length=strlen(guess);
    for(int i=1;i<length;i++){
        if(guess[i]!=p.smallmap[(r+dr*i)][c+dc*i]){
            //Kelime bu yonde degil
            return p;
        }
    }
    p=mark_as_found(p,kelime_numarasi);
    p=uppercase_found_word(p,length,r,c,dr,dc);
    return p;
}
puzzle uppercase_found_word(puzzle p,int length_of_word,int r,int c,int dr,int dc){
    for(int i=0;i<length_of_word;i++){
        if(p.map[(r+dr*i)][(c+dc*i)]>96){
            p.map[(r+dr*i)][(c+dc*i)]-=32;
            //cakışan kelimelerde harflerin ascii değerini iki kere azaltmamak için
        }
    }
    return p;
}
puzzle mark_as_found(puzzle p,int i){
    p.word[i].found=1;
    return p;
}
int all_words_found(puzzle p){
    for(int i=0;i<MAXWORD;i++){
        if(p.word[i].found==0){
            return 0;
        }
    }
    return 1;
}

#endif
