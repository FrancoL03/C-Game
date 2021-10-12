#include "ProtagonistItem.h"
#include <QTimer>
#include <cmath>
ProtagonistItem::~ProtagonistItem()
{
}

ProtagonistItem::ProtagonistItem()
{
    loop = new QTimer();
    connect(loop, SIGNAL(timeout()), this, SLOT(changePos()));
    loop->setInterval(10);
}

ProtagonistItem::ProtagonistItem(QGraphicsScene *scene)
    :scene{scene}
{
    pix = scene->addPixmap(QPixmap(":/images/pa.png"));
//    pix->setOffset(50,32);
    loop = new QTimer();
    loop->setInterval(10);
    connect(loop, SIGNAL(timeout()), this, SLOT(changePos()));
}

void ProtagonistItem::setScene(QGraphicsScene *value)
{
    scene = value;
    pix = scene->addPixmap(QPixmap(":/images/pa.png"));

}

void ProtagonistItem::setProtagonist()
{
    auto wm = WorldModel::ModelInstance();
    Protagonist = wm->getProtagonist();
}

void ProtagonistItem::attack()
{
    pix->setZValue(20);
    pix->setPixmap(QPixmap(":/images/attack.png"));
    attacking = 1;
}

void ProtagonistItem::changePos(int XPos, int YPos)
{

    auto wm = WorldModel::ModelInstance();
    expectedx = wm->ratio*XPos;
    expectedy = wm->ratio*YPos;
//     = new QTimer();
    loop->interval();
    loop->start();
}

void ProtagonistItem::changePos()
{

    pix->setPixmap(QPixmap(":/images/pa.png"));
    int x = pix->offset().toPoint().x();
    int y = pix->offset().toPoint().y();
    if(x!=expectedx){
        if(x<expectedx)
            pix->setOffset(x+1,y);
        else
            pix->setOffset(x-1,y);
    }else{
        if(y!=expectedy){
            if(y<expectedy)
                pix->setOffset(x,y+1);
            else
                pix->setOffset(x,y-1);
        }else{
            loop->stop();
            emit Poschanged();
        }
    }
    emit Poschanged(pix->offset().toPoint().x(),pix->offset().toPoint().y());
}

void ProtagonistItem::protagonistDead()
{
    pix->setPixmap(QPixmap(":/images/dead.png"));
}

void ProtagonistItem::changeSpeed(int value)
{
    loop->setInterval(round(-9*value/50+19));
}

