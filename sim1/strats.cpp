#include<stdio.h>
#include<stdlib.h>
#include "strats.hpp"


int selfish(Player * p, Chain * c){
   Block * f = p->chain->origin;
   int len = p->chain->length + f->blockID;
   int comp = c->race_set->getLength();
   int pub_len = p->chain->origin->blockID;
   //   printf("p->id = %d\n",p->id);
   // printf("publen = %d\n",pub_len);
   // printf("comp=%d\n",comp);
   if((len<comp||((p->chain->length==0) && ((p->chain->origin->playerID)!=(p->id))))){
     /*give up*/
     // printf("p: %d, h1\n",p->id);

     p->chain->origin = c->race_set->choose();
     p->chain->head = NULL;
     p->chain->tail = NULL;
     p->chain->length = 0;
     return 0;
   }
   else if(len==comp && len!=0 && p->chain->length!=0){/*race*/
     // printf("p: %d, h2\n",p->id);
     Block * b = p->chain->tail;
     Block * n = p->chain->head;
     
     n->prev = p->chain->origin;
     p->chain->length = 0;
     p->chain->head = NULL;
     p->chain->tail = NULL;
     p->chain->origin = b; 
     c->appendBlock(b,b->prev,p->gamma);

     return 1;
     /*release - len should equal 1 in this case*/
   }
   else if(c->race_set->numTied()>1 && len==(comp+1)){/*break tie by releasing*/
     // printf("p: %d, h3\n",p->id);
     
     Block * n = p->chain->tail;
     // printf("blockID released = %d\n",n->blockID);
     Block * n2 = p->chain->head;
     n2->prev = p->chain->origin;
     c->appendBlock(n,n->prev,p->gamma);
     p->chain->origin = n;
     p->chain->length=0;
     p->chain->tail=NULL;
     p->chain->head = NULL;
     // printf("got here\n");
     return 1;
   }
   else if(len==(comp+1) && pub_len<comp){/*release both blocks*/
     // printf("p: %d, h4\n",p->id);
      Block * n = p->chain->tail;
      Block * s = p->chain->head;
      s->prev = p->chain->origin;
      c->appendBlock(n,n->prev,p->gamma);
     
      p->chain->length=0;
      p->chain->tail=NULL;
      p->chain->head=NULL;
      p->chain->origin = n;

      return 1;
   }
   else if((len-comp)>=2 && pub_len<comp){/*publish 1 block - */
     // printf("p: %d, h5\n",p->id);
     Block * n = p->chain->head;
     // printf("blockID released = %d\n",n->blockID);
     p->chain->head = p->chain->head->next;
     p->chain->origin = n;
     p->chain->length--;
     c->appendBlock(n,f,p->gamma); 
     return 1;
   }
   else{/*do nothing*/
     // printf("do nothingllll\n");
     return 0;
   }
   
}



int honestMine(Player * p, Chain * c){
  
  int len = p->chain->length;
  if(len==0){
    p->chain->origin = c->race_set->choose();
    // printf("Honest ID chosen = %d\n",p->chain->origin->blockID);
    // printf("HonestMinenumTied = %d\n",c->race_set->numTied());
    /*do nothing*/
  
    return 0;
  }
  else{
  
    Chain * pChain = p->chain;
    Block * t = pChain->head;
    Block * target = pChain->origin;

    pChain->origin = pChain->head;
    pChain->head = pChain->head->next;  
    pChain->length--;
    c->appendBlock(t,target,p->gamma);
  
    return 1;
  }
  /**/
}



