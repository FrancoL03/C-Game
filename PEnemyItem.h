#ifndef PENEMYITEM_H
#define PENEMYITEM_H
#include "EnemyItem.h"
#include "TileItem.h"
class PEnemyItem : public EnemyItem
{
public:
    PEnemyItem();
    PEnemyItem(QGraphicsScene * scene);
    PEnemyItem(QGraphicsScene * scene, unique_ptr<Enemy> * enemy);
    vector<TileItem*> surrondingTiles;

    void addSurrondingTiles( TileItem * tile);
};

#endif // PENEMYITEM_H
