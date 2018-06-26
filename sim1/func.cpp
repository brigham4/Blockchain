#include <stdio.h>
#include <stdlib.h>
#include<set>
#include<map>

class Block{
public:
  static int count;
  int playerID;
  int blockID;
  int ID;
  Block * next;
  Block * prev;
  bool operator<(const Block & b);
  Block(int id);
};


class RaceSet{
public:
  bool r;
  int size;
  Block ** options;
  float prop [3];
  float prop2 [3];
  std::map <Block *, float> * optgamma;
  std::set<Block *> opts;
  Block * choose();
  int add(Block * n, Block * t,float gamma);
  int getLength();
  int numTied();
  int prune();
  RaceSet();
};



class Chain{
public:
  bool race;
  int length;
  Block * origin;
  Block * head;
  Block * tail;
  RaceSet * race_set;
  int mineBlock(int id);
  int appendBlock(Block * b, Block * target,float gamma);
  Chain();
};

class Player{
public:
  int id;
  int mined;
  float hash;
  float gamma;
  Chain * chain;
  int (*strat)(Player * p, Chain * c);
  int strategy(Chain * c){
    (*strat)(this,c);
    return 0;
  }
  int mine_block(Chain * chain);
  Player(float hash, int id);
   
};


bool Block::operator<(const Block & b1){
  return this->ID<b1.ID;
}

int RaceSet::numTied(){
  int num = 0;
  int len = this->getLength();
  /*  return num;*/

  num = 0;
  std::map<Block *,float>::iterator it = optgamma->begin();
  while(it!=optgamma->end()){
    if(it->first->blockID==len){
      num++;
    }
    it++;
  }
  return num;

  
}




int RaceSet::getLength(){
  int i = 0;
  int max = 0;
  
  /*  return max;*/
  max = 0;
  
  std::map<Block *,float>::iterator it = optgamma->begin();
  while(it != optgamma->end()){
    int b =it->first->blockID; 
    if(max<b){
      max = b;
    }
    it++;
  }
  return max;

  
  for(i = 0;i<3;i++){
    if(this->options[i]->blockID>max){
      max = this->options[i]->blockID;
    }   
  }
  return max;
  
}


int RaceSet::prune(){

  int max = this->getLength();
  
  int i = 0;
  int length = optgamma->size();
   std::map<Block *,float>::iterator it = optgamma->begin();
  for(i=0;i<length;i++){
    int flag = 0;
    it = optgamma->begin();
    while(flag==0 && it!=optgamma->end()){

      if(it->first->blockID<max){

	 optgamma->erase(it->first);
        flag=1;
      }
      else{
         it++;
      }
    }
  }

  return 0;
}



RaceSet::RaceSet(){
  this->size = 0;
  this->options = new Block*[3];
  this->optgamma = new std::map <Block *, float>();
}


int RaceSet::add(Block * n, Block * t,float gamma){
  int i = 0;
  Block * curr=NULL;
  
 
  std::map<Block *,float>::iterator it = optgamma->begin();
  while(it!=optgamma->end()){
    if(it->first==t){

      optgamma->erase(it);
      optgamma->insert(std::pair <Block *,float> (n,gamma));
      return 0;
    }
    it++;
  }
  optgamma->insert(std::pair <Block *,float> (n,gamma));

  
  
  return 0;
}






Block * RaceSet::choose(){
  float r = ((float)rand())/((float)RAND_MAX);
  float total = 0;
  float curr = 0;
  int i = 0;
  Block * b = NULL;

  /*  printf("total = %f\n",total);*/
 
  /* while(i<3){
    curr = curr + (this->prop2[i])/(total);
    
    if(r<=curr){
      if(i==0){
	return t1;
      }
      else if(i==1){
	return t2;
      }
      else{
	return t3;
      }


      i = 4;
      b = this->options[i];
    }
    i++;
    }*/

  float total2 = 0;
  std::map<Block *,float>::iterator it = this->optgamma->begin();
  while(it != optgamma->end()){
    total2 = total2 + it->second;
    it++;
  }
  std::map<Block *,float>::iterator it2 = this->optgamma->begin();
  float curr2 = 0;
  while(it2 != optgamma->end()){
    curr2 = curr2 + (it2->second/total2);
    if(r<=curr2){
      /* if(it2->first->blockID==this->getLength()){*/
         return it2->first;
    }
    else{
      it2++;
    }
  }


  

  return NULL;
}



int Player::mine_block(Chain * chain){  
  this->chain->mineBlock(this->id); 
  return 0;
}


int Block::count = 0;


Block::Block(int id){
  this->blockID = count;
  this->ID = count;
  this->count++;
  this->playerID = id;
  this->prev = NULL;
  this->next = NULL;
}


int Chain::appendBlock(Block * b, Block * target,float gamma){

  if(this->length==0){/*if chain is empty*/
    this->head = b;
    this->tail = b;
    b->prev = NULL;
    this->length++;
   
    b->blockID = 0;
    this->race_set->add(b,NULL,gamma);
  }
  else{

    b->prev = target;

    b->blockID = target->blockID + 1;

    target->next = b;
    this->race_set->add(b,target,gamma);
    if(this->tail==target){
      this->tail=this->tail->next;
      this->length++;	
    }
    b->next = NULL;
  
  }
  return 0;
}


Chain::Chain(){
  this->race = false;
  this->length = 0;
  this->origin = NULL;
  this->head = NULL;
  this->tail = NULL;
  RaceSet * r = new RaceSet();
  this->race_set = r;
}


int Chain::mineBlock(int id){

  Block * b = new Block(id);

  if(this->head==NULL){
    this->head = b;
    this->tail = b;
    this->length++;
    if(this->origin!=NULL){

      b->blockID = this->origin->blockID + 1;

    }
    else{
      b->blockID = 0;
    }
  }
  else{

    this->tail->next = b;

    b->prev = this->tail;
    b->blockID = this->tail->blockID + 1;
    this->tail = this->tail->next;
    this->length++;

  }
  return 0; 
}


Player::Player(float hash, int id){
  this->hash = hash;
  this->id = id;
  Chain * c = new Chain();
  this->chain = c;
  this->gamma = 1000;
  this->mined = 0;
}







int overide(Player * p, Chain * c){
  Block * f = p->chain->origin;
  f->next = p->chain->head;
  c->length = c->length + p->chain->length;
  p->chain->origin = p->chain->tail;
  p->chain->head = NULL;
  p->chain->tail = NULL;
  p->chain->length = 0;
  /*re number blocks*/
  Block * curr = f->next;
  while(curr!=NULL){
    curr->blockID = curr->prev->blockID + 1;
    curr = curr->next;
  }
  return 0; 
}







int selfish(Player * p, Chain * c){
  
   Block * f = p->chain->origin;
  
   int len = p->chain->length + f->blockID;

   int comp = c->race_set->getLength();
   int pub_len = p->chain->origin->blockID;
  
   if((len<comp||((p->chain->length==0) && ((p->chain->origin->playerID)!=(p->id))))){
     /*give up*/
     printf("h1\n");
     printf("p %d\n",p->id);
     p->chain->origin = c->race_set->choose();
     p->chain->head = NULL;
     p->chain->tail = NULL;
     p->chain->length = 0;
     return 0;
   }
   else if(len==comp && len!=0 && p->chain->length!=0){/*race*/
     printf("h2\n");
     printf("p %d\n",p->id);
     Block * b = p->chain->tail;
     p->chain->length = 0;
     p->chain->head = NULL;
     p->chain->tail = NULL;
     p->chain->origin = b; 
     c->appendBlock(b,f,p->gamma);

     return 1;
     /*release - len should equal 1 in this case*/
   }
   else if(c->race_set->numTied()>1 && len==(comp+1)){
       printf("h3\n");
         printf("p %d\n",p->id);
     Block * n = p->chain->tail;
     Block * n2 = p->chain->head;
     n2->prev = p->chain->origin;
     c->appendBlock(n,n->prev,p->gamma);
     p->chain->length=0;
     p->chain->tail=NULL;
     p->chain->head = NULL;
     return 1;
   }
   else if(len==(comp+1) && pub_len<comp){/*release both blocks*/
           printf("h4\n");
	     printf("p %d\n",p->id);
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
   else if((len-comp)>=2 && pub_len<comp){/*publish 1 block*/
     printf("h5\n");
       printf("p %d\n",p->id);
     Block * n = p->chain->head;
     p->chain->head = p->chain->head->next;
     p->chain->origin = n;
     p->chain->length--;
     c->appendBlock(n,f,p->gamma); 
     return 1;
   }
   else{/*do nothing*/
     /*  printf("do nothing\n");*/
     return 0;
   }
   
}




int honestMine(Player * p, Chain * c){
  float gamma = 0.5;
  int len = p->chain->length;
  if(len==0){
    p->chain->origin = c->race_set->choose();

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
      
  }
  chain->race_set->prune();
  for(i=0;i<num_players;i++){
    p = players[i];
    p->strategy(chain);
  }
  return 0;
}



int rounds(int num_players, int rounds,Player ** players ,Chain * chain){
  Player * p1 = players[0];
  Player * p2 = players[1];
  float h1 = p1->hash;
  float h2 = p2->hash;
  int i = 0;
  float vec[10]={.1,.1,.7,.7,.8,.8,.1,.8,.1,.1};  
  for(i=0;i<rounds;i++){
    int j = 0;
    float total = 0;
    Player * p;
    float r = ((float)rand())/((float)RAND_MAX);

    r=vec[i];
  
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
   return 0;
}



  

int main(){

  Chain * chain = new Chain();
  Player * p1 = new Player(.5,1);
  Player * p2 = new Player(.25,2);
  Player * p3 = new Player(.25,3);
  Player * p4 = new Player(0,4);
  Block * gen = new Block(-1);
  chain->appendBlock(gen,NULL,1); 
  p1->chain->origin = gen;
  p2->chain->origin = gen;
  p3->chain->origin = gen;
  p4->chain->origin = gen;
  

  int (*h)(Player * p, Chain * c) = honestMine;
  int (*s)(Player * p, Chain * c) = selfish;
  p1->strat = h;
  p2->strat = s;
  p3->strat = s;
  p4->strat = s;
  int num_players = 3;
  int num_rounds = 10;
  Player * arr[3] = {p1,p2,p3};
  rounds(num_players,num_rounds,arr,chain);
  Block * curr = chain->race_set->choose();
  while(curr!=NULL){
    if(curr->playerID==1){     
      p1->mined++;
      printf("blockID = %d\n",curr->blockID);
    }
    else if(curr->playerID==2){
      printf("blockID = %d\n",curr->blockID);
      p2->mined++;
    }
    else if(curr->playerID==3){
      p3->mined++;
       printf("blockID = %d\n",curr->blockID);
    }
    else{
      p4->mined++;
        printf("blockID = %d\n",curr->blockID);
    }
    curr=curr->prev;
  }
  float m11 = (float)p1->mined;
  float m22 = (float)p2->mined;
  float m33 = (float)p3->mined;
  float m44 = (float)p4->mined;
  float m1 = m11/(m11+m22+m33+m44);
  float m2 = m22/(m11+m22+m33+m44);
  float m3 = m33/(m11+m22+m33+m44);
  float m4 = m44/(m11+m22+m33+m44);
  printf("p1: %f, p2: %f, p3: %f, p4: %f\n",m1,m2,m3,m4);
 
}







