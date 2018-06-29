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
  ~Block();
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
  ~Chain();
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
    return (*strat)(this,c);    
  }
  int mine_block(Chain * chain);
  Player(float hash, int id);
   
};







