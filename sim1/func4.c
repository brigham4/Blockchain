
#include <stdio.h>
#include <stdlib.h>

#define NUM 100000000
#define H1 .30
#define H2 .25

typedef struct Player{
  int mined; /*number mined in perm chain*/
  float hash; 
  int chain; /*length of private chain*/
  int mtemp; /*number mined in private chain*/
  int c_mtemp;
  int b; /*branch point*/
}Player;


typedef struct Chain{
  int length;
  int b1;
  int b2;
}Chain;



int partialWin(Player *p1, Player *p2, Chain* trunk){/*p1 wins conditionally*/
  (*p1).c_mtemp = (*p1).c_mtemp+(*p1).mtemp;
  (*trunk).length=(*p1).b+(*p1).chain;
  (*p1).b = (*trunk).length;
  (*p1).mtemp = 0;
  (*p1).chain = 0;
}


int winner(Player *p1,Player *p2,Chain * trunk, int win){
  /*Player 1 wins, player 2 quits*/
  (*p1).mined=(*p1).mined+(*p1).mtemp+(*p1).c_mtemp;
  (*p1).c_mtemp=0;
  (*p2).c_mtemp=0;
  (*trunk).length=(*p1).b+(*p1).chain;
  
  (*p1).b=(*trunk).length;
  (*p2).b=(*trunk).length;
   
  (*p1).chain=0;
  (*p2).chain=0;
 
  (*p1).mtemp=0; 
  (*p2).mtemp=0;
}



int detlong(Player* p1, Player * p2, Chain* trunk){/*finds which player has longer chain*/
  int publen = trunk->length;
  int l1 = (p1->b)+(p1->chain);
  int l2 = (p2->b)+(p2->chain);
  if(l1>l2){
    return 1;
  }
  else if(l2>l1){
    return -1;
  }
  else{
    return 0;
  }
}


int racehelp2(Player *p1, Player *p2, Chain* trunk, float r){/*3 way race, 2 out of 3 will quit in each case*/
  if(r<(*p1).hash){/*p1 wins*/
    winner(p1,p2,trunk,1);
  }
  else if(r<((*p1).hash+(*p2).hash)){/*p2 wins*/
    winner(p2,p1,trunk,1);
  }
  else{
    (*p1).b=(*trunk).length;
    (*p2).b=(*trunk).length;

    (*p1).mtemp = 0;
    (*p2).mtemp = 0;
    
    (*p1).chain = 0;
    (*p2).chain = 0;
  }  
}






int racehelp(Player *p1, Player *p2, Chain * trunk, float r){/*player 1 races chain and player 2 reacts*/
  int c1 = 0;
  int c2 = 0;
  int dist = 0;
  dist = (*p1).b-(*p2).b;
  c1 = (*p1).chain+dist;
  c2 = (*p2).chain;
  if(c2==(c1-1)){/*p2 releases everyone else quits*/
    winner(p2,p1,trunk,1);
  }
  else if(c2==c1){/*3 way race, 2 out of 3 will quit*/
    if(r<(*p1).hash){/*p1 wins*/
      winner(p1,p2,trunk,1);
    }
    else if(r<((*p1).hash+(*p2).hash)){/*p2 wins*/
      winner(p2,p1,trunk,1);
    }
    else{/*trunk wins*/
      (*p1).b=(*trunk).length;
      (*p2).b=(*trunk).length;

      (*p1).chain=0;
      (*p2).chain=0;

      (*p1).mtemp=0;
      (*p2).mtemp=0;

      (*p1).c_mtemp=0;
      (*p2).c_mtemp=0;
       
    }
  }
  else{/*2 way race, p2 does nothing*/
    float k = (1-(*p1).hash-(*p2).hash);
    if(r<(*p1).hash+.5*k){/*p1 wins*/
      partialWin(p1,p2,trunk);  
    }
    else{/*main chain wins, p1 quits, p2 does nothing*/
      (*p1).b = (*trunk).length;
      (*p1).mtemp = 0;
      (*p1).c_mtemp = 0;
      (*p1).chain=0;
    }
  }
}


int help2(Player *p1, Player *p2, Chain* trunk, float r){/*both p1 and p2 release*/
  /*p1 and p2 are both ahead by 1*/
  int c1 = 0;
  int c2 = 0;
  if(r>.5){/*p1 wins*/
    winner(p1,p2,trunk,1);
  }
  else{
    winner(p2,p1,trunk,1);
  }
}

int help1(Player *p1, Player *p2, Chain * trunk, float r){/*p1 decides to release, everyone else reacts*/
  
  int comp = detlong(p1,p2,trunk);
  
    if(comp>0){/*minor 2 quits*/
      winner(p1,p2,trunk,1);
    }
    else if(comp<0){/*minor 2 remains, minor 1 releases*/
      /*DO STUFF*/
      (*trunk).length=(*p1).b+(*p1).chain;
    }
    else{/*RACE*/
      /*r<p1->hash*/
      if(r<(p1->hash)){/*P1 WINS*/
	winner(p1,p2,trunk,1);
      }
      else if(r<(p1->hash)+(p2->hash)){
	winner(p2,p1,trunk,1);
      }
      else{
	(*p1).chain=0;
	(*p2).chain=0;
	(*p1).b=(*trunk).length;
	(*p2).b=(*trunk).length;
      }
    }
return 0;
}

int  strat2(Player* p1, Player*  p2, Player * p3,Chain* trunk){
  int publen = trunk->length;
  int c1 = publen-(p1->b);
  int c2 = publen-(p2->b);
  int im1 = 0;
  int im2 = 0;
  int rel1 = 0;
  int rel2 = 0;
  int race1 = 0;
  int race2 = 0;
   float r = ((float)rand())/((float)RAND_MAX);
  int comp = detlong(p1,p2,trunk);
  if((p1->chain)==0 && (p2->chain)==0){/*ONE CHAIN ONLY*/

    (*p1).b++;
    (*p2).b++;
    return 0;
  }
 
  /*CHECK RACING*/
  if(c1==(p1->chain)){
    race1 = 1;
  }
  if(c2==(p2->chain)){
    race2 = 1;
  }
  /*END CHECK RACING*/

  if(race1==1 || race2==1){

    /*CASES FOR RACES*/
    if(race1==1 && race2 == 0){
      racehelp(p1,p2,trunk,r);
    }
    if(race1==0 && race2==1){
      racehelp(p2,p1,trunk,r);
    }
    if(race1*race2==1){
      racehelp2(p1,p2,trunk,r);
    }
  }
  /*END RACE CASES*/
  
  /*-----------------------------------------------------------------------*/
  
  /*NON RACE CASES*/
  if(c1==((p1->chain)-1)){/*player 1 init decision*/
    im1 = (*trunk).b1+(p1->chain); /*update chain length*/
    rel1=1;
  }
   publen = trunk->length;
   /*c2 = publen-(trunk->b2);*/
   if(c2==((p2->chain)-1)){/*player 2 init decision*/
     im2 = (*trunk).b2+(p2->chain);
     rel2=1;
  }
   /*END NON RACE CASES*/

  if(rel1==1 || rel2==1){

    /*FINAL DECISIONS NON RACING*/
    if(rel1==1 && rel2==0){/*minor 1 releases*/
       help1(p1,p2,trunk,r);  
    }
    else if(rel1==0 && rel2 == 1){/*minor 2 releases*/
       help1(p2,p1,trunk,r);
    }
    else if(rel1==0 && rel2==0){/*nothing happens, this is fine*/

    }
    else{/*both decide to release....???*/
       help2(p1,p2,trunk,r);
    }
  }
  
  return 0;
}


/*runs the mining rounds*/
int rounds(int num,Player* p1,Player* p2, Player* p3,Chain * chain){
  int i = 0;
  int r = 0;
  int r1 = 0;
  int r2 = 0;
  int winner = -1;
  int state1[3] = {0,0,0}; /*[private chain, public chain, trunk]*/
  int state2[3] = {0,0,0};
  float h1 = p1->hash;
  float h2 = p2->hash;
  int trunk = 0;
  int j = 1;
  for(i=0;i<num;i++){
    int total = 0;
    float m1 = 0;
    float m2 = 0;
    float m3 = 0;
    int k = 0;
    /*  float r1 = ((float)rand())/((float)RAND_MAX);*/
        float r2 = ((float)rand())/((float)RAND_MAX);
	    float r = ((float)rand())/((float)RAND_MAX);
    /*    float r2 = ((float)rand())/((float)RAND_MAX);*/
    

    if(r<=h1){/*p1 wins*/
      (*p1).mtemp= (*p1).mtemp+1;
      (*p1).chain = (*p1).chain+1;
      state1[0] = state1[0]+1;
      winner = 1;
      /* printf("%d\n",(*p1).mined);*/
    }
    else if(r>h1 && r<=h1+h2){/*player 2 wins*/
      state2[0]=state2[0]+1;
       (*p2).mtemp = (*p2).mtemp+1;
       (*p2).chain = (*p2).chain+1;
       winner = 2;

    }
    else{
      state1[1]=state1[1]+1;
      state1[1]=state1[1]+1;
      winner = 3;
      (*chain).length=(*chain).length+1;
      strat2(p1,p2,p3,chain);
      /*printf("%d : %f\n",(*chain).length,r);*/
    }

    if((i%100)==0){
      total = (*chain).length;
      k=(*chain).length-(*p1).mined-(*p2).mined;
      m1 = ((float) (*p1).mined)/((float) total);
      m2 = ((float) (*p2).mined)/((float) total);
     m3 = ((float) k)/((float) total);
     /* printf("%d p1: %f  p2: %f  p3: %f\n",j,m1,m2,m3);*/
    j++;
    }
    /*state1 = strat1(p1,state1);*/
    /*state2 = strat2(p2,state2);*/

  }
}




int main(){
  float m1 = 0;
  float m2 = 0;
  float m3 = 0;
  int num = 10;
  int total = 0;
  int k = 0;
  Player p1 = {0,H1,0,0,0};
  Player p2 = {0,H2,0,0,0};
  Player p3 = {0,.50,0,0,0};
  Chain chain = {0,0,0};
  int ans = rounds(NUM,&p1,&p2,&p3,&chain);
  total=chain.length;
  k=chain.length-p1.mined-p2.mined;
  m1 = ((float) p1.mined)/((float) total);
  m2 = ((float) p2.mined)/((float) total);
  m3 = ((float) k)/((float) total);
  
  printf("p1: %f \n p2: %f \n p3: %f\n",m1,m2,m3);
  printf("Length: %d\n",chain.length);
}








