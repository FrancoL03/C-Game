#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H
#include <QDialog>
#include <QStringList>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
class CustomDialog  :   public QDialog
{
    Q_OBJECT
public:
    CustomDialog()
        {

            QGridLayout *gridLayout = new QGridLayout;
//            setLayout(new QFormLayout());

            box = new QComboBox;

            mapLabel = new QLabel(tr("Map:"));
            nrHealthpack = new QLabel(tr("Number fo Health Packs:"));
            nrEnemy = new QLabel(tr("Number of Enemies:"));

            hbox = new QComboBox;
            ebox = new QComboBox;
            QStringList itemList({"Map1", "Map2", "Maze1","Maze2","Maze3"});
            box->addItems(itemList);
            hbox->addItems(QStringList {"0","1","2","3","4"});
            ebox->addItems(QStringList {"4","5","6","7","8"});
            gridLayout->addWidget(mapLabel,0,0);
            gridLayout->addWidget(box,0,1);
            gridLayout->addWidget(nrHealthpack,1,0);
            hbox->setCurrentIndex(2);
            gridLayout->addWidget(hbox,1,1);
            gridLayout->addWidget(nrEnemy,2,0);
            gridLayout->addWidget(ebox,2,1);

            QPushButton* ok = new QPushButton("ok");
            QPushButton* cancel = new QPushButton("cancel");



            gridLayout->addWidget(ok);
            gridLayout->addWidget(cancel);
            connect(ok, &QPushButton::clicked, this, [this]()
            {
                emit select(box->currentIndex(),hbox->currentIndex(),ebox->currentIndex());
                accept();
            });
            setLayout(gridLayout);
        }

        QComboBox* comboBox() { return box; }

    private:
        QLabel * mapLabel;
        QLabel * nrHealthpack;
        QLabel * nrEnemy;
        QComboBox* box;
        QComboBox* hbox;
        QComboBox* ebox;
signals:
        void select(int map, int hl, int enemy);


};

#endif // CUSTOMDIALOG_H
