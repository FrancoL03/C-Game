#ifndef PATHNODE_H
#define PATHNODE_H
#include"world.h"
#include<iostream>
#include<memory>
#include<cmath>
#define INIT_ESTIMATION -1
class PathNode : public Tile
{
    using Tile::Tile;

private:

    float hCost;    //heuristic cost
    float sCost=0;    //curriculum cost
    float heuristic=1;
    std::pair<int,int> parent=std::make_pair(0,0);
    float optimalEstimation=INIT_ESTIMATION;
    bool  closed =false;
public:
    PathNode(const std::unique_ptr<PathNode>& other,float curCost,float heuristic, int destX,int destY,int currentX,int currentY);
    PathNode(std::unique_ptr<Tile>& tile);  //only used for initialization, so don't need heuristic and dest


    std::pair<int,int> getParents()const {return parent;};
    float getHCost()const {return hCost;} ;
    float getSCost()const {return sCost;} ;
    void setSCost(float sCost){this->sCost=sCost;};
    void setClosed(bool closed){this->closed=closed;};
    bool getClosed()const{return closed;};
    float getCost()const {return value;} ;
    void setParent(std::pair<int,int> parent){this->parent=parent;};
    float getFCost()const{return heuristic*hCost+sCost;};
    void setValue(float value){this->value=value;};
    void  setHeuristic(float heuristic){this->heuristic=heuristic;};
    float  getOptimalEstimation()const{return optimalEstimation;};
    void    setOptimalEstimation(float optimalEstimation){this->optimalEstimation=optimalEstimation;};
    void    reset(){optimalEstimation=INIT_ESTIMATION;closed=false;};//Csalled by resetPathFinder in PathFinder class
    friend bool operator<(const std::unique_ptr<PathNode>& other,const std::unique_ptr<PathNode>& another)
    { return (another->getFCost())<(other->getFCost());};
};








#endif // PATHNODE_H
