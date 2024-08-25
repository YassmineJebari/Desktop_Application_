#include "capteurultrason.h"
#include "ui_capteurultrason.h"
#include <QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsEllipseItem>
#include<QGraphicsProxyWidget>
#include<QGraphicsItemAnimation>
#include <iostream>
#include<string>
#include <algorithm>
#include<math.h>
#include <QDebug>

using std::uint8_t;
using namespace std;

Capteurultrason::Capteurultrason(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Capteurultrason)
{
    ui->setupUi(this);

    animation= new QPropertyAnimation(ui->frame_2,"geometry");
    animation->setDuration(2000);
    animation->setStartValue(QRect(-341,0,341,531));
    animation->setEndValue(ui->frame_2->geometry());
    animation->start();
    ui->graphicsView->setVisible(false);
                ui->car->setVisible(false);
                ui->lineEdit_2->setText("");

   int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(onMsg())); // permet de lancer
     int x=calculerplace();
     if(x==0){ui->lineEdit_3->setText(QString::number(x));
         ui->lineEdit_3->setStyleSheet("QLineEdit {color :red;}");
     A.write_to_arduino("0");
     //A.write_to_arduino("2");
     }
     else{ui->lineEdit_3->setText(QString::number(x));
         ui->lineEdit_3->setStyleSheet("QLineEdit {color :green;}");
     A.write_to_arduino("1");
     //A.write_to_arduino("2");
     }
}

Capteurultrason::~Capteurultrason()
{
    delete ui;
    A.close_arduino();
    qDebug() << "destructeur capteur";
}
bool Capteurultrason::cherchermain(int numborne){
     QString res = QString::number(numborne);
     QSqlQuery query;
  query.prepare("select * from BORNES where NUMBORNE=:NUMBORNE ");
  query.bindValue(":NUMBORNE",numborne);
  query.exec();
  if(!query.next())return false;
  else {
      etatborne=query.value("ETATBORNE").toString();
      return true;}
}

void Capteurultrason::onMsg()
{
    while(A.getserial()->canReadLine())
    {
        data=A.getserial()->readLine();
        data.chop(2);
        qDebug()<<"Data Received: " <<data;
         int station = data.split(':')[0].toInt();
         ui->lineEdit->setText(QString::number(station));
      bool x=cherchermain(station);
        if(x!=false && etatborne=="fonctionelle")
        {
            ui->graphicsView->setVisible(false);
            ui->lineEdit_2->setText("Fonctionelle");
            ui->lineEdit_2->setStyleSheet("QLineEdit {color :green;}");
            qDebug()<<"distance: "<< data.split(':')[1];
            int rawDistance = data.split(':')[1].toInt();
            if(lastPlateValues.size() == 10)
            {
                lastPlateValues.pop_front();
            }

            lastPlateValues.push_back(rawDistance);
            int total = 0;
            int presentNum = 0;
            int totalPresent = 0;
            for(unsigned int s=0;s<lastPlateValues.size();s++)
            {
                total += lastPlateValues[s];
                if(lastPlateValues[s] < 100)
                {
                    presentNum++;
                    totalPresent += lastPlateValues[s];
                }
            }

            int distance = total/lastPlateValues.size();
            if(presentNum == 0) presentNum = 1;
            int distancePresent = totalPresent/presentNum;
            if(distance > 100)
            {
                ui->car->setVisible(false);
                //ui->okplate->setVisible(false);
                lastPlateValues.clear();
            }
            else
            {
               ui->car->setVisible(true);
                int finalDist = max(0,distancePresent-safeZonePlate);
                lastDist = distancePresent;
                ui->car->move(350,210-finalDist*7.8);
                if(finalDist == 0)
                {
                  deuxieme=new deuxiemeinterface(QString::number(station),this);
                  connect(deuxieme, SIGNAL(finished(int)), this, SLOT(onDialogFinished(int)));
                    deuxieme->show();
                    A.close_arduino();
                }

            }

        }
        else if(etatborne!="fonctionelle")
          { A.write_to_arduino("5");
            ui->graphicsView->setVisible(true);
            ui->car->setVisible(false);
            ui->lineEdit_2->setText("En panne");
            ui->lineEdit_2->setStyleSheet("QLineEdit {color :red;}");
        }

    }



}
void Capteurultrason::onDialogFinished(int x){
    delete deuxieme;
        deuxieme = nullptr;
        Capteurultrason::close();

}

int Capteurultrason::calculerplace(){
    int x=0;
    QSqlQuery query,query1;
 query.prepare("select * from BORNES ");
 query.exec();
 while(query.next()){
 if (query.value("ETATBORNE").toString()=="fonctionelle") x++;
 }
 query1.prepare("select * from HISTORIQUE ");
 query1.exec();
 while(query1.next()){
  x--;
 }

    return x;
}

