
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"status.h"
#include"map.h"
#include<string.h>
#define INFINITY 9999

static node *available=0;
static int nlist=0;

/** \brief estimation of distance
 *
 * \param c1 city*
 * \param c2 city*
 * \return int
 *
 */
int hofn(city* c1,city* c2)
{
    return((abs(c1->lat-c2->lat)+abs(c1->longt-c2->longt))/4);
}

/** \brief distance between two cities
 *
 * \param n1 void*
 * \param n2 void*
 * \return int
 *
 */
int compdistance(void* n1,void* n2)
{
     return(((neighnode*)n1)->dist-((neighnode*)n2)->dist);
}

/** \brief compare distance from the start and current cities
 *
 * \param c1 void*
 * \param c2 void*
 * \return int
 *
 */
int compgofn(void * c1, void * c2){
    return (((city *)c1)->dfstart - ((city *)c2)->dfstart);
}
/** \brief comparing strings ,same as that of strcmp function
 *
 * \param s1 void*
 * \param s2 void*
 * \return int
 *
 */
int comparestring(void *s1,void *s2)
{
    if(strcmp(s1,s2)<0)
        return -1;
    else if(strcmp(s1,s2)>0)
        return 1;
    else
        return 0;
}

/** \brief print list of cities
 *
 * \param s void*
 * \return void
 *
 */
void printCity(void *s)
{
    city * t=s;
    printf("\n%s %d %d %d\t",t->name,t->lat,t->longt,t->neighbour->nelts);
    printf("\n");
    forEach(t->neighbour,printneighbour);
}

/** \brief print list of neighbours for each cities
 *
 * \param e void*
 * \return void
 *
 */
void printneighbour(void* e)
{
    neighnode* n=e;
    printf("%s\t",n->neighcity->name);
    printf("%d\t",n->dist);
}

/** \brief
 *
 * \param void*
 * \return void forEach(list*
 *
 */
void forEach(list* l,void(*fptr)(void*))
{
    node* tmp;
    tmp=l->head;
    while(tmp)
    {
        fptr(tmp->value);
        tmp=tmp->next;
    }
}


/** \brief
 *for creating a newlist
 * \param cmp pointer to compare function
 * \param prf pointer to print function
 * \return pointer to a new list
 *
 */
list* newlist(compFnc cmp,prFnc prf){
    list* nwlist=(list*)malloc(sizeof(list));
    if(!nwlist)
        return 0;
    nwlist->head=0;
    nwlist->nelts=0;
    nwlist->comp=cmp;
    nwlist->pr=prf;
    return nwlist;
}

/** \brief
 *creating cities
 * \param cname char* name of city
 * \param lat int latitude of city
 * \param longt int longitude of city
 * \param cmp compFnc
 * \param prf prFnc
 * \return city*
 *
 */
city* createcity(char* cname,int lat,int longt)
{
    city* ctemp=(city*)malloc(sizeof(city));
    if(!ctemp)
        return 0;
    strcpy(ctemp->name,cname);
    ctemp->lat=lat;
    ctemp->longt=longt;
    ctemp->neighbour=newlist(compdistance,printCity);
    ctemp->dfstart=INFINITY;
    ctemp->links=0;

    return ctemp;
}

/** \brief
 *function to add cities to the list
 * \param l list* pointer to ist of cities
 * \param e void* pointer to city
 * \return status
 *
 */
status addtolist(list* l,void* e)
{
    node* tmp=l->head;
    node* temp;

    if(available)
    {
        temp=available;
        available=temp->next;
    }
    else{
        temp=(node*)malloc(sizeof(node));
        if(!temp)
            return ERRALLOC;
    }
    if(!l->comp)//no comparison fnc error
        return ERRUNABLE;


            //adding first element
    if((!tmp) || l->comp(e,l->head->value)<0){
        temp->next=tmp;
        l->head=temp;
        temp->value=e;
        l->nelts++;
        return OK;
    }
    while(tmp && tmp->next && l->comp(tmp->next->value,e )<0)//finding positions
    {
        tmp=tmp->next;
    }
    temp->value=e;
    temp->next=tmp->next;
    tmp->next=temp;
    l->nelts++;
    return OK;
 }

/** \brief displaying the map structure
 *
 * \param l list* pointer to the list of cities
 * \return status
 *
 */
status displaymap(list* l)
{
    node* tmp;
    if(!(l->pr))
        return ERRUNABLE;

    tmp=l->head;
    printf("[");
    while(tmp)
    {
        l->pr(tmp->value);
        tmp=tmp->next;
    }
    printf("]");
    return OK;
}

/** \brief find the nth node in the list
 *
 * \param ls list* pointer to list of cities
 * \param pos int position of node
 * \param e void** pointer to the node
 * \return status return OK if node exists within given position else return ERRINDEX
 *
 */
status nthInlist(list* ls,int pos,void** e)
{
      node* temp=ls->head;
      int c=1;
      while((temp) && (c!=pos))
      {
          temp=temp->next;
          c++;
      }
      if(c==pos)
      {
          *e=temp->value;
           return OK;
      }
      else
        return ERRINDEX;
}

/** \brief function to check whether a node exist or not
 *
 * \param l list* pointer to list
 * \param e void*
 * \return node* return predecessor if exist,1 if the node is the first element of the list and 0 if not exist
 *
 */
node* isInList(list* l,void* e)
{
    node* temp=l->head;
    node* prev;
    if(!temp)
        return 0;
    if(l->comp(temp->value,e)==0)
        return (node*)1;
    while(temp && (l->comp(temp->value,e)!=0))
    {
        prev=temp;
        temp=temp->next;
    }
    if(temp)
        return prev;
    else
        return 0;

}

/** \brief
 *
 * \param l list*
 * \param e void*
 * \return status
 *
 */
status remfromlist(list* l,void* e)
{
    node* temp=l->head;
    node* prev;
    if(!(l->comp))
        return ERRUNABLE;
    while(l->comp(temp->value,e)!=0)
    {
        prev=temp;
        temp=temp->next;
    }
    prev->next=temp->next;
    temp->next=available;
    available=temp;
}

/** \brief
 *
 * \param l list*
 * \return int
 *
 */
int lengthList(list* l)
{
    return l->nelts;

}

/** \brief
 *
 * \param l list*
 * \param pos int
 * \param s void**
 * \return status
 *
 */
status removeFromListAt(list* l,int pos,void** s)
{
    node* temp,*prev;
    if(l->nelts<pos)
        return ERRINDEX;
    temp=l->head;

    if(pos==1)
        l->head=temp->next;

    else
    {
        prev=temp;
        while(pos-- >2) prev=prev->next;
        temp=prev->next;
        prev->next=temp->next;
    }

    *s=temp->value;
    temp->next=available;
    available=temp;
    return OK;
}


/** \brief to find the neighbour cities
 *
 * \param l list* pointer to list of neighbours of current city
 * \param nname char* name of neighbour
 * \param distance int distance of neighbour and current city
 * \return neighnode*
 *
 */
neighnode* getneighbourpointer(list* l,char* nname,int distance){
    node* temp;
    neighnode* nnode=(neighnode*)malloc(sizeof(neighnode));
    nnode->dist=distance;
    temp=isInList(l,nname);
    if(!temp)
        return 0;
    if(temp==1)
    {
        temp=l->head;
        nnode->neighcity=temp->value;
        return nnode;
    }
    temp=temp->next;
    nnode->neighcity=temp->value;
    return nnode;
}

/** \brief get neighbours from the file for each city and add those neighbours to its list of neighbours
 *
 * \param l list* pointer to list of cities
 * \param path[] char file path
 * \param curcity city* pointer to current city whose neighbour is being added
 * \return status
 *
 */
status readneighbour(list* l,char path[],city* curcity){
    char cname[15];
    int lat,longt,nc,dist;
    neighnode* nptr;
    FILE* fp=fopen(path,"r");
    if(!fp)
        return ERROPEN;
    while(!feof(fp))
    {
        nc=fscanf(fp,"%s %d %d",cname,&lat,&longt);
        if(nc!=3)
            continue;
       if(l->comp(curcity,cname)!=0)
            continue;
       do
       {
           nc=fscanf(fp,"%s %d %d",cname,&dist,&longt);
           if(nc==3||nc==-1){
            fclose(fp);
            return OK;
           }
        nptr=getneighbourpointer(l,cname,dist);
        addtolist(curcity->neighbour,nptr);
        }while(nc==2);
    }
    fclose(fp);
    return OK;
}

/** \brief get each city from the list and read its neighbours from the file
 *
 * \param l list* pointer to list
 * \param path[] char file path
 * \return status
 *
 */
status getneighbours(list* l,char path[]){
   int i,n=l->nelts;
   city* current;
   for(i=1;i<=n;i++){
        nthInlist(l,i,(void**)&current);
        readneighbour(l,path,current);
    }
    return OK;
}

city* findcity(list* l,char* cname)
{
    if(!l->head) return NULL;
    node* temp=l->head;
    while(temp){
            city* ct=temp->value;
        if(strcmp(ct->name,cname) == 0){
            return temp->value;
        }
        temp=temp->next;
    }
    return 0;
}


