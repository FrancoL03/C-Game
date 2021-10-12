#include "PEnemyItem.h"

PEnemyItem::PEnemyItem()
{

}

PEnemyItem::PEnemyItem(QGraphicsScene *scene, unique_ptr<Enemy> *enemy):
    EnemyItem(scene,enemy)
{
    pix->setPixmap(QPixmap(":/images/penemy.png"));
}

void PEnemyItem::addSurrondingTiles(TileItem * tile)
{
    surrondingTiles.push_back(tile);
}


