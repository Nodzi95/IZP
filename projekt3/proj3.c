#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#define LEVA 0
#define PRAVA 1
#define HORNI_DOLNI 2
typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
}Map;

typedef struct { int dr; int ds;} Prirustek;

bool jecislo(char *retezec)
{
    unsigned int b;
    for(b=0;b<strlen(retezec);b++)
        if(!isdigit(retezec[b]))             /*prochazi pole znaku, znak po znaku a jestli znak neni cislo nebo specialni znak vrati false*/
            return false;
    return true;
}

void allocMap(Map *map, int rows, int cols)
{
      map->rows = rows;
      map->cols = cols;
      if ((map->cells =(unsigned char *) malloc(map->rows*map->cols*sizeof(char))) == NULL){
        fprintf(stderr,"Nepodarilo se alokovat pamet\n");
        return;
      }

      return;
}

void addMap(Map *map, FILE *f)                                  /*funkce nacte hodnoty ze souboru a ulozi na alokovane misto*/
{
    int i,j;
    unsigned char n;
    i = map->rows * map->cols;
    for(j=0; j<i; j++)
    {
        fscanf(f, " %c ",&n);
        map->cells[j] = n;
    }
    return ;
}

bool isborder(Map *map, int r, int c, int border)               /*funkce na zjisteni pozadovane hranice*/
{
    int pole[3] = {1, 2, 4};
    //printf("%d\n", map->cells[r * map->cols + c] - '0');
    if((map->cells[r * map->cols + c] - '0') & pole[border])
        return true;
    else
        return false;
}

bool test(Map *map, FILE *f)                                    /*funkce na validnost mapy*/
{
    int i,j,hodnota,r,c;
    fscanf(f,"%d %d",&r,&c);
    allocMap(map, r, c);                                        /*volani funkce na alokaci pameti*/
    addMap(map, f);
    hodnota = 0;
    for(j=0 ; j<r ; j++){                                       /*kontrola policek vedle sebe*/
        for(i=0 ; i<c-1 ; i++){
            if(isborder(map, j, i, 1) == isborder(map, j, i+1, 0));
            else
                hodnota++;
        }
    }
    for(j=0 ; j<r-1 ; j++){                                     /*kontrola policek pod sebou*/
        for(i=0 ; i<c ; i++){
            if((j%2 ==0 && i%2 == 1) || (j%2 == 1 && i%2 == 0)){
                if((isborder(map, j, i, 2) == isborder(map, j+1, i, 2)));
                else
                    hodnota++;
            }
        }
    }
    for(j=0 ; j< r ; j++){                                      /*kontrola cisel v rozsahu 0-7*/
        for(i=0 ; i<c ; i++){
            if(map->cells[j*c+i] >= '0' && map->cells[j*c+i] <= '7');
            else
                hodnota++;
        }
    }
    if(hodnota != 0)
        return false;
    return true;

}

int start_border(Map *map, int r, int c, int leftright)         /*funkce na zjisteni, ktera hrana se ma nasledovat po vstupu do bludiste*/
{
    if (leftright == PRAVA){
        if(r%2 == 1 && c == 0 && c%2 == 0 && !isborder(map, r, c, 0))
            return HORNI_DOLNI;
        else if(r%2 == 0 && c == 0 && c%2 == 0 && !isborder(map, r, c, 0))
            return PRAVA;
        else if(r%2 == 0 && c%2 == 0 && c == map->cols-1 && !isborder(map, r, c, 1))
            return HORNI_DOLNI;
        else if(r%2 == 1 && c%2 == 0 && c == map->cols-1 && !isborder(map, r, c, 1))
            return LEVA;
        else if(c%2 == 0 && r == 0 && r%2 == 0 && !isborder(map, r, c, 2))
            return LEVA;
        else if(c%2 == 0 && r == map->rows-1 && r%2 == 1 && !isborder(map, r, c, 2))
            return PRAVA;
        else
            return -1;

    }
    else if(leftright == LEVA){
        if(r%2 == 1 && c == 0 && c%2 == 0 && !isborder(map, r, c, 0))
            return PRAVA;
        else if(r%2 == 0 && c == 0 && c%2 == 0 && !isborder(map, r, c, 0))
            return HORNI_DOLNI;
        else if(r%2 == 0 && c%2 == 0 && c == map->cols-1 && !isborder(map, r, c, 1))
            return LEVA;
        else if(r%2 == 1 && c%2 == 0 && c == map->cols-1 && !isborder(map, r, c, 1))
            return HORNI_DOLNI;
        else if(c%2 == 0 && r == 0 && r%2 == 0 && !isborder(map, r, c, 2))
            return PRAVA;
        else if(c%2 == 0 && r == map->rows-1 && r%2 == 1 && !isborder(map, r, c, 2))
            return LEVA;
        else
            return -1;

    }
    else
        return -1;

}

int rpath(Map *map, int r, int c)                               /*algoritmus pruchodu bludistem, aplikovanim pravidla prave ruky*/
{
    int smer;
    Prirustek d[] = {{0,1},{1,0},{0,-1},{-1,0}};
    smer = start_border(map, r, c, PRAVA);
    if(smer >= 0){
        printf("%d,%d\n",r+1,c+1);
        while(r < map->rows && c < map->cols && c >= 0 && r >= 0){
            if((r%2 == 1 && c%2 == 0) || (r%2 == 0 && c%2 == 1)){
                if(smer == PRAVA){
                    if(!isborder(map, r, c, 1)){
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = PRAVA;
                    }
                    else if(!isborder(map, r, c, 0)){
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = HORNI_DOLNI;
                    }
                    else{
                        r += d[1].dr;
                        c += d[1].ds;
                        smer = LEVA;
                    }
                }
                else if(smer == LEVA){
                    if(!isborder(map, r, c, 0)){
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = HORNI_DOLNI;
                    }
                    else if(!isborder(map, r, c, 2)){
                        r += d[1].dr;
                        c += d[1].ds;
                        smer = LEVA;
                    }
                    else{
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = PRAVA;
                    }
                }
                else if(smer == HORNI_DOLNI){
                    if(!isborder(map, r, c, 2)){
                        r += d[1].dr;
                        c += d[1].ds;
                        smer = LEVA;
                    }
                    else if(!isborder(map, r, c, 1)){
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = PRAVA;
                    }
                    else{
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = HORNI_DOLNI;
                    }
                }
            }
            else if((r%2 == 1 && c%2 == 1) || (r%2 == 0 && c%2 == 0)){
                if(smer == PRAVA){
                    if(!isborder(map, r, c, 1)){
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = HORNI_DOLNI;
                    }
                    else if(!isborder(map, r, c, 2)){
                        r += d[3].dr;
                        c += d[3].ds;
                        smer = PRAVA;
                    }
                    else{
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = LEVA;
                    }
                }
                else if(smer == LEVA){
                    if(!isborder(map, r, c, 0)){
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = LEVA;
                    }
                    else if(!isborder(map, r, c, 1)){
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = HORNI_DOLNI;
                    }
                    else{
                        r += d[3].dr;
                        c += d[3].ds;
                        smer = PRAVA;
                    }
                }
                else if(smer == HORNI_DOLNI){
                    if(!isborder(map, r, c, 2)){
                        r += d[3].dr;
                        c += d[3].ds;
                        smer = PRAVA;
                    }
                    else if(!isborder(map, r, c, 0)){
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = LEVA;
                    }
                    else{
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = HORNI_DOLNI;
                    }
                }
            }
            if(r == map->rows || c == map->cols || r < 0 || c < 0);
            else
                printf("%d,%d\n",r+1,c+1);
        }
    }
    else{
        fprintf(stderr,"Chyba vstupu : do bludiste z tohoto mista nelze vstoupit\n");
        return 1;
    }
    return 0;
}

int lpath(Map *map, int r, int c)                               /*algoritmus pruchodu bludistem, aplikovanim pravidla leve ruky*/
{
    int smer;
    Prirustek d[] = {{0,1},{1,0},{0,-1},{-1,0}};        /*nadefinovane pohyby v bludisti*/
    smer = start_border(map, r, c, LEVA);               /*zjisteni pocatectiho smeru*/
    if(smer >= 0){
        printf("%d,%d\n",r+1,c+1);
        while(r < map->rows && c < map->cols && c >= 0 && r >= 0){
            if((r%2 == 1 && c%2 == 0) || (r%2 == 0 && c%2 == 1)){   /*trojuhelnik s hranou dole*/
                if(smer == PRAVA){
                    if(!isborder(map, r, c, 1)){        /*pokud neni leva hrana, provede se posun doleva a zmeni se smer*/
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = HORNI_DOLNI;
                    }
                    else if(!isborder(map, r, c, 2)){   /*pokud neni prava hrana, provede se posun doprava a zmeni se smer*/
                        r += d[1].dr;
                        c += d[1].ds;
                        smer = PRAVA;
                    }
                    else{                               /*pokud je prava i leva hrana, provede se posun dolu a zmeni se smer*/
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = LEVA;
                    }
                }
                else if(smer == LEVA){
                    if(!isborder(map, r, c, 0)){
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = LEVA;
                    }
                    else if(!isborder(map, r, c, 1)){
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = HORNI_DOLNI;
                    }
                    else{
                        r += d[1].dr;
                        c += d[1].ds;
                        smer = PRAVA;
                    }
                }
                else if(smer == HORNI_DOLNI){
                    if(!isborder(map, r, c, 2)){
                        r += d[1].dr;
                        c += d[1].ds;
                        smer = PRAVA;
                    }
                    else if(!isborder(map, r, c, 0)){
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = LEVA;
                    }
                    else{
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = HORNI_DOLNI;
                    }
                }
            }
            else if((r%2 == 1 && c%2 == 1) || (r%2 == 0 && c%2 == 0)){  /*trojuhelnik s hranou nahore*/
                if(smer == PRAVA){
                    if(!isborder(map, r, c, 1)){
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = PRAVA;
                    }
                    else if(!isborder(map, r, c, 0)){
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = HORNI_DOLNI;
                    }
                    else{
                        r += d[3].dr;
                        c += d[3].ds;
                        smer = LEVA;
                    }
                }
                else if(smer == LEVA){
                    if(!isborder(map, r, c, 0)){
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = HORNI_DOLNI;
                    }
                    else if(!isborder(map, r, c, 2)){
                        r += d[3].dr;
                        c += d[3].ds;
                        smer = LEVA;
                    }
                    else{
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = PRAVA;
                    }
                }
                else if(smer == HORNI_DOLNI){
                    if(!isborder(map, r, c, 2)){
                        r += d[3].dr;
                        c += d[3].ds;
                        smer = LEVA;
                    }
                    else if(!isborder(map, r, c, 1)){
                        r += d[0].dr;
                        c += d[0].ds;
                        smer = PRAVA;
                    }
                    else{
                        r += d[2].dr;
                        c += d[2].ds;
                        smer = HORNI_DOLNI;
                    }
                }
            }
            if(r == map->rows || c == map->cols || r < 0 || c < 0); /*kontrola zda nejsem mimo bludiste*/
            else
                printf("%d,%d\n",r+1,c+1);                          /*vypsani aktualni souradnice*/
        }
    }
    else{
        fprintf(stderr,"Chyba vstupu : do bludiste z tohoto mista nelze vstoupit\n");
        return 1;
    }
    return 0;
}

int pruchod(char **argv, int path, Map *map)                    /*mezi funkce*/
{
    FILE *f;
    int r,c;
    char *soubor;
    soubor = argv[4];                                               /*nacteni nazvu souboru od uzivatele*/
    if((f = fopen(soubor, "r")) == NULL){                           /*kontrola otevreni souboru*/
        fprintf(stderr,"Chyba nacteni : Spatny nazev souboru\n");
        return 1;
    }
    else{
        if(!test(map,f)){
            printf("Pole neni validni\n");
            fclose(f);
            return 0;
        }
        if(jecislo(argv[2])){
            r = atoi(argv[2]);                                      /*nacteni pocatecni souradnice od uzivatele*/
            if(r>map->rows){
                printf("Z tohoto mista nelze vstoupit\n");
                fclose(f);
                return 0;
            }
        }
        else{
            fprintf(stderr,"Chyba vstupu : argument cislo 2 neni cislo\n");
            fclose(f);
            return 1;
        }
        if(jecislo(argv[3])){
            c = atoi(argv[3]);
            if(c>map->cols){
                printf("Z tohoto mista nelze vstoupit\n");
                fclose(f);
                return 0;
            }
        }
        else{
            fprintf(stderr,"Chyba vstupu : argument cislo 3 neni cislo\n");
            fclose(f);
            return 1;
        }
        if(path == PRAVA)
            rpath(map, r-1, c-1);                                   /*volani funkce rpath*/
        if(path == LEVA)
            lpath(map, r-1, c-1);                                   /*volani funkce lpath*/
        fclose(f);
        return 0;
    }
}

int main(int argc, char **argv)
{
    Map map;
    if(argc == 1){
        fprintf(stderr,"Chyba vstupu : nezadane argumenty\n");
        return 1;
    }
    else if(argc == 2 && strcmp(argv[1],"--help") == 0){
        printf("Autor : Petr Nodzak\n"
               "Nick : xnodza00\n"
               "prepinac --test pozaduje jako nasledujici argument nazev souboru\n"
               "prepinac -rpath pozaduje jako nasledujici argument pocatecti souradnice X Y\n"
               "a nazev souboru\n"
               "prepinac -lpath funguje stejne jako -rpath\n");
    }
    else if(argc == 3 && strcmp(argv[1],"--test") == 0){
        FILE *f;
        char *soubor;
        soubor = argv[argc-1];
        if((f = fopen(soubor, "r")) == NULL){
            fprintf(stderr,"chyba nacteni souboru\n");
            return 1;
        }
        else{
            if(test(&map,f)){
                printf("Valid\n");
            }
            else{
                printf("Invalid\n");
            }
            fclose(f);
            free(map.cells);
        }
    }
    else if(argc == 5){
        if(strcmp(argv[1],"-rpath") == 0)
            pruchod(argv, PRAVA, &map);
        else if(strcmp(argv[1],"-lpath") == 0)
            pruchod(argv, LEVA, &map);
        else{
            fprintf(stderr,"Chyba vstupu : spatne zvolene pravidlo\n");
            return 1;
        }
        free(map.cells);
    }
    else{
        fprintf(stderr,"Chyba vstupu : spatne argumenty\n");
        return 1;
    }
    return 0;
}
