#include "engine.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>

/*sets up and plays a game and returns fraction mined by each player*/
float * game0(int num_players, int num_rounds,float * powers,int (*strategy[])(Player * p, Chain * c)){

  Chain * chain = new Chain();
  Block * gen = new Block(-1);
  chain->appendBlock(gen,NULL,1); 
  Player ** list = new Player*[num_players];
  for(int i=0;i<num_players;i++){
    list[i] = new Player(powers[i],i+1);
    list[i]->chain->origin = gen;
    list[i]->strat = strategy[i];
  }
  
  int total = rounds(num_players,num_rounds,list,chain);
  float * results = new float[num_players];
  for(int i=0;i<num_players;i++){
    results[i] = ((float)list[i]->mined)/((float)total);
  }
  delete chain;  
  return results;
}

/*2 selfish miners, first selfish miner size is fixed and second selfish miner size is varied*/
int oneDimPlot(int num_rounds, float resolution, float a){
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  float b = 0;
  float honest = 1-a-b;
  float inc = .01;
  
   std::ofstream myfile;
   myfile.open("test2.txt");
  for(int i=0;i<40;i++){
    b = b+inc;
    honest = 1-a-b;
    int (*strategy[])(Player * p, Chain * c) = {h,s,s};
    float sizes[3] = {honest,a,b};
    float * results = game0(3,num_rounds,sizes,strategy);
    float rewards = results[1];
    int rewards2 = (int)(1000*rewards);
    rewards = ((float)rewards2)/((float)1000);
     std::ostringstream ss;
     std::ostringstream ss2;
    ss << b;
    std::string s(ss.str());
    ss2 << rewards;
    std::string s2(ss2.str());
    std::string k = s+","+s2;
    myfile << k+"\n";
  }
  
  return 0;
}

int twoDimPlot(int num_rounds){
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  float a=0;
  float b=0;
  float honest = 1-a-b;
  float inc = .01;
 
  int (*strategy[])(Player * p, Chain * c) = {h,s,s};

  
  std::ofstream myfile;
  myfile.open("test3.txt");
  for(int i=0;i<40;i++){
 
    a=0;
    for(int j=0;j<40;j++){
     
      honest = 1-a-b;
      float sizes[3] = {honest,a,b};
      float * results = game0(3,num_rounds,sizes,strategy);
      float rewards = results[1];
      float rewards2 = results[2];
      int r = (int)(1000*rewards);
      int r2 = (int)(1000*rewards2);
      rewards = ((float)r)/((float)1000);
      rewards2 = ((float)r2)/((float)1000);
      if(rewards>a && rewards2>b){
	rewards = 1;
      }
      else{
	rewards = 0;
      }
      std::ostringstream ss;
      std::ostringstream ss2;
      ss<<rewards;
      std::string s(ss.str());
      std::string k = s+",";
      myfile<<k;
      a = a + inc;
    }
    myfile<<"\n";
     b = b+inc;
  }
  

  return 0;

}


int game1(){
  Block * gen = new Block(-1);
  Chain * chain = new Chain();
  chain->appendBlock(gen,NULL,1); 
  Player ** list = new Player*[3];
  float power[] = {.5,.25,.25};
  for(int i=0;i<3;i++){
     list[i] = new Player(power[i],i+1);
  }
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  
  list[0]->strat = h;
  list[1]->strat = s;
  list[2]->strat = s;
  int num_rounds = 10;
  int num_players = 3;

  std::ofstream myfile;
  myfile.open("test1.txt");
  

  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      int total = rounds(num_players,num_rounds,list,chain);
      
      for(int k=0;k<num_players;k++){


      }

    }
    myfile << "\n";
  }
   
  return 0;
}



  

int game2(){
  Chain * chain = new Chain();
  Player * p1 = new Player(.2,1);
  Player * p2 = new Player(.4,2);
  Player * p3 = new Player(.4,3);
  Player * p4 = new Player(.125,4);
  Player * p5 = new Player(.125,5);
  Player * p6 = new Player(.125,6);
  Block * gen = new Block(-1);
  chain->appendBlock(gen,NULL,1); 
  p1->chain->origin = gen;
  p2->chain->origin = gen;
  p3->chain->origin = gen;
  p4->chain->origin = gen;
  p5->chain->origin = gen;
  p6->chain->origin = gen;
  // p2->gamma=1;
  // p3->gamma=1;
  std::ofstream myfile;
  myfile.open("sdfdf.txt");
  myfile << "sdfd";
  myfile.close();
  
  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  p1->strat = h;
  p2->strat = s;
  p3->strat = s;
  p4->strat = s;
  p5->strat = s;
  p6->strat = s;
  
  int num_players = 3;
  int num_rounds = 5000000;
  Player * arr[] = {p1,p2,p3};
  int total =  rounds(num_players,num_rounds,arr,chain);
  Block * curr = chain->race_set->choose();


  float * m = new float[num_players];
  while(curr!=NULL){
    for(int i=0;i<num_players;i++){
      if(curr->playerID==i+1){
	arr[i]->mined++;
	total++;
	//	printf("BlockID = %d, p%d\n",curr->blockID,arr[i]->id);
      }
    }
    curr = curr->prev;
  }

  
  for(int i=0;i<num_players;i++){
    float m1 = ((float)arr[i]->mined)/total;
    printf("p%d: %f\n",(i+1),m1);
  }
  //  delete chain;
  return 0;
}



int game3(){
  Chain * c = new Chain();
  


  return 0;
}

int main(){
  game2();
  // twoDimPlot(50000);
}
