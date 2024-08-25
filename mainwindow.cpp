#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "application.h"
#include <QString>
#include <QMessageBox>
#include "mainwindow.h"
#include <QMovie>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui -> setupUi(this);
  ui -> stackedWidget -> setCurrentIndex(0);
  mail_regex = QRegExp("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");



  int ret=A.connect_arduino(); // lancer la connexion à arduino
  switch(ret){
  case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
      break;
  case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
     break;
  case(-1):qDebug() << "arduino is not available";
  }
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
   //le slot update_label suite à la reception du signal readyRead (reception des données).
    //QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(on_enter_clicked())); // permet de lancer
    QMovie *movie = new QMovie("../2a6-e-charging/img/back.gif");
      ui->ani->setMovie(movie);
      movie->start();

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_exist_clicked()
{
  MainWindow::close();

}


void MainWindow::update_label()
{
          data=A.read_from_arduino();

           //data=A.getserial()->readLine();
           data_text += QString::fromStdString(data.toStdString());

           if (data_text.endsWith("\r\n"))
           //if(data_text =="E326C22E\r\n")
               {
               data_text = data_text.trimmed();
               qDebug()<<"Data Received: " <<data_text;

                QSqlQuery query;
                  query.exec("select * from AUTHENTIFICATION where RFID='"+data_text+"'");
                  bool rfid_found = false;

                   while (query.next()) {
                   // query.first();
                 // ui->nameenter->setText("Bonjour "+query.value(2).toString());
                       rfid_found = true;
                       A.close_arduino();

                        a = new application(log -> fetch_id(query.value(0).toString()),query.value(2).toString());
                       connect(a, SIGNAL(finished(int)), this, SLOT(onDialogFinished(int)));

                  a -> setcurrent_user(query.value(2).toString());
                 a -> show();
                 MainWindow::close();
                   data = "";
                   data_text = "";

   }

                   if (!rfid_found) {
                              QMessageBox::critical(this, "Error", "RFID not found in database.");

                              data = "";
                              data_text = "";
                          }





                   data = "";
                   data_text = "";



}

}
void MainWindow::onDialogFinished(int x){
    delete a;
        a = nullptr;
        int ret=A.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }
         QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));
}




void MainWindow::on_enter_clicked() {
  bool mail_verif = mail_regex.exactMatch(ui -> nameenter -> text());

  ui -> labelemailsign -> clear();
  ui -> labelpass -> clear();

  if (ui -> passenter -> text() != ""  && ui -> nameenter -> text() != "")
  {
      if (mail_verif)
      {
          bool chercherem = log -> chercheremail(ui -> nameenter -> text());
          if (chercherem)
          {

  if (log -> sign_in(ui -> nameenter -> text(), ui -> passenter -> text()))
  {
      A.close_arduino();

    //nourbenyounes@esprit.tn
       a = new application(log -> fetch_id(ui -> nameenter -> text()),log -> fetch_user(ui -> nameenter -> text()));

    a -> setcurrent_user(log -> fetch_user(ui -> nameenter -> text()));

    ui -> nameenter -> clear();
    ui -> passenter -> clear();
    a -> show();
    connect(a, SIGNAL(finished(int)), this, SLOT(onDialogFinished(int)));

    MainWindow::close();
  }
  else
  {
    QMessageBox::warning(this, tr("Connexion"), tr("password doesnt match email"));
    test = false;
  }
      }
      else
      {
      //  QMessageBox::warning(this, tr("Changement du MDP"), tr("This user doesnt exist"));
          ui -> labelemailsign -> setText("This user doesnt exist");
          ui -> labelemailsign -> setStyleSheet("color :orange");
      }
       }
      else

      {

          if (!mail_verif)
          {

            ui -> labelemailsign -> setText("Email doesnt meet requirements");
            ui -> labelemailsign -> setStyleSheet("color :red");
          }

      }
  }
  else
  {

    ui -> labelpass -> setText("please fill in everything");
    ui -> labelpass -> setStyleSheet("color :red");

    // QMessageBox::warning(this,tr("Changement du MDP"),tr("Veuillez remplir tous les champs"));

  }
}

void MainWindow::on_pushButton_2_clicked()
{
  ui -> stackedWidget -> setCurrentIndex(1);

}

void MainWindow::on_enterp_clicked()
{
  ui -> stackedWidget -> setCurrentIndex(0);

}

void MainWindow::on_signp_clicked()
{
  bool mail_verif = mail_regex.exactMatch(ui -> nameenterp -> text());
  ui -> labelchangepass -> clear();
  ui -> labelemailchange -> clear();
  if (ui -> changepwd -> text() == ui -> confirmpwd -> text())
  {
    if (ui -> changepwd -> text() != "" && ui -> confirmpwd -> text() != "" && ui -> nameenterp -> text() != "")
    {

      if (mail_verif)
      {
        bool chercherem = log -> chercheremail(ui -> nameenterp -> text());
        if (chercherem)
        {
          bool testch = log -> modifier_pwd(ui -> nameenterp -> text(), ui -> confirmpwd -> text());


          if (!testch)

          {
            // QMessageBox::warning(this,tr("Changement du MDP"),tr("Erreur lors du changement du MDP"));

            ui -> labelchangepass -> setText("erreur when changing password");
            ui -> labelchangepass -> setStyleSheet("color :red");

          }
          else
          {

            ui -> labelchangepass -> clear();
            ui -> labelemailchange -> clear();
            ui -> changepwd -> clear();
            ui -> confirmpwd -> clear();
            ui -> nameenterp -> clear();
            ui->stackedWidget->setCurrentIndex(0);

          }
        }
        else
        {
        //  QMessageBox::warning(this, tr("Changement du MDP"), tr("This user doesnt exist"));
            ui -> labelemailchange -> setText("This user doesnt exist");
            ui -> labelemailchange -> setStyleSheet("color :orange");
        }

      }
      else
      {
        if (!mail_verif)
        {

          ui -> labelemailchange -> setText("Email doesnt meet requirements");
          ui -> labelemailchange -> setStyleSheet("color :red");
        }
      }

    }
    else
    {

      ui -> labelchangepass -> setText("please fill in everything");
      ui -> labelchangepass -> setStyleSheet("color :red");

      // QMessageBox::warning(this,tr("Changement du MDP"),tr("Veuillez remplir tous les champs"));

    }
  }
  else
  {

    ui -> labelchangepass -> setText("the two passwords doesnt match");
    ui -> labelchangepass -> setStyleSheet("color :red");

    //QMessageBox::warning(this,tr("Changement du MDP"),tr("Les deux mots de passe ne sont pas identiques"));

  }

}
