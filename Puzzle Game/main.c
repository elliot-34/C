#include "functions.h"
int main(){
    srand(time(NULL));
    puzzle p;
    p=fill_random_map(p);
    printf("==========PUZZLE GAME==========\n");
    p=place_all_words(p);
    if(p.dosya_acildimi==0){
        printf("A problem occured while opening the file.\n");
        return 0;
    }
    print_puzzle(p);
    char guess[SIZE+1],ch; 
    char exit[]="exit game";
    while(1){
        int sayac=0;
        while(1){
            printf("Enter word or type 'exit game' to exit:");
            while(1){
                if(sayac==SIZE){
                    while(getchar()!='\n');
                    printf("A word that long does not exist in the game.Try again.\n");
                    break;
                }
                ch=getchar();
                if(ch!='\n'){
                    guess[sayac]=ch;
                    sayac++;
                }
                else{
                    break;
                }
            }
            if(sayac==SIZE){
                sayac=0;
                continue;
            }
            else{
            guess[sayac]='\0';
            break;
            }
        }
        if(strcmp(guess,exit)==0){
            printf("Game session terminated.\n");
            return 0;
        }
        int r,c;
        printf("Row ve column:");
        while(scanf(" %d %d",&r,&c)!=2){
            while(getchar()!='\n');
            printf("Enter two integers.\n");
            printf("Row ve column:");
        }
        while(getchar()!='\n'); //1 4aaaaa girişi yapılırsa aaaa bufferda kalmasın
        int girdi=0;
        for(int i=0;i<MAXWORD;i++){
            if((strcmp(guess,p.word[i].word)==0)&&(p.word[i].found==0)){
                girdi=1;
                p=find_word_direction(p,i,guess,r,c);
                if(p.word[i].found==1){
                    print_puzzle(p);
                }
                if(all_words_found(p)){
                    printf("You have found all the words!\n");
                    return 0;
                }
            }
            else if((strcmp(guess,p.word[i].word)==0)&&(p.word[i].found==1)){
                printf("This word has already been found.\n");
                break;
            }
            else if(girdi==0&&i==MAXWORD-1){
                printf("This word does not exist in the puzzle.\n");
            }
        }
    }
}