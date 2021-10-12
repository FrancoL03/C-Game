#include "HealthPackItem.h"
#include <QFont>
HealthPackItem::HealthPackItem()
{

}

HealthPackItem::HealthPackItem(QGraphicsScene *scene)
    :scene{scene}
{
    pix = scene->addPixmap(QPixmap(":/images/healthpack.png"));
    pix->setOffset(0,0);

}

HealthPackItem::HealthPackItem(QGraphicsScene *scene, unique_ptr<Tile> *healthPack)
    :scene{scene},healthPack{healthPack}
{
    pix = scene->addPixmap(QPixmap(":/images/healthpackBefore.png"));
    pix->setOffset(0,0);
    QFont font {};
    font.setPointSize(20);
    font.setStyleHint(QFont::Times);
    text = scene->addSimpleText(QString::number((*healthPack)->getValue()),font);

}

void HealthPackItem::initialize(int xpos,int ypos)
{
    auto wm = WorldModel::ModelInstance();
    text->setPos(xpos,ypos-wm->ratio);
    pix->setOffset(xpos,ypos);
}

void HealthPackItem::setHealthPack(unique_ptr<Tile> *value)
{
    healthPack = value;
}

void HealthPackItem::takeHP()
{
    text->setText(QString::number(0));
    pix->setPixmap(QPixmap(":/images/healthpack.png"));
    (*healthPack)->setValue(0);
}
