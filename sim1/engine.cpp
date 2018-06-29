#include <stdio.h>
#include <stdlib.h>
#include<set>
#include<map>
#include<vector>
#include<iostream>
#include<fstream>
#include<iostream>
#include "engine.hpp"




int states(Player ** players, int num_players, Chain * chain){
  int done = 1;
  Player * p;
     int i = 0;
  while(done!=0){
     done = 0;
  
     for(i=0;i<num_players;i++){
       p = players[i];
       done = done +  p->strategy(chain);
      
     }
     //  printf("done = %d\n",done);
  }
  
  chain->race_set->prune();
  // printf("chain is pruned\n");
  for(i=0;i<num_players;i++){
    p = players[i];
    p->strategy(chain);
  }
  // printf("done choosing\n");
  return 0;
}



int rounds(int num_players, int rounds,Player ** players ,Chain * chain){
  Player * p1 = players[0];
  Player * p2 = players[1];
  float h1 = p1->hash;
  float h2 = p2->hash;
  int i = 0;
  float vec[]={.1,.1,.7,.7,.7,.8,.8,.1,.1,.1};
  int interval = rounds/100;
   printf("interval = %d\n",interval);
  for(i=0;i<rounds;i++){
    if(interval==0||i%interval==0){
      printf("#");
      std::cout.flush(); 
    }
    int j = 0;
    float total = 0;
    Player * p;
    float r = ((float)rand())/((float)RAND_MAX);
    
    //r=vec[i];
  
    while(j<num_players){
      total = total + players[j]->hash;
      if(r<total){
        p = players[j]; 
	p->mine_block(chain);
	p->strategy(chain);
	j = num_players + 1;
      }
      j++;
    }
    states(players,num_players,chain); 
  }
  printf("\n");
  int total = 0;
  Block * curr = chain->race_set->choose();
  while(curr!=NULL){
    for(int i=0;i<num_players;i++){
      if(curr->playerID==i+1){
	players[i]->mined++;
        total++;
	//	printf("BlockID = %d, p%d\n",curr->blockID,players[i]->id);
      }
    }
    curr = curr->prev;
  }


   return total;
}
