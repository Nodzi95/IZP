#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define EPS 0.0000000001                                                        /*pozadovana odchylka od dvou vysledku funkce cfrac_tan*/
double cfrac_tan(double x, unsigned int n);                                     /*funkce na vypocet tangens pomoci zretezenych zlomku*/
double taylor_tan(double x, unsigned int n);                                    /*funkce na vypocet tangens pomoci Taylorova polynomu*/
double vypocet_delky(double uhel_a, double vyska_p);                            /*funkce na vypocet vzdalenosti od objektu*/
double vypocet_vysky(double uhel_b, double delka, double vyska_p);              /*funkce na vypocet vysky objektu*/
double absolutni_hodnota(double vysledek);                                      /*funkce absolutni hodnota*/
void srovnani(double uhel, unsigned int n, unsigned int m);                     /*funkce na vypis srovnani s knihovni hodnotou tangens*/
void vypocet(double uhel_a, double uhel_b, double vyska_p);                     /*funkce na nacteni uhlu alpha (pokud jsou tak i uhel beta a vyska pozorovatele) do promennych*/
void srovnani_tan(char **argv);                                                 /*funkce na osetreni vstupu a nastaveni promennych*/
void vypocty(int argc, char **argv);                                            /*funkce na osetreni velikosti radianu*/
void napoveda();                                                                /*funkce na vypis napovedy k programu*/
bool jecislo(char *retezec);                                                    /*funkce na zjisteni zda retezec je cislo*/
unsigned int pocet_iteraci(double uhel);                                        /*funkce na zjisteni poctu iteraci, ktere odpovidaji presnosti 1e-10*/


int main(int argc, char **argv)
{
    if(argc == 1){                                              /*pokud na stdin nebude od uzivatele nic zadano vypise se chyb. hlaska*/
        fprintf(stderr,"Nezadal si zadne argumenty\n");
        return 1;
    }
    if(argc == 5 && strcmp(argv[1],"--tan") == 0)               /*pokud prvni argument je prepinac --tan, zavola se funkce srovnani_tan*/
        srovnani_tan(argv);
    else if(argc >= 3)                                          /*pokud na vstupu je mene nez 3 argumenty, vypise se chybova hlaska*/
        vypocty(argc,argv);
    else if(argc == 2 && (strcmp(argv[1],"--help") == 0))
        napoveda();
    else{
        fprintf(stderr,"Chyba vstupu : Malo zadanych argumentu\n");
        return 1;
    }
    return 0;
}

void srovnani_tan(char **argv)
{
    double uhel_alpha;
    unsigned int od_iterace, do_iterace;
    if(jecislo(argv[2])){
        uhel_alpha = atof(argv[2]);                             /*prevod retezce na double*/
        if(uhel_alpha >0 && uhel_alpha <= 1.4)                  /*pokud uhel alpha(v rad.) je v pozadovanem intervalu pokracuje se dal*/
            ;
        else{
            fprintf(stderr,"Chyba nacteni uhlu Alpha : Uhel zadavejte v radianech v intervalu (0, 1.4>\n");
            exit(1);
        }
    }
    else{
        fprintf(stderr,"Chyba vstupu : 2. argument neni cislo\n");
        exit(1);
    }
    if(jecislo(argv[3]))
        od_iterace = atoi(argv[3]);                             /*prevod retezce na integer*/
    else{
        fprintf(stderr,"Chyba vstupu : 3. argument neni cislo\n");
        exit(1);
    }
    if(jecislo(argv[4]))
        do_iterace = atoi(argv[4]);
    else{
        fprintf(stderr,"Chyba vstupu : 4. argument neni cislo\n");
        exit(1);
    }
    srovnani(uhel_alpha, od_iterace, do_iterace);               /*volani funkce srovnani*/
    return;
}

void srovnani(double uhel, unsigned int n, unsigned int m)
{
    double taylor,cfrac,tang;
    tang = tan(uhel);                                           /*nacteni knihovni hodnoty tangens*/
    if(n > 0 && n <= m && m < 14)                               /*osetreni zadani iteraci 0 < n <= m < 14*/
        while(n != (m+1)){                                      /*cyklus se provede (m-n)+1 krat*/
            taylor = taylor_tan(uhel, n);                       /*nacteni hodnoty tangens pomoci Taylorova polynomu*/
            cfrac = cfrac_tan(uhel, n);                         /*nacteni hodnoty tangens pomoci zretezenych zlomky*/
            printf("%d %e %e %e %e %e\n", n, tang, taylor, absolutni_hodnota(tang - taylor), cfrac, absolutni_hodnota(tang - cfrac));
            n++;                                                /*inkrement promenne n*/
        }
    else{
            fprintf(stderr,"Chyba vstupu : Spatne hodnoty iteraci\n");
            exit(1);
        }
    return;
}

void vypocty(int argc, char **argv)                             /*nastaveni uhlu alpha pokud je, tak i beta + nastaveni vysky pozorovatele*/
{
    double uhel_alpha, uhel_beta, vyska;
    vyska = 1.5;                                                /*implicitni nastaveni vysky pozorovatele*/
    uhel_beta = -1;
    if(argc >= 3 && argc <= 4 && strcmp(argv[1],"-m") == 0){    /*provede se pokud prvni argument je prepinac -m*/
        if(jecislo(argv[2])){                                   /*funkce na kontrolu cisla*/
            uhel_alpha = atof(argv[2]);                         /*nacteni uhlu alpha*/
            if(argc == 4){
                if(jecislo(argv[3]))
                    uhel_beta = atof(argv[3]);                  /*nacteni uhlu beta*/
                else{
                    fprintf(stderr,"Chyba vstupu : 3. argument neni cislo\n");
                    exit(1);
                }
            }
        }
        else{
            fprintf(stderr,"Chyba vstupu : 2. argument neni cislo\n");
            exit(1);
        }
    }
    else if(argc >= 5 && argc <= 6 && strcmp(argv[1],"-c") == 0 && strcmp(argv[3],"-m") == 0){  /*provede se pokud je prvni argument prepiman -c a treti prepinac -m*/
        if(jecislo(argv[2])){
            vyska = atof(argv[2]);                              /*nastaveni vysky pozorovatele*/
            if(vyska <= 0 || vyska >100){                       /*osetreni nastaveni vysky pozorovatele*/
                fprintf(stderr,"Chyba vstupu : 2. argument musi byt kladne nenulove cislo mensi nez 100\n");
                exit(1);
            }
        }
        else{
            fprintf(stderr,"Chyba vstupu : 2. argument neni cislo\n");
            exit(1);
        }
        if(jecislo(argv[4]))
            uhel_alpha = atof(argv[4]);
        else{
            fprintf(stderr,"Chyba vstupu : 4. argument neni cislo\n");
            exit(1);
        }
        if(argc == 6){                                          /*pokud je 6 argumentu nacte se uhel beta*/
            if(jecislo(argv[5]))
                uhel_beta = atof(argv[5]);
            else{
                fprintf(stderr,"Chyba vstupu 5. argument neni cislo\n");
                exit(1);
            }
        }
    }
    else{
        fprintf(stderr,"Chyba nacteni vstupnich dat\n");
        exit(1);
    }
    vypocet(uhel_alpha, uhel_beta, vyska);                      /*funkce predavam uhel alpha, beta a vysku 1,5 nebo nastavenou od uzivatele*/
    return;
}

void vypocet(double uhel_a, double uhel_b, double vyska_p)
{
    double delka,vyska;
    if(uhel_a > 0 && uhel_a <= 1.4){                            /*osetreni zadani velikosti radianu v intervalu (0;1.4>*/
        delka = vypocet_delky(uhel_a, vyska_p);                 /*funkce vypocita vzdalenost od objektu*/
        printf("%.10e\n", delka);
    }
    else{
        fprintf(stderr,"Chyba nacteni uhlu Alpha : Uhel zadavejte v radianech v intervalu (0, 1.4>\n");
        exit(1);
    }
    if(uhel_b > 0 && uhel_b <= 1.4){
        vyska = vypocet_vysky(uhel_b, delka, vyska_p);          /*funkce vypocita velikost pozorovaneho objektu*/
        printf("%.10e\n",vyska);
    }
    else{
        if(uhel_b == -1);
        else{
        fprintf(stderr,"Chyba nacteni uhlu Beta : Uhel zadavejte v radianech v intervalu (0, 1.4>\n");
        exit(1);
        }
    }
    return;
}
double vypocet_vysky(double uhel_b, double delka, double vyska_p)     /*funkce dostane velikost uhlu beta(v rad.), vzdalenost od objektu a vysku pozorovatele*/
{
    double cfrac, vyska;
    cfrac = cfrac_tan(uhel_b, pocet_iteraci(uhel_b));           /*ulozi vysledek hodnoty tangens, pocet iteraci udava presnost 1e-10 pro dany uhel*/
    vyska = cfrac * delka;                                      /*vypocet vysky pozorovaneho objektu*/
    return vyska + vyska_p;                                     /*funkce vraci celkovou vysku objektu*/
}

double vypocet_delky(double uhel_a, double vyska_p)             /*funkce dostane velikost uhlu alpha(v rad.) a vysku pozorovatele*/
{
    double cfrac, delka;
    cfrac = cfrac_tan(uhel_a, pocet_iteraci(uhel_a));           /*ulozi vysledek hodnoty tangens, pocet iteraci udava presnost 1e-10 pro dany uhel*/
    delka = vyska_p / cfrac;                                    /*vypocet vzdalenost od objektu*/
    return delka;                                               /*funkce vraci vzdalenost od objektu*/
}

double cfrac_tan(double x, unsigned int n)
{
    double vysledek,b,a;
    b = (n*2) - 1;                                              /*nastaveni b*/
    a = 1;
    vysledek=a/(b/x);                                           /*prvni vypocet zlomku (ignorovani rozdilu)*/
    for(n--; n >= 1 ; n--)                                      /*kazdou iteraci se odecte od n 1, dokud neni n rovno 1, tak se cyklus opakuje*/
    {
        b -= 2;                                                 /*kazdou iteraci se od b odecte 2*/
        vysledek = a / ((b/x) - vysledek);                      /*dalsi vypocet zlomku s odectenim predchoziho vysledku*/
    }
    return vysledek;                                            /*funkce vraci vysledek*/
}

unsigned int pocet_iteraci(double uhel)
{
    double x0, x1;
    unsigned int n = 5;
    x0 = 0;
    do{
        x1 = x0;                                                /*ulozeni predchoziho vysledku*/
        x0 = cfrac_tan(uhel, ++n);                              /*nacteni nasledujiciho vysledku*/
    }while(absolutni_hodnota(x0-x1) >= EPS);                    /*cyklus se opakuje dokud absolutni hodnota vysledku nebude mensi nebo rovna 1e-10*/
    return n;                                                   /*funkce vrati pocet iteraci odpovidajici presnosti 1e-10 pro dany uhel (v rad.)*/
}

double taylor_tan(double x, unsigned int n)
{
    double jmenovatel[13]={1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875,
    1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};                /*pole jmenovatelu o 13 prvcich*/
    double citatel[13]={1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162,
    18888466084, 113927491862, 58870668456604};                                             /*pole citatelu o 13 prvcich*/
    double vysledek,m;
    unsigned int c;
    vysledek = 0;                                               /*inicializace prvniho vysledku*/
    m = x;
    for(c=0;c < n;c++)
    {
        vysledek += (citatel[c]*x) / jmenovatel[c];             /*k predchozimu vysledku se pricte novy vysledek => vznika polynom*/
        x *= m*m;                                               /*kazde nove x se vynasobi prvnim x**x*/
    }
    return vysledek;
}

bool jecislo(char *retezec)
{
    unsigned int b;
    for(b=0;b<strlen(retezec);b++)
        if(isalpha(retezec[b]) || retezec[b]==',' )             /*prochazi pole znaku, znak po znaku a jestli znak neni cislo nebo specialni znak vrati false*/
            return false;
    return true;
}

double absolutni_hodnota(double vysledek)                       /*jednoducha absolutni hodnota*/
{
    if(vysledek >= 0)
        return vysledek;
    else
        return -vysledek;
}

void napoveda()
{
    printf("****************************************************\n"
           "***********                             ************\n"
           "********        Autor : Petr Nodzak        *********\n"
           "********        login : xnodza00           *********\n"
           "***********                             ************\n"
           "******** Program pracuje s tremi prepinaci *********\n"
           "***********                             ************\n"
           "******** Prvni je --tan, ktery vyzaduje 3  *********\n"
           "******** argumenty, 1. je uhel, 2. cislo   *********\n"
           "********    iterace, od ktere se zacne     *********\n"
           "******** vypisovat, 3. je cislo iterace,   *********\n"
           "********       kde se program zastavi      *********\n"
           "********    program vypise srovnani mych   *********\n"
           "********      vypoctu s mat. knihovnou     *********\n"
           "***********                             ************\n"
           "********  Druhy je -m, ktery vyzaduje 1-2  *********\n"
           "******** argumenty, 1. je uhel pro vypocet *********\n"
           "********  delky, 2. je volitelny uhel pro  *********\n"
           "********           vypocet vysky           *********\n"
           "***********                             ************\n"
           "********   Treti je volitelny -c, ktery    *********\n"
           "********   nastavi vysku pozorovatele,     *********\n"
           "********       nasleduje prepinac -m       *********\n"
           "***********                             ************\n"
           "****************************************************\n");
}
