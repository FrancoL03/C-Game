#include"pathNode.h"
#include <iostream>
#include<algorithm>
#include<iterator>
#include"world.h"

PathNode::PathNode(const std::unique_ptr<PathNode>& other,float curCost,float heuristic, int destX,int destY,int currentX,int currentY):
    Tile{other->xPos,other->yPos,other->value},heuristic{heuristic}
{
   this->hCost=abs(xPos-destX)+abs(yPos-destY);
   if(abs(xPos-currentX)+abs(yPos-currentY)==2)
       value=other->value*1.414;   //check if move in diagonal
    sCost=curCost+value;
}

PathNode::PathNode(std::unique_ptr<Tile> &tile):
    Tile{0,0,0}
{
    this->xPos=tile->getXPos();
    this->yPos=tile->getYPos();
    if(tile->getValue()!=INFINITY)
        this->value=1-tile->getValue();
    else
        this->value=tile->getValue();
}
