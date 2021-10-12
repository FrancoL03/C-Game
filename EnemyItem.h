#ifndef ENEMYITEM_H
#define ENEMYITEM_H
#include "world.h"
#include <memory>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <iostream>
#include "worldmodel.h"
using namespace std;
class EnemyItem :public QObject
{
    Q_OBJECT
public:
    EnemyItem();
    EnemyItem(QGraphicsScene * scene);
    EnemyItem(QGraphicsScene * scene, unique_ptr<Enemy> * enemy);

    QGraphicsPixmapItem * pix;
    QGraphicsSimpleTextItem * text;
    virtual void initialize(int xpos, int ypos) ;
    long mapped;
    bool collided=0;
    void resetCollided() {collided=0;};
    unique_ptr<Enemy> * enemy;
private:

    QGraphicsScene * scene;

public slots:
    void enemyAttacked(int health);
    void enemyDead();
};

#endif // ENEMYITEM_H
