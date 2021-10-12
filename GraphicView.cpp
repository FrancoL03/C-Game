#include "GraphicView.h"
#include <cmath>
#include "QKeyEvent"
#include <QTimer>
//GraphicView::GraphicView()
//    :protagonistItem()
//{
//}

GraphicView::GraphicView():protagonistItem()
{

}

GraphicView::GraphicView(QWidget *parent)
    :QGraphicsView(parent),protagonistItem()
{
    collideLoop = new QTimer();
}

//void GraphicView::setScene(QGraphicsScene *value)
//{
//    scene = value;
//}

void GraphicView::initialize()
{
    auto wm = WorldModel::ModelInstance();  //get worldmodel instance
    //protagonist item
    auto tiles = wm->getAllTile();
    QGraphicsRectItem q{};
    scene()->addItem(&q);

    if ((*tiles).size()<250001){
        wm->setRatio(32);
         r = wm->ratio;
            for (uint i = 0; i< (*tiles).size() ; i++ ) {
//                auto tt = *(*tiles)[i];
                auto t = make_unique<TileItem>((double)(*tiles)[i]->getXPos()*r ,(double)(*tiles)[i]->getYPos()*r ,r ,r ,(*tiles)[i]->getValue());
                t->initialize(scene());
//                scene()->addItem(t);
                tileItems.push_back(std::move(t));
            }

    } else{
        scene()->addPixmap(wm->pathName);
    }

    auto healthpack = wm->getallHealthPacks();
    for (uint i = 0; i<(*healthpack).size();i++){
        auto v = make_unique<HealthPackItem> (scene(),&(*healthpack)[i]);
        v->initialize((*healthpack)[i]->getXPos()*r ,(*healthpack)[i]->getYPos()*r );
        long index = (*healthpack)[i]->getXPos()+(*healthpack)[i]->getYPos()*wm->numCol;
        pair<long,unique_ptr<Tile> *> m (index,&(*healthpack)[i]);
        wm->addMappedTiles(index,0);
//        wm->addMappedTiles(m);
        v->mapped = index;
        hpItems.push_back(std::move(v));         //move unique_ptr, v cant be accessed afterwards
    }
    auto enemies = wm->getallEnemies();
    for (uint i = 0; i<(*enemies).size();i++){
        auto p =dynamic_cast<PEnemy*>(&*(*enemies)[i]);
        long index = (*enemies)[i]->getXPos()+(*enemies)[i]->getYPos()*wm->numCol;
        if (p!=nullptr)
        {
            auto pe = make_unique<PEnemyItem> (scene(),&(*enemies)[i]);
            pe->initialize((*enemies)[i]->getXPos()*r ,(*enemies)[i]->getYPos()*r );
            pe->mapped=index;

            QColor c(p->getPoisonLevel(),0,0);

            QColor c2(round(255-p->getPoisonLevel()/2),0,0);

//            auto mp = make_unique<Tile>  (enemies[i]);
//            pair<long,unique_ptr<Tile> *> m (index,&(*enemies)[i]);
            tileItems[index-1]->setBrush(QBrush(c));
            tileItems[index+1]->setBrush(QBrush(c));
            tileItems[index-wm->numCol]->setBrush(QBrush(c));
            tileItems[index+wm->numCol]->setBrush(QBrush(c));

            tileItems[index+wm->numCol+1]->setBrush(QBrush(c2));
            tileItems[index+wm->numCol-1]->setBrush(QBrush(c2));
            tileItems[index-wm->numCol-1]->setBrush(QBrush(c2));
            tileItems[index-wm->numCol+1]->setBrush(QBrush(c2));


//            pe->addSurrondingTiles(tileItems[index-1]);
//            pe->addSurrondingTiles(tileItems[index+1]);
//            pe->addSurrondingTiles(tileItems[index-wm->numCol]);
//            pe->addSurrondingTiles(tileItems[index+wm->numCol]);

//            pe->addSurrondingTiles(tileItems[index+wm->numCol+1]);
//            pe->addSurrondingTiles(tileItems[index+wm->numCol-1]);
//            pe->addSurrondingTiles(tileItems[index-wm->numCol-1]);
//            pe->addSurrondingTiles(tileItems[index-wm->numCol+1]);



            wm->addMappedTiles(index-1,2);
            wm->addMappedTiles(index+1,2);
            wm->addMappedTiles(index-wm->numCol,2);
            wm->addMappedTiles(index+wm->numCol,2);

            wm->addMappedTiles(index+wm->numCol-1,2);
            wm->addMappedTiles(index+wm->numCol+1,2);
            wm->addMappedTiles(index-wm->numCol-1,2);
            wm->addMappedTiles(index-wm->numCol+1,2);
            enemyItems.push_back(std::move(pe));

        }
        else
        {
        auto e = make_unique<EnemyItem> (scene(), &(*enemies)[i]);
        e->initialize((*enemies)[i]->getXPos()*r ,(*enemies)[i]->getYPos()*r );
        e->mapped = index;
        enemyItems.push_back(std::move(e));
        }
        wm->addMappedTiles(index,1);


    }

    protagonistItem.setScene(scene());
    protagonistItem.setProtagonist();
}

void GraphicView::reset()
{
    hpItems.clear();
    enemyItems.clear();
    tileItems.clear();
    scene()->clear();
//    protagonistItem.pix->setOffset(0,0);

}

void GraphicView::keyPressEvent(QKeyEvent *event)
{
    auto wm = WorldModel::ModelInstance();
    auto pressed = event->key();
    if (!wm->getProtagonistDead()){
    switch (pressed){

    case 'A' :
    case 0x01000012:
        if(wm->protagonistSelected)
            wm->setProtagonistPos(round((double)protagonistItem.pix->offset().toPoint().x()/r)-1,round((double)protagonistItem.pix->offset().toPoint().y()/r));
        break;
    case 'W' :
    case 0x01000013:
        if(wm->protagonistSelected)
            wm->setProtagonistPos(round((double)protagonistItem.pix->offset().toPoint().x()/r),round((double)protagonistItem.pix->offset().toPoint().y()/r)-1);
        break;
    case 'D' :
    case 0x01000014:
        if(wm->protagonistSelected)
            wm->setProtagonistPos(round((double)protagonistItem.pix->offset().toPoint().x()/r)+1,round((double)protagonistItem.pix->offset().toPoint().y()/r));
        break;
    case 'S' :
    case 0x01000015:
        if(wm->protagonistSelected)
            wm->setProtagonistPos(round((double)protagonistItem.pix->offset().toPoint().x()/r),round((double)protagonistItem.pix->offset().toPoint().y()/r)+1);
        break;
    case 'J':
       if(wm->protagonistSelected && collided && type==1 && !(*enemyItems[collideIndex]->enemy)->getDefeated()){


            enemyItems[collideIndex]->pix->setPixmap(QPixmap(""));
//            QImage img (":/images/enemy.png");
//            auto b =  QPixmap::fromImage(img);
            auto health = wm->fightEnemy(enemyItems[collideIndex]->enemy);
            connect(this,SIGNAL(proAttack(int)),&(*enemyItems[collideIndex]),SLOT(enemyAttacked(int)));
            emit proAttack(health);
            protagonistItem.attack();
            disconnect(this,SIGNAL(proAttack(int)),&(*enemyItems[collideIndex]),SLOT(enemyAttacked(int)));
        }
        if(wm->protagonistSelected&&collided&&type==0){
            wm->takeHealthPack(hpItems[collideIndex]->healthPack);
            connect(this,SIGNAL(proTakeHP()),&(*hpItems[collideIndex]),SLOT(takeHP()));
            emit proTakeHP();
            disconnect(this,SIGNAL(proTakeHP()),&(*hpItems[collideIndex]),SLOT(takeHP()));

        }


        break;
    case 'K':
        if(wm->protagonistSelected){
//            *protagonistItem.Protagonist->setYPos(*protagonistItem.Protagonist->getYPos()-1);
            int currentx = (*protagonistItem.Protagonist)->getXPos();
            int currenty = (*protagonistItem.Protagonist)->getYPos();
            list<int> pathX = {currentx,currentx,currentx+1,currentx+2,currentx+2};
            list<int> pathY = {currenty,currenty-1,currenty-1,currenty-1,currenty};
            pair<int,int> p1 (currentx,currenty), p2(currentx,currenty-1),p3(currentx+1,currenty-1),p4(currentx+2,currenty-1),p5(currentx+2,currenty);
            list<pair<int,int>> path = {p1,p2,p3,p4,p5};
//            make_pair(currentx,currenty);
//            list<pair<int,int>> list = {make_pair(currentx,currenty),(currentx,currenty-1),(currentx+1,currenty-1),(currentx+2,currenty-1),(currentx+2,currenty)} ;
        //wm->setPath(path);
        }
    default:
        break;
    }
    }
}

void GraphicView::resetCollided()
{
    auto wm = WorldModel::ModelInstance();
    for (uint i = 0;i<hpItems.size();i++){
        hpItems[i]->collided=false;
    }
    for (uint i = 0;i<enemyItems.size();i++){
        enemyItems[i]->collided=false;
        if (!(*enemyItems[i]->enemy)->getDefeated())
        {
            auto p = dynamic_cast<PEnemyItem*>(&*enemyItems[i]);
            if(p!=nullptr)
                enemyItems[i]->pix->setPixmap(QPixmap(":/images/penemy.png"));
            else
                enemyItems[i]->pix->setPixmap(QPixmap(":/images/enemy.png"));
        }
    }

    collided=0;
    type = 10;
    collideLoop->stop();
    disconnect(collideLoop,SIGNAL(timeout()),wm,SLOT(proAttacked()));
    disconnect(collideLoop,SIGNAL(timeout()),this,SLOT(proAttacked()));
    emit setC(false);
}

void GraphicView::setCollided(long mapped, int t)
{
    WorldModel * wm = WorldModel::ModelInstance();
    type = t;
    switch (t) {
    case 0:
        for (uint i = 0;i<hpItems.size();i++){
            if (hpItems[i]->mapped==mapped){
                hpItems[i]->collided=true;
                collideIndex = i;
                wm->collidedIndex=i;
            }
        }
        break;
    case 1:

        for (uint i = 0;i<enemyItems.size();i++){
            if (enemyItems[i]->mapped==mapped){
                enemyItems[i]->collided=true;
                collideIndex = i;
                wm->collidedIndex=i;
            }
        }
        if(!(*enemyItems[collideIndex]->enemy)->getDefeated()&&(enemyItems[collideIndex]->mapped)){
            disconnect (collideLoop,SIGNAL(timeout()),this,SLOT(proPoisoned()));
            disconnect(this,SIGNAL(propoisonedwm(int)),wm,SLOT(proPoisoned(int)));
            connect (collideLoop,SIGNAL(timeout()),wm,SLOT(proAttacked()));
            connect (collideLoop,SIGNAL(timeout()),this,SLOT(proAttacked()));
            collideLoop->start(1000);
        }

        break;
    case 2:
        collideIndex = mapped;
//        disconnect (collideLoop,SIGNAL(timeout()),wm,SLOT(proPoisoned()));
        disconnect (collideLoop,SIGNAL(timeout()),this,SLOT(proPoisoned()));
        disconnect(this,SIGNAL(propoisonedwm(int)),wm,SLOT(proPoisoned(int)));
//        connect (collideLoop,SIGNAL(timeout()),wm,SLOT(proPoisoned()));
        connect (collideLoop,SIGNAL(timeout()),this,SLOT(proPoisoned()));
        disconnect (collideLoop,SIGNAL(timeout()),wm,SLOT(proAttacked()));
        disconnect (collideLoop,SIGNAL(timeout()),this,SLOT(proAttacked()));
        collideLoop->start(1000);
        break;

    }
    collided=1;
    emit setC(true);

}

void GraphicView::proAttacked()
{
    WorldModel * wm = WorldModel::ModelInstance();
    if(!(*enemyItems[collideIndex]->enemy)->getDefeated()&& (*wm->getProtagonist())->getHealth()>0)
    {
        protagonistItem.pix->setPixmap(QPixmap(":/images/attacked.png"));

    }
}

void GraphicView::proPoisoned()
{
    WorldModel * wm = WorldModel::ModelInstance();
    if(tileItems[collideIndex]->brush().color().red()>=0)
      {
        protagonistItem.pix->setPixmap(QPixmap(":/images/attacked.png"));
        connect(this,SIGNAL(propoisonedwm(int)),wm,SLOT(proPoisoned(int)));
        emit propoisonedwm(2*(255-tileItems[collideIndex]->brush().color().red()));

    }
}

void GraphicView::enemyDead()
{
    auto wm = WorldModel::ModelInstance();
    disconnect(collideLoop,SIGNAL(timeout()),wm,SLOT(proAttacked()));
}

void GraphicView::protagonistDead()
{
    WorldModel * wm = WorldModel::ModelInstance();
    if(type==1){
    disconnect(collideLoop,SIGNAL(timeout()),wm,SLOT(proAttacked()));
    disconnect(collideLoop,SIGNAL(timeout()),this,SLOT(proAttacked()));
    enemyItems[collideIndex]->pix->setPixmap(QPixmap());
    }

    switch (type) {
    case 1:
        disconnect(collideLoop,SIGNAL(timeout()),wm,SLOT(proAttacked()));
        disconnect(collideLoop,SIGNAL(timeout()),this,SLOT(proAttacked()));
        enemyItems[collideIndex]->pix->setPixmap(QPixmap());
        break;
    case 2:
        disconnect (collideLoop,SIGNAL(timeout()),this,SLOT(proPoisoned()));
        disconnect(this,SIGNAL(propoisonedwm(int)),wm,SLOT(proPoisoned(int)));
        break;
    default:
        break;
    }
}


