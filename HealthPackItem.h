#ifndef HEALTHPACKITEM_H
#define HEALTHPACKITEM_H
#include "world.h"
#include <memory>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <iostream>
#include <QObject>
#include "worldmodel.h"
using namespace std;
class HealthPackItem : public QObject
{
    Q_OBJECT
public:
    HealthPackItem();
    HealthPackItem(QGraphicsScene * scene);
    HealthPackItem(QGraphicsScene * scene,unique_ptr<Tile> * healthPack);

    QGraphicsPixmapItem * pix;
    QGraphicsSimpleTextItem * text;
    void initialize(int xpos,int ypos);
    void setHealthPack(unique_ptr<Tile> *value);
    long mapped;
    bool collided=0;
    void resetCollided() {collided=0;};
    unique_ptr<Tile> * healthPack;
private:
    QGraphicsScene * scene;
public slots:
    void takeHP();

};

#endif // HEALTHPACKITEM_H
