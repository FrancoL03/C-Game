#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <iostream>
#include "world.h"
#include <QBrush>
#include <QPen>
#include<cmath>
#include "worldmodel.h"
#include <QString>
#include "ProtagonistItem.h"
#include <QAction>
//#include <QKeyEvent>
#include <QPushButton>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , messageBox()
    , customDialog()
{
    ui->setupUi(this);
//    ui->setView(&graphicView);  //set graphicView
    QGraphicsScene * scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
//    ui->graphicsView->setScene(scene);
//    scene->addPixmap(QPixmap(":/images/worldmap4.png"));
    QString s = ":/images/worldmap.png";

//    WorldModel * worldmodel =  WorldModel::ModelInstance(s,4,2);
    messageBox = new QMessageBox {this};
    connect(this,SIGNAL(changemap(QString, int ,int )),this,SLOT(changeWorld(QString, int, int)));
//    scene->addItem(&b);
//    b.setOffset(64,64);

    customDialog = new CustomDialog();
    connect(customDialog,SIGNAL(select(int, int ,int)),this,SLOT(selectGame(int , int , int )));

    ui->horizontalSlider->setValue(50);




}

MainWindow::~MainWindow()
{
    delete ui;
}



//void MainWindow::keyPressEvent(QKeyEvent *event)
//{
//    QString text = ui->pressinfo->toPlainText();
//    text += char(event->key());
//    ui->pressinfo->setText(text);
//}

void MainWindow::initialize()
{
    auto wm = WorldModel::ModelInstance();
    auto myProtagonist = wm->getProtagonist();
    connect(myProtagonist->get(),SIGNAL(posChanged(int, int)),&ui->graphicsView->protagonistItem,SLOT(changePos(int, int)));
    connect(&ui->graphicsView->protagonistItem,SIGNAL(Poschanged(int , int )),wm,SLOT(setProtaViewPos(int , int)));
    connect(wm,SIGNAL(protagonistPosUpdated(long )),wm,SLOT(ifcollide(long )));
    connect(wm,SIGNAL(setCollided(long,int)),ui->graphicsView,SLOT(setCollided(long ,int)));
    connect(wm,SIGNAL(setUnCollided()),ui->graphicsView,SLOT(resetCollided()));
    connect(ui->graphicsView,SIGNAL(setC(bool )),this,SLOT(collideInfo(bool)));
    connect(myProtagonist->get(),SIGNAL(healthChanged(int)),this,SLOT(healthInfo(int)));
    connect(wm,SIGNAL(setProtagonistDead()),&ui->graphicsView->protagonistItem,SLOT(protagonistDead()));
    connect(wm,SIGNAL(setProtagonistDead()),ui->graphicsView,SLOT(protagonistDead()));
    connect(wm,SIGNAL(setProtagonistDead()),this,SLOT(gameOver()));
    connect(wm,SIGNAL(win()),this,SLOT(win()));
    connect(wm,SIGNAL(enemydead(int)),this,SLOT(enemyDead(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),&ui->graphicsView->protagonistItem,SLOT(changeSpeed(int)));
    ui->horizontalSlider->setValue(50);
//    connect(wm->my)




    ui->LifeValue->setText(QString::number((*myProtagonist)->getHealth()));
    ui->EnegyValue->setText(QString::number((*myProtagonist)->getHealth()));


}
void MainWindow::on_actionChoose_Map_triggered()
{
    customDialog->exec();
}

void MainWindow::gameOver()
{
    auto wm = WorldModel::ModelInstance();
    disconnect(wm,SIGNAL(setProtagonistDead()),this,SLOT(gameOver()));
    QMessageBox msgBox;
    msgBox.setText("GameOver");
    int ret = msgBox.exec();
    if(ret)
        on_actionChoose_Map_triggered();
}

void MainWindow::selectGame(int map, int nrhp, int nren)
{
    QString s ;
        switch(map){
        case 0:
            s = ":/images/worldmap.png";
            break;
        case 1:
            s = ":/images/worldmap4.png";
            break;
        case 2:
            s = ":/images/maze1.png";
            break;
        case 3:
            s = ":/images/maze2.png";
            break;
        case 4:
            s = ":/images/maze3.png";
            break;
        default:
            cout<<"nothing selected";
            break;
        }
        if (!s.isNull()){
            emit changemap(s,nrhp,nren+4);
            ui->graphicsView->initialize();
            this->initialize();
        }
}

void MainWindow::changeWorld(QString s ,int  nrhp,int nren )
{
    auto m = WorldModel::ModelInstance();
    if (m==0)
    {
        WorldModel::ModelInstance(s,nren,nrhp);
//        m->graphicView = this->graphicView;
    }
    else{
        ui->graphicsView->reset();
        m->changeMap(s,nrhp,nren);
    }

//    WorldModel * worldmodel =  WorldModel::ModelInstance(s,4,2);

}

void MainWindow::collideInfo(bool c)
{
    if(c)
    {
        ui->textEdit->setText("Collided");
    }
    else{
        ui->textEdit->clear();
    }
}

void MainWindow::healthInfo(int health)
{
    auto s = ui->textEdit->toPlainText();
    ui->LifeValue->setText(QString::number(health));
    //    ui->textEdit->setText(s+QString::number(health));
}

void MainWindow::win()
{
    auto wm = WorldModel::ModelInstance();
    disconnect(wm,SIGNAL(setProtagonistDead()),this,SLOT(gameOver()));
    QMessageBox msgBox;
    msgBox.setText("Contratulations");
    msgBox.setInformativeText("YOU WIN!");
    int ret = msgBox.exec();
    if(ret)
        on_actionChoose_Map_triggered();
}

void MainWindow::enemyDead(int i)
{
    ui->pressinfo->setText(QString::number(i));
}





