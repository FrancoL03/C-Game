#include "EnemyItem.h"
#include "QImage"
EnemyItem::EnemyItem()
{

}

EnemyItem::EnemyItem(QGraphicsScene *scene)
    :scene{scene}
{
    pix = scene->addPixmap(QPixmap(":/images/enemy.png"));
}

EnemyItem::EnemyItem(QGraphicsScene *scene, unique_ptr<Enemy> *enemy)
    :scene{scene},enemy{enemy}
{

//    pix = scene->addPixmap(QPixmap(":/images/enemy.png"));
    QImage img (":/images/enemy.png");
    auto b =  QPixmap::fromImage(img);
    pix = scene->addPixmap( b);
    connect(&**enemy,SIGNAL(dead()),this,SLOT(enemyDead()));
    QFont font {};
    font.setPointSize(20);
    font.setStyleHint(QFont::Times);
    text = scene->addSimpleText( QString::number((*enemy)->getValue()),font);

}

void EnemyItem::initialize(int xpos, int ypos)
{   auto wm = WorldModel::ModelInstance();
    text->setPos(xpos,ypos-wm->ratio);
    pix->setOffset(xpos,ypos);
}

void EnemyItem::enemyAttacked(int health)
{
    text->setText(QString::number(health));
}

void EnemyItem::enemyDead()
{
    auto wm = WorldModel::ModelInstance();
    QImage img (":/images/enemy.png");
    img = img.convertToFormat(QImage::Format_Grayscale16);
    auto b =  QPixmap::fromImage(img);
    pix->setPixmap(b);
    wm->enemyDead();
//    disconnect(&**enemy,SIGNAL(dead()),wm,SLOT(enemyDead()));
}
