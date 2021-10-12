#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include "world.h"
#include "./PathFinder/pathfinder.h"
#include <vector>
#include "memory"
#include <QSound>
#include <QObject>
//#include "GraphicView.h"
    using namespace std;

class WorldModel : public QObject

{

    Q_OBJECT
    struct mappedTile{
      long index;
      int type;
    };

private:
    World gameWorld;
    vector <unique_ptr<Tile>> allTile;
    vector <unique_ptr<Enemy>> allEnemies;
    vector <unique_ptr<Tile>> allHealthPacks;
//    vector<unique_ptr<PoisonTile>> allPoisonTiles;
    unique_ptr<Protagonist> myProtagonist;
    vector<mappedTile> mappedTileS;
    vector<long> mappedTiles;
    long protagonistPos=0;
    int nrEnemiesDefeated=0;
    int nrEnemies;
    int nrHealthPack;

    int ProtagonistViewX;
    int ProtagonistViewY;
//    QSound * attackSound;   // Coach 说放在Window里

    WorldModel(QString name, int nrOfEnemies, int nrOfHealthpacks);
    static WorldModel * modelInstance;
    bool protagonistCollide=0;

    bool protagonistDead=0;
    int expectedX;
    int expectedY;
    vector<pair<long, unique_ptr<Tile> *>> mapTiles;
    shared_ptr<queue<pair<int,int>>> path;
    shared_ptr<pathFinder> pathfinder;
    float heuristic=0;
    bool diagonalEnable=false;
    float attackPower=2;
    bool GameOver=false;
    bool autoPlaying=false;
public:
    QString pathName;
    ~WorldModel()=default;
    static WorldModel * ModelInstance(QString name, int nrOfEnemies, int nrOfHealthpacks);
    static WorldModel * ModelInstance();
    static void Create(QString name, int nrOfEnemies, int nrOfHealthpacks);

    int collidedIndex;
    int numRow;
    int numCol;
    void moveTo(int x, int y);
    bool checkPositionValid(int x, int y);
    int fightEnemy(unique_ptr<Enemy> * enemy);
    int takeHealthPack(unique_ptr<Tile> * hp);
    unique_ptr<Enemy> * findtheNearestEnemy();

    unique_ptr<Tile> * findtheNearestHealthPack();
    std::shared_ptr<std::queue<std::pair<int,int>>> findPathToNearstDefeatableEnemy(int posX,int posY,float& cost,float health);
    std::shared_ptr<std::queue<std::pair<int,int>>> findPathToNearstEnemy();
    //return NULL if don't have enough energy, or no enemy
    std::shared_ptr<std::queue<std::pair<int,int>>>findPathToBestHealth();
    //return NULL if don't have enough energy, or no enemy

    float evaluateHealthPack(const unique_ptr<Tile>& pack);

    bool checkWin(Enemy * e);
    World * getGameWorld() ;
    vector<unique_ptr<Tile> > * getAllTile() ;
    unique_ptr<Protagonist> * getProtagonist();
    vector <unique_ptr<Enemy>> * getallEnemies();
    vector <unique_ptr<Tile>> * getallHealthPacks();



    void addMappedTiles(long value,int type);
    void addMappedTiles(pair<long, unique_ptr<Tile> *> maptile);
//    void addMappedTile(struct mappedTile t);
    bool protagonistSelected=0;
    int ratio;
    void setProtagonistPos(int XPos, int YPos);

    void changeMap(QString name,int  nrhp,int nren );
    void setProtagonistSelected(bool value);

    void setRatio(int value);
    void setProtagonistViewUnSelectedandStop();

    bool getProtagonistDead() const;
    void setPath(shared_ptr<queue<pair<int,int>>> path);

    long getProtagonistPos() const;



signals:
    void setProtagonistViewUnSelected(int XPos, int YPos);
    void protagonistPosUpdated(long protagonistPos);
    void setCollided(long mapped,int type);
    void setUnCollided();
    void setProtagonistDead();
    void enemydead(int i);
    void win();
    void atExpectedTile();
public slots:
    void autoPlay();
    void setPath();
    void setProtaViewPos(int X, int Y);
    void ifcollide(long protagonistPos);
    void proAttacked(int health=25);
    void proPoisoned(int poisonLevel);
    void enemyDead();
};

#endif // WORLDMODEL_H
