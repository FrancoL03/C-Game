#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "ProtagonistItem.h"
#include "HealthPackItem.h"
#include "EnemyItem.h"
#include <QObject>
#include "world.h"
#include <memory>
#include "TileItem.h"
//#include "MainWindow.h"
#include "iostream"
#include <QTimer>
#include "PEnemyItem.h"
using namespace std;
class GraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicView();
    GraphicView(QWidget*parent=nullptr);
//    QGraphicsView * graphicsView;
    ProtagonistItem protagonistItem;
    vector<unique_ptr<HealthPackItem>> hpItems;
    vector<unique_ptr<EnemyItem>> enemyItems;
    vector<unique_ptr<TileItem>> tileItems;
//    QGraphicsScene * scene;

//    void setScene(QGraphicsScene *value);
    void initialize();
    void reset();

    bool collided=0;
    long collideIndex;
    int type=10;
protected:
    void keyPressEvent(QKeyEvent * event);
private:
    QTimer * collideLoop;
    int r;
signals:
    void proAttack(int health);
    void proTakeHP();
    void setC(bool collided);
    void propoisonedwm(int poisionLevel);
public slots:
    void resetCollided();
    void setCollided(long mapped,int type);
    void proAttacked();
    void proPoisoned();
    void enemyDead();
    void protagonistDead();
};

#endif // GRAPHICVIEW_H
