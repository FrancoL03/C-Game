#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "GraphicView.h"
#include <QMessageBox>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "CustomDialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
    int rect;

//protected:
//    void keyPressEvent(QKeyEvent * event);
private:
    Ui::MainWindow *ui;
//    GraphicView graphicView;
    QMessageBox * messageBox;
    CustomDialog * customDialog;
    void initialize();

    QGridLayout *myGrid;

    //----<RELEVANT>-----
    QComboBox *myComboBox;
//    CustomDialog dialog;
signals:
    void changemap(QString s, int nrhp,int nren);
private slots:

    void on_actionChoose_Map_triggered();
    void gameOver();
    void selectGame(int map,int nrhp,int nren);
    void changeWorld(QString s,int  nrhp,int nren );
    void collideInfo(bool c);
    void healthInfo(int health);
    void win();
    void enemyDead(int i);
};
#endif // MAINWINDOW_H
