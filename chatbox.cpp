#include "chatbox.h"
#include "ui_chatbox.h"
#include <QTcpSocket>
#include <QTextStream>
#include<QMessageBox>

#include<QMovie>

namespace chatting{
chatbox::chatbox(QString nom,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chatbox)
{
    ui->setupUi(this);
    nickname = nom;
    nSocket=new QTcpSocket(this);
 connect(nSocket,&QTcpSocket::readyRead,[&](){QTextStream T(nSocket);auto text=T.readAll();ui->textEdit->append(text);});
/* QMovie *movie = new QMovie("../2a6-e-charging/img/chat.gif");
   ui->ani->setMovie(movie);
   ui->ani->resize(130,130);
   movie->start();
*/

}

chatbox::~chatbox()
{
    delete ui;
    delete nSocket;
}

void chatbox::on_send_clicked()
{

   QTextStream T(nSocket);

 // ui->textEdit->setTextColor(QColor(29, 185, 84));
 // ui->textEdit->setCurrentFont(QFont("Times New Roman",12));

      //T << QDateTime::currentDateTime().toString("HH:mm:ss") <<"   "<< nickname << " : \n" << ui->message->text();
      T  << "<em><font color=\"#1db954\" face=\"Times New Roman\" size=\"4\" > "<<"[" << QDateTime::currentDateTime().toString("HH:mm:ss")<<"]" <<"   "<< nickname << " : </em><br>" <<  "<font color=\"White\"  face=\"Monospace\" size=\"2\" > " << ui->message->text();





       nSocket->flush();
       ui->message->clear();



}

void chatbox::on_bind_clicked()
{
    /*ChatboxConnectionDialog D(this);
     if(D.exec()==QDialog::Rejected)
     {return;
     }*/
     nSocket->connectToHost("localhost",3333);


}




}
