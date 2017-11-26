#ifndef MAP_H
#define MAP_H
#include<stdlib.h>
#include"status.h"

typedef struct node{
    void *value;
    struct node *next;
}node;

typedef int (*compFnc) (void *,void *);
typedef void (*prFnc) (void*);

//list of cities
typedef struct list{
    int nelts;
    node *head;
    compFnc comp;
    prFnc pr;
}list;

//structure of city
typedef struct city{
    char name[10];
    list* neighbour;
    int longt;
    int lat;
    int dfstart;
    int dsttogoal;
    struct city* links;

}city;

typedef struct neighnode{
    int dist;
    city* neighcity;
}neighnode;

int comparestring(void *,void *);
void printCity(void *);
void printneighbour(void *);
void forEach(list* ,void(*fptr)(void*));

list *newlist(compFnc,prFnc);
status addtolist(list*,void*);
city* createcity(char*,int,int);
status displaymap(list*);
status removeFromListAt(list *,int,void **);
int lengthList(list*);
status remfromlist(list*,void*);

status  nthInlist(list *,int,void**);
status getneighbours(list* l,char*);
status readneighbour(list*,char*,city*);
node* isInList(list*,void*);

neighnode* getneighbourpointer(list*,char*,int);

int hofn(city*, city*);
int compdistance(void *, void *);
city* findcity(list*,char*);
int compgofn(void*,void*);

#endif // MAP_H



