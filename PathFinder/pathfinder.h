#ifndef PATHFINDER_H
#define PATHFINDER_H
#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include "pathNode.h"
#include <world.h>

class pathFinder
{
    using node_ptr=std::unique_ptr<PathNode>;
    using pairVector=std::vector<std::pair<int,int>>;
    using pairQueue=std::queue<std::pair<int,int>>;
#define INIT_ESTIMATION -1
public:
    pathFinder(int xBound,int yBound,std::vector<std::unique_ptr<Tile>>& tiles);
    void createNewPath(const std::unique_ptr<PathNode>& node); //create loop-free new pathNodes and add them to the priority_quene
    int coordTranstor(int PosX,int PosY){return PosX+PosY*xBound;}; //transfer 2D coordination to 1D
    std::unique_ptr<std::vector<int>> getPath(const std::unique_ptr<PathNode>& node);//get neighbor nodes
    bool isOptimal(node_ptr& node);
    bool isClosed( node_ptr& node) ;
    std::shared_ptr<pairQueue> findPath(int beginPosX, int beginPosY,int destPosX,int destPosY,float heuristic,bool diagonalEnable);
    //return total cost of the path and the returned Queue from begin to dest
    std::shared_ptr<pairQueue> findPath(int beginPosX, int beginPosY,int destPosX,int destPosY,float heuristic,bool diagonalEnable,float& curCost);

    void setHeuristic(float heuristic);
    void resetPathFinder();//reset the attribute of pathFinder in order to reuse the instance. 
    std::shared_ptr<pairQueue> trackPath();//generate the path returned by findPath func
    std::shared_ptr<pairQueue> trackPath(float& curCost);//generate the path returned by findPath func
    bool checkOutBoundary(int x,int y);
    void setNodeToINF(int x,int y);
private:

    std::unique_ptr<std::vector<node_ptr>> pathNodes;
    std::unique_ptr<std::priority_queue<node_ptr,std::vector<node_ptr>,std::less<node_ptr>>> queue;
    int beginPosX,beginPosY,xBound,yBound,destPosX,destPosY;
    float heuristic;
    bool reset;//used to indicate if the pathfinder is used before
    bool diagonalEnable;//enable diagonal movement
};

#endif // PATHFINDER_H

