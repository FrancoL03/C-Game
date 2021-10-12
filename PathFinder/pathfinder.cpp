#include "pathfinder.h"
#include<algorithm>
using namespace std;


pathFinder::pathFinder(int xBound, int yBound, std::vector<std::unique_ptr<Tile> > &tiles)
{
    this->xBound=xBound;
    this->yBound=yBound;
    pathNodes=make_unique<std::vector<node_ptr>>();
    for(unsigned long long i=0;i<tiles.size();i++)
    {
        pathNodes->push_back(make_unique<PathNode>(tiles[i]));
    }

}

shared_ptr<pathFinder::pairQueue> pathFinder::findPath(int beginPosX, int beginPosY,int destPosX,int destPosY,float heuristic,bool diagonalEnable)
{

    if(pathNodes->at(coordTranstor(destPosX,destPosY))->getValue()==INFINITY){
        cout<<"not reachable"<<endl;
        return NULL;
    }
    if(reset)
        resetPathFinder();  //if this pathFinder is not used before
    reset=true;
    this->diagonalEnable=diagonalEnable;
    this->destPosX=destPosX;
    this->destPosY=destPosY;
    this->beginPosX=beginPosX;
    this->beginPosY=beginPosY;
    this->heuristic=heuristic;
    queue=make_unique<priority_queue<std::unique_ptr<PathNode>,vector<node_ptr>,less<node_ptr>>>();
    queue->push(std::make_unique<PathNode>(pathNodes->at(coordTranstor(beginPosX,beginPosY)),0,heuristic,destPosX,destPosY,beginPosX,beginPosX));
    while (!queue->empty()) {
        if(queue->top()->getXPos()==destPosX&&queue->top()->getYPos()==destPosY)
            return trackPath();
        else
            createNewPath(queue->top());
    }
    cout<<"not reachable"<<endl;
    return NULL;
}
shared_ptr<pathFinder::pairQueue> pathFinder::findPath(int beginPosX, int beginPosY,int destPosX,int destPosY,float heuristic,bool diagonalEnable,float& curCost)
{

    if(pathNodes->at(coordTranstor(destPosX,destPosY))->getValue()==INFINITY){
        cout<<"not reachable"<<endl;
        return NULL;
    }
    if(reset)
        resetPathFinder();  //if this pathFinder is not used before
    reset=true;
    this->diagonalEnable=diagonalEnable;
    this->destPosX=destPosX;
    this->destPosY=destPosY;
    this->beginPosX=beginPosX;
    this->beginPosY=beginPosY;
    this->heuristic=heuristic;
    queue=make_unique<priority_queue<std::unique_ptr<PathNode>,vector<node_ptr>,less<node_ptr>>>();
    queue->push(std::make_unique<PathNode>(pathNodes->at(coordTranstor(beginPosX,beginPosY)),0,heuristic,destPosX,destPosY,beginPosX,beginPosX));
    while (!queue->empty()) {
        if(queue->top()->getXPos()==destPosX&&queue->top()->getYPos()==destPosY)
            return trackPath(curCost);
        else
            createNewPath(queue->top());
    }
    cout<<"not reachable"<<endl;
    return NULL;
}
void pathFinder::resetPathFinder()
{
    for(unsigned long long i=0;i<pathNodes->size();i++){
        pathNodes->at(i)->reset();
    }
    //queue->emplace();
}
std::shared_ptr<pathFinder::pairQueue> pathFinder::trackPath()
{
    auto goal=std::make_shared<pairQueue>();
    goal->push(make_pair(destPosX,destPosY));
    auto temp=pathNodes->at(coordTranstor(destPosX,destPosY))->getParents();
    goal->push(temp);
    while (temp.first!=beginPosX||temp.second!=beginPosY) {
        temp=pathNodes->at(coordTranstor(temp.first,temp.second))->getParents();
        goal->push(temp);
    }
    return goal;
}
std::shared_ptr<pathFinder::pairQueue> pathFinder::trackPath(float& curCost)
{
    auto goal=std::make_shared<pairQueue>();

    auto temp=make_pair(destPosX,destPosY);
    goal->push(temp);
    while (temp.first!=beginPosX||temp.second!=beginPosY) {
        curCost=curCost+1-pathNodes->at(coordTranstor(temp.first,temp.second))->getValue();
        temp=pathNodes->at(coordTranstor(temp.first,temp.second))->getParents();
        goal->push(temp);
    }
    return goal;
}

void pathFinder::createNewPath(const std::unique_ptr<PathNode> &node)
{

    float curCost=node->getSCost();
    auto paths=getPath(node);
    int xPos= node->getXPos();
    int yPos=node->getYPos();

    pathNodes->at(coordTranstor(xPos,yPos))->setClosed(true);
    queue->pop();
    std::for_each(paths->begin(),paths->end(),[this,curCost,xPos,yPos](int &n){
        if(isOptimal(pathNodes->at(n))&&!pathNodes->at(n)->getClosed()){
            this->queue->push(make_unique<PathNode>(pathNodes->at(n), curCost,heuristic,destPosX,destPosY,xPos,yPos));
            pathNodes->at(n)->setParent(make_pair(xPos,yPos));
        }
    });

}

std::unique_ptr<std::vector<int>> pathFinder::getPath(const std::unique_ptr<PathNode> &node)
{
    int x=node->getXPos();
    int y=node->getYPos();
    auto temp=make_unique<vector<int>>();

    if(checkOutBoundary(x+1,y))
        temp->push_back(coordTranstor(x+1,y));
    if(checkOutBoundary(x-1,y))
        temp->push_back(coordTranstor(x-1,y));
    if(checkOutBoundary(x,y+1))
        temp->push_back(coordTranstor(x,y+1));
    if(checkOutBoundary(x,y-1))
        temp->push_back(coordTranstor(x,y-1));
    if(diagonalEnable){
        if(checkOutBoundary(x+1,y+1))
            temp->push_back(coordTranstor(x+1,y+1));
        if(checkOutBoundary(x-1,y-1))
            temp->push_back(coordTranstor(x-1,y-1));
        if(checkOutBoundary(x-1,y+1))
            temp->push_back(coordTranstor(x-1,y+1));
        if(checkOutBoundary(x+1,y-1))
            temp->push_back(coordTranstor(x+1,y-1));
    }
    return move(temp);
}

bool pathFinder::checkOutBoundary(int x,int y){
    if(x>=0&&x<xBound&&y>=0&&y<yBound)
        return true;
    else
        return false;
}

void pathFinder::setNodeToINF(int x, int y)
{
    pathNodes->at(coordTranstor(x,y))->setValue(INFINITY);
}

bool pathFinder::isOptimal(pathFinder::node_ptr &node)
{
    if(node->getOptimalEstimation()==INIT_ESTIMATION||node->getOptimalEstimation()>node->getFCost())
    {
        if(node->getValue()==INFINITY)
            return false;
        node->setOptimalEstimation(node->getFCost());
        return true;
    }
    else
        return false;

}

bool pathFinder::isClosed(node_ptr &node )
{
    return node->getClosed();
}



