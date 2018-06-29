#include "definitions.hpp"
#include<stdio.h>
#include<stdlib.h>





int RaceSet::numTied(){
  int num = 0;
  int len = this->getLength();
  //  return num;
 
  num = 0;
  std::map<Block *,float>::iterator it = optgamma->begin();
  while(it!=optgamma->end()){
    if(it->first->blockID==len){
      num++;
    }
    it++;
  }
  //   printf("numTied=%d\n",num);
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


Block::~Block(){
  
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


Chain::~Chain(){
  Block * curr = this->race_set->choose();
  while(curr->prev!=NULL){
  if(curr->prev!=NULL){
    curr = curr->prev;
    delete curr->next;
  }
  }
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











