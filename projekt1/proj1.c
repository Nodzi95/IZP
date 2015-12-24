#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define bile_znaky " \t\n"                      /*znaky ktere bude program vynechavat*/

/*--------------------------PROTOTYPY FUNKCI--------------------------*/
/*------------FUNKCE PRACUJI S PARAMETRY PRIKAZOVE RADKY--------------*/
bool jecislo(char *bunka);
void sel(int argc,char **argv);
void max(int argc,char **argv);
void min(int argc,char **argv);
void avg(int argc,char **argv);
void sum(int argc,char **argv);
int vyber(int argc,char **argv, double *pom);
void napoveda();
/*--------------------------------------------------------------------*/
int main(int argc,char **argv)
{
    if (argc==1){                                               /*pokud nebude zadany zadny argument, program vypise chybovou hlasku na stderr*/
        fprintf(stderr,"\nNezadal jste zadne parametry\n");
        return 1;
        }
    else{
        if ((strcmp("select", argv[1]) == 0) && argc >= 4){     /*vyber funkce select*/
            sel(argc,argv);
            return 0;
        }
        else if ((strcmp("avg", argv[1]) == 0) && argc >= 4){   /*vyber funkce prumer ciselnyho hodnot*/
            avg(argc,argv);
            return 0;
        }
        else if ((strcmp("max", argv[1]) == 0) && argc >= 4){   /*vyber funkce maximalni hodnoty*/
            max(argc,argv);
            return 0;
        }
        else if ((strcmp("min", argv[1]) == 0) && argc >= 4){   /*vyber funkce minimalni hodnoty*/
            min(argc,argv);
            return 0;
        }
        else if ((strcmp("sum", argv[1]) == 0) && argc >= 4){   /*vyber funkce pocitani sumy*/
            sum(argc,argv);
            return 0;
            }
        else if((strcmp("--help",argv[1]) == 0)&& argc == 2){   /*vypis napovedy k pouzivani programu*/
            napoveda();
            return 0;
        }
        else{                                                   /*pokud nebudou platit podminky uvedeny vyse, vypise se chybova hlaska*/
            fprintf(stderr,"\nZadal jste spatnou operaci nebo malo argumentu,\npro napovedu napiste --help, jako prvni argument.\n");
            return 1;
        }

    }
}

/*Funkce naplni pomocne pole vysledky, se kterymi se pracuje v dalsich funkcich a vrati hodnotu pocet iteraci*/
int vyber(int argc,char **argv, double *pom)
{
    int argument1,argument2,argument3,argument4;                /*promenne zadanych argumentu z prik. radky*/
    int i,m,c,pocitadlo;                                          /*promenne pro cykly*/
    double mezi_vysledek;                                       /*promenna na meze vysledky*/
    char *buffer,*str;                                          /*pomocne ukazatele na znak*/
    char text[1024];                                            /*pole o max. delce 1024 znaku*/
    if(jecislo(argv[3]))
        argument1 = atoi(argv[3]);                              /*prevede na int*/
    else
        argument1 = 0;
    if (argc >= 5 && jecislo(argv[4]))
        argument2 = atoi(argv[4]);
    else
        argument2 = 0;
    if (argc == 7 && jecislo(argv[5]) && jecislo(argv[6])){
        argument3 = atoi(argv[5]);
        argument4 = atoi(argv[6]);
    }
    else{
        argument3 = 0;
        argument4 = 0;
    }
    m = 1;
    pocitadlo = 0;
    if((strcmp(argv[2],"col")==0) && argc == 4 && argument1 > 0){
        while(fgets(text,1024,stdin)!=NULL){                    /*nacte radek a ulozi do pole charu*/
            buffer="";                                          /*inicializase bufferu*/
            for(i=1,str=text;buffer!=NULL;i++,str=NULL)         /*prochazeni bunek radku*/
            {
                buffer=strtok(str,bile_znaky);
                if(i==(argument1) && buffer!=NULL){             /*pokud se 1. argument rovna aktualni pozici v radku, pracuje s danou bunkou*/
                    if(strcmp(argv[1],"select")!=0){
                        if(jecislo(buffer)){                    /*funkce na zjisteni zda je v retezci cislo*/
                            mezi_vysledek=atof(buffer);         /*prevede rezerec na double*/
                            pom[pocitadlo]=mezi_vysledek;       /*uklada mezi vysledky do pomocneho pole*/
                        }
                        else{                                   /*pokud v bunce je znak, vypise chybovou hlasku na stderr*/
                            fprintf(stderr,"Ve vyberu je neciselna hodnota\n");
                            exit(1);
                        }
                    }
                    else
                        puts(buffer);
                }
            }
            if(argument1 > (i-2)){                              /*osetreni poctu nactenych sloupcu*/
                fprintf(stderr,"Chyba nacteni sloupce\n");
                exit(1);
            }
            pocitadlo++;                                        /*spocita pocet mezi vysledku*/
        }
        return pocitadlo;                                       /*vrati pocet mezi vysledku*/
    }
    else if((strcmp(argv[2],"row") == 0) && argc == 4 && argument1 > 0){    /*podobny algoritmus jako u col*/
        for(c=0;c<argument1;c++){
            if(fgets(text,1024,stdin)!=NULL){                               /*osetreni poctu nactenych radku*/
                buffer="";
                for(str=text;buffer!=NULL;str=NULL)
                {
                    buffer=strtok(str,bile_znaky);
                    if(m == argument1 && buffer != NULL){                       /*pokud se 1. argument rovna aktualnimu radku postupne se vypisou vsechny bunky*/
                        if(strcmp("select",argv[1])==0)                         /*pokud je vybrana operace select vypise se bunka na stdout*/
                            puts(buffer);
                        else{
                            if(jecislo(buffer)){                                /*jestli je retezec cislo vrati True*/
                                mezi_vysledek=atof(buffer);                     /*prevede rezerec na double*/
                                pom[pocitadlo]=mezi_vysledek;                   /*uklada mezi vysledky do pomocneho pole*/
                                pocitadlo++;
                            }
                            else{                                               /*pokud ve vybranych bunkach nejsou cisla, vypise chybovou hlasku na stderr*/
                                fprintf(stderr,"Ve vyberu je neciselna hodnota\n");
                                exit(1);
                            }
                        }
                    }
                }
                m++;                                                            /*posune radek o 1*/
            }
            else{
                fprintf(stderr,"Chyba nacteni radku\n");
                exit(1);
            }
        }
        return pocitadlo;
    }
    else if((strcmp(argv[2],"cols")==0) && argc == 5 && argument1 > 0 && argument2 >= argument1){   /*podobny algoritmus jako u col*/
        while(fgets(text,1024,stdin)!=NULL){
            buffer="";
            for(i = 1, str = text;buffer != NULL; i++, str = NULL)
            {
                buffer=strtok(str,bile_znaky);
                if(i == (argument1) && buffer!=NULL){
                    if(strcmp(argv[1],"select")==0)
                        puts(buffer);
                    else{
                        if(jecislo(buffer)){                    /*funkce na zjisteni zda je v retezci cislo*/
                            mezi_vysledek=atof(buffer);
                            pom[pocitadlo]=mezi_vysledek;
                            pocitadlo++;
                        }
                        else{
                            fprintf(stderr,"Ve vyberu je neciselna hodnota\n");
                            exit(1);
                        }
                    }
                }
                if(argument1 < argument2 && i == argument1){     /*pokud se aktualni sloupec rovna 3. argumentu a 3. argument je mensi jak 4., pricte se 1 k 3. argumentu*/
                    argument1++;
                }
            }
            if(argument1 > (i-2)){
                fprintf(stderr,"Chyba nacteni sloupce\n");
                exit(1);
            }
            argument1=atoi(argv[3]);
        }
        return pocitadlo;
    }

    else if((strcmp(argv[2],"rows")==0) && argc == 5 && argument1 > 0 && argument2 >= argument1){
        for(c=0;c<argument2;c++){
            if(fgets(text,1024,stdin)!=NULL){
                buffer="";
                for(str=text;buffer!=NULL;str=NULL)
                {
                    buffer=strtok(str,bile_znaky);
                    if(m == (argument1) && buffer!=NULL){           /*stejne jako row*/
                        if(strcmp(argv[1],"select")==0)
                            puts(buffer);
                        else{
                            if(jecislo(buffer)){                    /*funkce na zjisteni zda je v retezci cislo*/
                                mezi_vysledek=atof(buffer);
                                pom[pocitadlo]=mezi_vysledek;
                                pocitadlo++;
                            }
                            else{
                                fprintf(stderr,"Ve vyberu je neciselna hodnota\n");
                                exit(1);
                            }
                        }
                    }
                }
                if((argument1 < argument2) && m == (argument1))     /*pokud se aktualni radek rovna 1. argumentu a 1. argument je mensi jak 2., pricte se 1 k 1. argumentu*/
                    argument1++;
                m++;                                                /*posune radek o 1*/
            }
            else{
                fprintf(stderr,"Chyba nacteni radku\n");
                exit(1);
            }
        }
        return pocitadlo;
    }
    else if((strcmp(argv[2],"range") == 0) && argc == 7 && argument1 > 0 && argument2 >= argument1 && argument3 > 0 && argument3 <= argument4){   /*spojeni algoritmu pro cols a rows*/
        for(c=0;c<argument2;c++){
            if(fgets(text,1024,stdin)!=NULL){
                buffer="";
                for(i=1,str=text;buffer!=NULL;i++,str=NULL)
                {
                    buffer=strtok(str,bile_znaky);
                    if(m == argument1 && i == argument3 && buffer!=NULL){   /*pokud se aktualni radek rovna 1. argumentu a aktualni sloupec 3. argumentu, ulozi bunku do bufferu*/
                        if(strcmp(argv[1],"select")==0)
                            puts(buffer);
                        else{
                            if(jecislo(buffer)){                            /*funkce na zjisteni zda je v retezci cislo*/
                                mezi_vysledek=atof(buffer);
                                pom[pocitadlo]=mezi_vysledek;
                                pocitadlo++;
                            }
                            else{
                                fprintf(stderr,"Ve vyberu je neciselna hodnota\n");
                                exit(1);
                            }
                        }
                    }
                    if(argument3 < argument4 && i == argument3)             /*pokud se aktualni sloupec rovna 3. argumentu a 3. argument je mensi jak 4., pricte se 1 k 3. argumentu*/
                        argument3++;
                }
                if(argument4 > (i-2)){
                    fprintf(stderr,"Chyba nacteni sloupce\n");
                    exit(1);
                }
                if((argument1 < argument2) && m == argument1)               /*pokud se aktualni radek rovna 1. argumentu a 1. argument je mensi jak 2., pricte se 1 k 1. argumentu*/
                    argument1++;
                m++;                                                        /*posune radek o 1*/
                argument3=atoi(argv[5]);                                    /*obnovi 3. argument*/
            }
            else{
                fprintf(stderr,"Chyba nacteni radku\n");
                exit(1);
            }
        }
        return pocitadlo;
    }
    else{
        fprintf(stderr,"Spatne zadany format argumentu\n");
        exit(1);
    }
}
/*--------------------------FUNKCE NA VYPSANI VYBRANYCH BUNEK ---------------------*/
void sel(int argc,char **argv)
{
    double pomoc[500]={0};
    vyber(argc,argv,pomoc);
}
/*--------------------------FUNKCE NA VYHLEDAVANI MAXIMALNI HODNOTY--------------------------*/
void max(int argc,char **argv)
{
    int iterace,indexace_pole;
    double vysledek;
    double pomoc[500]={0};                                      /*ukazatel na prvni cislo v poli*/
    iterace=vyber(argc,argv,pomoc);                             /*vyvolani funkce vyber s predanim parametru prik. radky a adresy pole*/
    vysledek = pomoc[0];                                        /*funkce vraci pocet iteraci*/
    for(indexace_pole=1 ; indexace_pole < iterace ; indexace_pole++)
        if(pomoc[indexace_pole] > vysledek)                     /*hledani nejvetsiho cisla z pole*/
            vysledek = pomoc[indexace_pole];
    printf("%.10g\n",vysledek);                                 /*vypsani nejvetsi hodnoty z pole*/
}

/*--------------------------FUNKCE NA VYHLEDAVANI MINIMALNI HODNOTY--------------------------*/
void min(int argc,char **argv)
{                                                   /*stejny algoritmus jak u hledani max. hodnoty s pozmenenou operaci*/
    int iterace,indexace_pole;
    double vysledek;
    double pomoc[500]={0};
    iterace=vyber(argc,argv,pomoc);
    vysledek = pomoc[0];
    for(indexace_pole = 1 ; indexace_pole < iterace ; indexace_pole++)
        if(pomoc[indexace_pole] < vysledek)
            vysledek = pomoc[indexace_pole];
    printf("%.10g\n",vysledek);

}

/*--------------------------FUNKCE NA POCITANI PRUMERNE HODNOTY------------------------------*/
void avg(int argc,char **argv)
{                                                   /*algoritmus stejny jako u funkce sum s vydelenim vysledku, poctem iteraci*/
    int iterace,indexace_pole;
    double vysledek;
    double pomoc[500]={0};
    iterace=vyber(argc,argv,pomoc);
    vysledek = pomoc[0];
    for(indexace_pole=1 ; indexace_pole < iterace ; indexace_pole++)
        vysledek+= pomoc[indexace_pole];
    printf("%.10g\n",vysledek/iterace);

}

/*--------------------------FUNKCE NA VYPOCET SOUCTU HODNOT----------------------------------*/
void sum(int argc,char **argv)
{                                                   /*funkce secte vsechny hodnoty z pole*/
    int iterace,indexace_pole;
    double vysledek;
    double pomoc[500]={0};
    iterace=vyber(argc,argv,pomoc);
    vysledek = pomoc[0];
    for(indexace_pole = 1 ; indexace_pole < iterace ; indexace_pole++)
        vysledek+= pomoc[indexace_pole];
    printf("%.10g\n",vysledek);
}
/*---------------------------FUNKCE NA ZJISTENI, ZDA JE RETEZEC CISLO------------------------*/
bool jecislo(char *bunka)
{
    unsigned int b;
    for(b=0;b<strlen(bunka);b++)
        if(isalpha(bunka[b]))                       /*prochazi pole znaku, znak po znaku a jestli je znak neni cislo vrati false*/
            return false;
    return true;
}
/*--------------------------FUNKCE NA VYPSANI NAPOVEDY---------------------------------------*/
void napoveda()
{
    printf("\n******************************************************************"
           "\n******* Vitejte v programu na zpracovavani textove tabulky *******"
           "\n******************************************************************"
           "\n********  Jako 1. argument zadavejte operaci z vyberu:    ********"
           "\n*******       (select, max, min, avg, sum a --help)        *******"
           "\n******    Jako 2. argument zadavejte nasledujici:           ******"
           "\n*****         (row, col, rows, cols a range)                 *****"
           "\n****      K vyberu row a col zadejte 1 ciselny argument       ****"
           "\n****          ve formatu row X, col X     -> X > 0            ****"
           "\n*****     K vyberu rows a cols zadavejte 2 ciselne argumenty *****"
           "\n******        ve formatu rows X Y, col X Y   -> 0 < X <= Y  ******"
           "\n*******   K vyberu range zadavejte 4 ciselne argumenty     *******"
           "\n********      ve formatu range X Y A B X,Y = radky        ********"
           "\n*********     A, B = sloupce -> 0 < X <= Y a 0 < A <= B  *********"
           "\n******************************************************************\n");
}
