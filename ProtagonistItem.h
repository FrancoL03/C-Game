#ifndef PROTAGONISTVIEW_H
#define PROTAGONISTVIEW_H
#include "world.h"
#include <memory>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <iostream>
#include "worldmodel.h"
using namespace std;
class ProtagonistItem : public QObject
{
    Q_OBJECT
public:
    ~ProtagonistItem();
    ProtagonistItem();
//    ProtagonistView(unique_ptr<Protagonist> * healthPack);

    ProtagonistItem(QGraphicsScene * scene);

    QGraphicsPixmapItem * pix;
    QTimer * loop;
    void setScene(QGraphicsScene *value);

    void setProtagonist();
    bool selected=0;
    bool attacking=0;
    unique_ptr<Protagonist> * Protagonist;
private:

    QGraphicsScene * scene;
    int expectedx;
    int expectedy;
signals:
    void Poschanged(int XPos, int YPos);
    void Poschanged();
public slots:
    void attack();
    void changePos(int XPos, int YPos);
    void changePos();
    void protagonistDead();
    void changeSpeed(int value);
};

#endif // PROTAGONISTVIEW_H
