
#include<stdio.h>
#include"status.h"
#include"map.h"
#include<string.h>

int main(){

    int n;
    char cname[15],source[15],dest[15];
    int lat,longt;
    list* OPEN,*CLOSED;
    city * cptr,*START,*GOAL;
    char ch[15]="france.map";

    FILE *f = fopen(ch,"r") ;
    if(!f)
        return 0;
    list* l = newlist(comparestring,printCity);
    if (!l) return 1;
    while(!feof(f))
        {
            n=fscanf(f,"%s %d %d",cname,&lat,&longt);
            if(n!=3)
                continue;
            cptr=createcity(cname,lat,longt);
            if(!cptr)
                return 1;
            addtolist(l,cptr);
        }
    fclose(f);
    getneighbours(l,ch);
    displaymap(l);


    OPEN=newlist(compgofn,printCity);
    CLOSED=newlist(comparestring,printCity);

    printf("\nEnter source city:");
    gets(source);
    printf("\nEnter destination city:");
    gets(dest);

    START=findcity(l,source);
    GOAL=findcity(l,dest);

    addtolist(OPEN,START);
    START->dfstart=0;
    START->dsttogoal=hofn(START,GOAL);

    while(OPEN->head)
    {
        void* e;
        int dist_covered;
        removeFromListAt(OPEN,1,&e);
        addtolist(CLOSED,e);
        city* c=(city*)e;
        if(c==GOAL)
        {
            printf("\nThe shortest path is\n");
            while(c){
                printf("%s<-",c->name);
                c=c->links;
            }
            return 0;
        }
        node* temp=c->neighbour->head;
        while(temp){
            neighnode* ntemp=temp->value;
            city* successor=ntemp->neighcity;
            int distance=ntemp->dist;

            dist_covered=c->dfstart+distance;

            if((isInList(OPEN,successor)!=0) ||((isInList(CLOSED,successor)!=0) && (dist_covered>successor->dfstart))) {
                temp=temp->next;
            }
            else{
                if(isInList(OPEN,successor)!=0)
                    remfromlist(OPEN,successor);
                if(isInList(CLOSED,successor)!=0)
                    remfromlist(CLOSED,successor);
                successor->dfstart=dist_covered;
                successor->links=c;
                addtolist(OPEN,successor);
                temp=temp->next;
            }
        }
    }
    printf("failed");
    return 0;

}



