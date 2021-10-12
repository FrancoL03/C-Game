#include "worldmodel.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <QTimer>
WorldModel * WorldModel:: modelInstance = 0;





long WorldModel::getProtagonistPos() const
{
    return protagonistPos;
}

void WorldModel::autoPlay()//get one step path
{
    autoPlaying=true;
   // float cost=0;
    //auto enemy = findtheNearestEnemy();
    //auto path=pathfinder->findPath(myProtagonist->getXPos(),myProtagonist->getYPos(),(*enemy)->getXPos(),(*enemy)->getYPos(),heuristic,diagonalEnable,cost);
    float cost=0;

    path=NULL;
    path=findPathToNearstDefeatableEnemy(myProtagonist->getXPos(),myProtagonist->getYPos(),cost,myProtagonist->getHealth());
    if(path==NULL)
    {
        path=findPathToBestHealth();
        if(path==NULL){
            cout<<"Can not find the solution to win! GOOD LUCK! Bye!!"<<endl;
            autoPlaying=false;
            return ;
        }else{
            if(path->size()!=1)
                setPath(path);
            else
            {
                for(unsigned long long i=0;i<allHealthPacks.size();i++)
                {
                    if(allHealthPacks.at(i)->getXPos()==path->front().first&&allHealthPacks.at(i)->getYPos()==path->front().second)
                        takeHealthPack(&allHealthPacks.at(i));
                }
            }
            //go to the health pack and take it
        }

    }
    else
    {
        if(path->size()!=1)
            setPath(path);
        else
        {
            for(unsigned long long i=0;i<allEnemies.size();i++)
            {
                if(allEnemies.at(i)->getXPos()==path->front().first&&allEnemies.at(i)->getYPos()==path->front().second)
                    fightEnemy(&allEnemies.at(i));
            }
        }
        //go to the enermy and attack it
    }




}
shared_ptr<queue<pair<int,int>>> WorldModel::findPathToNearstEnemy()
{
    if(allEnemies.size()==0)
        return NULL;
    shared_ptr<queue<pair<int,int>>> shortest,temp;
    float distance, shortestDistence;
    if(allEnemies.at(0)->getDefeated())
        shortestDistence=INFINITY;
    else
        shortest=pathfinder->findPath(myProtagonist->getXPos(),myProtagonist->getYPos(),allEnemies.at(0)->getXPos(),allEnemies.at(0)->getYPos(),heuristic,diagonalEnable,shortestDistence);

    for(unsigned long long i=1;i<allEnemies.size();i++)
    {
        if(!allEnemies.at(i)->getDefeated())
        {
            temp=pathfinder->findPath(myProtagonist->getXPos(),myProtagonist->getYPos(),allEnemies.at(i)->getXPos(),allEnemies.at(i)->getYPos(),heuristic,diagonalEnable,distance);
            if(shortestDistence>distance)
                shortest=temp;
        }
    }
    return shortest;
}
shared_ptr<queue<pair<int,int>>> WorldModel::findPathToNearstDefeatableEnemy(int posX,int posY,float& cost,float health)
{
    cost=INFINITY;
    if(allEnemies.size()==0)
        return NULL;
    shared_ptr<queue<pair<int,int>>> shortest,temp;
    float distance, shortestDistence;

    if(allEnemies.at(0)->getValue()>attackPower*health&&!allEnemies.at(0)->getDefeated())
        shortestDistence=INFINITY;
    else
         shortest=pathfinder->findPath(posX,posY,allEnemies.at(0)->getXPos(),allEnemies.at(0)->getYPos(),heuristic,diagonalEnable,shortestDistence);
    for(unsigned long long i=1;i<allEnemies.size();i++)
    {
        if(allEnemies.at(i)->getValue()>attackPower*health&&!allEnemies.at(i)->getDefeated())
        {
            temp=pathfinder->findPath(posX,posY,allEnemies.at(i)->getXPos(),allEnemies.at(i)->getYPos(),heuristic,diagonalEnable,distance);
            if(shortestDistence>distance)
                shortest=temp;
        }
    }
    if(shortestDistence!=INFINITY)
    {
        cost=shortestDistence;
        return shortest;
    }
    cost=INFINITY;
    return NULL;
}
shared_ptr<queue<pair<int,int>>> WorldModel::findPathToBestHealth()
{

    if(allHealthPacks.size()==0)
        return NULL;
    shared_ptr<queue<pair<int,int>>> best=NULL;
    float score=0, bestScore=0;
    for(unsigned long long i=0;i<allHealthPacks.size();i++)
    {
        if(allHealthPacks.at(i)->getValue()!=0)
        {
            score=evaluateHealthPack(allHealthPacks.at(i));
            if(score>bestScore)
            {
                best =pathfinder->findPath(myProtagonist->getXPos(),myProtagonist->getYPos(),allHealthPacks.at(i)->getXPos(),allHealthPacks.at(i)->getYPos(),heuristic,diagonalEnable);

            }
        }

    }
    if(bestScore<=0)
        return NULL;
    return best;
}
float WorldModel::evaluateHealthPack(const unique_ptr<Tile>& pack)
{
    float cost1,cost2,f1,f2,f3;
    pathfinder->findPath(myProtagonist->getXPos(),myProtagonist->getYPos(),allHealthPacks.at(0)->getXPos(),allHealthPacks.at(0)->getYPos(),heuristic,diagonalEnable,cost1);
    f1=(cost1<myProtagonist->getEnergy()?1:0);
    f2=(pack->getValue()+myProtagonist->getHealth()>100?100-myProtagonist->getHealth():pack->getValue());
    findPathToNearstDefeatableEnemy(pack->getXPos(),pack->getYPos(),cost2,myProtagonist->getHealth()+f2);
    f3=myProtagonist->getEnergy()-cost2-cost1;
    return f1*(f2+f3);

}


WorldModel::WorldModel(QString name, int nrOfEnemies, int nrOfHealthpacks)
    :nrEnemies{nrOfEnemies},nrHealthPack{nrOfHealthpacks},pathName{name}
{
    gameWorld = {};
    gameWorld.createWorld(pathName,nrOfEnemies,nrOfHealthpacks,0.25);
    allTile = gameWorld.getTiles();
    allEnemies = gameWorld.getEnemies();
    allHealthPacks = gameWorld.getHealthPacks();
    myProtagonist = gameWorld.getProtagonist();
    numRow = gameWorld.getRows();
    numCol = gameWorld.getCols();
    myProtagonist->setPos(0,0);
    pathfinder=make_shared<pathFinder>(numCol,numRow,allTile);
    connect(this,SIGNAL(protagonistPosUpdated(long)),this,SLOT(setPath()));
    connect(this,SIGNAL(atExpectedTile()),this,SLOT(autoPlay()));
}

bool WorldModel::getProtagonistDead() const
{
    return protagonistDead;
}

void WorldModel::setPath(shared_ptr<queue<pair<int, int> >> path)
{
    this->path = path;
    if(path->size()!=1){
        if(round(ProtagonistViewX/ratio)==path->front().first&&round(ProtagonistViewY/ratio)==path->front().second)
        {
            this->path->pop();
            myProtagonist->setPos(this->path->front().first,this->path->front().second);
        }else{
            myProtagonist->setPos(this->path->front().first,this->path->front().second);
        }
    }
}

void WorldModel::setPath()
{
    if(path!=NULL&!path->empty())
        setPath(path);
    else
    {
        if(autoPlaying)
            emit atExpectedTile();
    }
}


WorldModel* WorldModel::ModelInstance(QString name, int nrOfEnemies, int nrOfHealthpacks)
{
    Create(name, nrOfEnemies, nrOfHealthpacks);
    return modelInstance;
}

WorldModel *WorldModel::ModelInstance()
{
    return modelInstance;
}

void WorldModel::Create(QString name, int nrOfEnemies, int nrOfHealthpacks)
{
     if(modelInstance != 0)
        return;

     modelInstance= new WorldModel(name, nrOfEnemies, nrOfHealthpacks);
}

int WorldModel::fightEnemy(unique_ptr<Enemy> *enemy)
{
    auto attack =  (rand() % (30-5+1))+5;
    auto eneCurValue = (*enemy)->getValue();
    if (eneCurValue!=0){
    if (eneCurValue-attack>0)
    {
        (*enemy)->setValue(eneCurValue-attack);
    }
    else
    {    (*enemy)->setValue(0);
        (*enemy)->setDefeated(true);
    }
    }

    return (*enemy)->getValue();
}

int WorldModel::takeHealthPack(unique_ptr<Tile> *hp)
{
    auto hpHealth = (*hp)->getValue();
    auto protCurHealth = myProtagonist->getHealth();
    if(protCurHealth+hpHealth<=100)
        myProtagonist->setHealth(protCurHealth+hpHealth);
    else
        myProtagonist->setHealth(100);
    return (*hp)->getValue();
}






World * WorldModel::getGameWorld()
{
    return & gameWorld;
}

vector<unique_ptr<Tile> > * WorldModel::getAllTile()
{
    return & allTile ;
}

unique_ptr<Protagonist> *WorldModel::getProtagonist()
{
    return & myProtagonist;
}

vector<unique_ptr<Enemy> > *WorldModel::getallEnemies()
{
    return & allEnemies;
}

vector<unique_ptr<Tile> > *WorldModel::getallHealthPacks()
{
    return & allHealthPacks;
}



void WorldModel::addMappedTiles(long value, int type)
{
    mappedTiles.push_back(value);
    struct mappedTile m{value,type};
    mappedTileS.push_back(m);

}

void WorldModel::addMappedTiles(pair<long, unique_ptr<Tile> *> maptile )
{
    mapTiles.push_back(maptile);
}

//void WorldModel::addMappedTile(struct mappedTile t)
//{
//    mappedTile.push_back(t);
//}

void WorldModel::setRatio(int value)
{
    ratio = value;
}

void WorldModel::setProtaViewPos(int X, int Y)
{
    ProtagonistViewX = X;
    ProtagonistViewY = Y;
    if (ProtagonistViewX%ratio==0&&ProtagonistViewY%ratio==0){
        if(protagonistPos!=((double)ProtagonistViewX/ratio)+((double)ProtagonistViewY/ratio)*numCol){
            protagonistPos = ((double)ProtagonistViewX/ratio)+((double)ProtagonistViewY/ratio)*numCol;
            emit protagonistPosUpdated(protagonistPos);
        }
    }
}

void WorldModel::setProtagonistViewUnSelectedandStop()
{
    setProtagonistPos(round((double)ProtagonistViewX/ratio),round((double)ProtagonistViewY/ratio));
}

void WorldModel::setProtagonistPos(int XPos, int YPos)
{
    if(XPos<0)
        expectedX=0;
    else if(XPos>=numCol)
        expectedX=numCol;
    else
    expectedX = XPos;
    if(YPos<0)
        expectedY=0;
    else if(YPos>=numCol)
        expectedY=numRow;
    else
    expectedY = YPos;
    auto currentX = myProtagonist->getXPos();
    auto currentY = myProtagonist->getYPos();
    if(currentX!=expectedX){
        if(currentX<expectedX)
            myProtagonist->setXPos(currentX+1);
        else
            myProtagonist->setXPos(currentX-1);
        setProtagonistPos(XPos,YPos);
    }else {
        if(currentY!=expectedY){
            if (currentY<expectedY)
                myProtagonist->setYPos(currentY+1);
            else
                myProtagonist->setYPos(currentY-1);
            setProtagonistPos(XPos,YPos);
        }
    }
}


void WorldModel::changeMap(QString name, int nrhp, int nren)
{
    nrEnemies = nren;
    nrHealthPack = nrhp;
    gameWorld.createWorld(name,nren,nrhp);
    allTile.clear();
    allTile = gameWorld.getTiles();
    allEnemies.clear();
    allEnemies = gameWorld.getEnemies();
    allHealthPacks.clear();
    allHealthPacks = gameWorld.getHealthPacks();
    myProtagonist = gameWorld.getProtagonist();
    numRow = gameWorld.getRows();
    numCol = gameWorld.getCols();
    nrEnemiesDefeated=0;
    protagonistCollide=false;
    emit setUnCollided();
    protagonistDead=false;
}



void WorldModel::ifcollide(long protagonistPos)
{
    vector<long>::iterator result = find(mappedTiles.begin(),mappedTiles.end(),protagonistPos);
    if(result==mappedTiles.end())
    {
        protagonistCollide = false;
        emit setUnCollided();
    }
    else
    {
        int nPosition = distance(mappedTiles.begin(),result);
        protagonistCollide = true;
        auto t = mappedTileS[nPosition].type;
        emit setCollided(protagonistPos,t);
    }
}

void WorldModel::proAttacked(int health)
{
    health =  (rand() % (10-5+1))+5;
    auto currentHealth = myProtagonist->getHealth();
    if(!allEnemies[collidedIndex]->getDefeated()&&currentHealth>0)
    {
        if (currentHealth-health>=0)
        myProtagonist->setHealth(currentHealth-health);
        else{
            myProtagonist->setHealth(0);
        }
        if (myProtagonist->getHealth()==0){
            protagonistDead=true;
            emit setProtagonistDead();

        }
    }


}

void WorldModel::proPoisoned(int poisonLevel)
{
    auto currentHealth = myProtagonist->getHealth();
    if(currentHealth>0)
    {
        if (currentHealth-poisonLevel>=0)
        myProtagonist->setHealth(currentHealth-poisonLevel);
        else{
            myProtagonist->setHealth(0);
        }
        if (myProtagonist->getHealth()==0){
            protagonistDead=true;
            emit setProtagonistDead();

        }
    }
}

void WorldModel::enemyDead()
{

     nrEnemiesDefeated++;
         emit enemydead(nrEnemiesDefeated);
     if (nrEnemies==nrEnemiesDefeated){
         emit win();
     }
}



void WorldModel::setProtagonistSelected(bool value)
{
    protagonistSelected = value;
}
