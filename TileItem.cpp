#include "TileItem.h"
#include <cmath>
TileItem::TileItem()
{

}

TileItem::TileItem(qreal x, qreal y, qreal width, qreal height,float value)
    :QGraphicsRectItem(x,y,width,height)
{
    QColor c(round(value*255),round(value*255),round(value*255));
    setBrush(QBrush(c));
    setPen(QPen(Qt:: NoPen));
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void TileItem::initialize(QGraphicsScene * scene)
{
    scene->addItem(this);
}

QRectF TileItem::boungdingRect() const
{
    return QRectF (0,0,100,100);
}

//void TileItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    QRectF rec = boundingRect();
//    QBrush brush(QColor(100,123,23,255));
//    painter->fillRect(rec,brush);
//    painter->drawRect(rec);
//}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent  *event)
{
    auto wm = WorldModel::ModelInstance();
    if (event->buttons() & Qt::RightButton)
        {
            if(wm->protagonistSelected)
                wm->protagonistSelected=0;
            else{
                wm->setProtagonistViewUnSelectedandStop();
            }
//            emit setSelected(false);
        }
    else{

        if(!wm->getProtagonistDead()){
        auto x = this->rect().x();
        auto y = this->rect().y();
        int r = wm->ratio;
        if((*wm->getProtagonist())->getXPos()*r==x && (*wm->getProtagonist())->getYPos()*r==y)
            wm->protagonistSelected=1;
        if (wm->protagonistSelected){
            wm->setProtagonistPos((int) x/r,(int) y/r);
        }

        pressed = true;
        }

    }
    QGraphicsItem::mousePressEvent(event);
}
