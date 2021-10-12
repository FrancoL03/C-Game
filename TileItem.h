#ifndef TILEITEM_H
#define TILEITEM_H
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QDebug>
#include <iostream>
#include <QObject>
#include "QRectF"
#include "worldmodel.h"
#include <QGraphicsSceneMouseEvent>
//#include <QKeyEvent>
using namespace std;
class TileItem  : public QGraphicsRectItem
{
public:
    TileItem();
    TileItem(qreal x, qreal y, qreal width, qreal height,float value);
    bool pressed=0;
    void initialize(QGraphicsScene * scene);
    QRectF boungdingRect () const;
//    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
private:
    int xpos;
    int ypos;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
//    void setSelected(bool);
    void itemPressed(int x, int y);

};

#endif // TILEITEM_H
