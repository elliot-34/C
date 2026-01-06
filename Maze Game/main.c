#include "header.h"

int main(){
    char *filename="map.txt";
    int rows=countrows(filename);//row pdfde direkt verilmemis, almaliyiz
    if(rows==-1){
        printf("Map file cannot be opened\n");
        return 0;
    }
    char (*map)[COLS]=allocateMap(rows);//dinamik memory ile map olusturuyoruz
    int check=loadMap(map,rows,filename);//mapi alacak
    if(check==-1){
        printf("Map file cannot be opened\n");
        return 0;
    }
    player p;
    scoreboard sb;
    check=findPlayer(&p,map,rows);//p harfini bulcak
    if(check==-1){
        printf("Player not found on map\n");
        return 0;
    }
    sb.score=0;
    sb.sCount=0;
    sb.gCount=0;
    sb.dCount=0;
    sb.flag=0;
    int nextroundcheck=0;//neden oldugunun aciklamasi altta yer aliyor
    time_t start=time(NULL);
    while(1){
        printMap(map,rows);
        time_t moment=time(NULL);
        int dif=(int)difftime(moment,start);
        printcontrols(&sb,dif);
        while(1){
            char c=getchar();
            if(c!='\n'){
                switch(c){
                    case 'w':
                    move[0](rows,&p,map,&sb,-1,0);
                    break;
                    case 's':
                    move[1](rows,&p,map,&sb,1,0);
                    break;
                    case 'a':
                    move[2](rows,&p,map,&sb,0,-1);
                    break;
                    case 'd':
                    move[3](rows,&p,map,&sb,0,1);
                    break;
                    default:
                    while(getchar()!='\n');
                    printf("Do you want to exit? Press y(YES) or n(NO):");
                    c=getchar();
                    if(c=='y'){
                        time_t end=time(NULL);
                        int duration=(int)difftime(end,start);
                        saveScoreToFile(&sb,duration);
                        freeMap(map);
                        return 0;
                    }
                    break;
                }
                if(nextroundcheck==1){
                    if(hasreachableitem(map,rows,&p)==0){
                        printMap(map,rows);
                        time_t end=time(NULL);
                        int duration=(int)difftime(end,start);
                        saveScoreToFile(&sb,duration);
                        freeMap(map);
                        return 0;
                    }
                }
                if(sb.flag==1){
                    nextroundcheck=1;
                    //henüz daha eşya yeni toplandı, duvara dönmedi bir dahaki hamle başında dönecek
                    //o yüzden oyuncu hapsoldu mu kontrolüde bir dahaki tur sonunda yapılacak
                }
                else{
                    nextroundcheck=0;
                }
                if(hasexit(map,rows)==0){
                    printMap(map,rows);
                    time_t end=time(NULL);
                    int duration=(int)difftime(end,start);
                    saveScoreToFile(&sb,duration);
                    freeMap(map);
                    return 0;
                }
            }
            else{
                break;
            }
        }
    }
    freeMap(map);
    return 0;
}
