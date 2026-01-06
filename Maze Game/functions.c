#include "header.h"

void (*move[4])(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny)={moveup, movedown, moveleft, moveright};

int countrows(const char *name){
    FILE *fp = fopen(name, "r");
    if (fp==NULL) {
        return -1;
    }
    int rows=0;
    char buffer[100];
    while (fgets(buffer,sizeof(buffer),fp)){
        // satır boşsa geç
        if (buffer[0]=='\n'||buffer[0]=='\r'){//bosluk icin isletim sistemine gore  \r\n, \r, \n birisi var 
            continue;
        }
        // satırda en az bir karakter
        else{
            ++rows;
            buffer[0]='\n';
        }
    }
    fclose(fp);
    return rows;
}

char (*allocateMap(int rows))[COLS]{
    char (*map)[COLS]=malloc(rows*sizeof(*map));
    return map;
}

int loadMap(char (*map)[COLS],int rows,char *filename){
    FILE *fp=fopen(filename,"r");
    if (fp==NULL) {
        return -1;
    }
   for (int i=0;i<rows;i++) {
        fscanf(fp," %s",map[i]);
        map[i][COLS-1]='\0';
        //rowları string olarak kullanabilmek icin sonuna \0 koydum
    }
    fclose(fp);
    return 0;
} 

void printMap(char (*map)[COLS],int rows){
    for (int i=0;i<rows;i++){
        printf("%s",map[i]);
        //rowu string gibi kullaniyorum
        printf("\n");
    }
}

int findPlayer(player *p,char (*map)[COLS],int rows){
    for (int i=0;i<rows;i++){
        for (int j=0;j<COLS;j++){
            if (map[i][j]=='P') {
                (*p).x=i;
                (*p).y=j;
                return 0;
            }
        }
    }
    return -1;
}

void moveup(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny){
    handleMove(rows,p,map,sb,(*p).x+nx,(*p).y+ny);
}
void movedown(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny){
    handleMove(rows,p,map,sb,(*p).x+nx,(*p).y+ny);
}
void moveleft(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny){
    handleMove(rows,p,map,sb,(*p).x+nx,(*p).y+ny);
}
void moveright(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny){
    handleMove(rows,p,map,sb,(*p).x+nx,(*p).y+ny);
}

void handleMove(int rows,player *p,char (*map)[COLS],scoreboard *sb,int nx,int ny){
    if(nx<0 || nx>=rows || ny<0 || ny>=COLS-1){
        //sınırdışı
        return;
    }
    if(map[nx][ny]=='#'){
        //duvar
        return;
    }
    if((*sb).flag==1){
        map[(*p).x][(*p).y]='#';
        (*sb).flag=0;
    }
    else{
        map[(*p).x][(*p).y]='.';
    }
    char c=map[nx][ny];
    switch(c){
        case '.':
        break;
        case 's':
        (*sb).sCount++;
        (*sb).score+=10;
        (*sb).flag=1;
        break;
        case 'g':
        (*sb).gCount++;
        (*sb).score+=20;
        (*sb).flag=1;
        break;
        case '$':
        (*sb).dCount++;
        (*sb).score+=30;
        (*sb).flag=1;
        break;
        }
    (*p).x=nx;
    (*p).y=ny;
    map[(*p).x][(*p).y]='P';
}

int hasexit(char (*map)[COLS],int rows){
    char c;
    for(int i=0;i<rows;i++){
        for(int j=0;j<COLS-1;j++){
            c=map[i][j];
            if(c=='s'||c=='g'||c=='$'){
                return 1;
            } 
        }
    }
    printf("You collected every item on the map.\n");
    return 0;
}
int hasreachableitem(char (*map)[COLS],int rows,player *p){
    char (*truthmap)[COLS-1]=malloc(rows*sizeof(*truthmap));//\0 gerek yok sadece karakterlere bakacağım
    for(int i=0;i<rows;i++){
        for(int j=0;j<COLS-1;j++){
            truthmap[i][j]=map[i][j];
        }
    }
    if(dfs(truthmap,(*p).x,(*p).y)==0){
        printf("You can't reach more items.\n");
        free(truthmap);
        return 0;
    }
    //eğer if'e girmediyse hala şans vardır
    free(truthmap);
    return 1;
}

int dfs(char (*truthmap)[COLS-1],int x,int y){
    //depth first search yontemi
    if(truthmap[x][y]=='1'||truthmap[x][y]=='#'){
        return 0;
    }
    if(truthmap[x][y]=='s'||truthmap[x][y]=='g'||truthmap[x][y]=='$'){
        return 1;
    }
    truthmap[x][y]='1';
    //baktigi kutuyu 1 olarak isaretliyor birdaha girmemek icin
    if(dfs(truthmap,x-1,y)==1) 
    return 1;
    if(dfs(truthmap,x+1,y)==1) 
    return 1;
    if(dfs(truthmap,x,y-1)==1) 
    return 1;
    if(dfs(truthmap,x,y+1)==1) 
    return 1;
    //hepsi başarısızsa artık alınabilecek eşya yoktur
    return 0;
}
void printcontrols(scoreboard *sb,int dif){
    printf("====CONTROLS====\n");
    printf("UP w,DOWN s,LEFT a,RIGHT d\n");
    printf("Score=%d | Time=%d\n",(*sb).score,dif);
    printf("Move:");
}
void saveScoreToFile(scoreboard *sb,int duration){
    FILE *fp=fopen("scores.txt", "a");
    if (fp==NULL){
        printf("Error opening scores file!\n");
        return;
    }
    //şuanki zaman
    time_t t=time(NULL);
    struct tm tm=*localtime(&t);
    int finalScore;
    if(((*sb).score-duration)>0){
        finalScore=(*sb).score-duration;
    }
    else{
        finalScore=0;
    }
    // Formatimiz tarih saat | skor | oyun suresi | Silver Gold Dolar
    fprintf(fp,"%02d-%02d-%d %02d:%02d:%02d | Score: %d | Time: %ds | S:%d G:%d D:%d\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec,
            finalScore, duration, 
            sb->sCount, sb->gCount, sb->dCount);

    fclose(fp);
    gameend(sb, duration); //ekrana yazdırcak
}

void gameend(scoreboard *sb,int duration){
    printf("====SCOREBOARD====\n");
    printf("Silver collected:%d\n",(*sb).sCount);
    printf("Gold collected:%d\n",(*sb).gCount);
    printf("Dollar collected:%d\n",(*sb).dCount);
    printf("Time:%d sec\n",duration);
    if(((*sb).score-duration)>0){
        printf("Score:%d\n",(*sb).score-duration);
    }
    else{
        printf("Score:0\n");
    }
}
void freeMap(char (*map)[COLS]){
    free(map);
}