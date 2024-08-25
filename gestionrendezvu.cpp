#include "gestionrendezvu.h"
#include "ui_gestionrendezvu.h"
#include "rdv.h"
#include "application.h"
#include "gestionborne.h"
#include "interfaceclient.h"
#include "inteface_produit.h"
#include "mainwindow.h"


#include <QString>
#include <QMessageBox>

#include <QPrinter>
#include<QTextStream>
#include<QGraphicsView>
#include<QtPrintSupport/QPrintDialog>
#include<QPdfWriter>
#include<QUrlQuery>
#include <QDesktopServices>


#include <QTextDocument>

#include <QChartView>
#include <QPieSeries>
#include <QtCharts>
#include <QtSql/QSqlQueryModel>
gestionrendezvu::gestionrendezvu(QString currentid,QString currentuser,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gestionrendezvu)
{
    ui->setupUi(this);
    ui->tableView->setModel(supp.afficher());
    current_id=  currentid;
    current_user=currentuser;
    ui->username->setText(current_user);
    ui->CIN->setValidator(new QIntValidator(0, 999999, this)) ;
    ui->PHONE->setValidator(new QIntValidator(0, 99999999, this)) ;

    show_tables();
    email_regex=QRegExp("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
         cin_regex=QRegExp("^[0-9]{1,2}/[0-9]{1,2}/[0-9]{4}$");
         chaine_regex=QRegExp("[a-zA-Z ]{2,20}$");
        // tel_regex=QRegExp("[0-9]{8}$");
         salaire_regex=QRegExp("[0-9]+$");
         horaire_regex=QRegExp("^([0-1]?[0-9]|2[0-3]):[0-5][0-9]$");
         QList<QDate> dates = supp.recupererDatesDeLaBaseDeDonnees(ui->calendarWidget);

}

gestionrendezvu::~gestionrendezvu()
{
    delete ui;
}


void gestionrendezvu::on_tableView_clicked(const QModelIndex &index)
{
    selected=ui->tableView->model()->data(index).toInt();
}

void gestionrendezvu::on_add_clicked()
{
        ui->labelemail->setText("");
        ui->labelCin->setText("");
        ui->labelphone->setText("");
        ui->labelnom->setText("");
        ui->labelprenom->setText("");
        ui->labeldate->setText("");
        ui->labelheure->setText("");


       int id_employe=1;
       int idrdv=ui->CIN->text().toInt();
       QString nom=ui->NOM->text();
        QString prenom=ui->PRENOM->text();
        QString email=ui->EMAIL->text();
        int phone=ui->PHONE->text().toInt();
      //  QDate daterdv=ui->DATE->text();
        QDate daterdv=ui-> dateEdit->date();
        QString heure=ui->HEURE->text();

         //QString heure=ui->timeEdit->text();

       bool mail_verif = email_regex.exactMatch(ui->EMAIL->text());
                          bool cin_verif=salaire_regex.exactMatch(ui->CIN->text());
                          bool nom_verif=chaine_regex.exactMatch(ui->NOM->text());
                          bool prenom_verif=chaine_regex.exactMatch(ui->PRENOM->text());
                          bool phone_verif=salaire_regex.exactMatch(ui->PHONE->text());

                         // bool date_verif=cin_regex.exactMatch(ui->DATE->text());
                        //  bool heure_verif=horaire_regex.exactMatch(ui->timeEdit->text());
         bool heure_verif=horaire_regex.exactMatch(ui->HEURE->text());

        if(cin_verif && nom_verif && prenom_verif && phone_verif && mail_verif && heure_verif)
            {

      rdv E(idrdv,nom,prenom,email,phone,daterdv,heure,current_id);


      bool chercher=supp.chercherexistid(idrdv);

      if(!chercher){
       bool test=E.ajouter();
       if(test){
           ui->tableView->setModel(supp.afficher());
           show_tables();
           QList<QDate> dates = supp.recupererDatesDeLaBaseDeDonnees(ui->calendarWidget);

           QMessageBox::information(nullptr, QObject::tr("ok"),
                       QObject::tr("ajouter.\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);

                        ui->CIN->setText("");
                        ui->NOM->setText("");
                        ui->PRENOM->setText("");
                        ui->PHONE->setText("");
                        ui->EMAIL->setText("");
                       // ui->DATE->setText("");
                        ui->HEURE->setText("");

       }

        }else {


            ui->labelCin->setText("cin already exist in table");
            ui->labelCin->setStyleSheet("color: red");




}}
       else
       {
            if (!mail_verif){

                         ui->labelemail->setText("Email doesnt meet requirements");
                             ui->labelemail->setStyleSheet("color :red");

                     }


                     if (!cin_verif){

                         ui->labelCin->setText("Only numbers are acceptable");
                         ui->labelCin->setStyleSheet("color: red");


                     }

                     if (!phone_verif){

                         ui->labelphone->setText("Only numbers are acceptable");
                         ui->labelphone->setStyleSheet("color: red");

                     }

                     if (!nom_verif){

                         ui->labelnom->setText("need at least 2 characters");
                         ui->labelnom->setStyleSheet("color: red");

                     }

                     if (!prenom_verif){
                         ui->labelprenom->setText("need at least 2 characters");
                         ui->labelprenom->setStyleSheet("color: red");
                     }

                     /* if (!date_verif){

                         ui->labeldate->setText("Only numbers are acceptable");
                         ui->labeldate->setStyleSheet("color: red");
                           } */
                     if (!heure_verif){

                         ui->labelheure->setText("need at least 2 characters");
                         ui->labelheure->setStyleSheet("color: red");
                           }

           QMessageBox::critical(nullptr, QObject::tr("not ok "),
                       QObject::tr("ajout failed.\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);
       }

}





void gestionrendezvu::on_pushButton_5_clicked()
{
    //int idrdv=ui->CIN->text().toInt();
  bool test=supp.supprimer(selected);

  if(test){

      ui->tableView->setModel(supp.afficher());
      show_tables();
      QList<QDate> dates = supp.recupererDatesDeLaBaseDeDonnees(ui->calendarWidget);



      QMessageBox::information(nullptr, QObject::tr("ok"),
                  QObject::tr("supprimer.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
  }
  else
  {
      QMessageBox::critical(nullptr, QObject::tr("not ok "),
                  QObject::tr("supprimer failed.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
  }

}


void gestionrendezvu::on_update_clicked()
{

    ui->labelemail->setText("");
    ui->labelCin->setText("");
    ui->labelphone->setText("");
    ui->labelnom->setText("");
    ui->labelprenom->setText("");
    ui->labeldate->setText("");
    ui->labelheure->setText("");

    int idrdv = ui->CIN->text().toInt();
    QString nom = ui->NOM->text();
    QString prenom = ui->PRENOM->text();
    QString email = ui->EMAIL->text();
    int phone = ui->PHONE->text().toInt();

    QDate daterdv=ui-> dateEdit->date();





 QString heure = ui->HEURE->text();

    bool mail_verif = email_regex.exactMatch(ui->EMAIL->text());
                       bool cin_verif=salaire_regex.exactMatch(ui->CIN->text());
                       bool nom_verif=chaine_regex.exactMatch(ui->NOM->text());
                       bool prenom_verif=chaine_regex.exactMatch(ui->PRENOM->text());
                       bool phone_verif=salaire_regex.exactMatch(ui->PHONE->text());
                      // bool date_verif=cin_regex.exactMatch(ui->DATE->text());
                       bool heure_verif=horaire_regex.exactMatch(ui->HEURE->text());


     if(cin_verif && nom_verif && prenom_verif && phone_verif && mail_verif && heure_verif)
         {



        bool test = supp.update(idrdv, nom, prenom, email, phone, daterdv, heure);

        if (test)
        {

            ui->tableView->setModel(supp.afficher());
            show_tables();
            QList<QDate> dates = supp.recupererDatesDeLaBaseDeDonnees(ui->calendarWidget);


            QMessageBox::information(nullptr, QObject::tr("ok"),
                                     QObject::tr("update.\n"
                                                 "Click Cancel to exit."),
                                     QMessageBox::Cancel);

            ui->CIN->setText("");
            ui->NOM->setText("");
            ui->PRENOM->setText("");
            ui->PHONE->setText("");
            ui->EMAIL->setText("");
         //   ui->DATE->setText("");
            ui->HEURE->setText("");

}
    }

    else
    {
         if (!mail_verif){

                      ui->labelemail->setText("Email doesnt meet requirements");
                          ui->labelemail->setStyleSheet("color :red");

                  }


                  if (!cin_verif){

                      ui->labelCin->setText("Only numbers are acceptable");
                      ui->labelCin->setStyleSheet("color: red");


                  }

                  if (!phone_verif){

                      ui->labelphone->setText("Only numbers are acceptable");
                      ui->labelphone->setStyleSheet("color: red");

                  }

                  if (!nom_verif){

                      ui->labelnom->setText("need at least 2 characters");
                      ui->labelnom->setStyleSheet("color: red");

                  }

                  if (!prenom_verif){
                      ui->labelprenom->setText("need at least 2 characters");
                      ui->labelprenom->setStyleSheet("color: red");
                  }

                 /* if (!date_verif){

                      ui->labeldate->setText("Only numbers are acceptable");
                      ui->labeldate->setStyleSheet("color: red");
                        } */

                  if (!heure_verif){

                      ui->labelheure->setText("need at least 2 characters");
                      ui->labelheure->setStyleSheet("color: red");
                        }

        QMessageBox::warning(nullptr, QObject::tr("not ok "),
                             QObject::tr("update failed \n"
                                         "Click Cancel to exit."),
                             QMessageBox::Cancel);
    }
}



void gestionrendezvu::on_tableView_activated(const QModelIndex &index)
{
    QString val=ui->tableView->model()->data(index).toString();
            QSqlQuery query;
         query.prepare("select idrdv,nom,prenom,email,phone,to_char(daterdv,'DD/MM/RRRR'),heure from RENDEZVOUS where  idrdv='"+val+"' ");
        if(query.exec()){

            while(query.next()){
                ui->CIN->setText(query.value(0).toString());
                ui->NOM->setText(query.value(1).toString());
                ui->PRENOM->setText(query.value(2).toString());
                ui->EMAIL->setText(query.value(3).toString());
                ui->PHONE->setText(query.value(4).toString());
                QString dates=query.value(5).toString();
                int jour = dates.split("/")[0].toInt();
                int month = dates.split("/")[1].toInt();
                int year = dates.split("/")[2].toInt();
                QDate date = QDate::fromString(dates);
                ui->dateEdit->setDate(date);

               // ui->DATE->setText(query.value(5).toString());
                ui->HEURE->setText(query.value(6).toString());


        }
        }
}



void gestionrendezvu::on_envoyerSMS_clicked()
{


       QNetworkAccessManager * manager = new QNetworkAccessManager(this);


       QUrl url();// 

            QNetworkRequest request(url);

            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


            QUrlQuery params;
            params.addQueryItem("From", "+....");
            params.addQueryItem("To",ui->numero->text() );//
            params.addQueryItem("Body", ui->message->toPlainText());
           // params.addQueryItem("Body", "test");

            // etc

            connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply*)));

         manager->post(request, params.query().toUtf8());


QMessageBox::warning(this, tr("SMS envoyé"), tr("SMS envoyé"));



}


void gestionrendezvu::replyFinished(QNetworkReply* reply)
{
    //QByteArray bts = rep->readAll();


    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QJsonObject response = jsonReply["response"].toObject();
    QJsonArray  data     = jsonReply["data"].toArray();
    qDebug() << data;
    qDebug() << response;


}


//tri recherche
void gestionrendezvu::show_tables()
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QDate date = QDate::currentDate();
     ui->dateEdit->setDate(date);
    //permet recherche et tri
        proxy = new QSortFilterProxyModel();
        //definir la source (tableau original)
        proxy->setSourceModel(supp.afficher());
       //pour la recherche
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxy->setFilterKeyColumn(-1); // chercher dans tout le tableau (-1) ou donner le numero de la colone
        //faire tri
        proxy->setDynamicSortFilter(true);
        ui->tableView->setSortingEnabled(true);
        //remplissage tableau avec le masque
        ui->tableView->setModel(proxy);

}

void gestionrendezvu::on_lineEdit_5_textChanged(const QString &arg1)
{
    proxy->setFilterFixedString(arg1);
}





void gestionrendezvu::on_PDF_clicked()
{

  QPdfWriter pdf("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/rdv.pdf");
    QPainter painter(&pdf);






  //  painter.fillRect(0,0,9600 ,13650,QColor(248, 246, 231));
     painter.fillRect(0,0,9600 ,13650,QColor(245, 246, 226));


   int i = 4000;

   painter.fillRect(100,100,6000,1900,QColor(217, 224, 197));

        painter.setPen(QColor(16, 127, 54));
        painter.setFont(QFont("Comic Sans MS", 30));
        painter.drawText(700,1100,"Liste Des rendezvous ");
        painter.setPen(QColor(135, 184, 147));
        painter.setFont(QFont("Comic Sans MS", 50));
        painter.drawRect(100,100,6000,1900);
        painter.drawPixmap(QRect(6500,70,3000,2200),QPixmap("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/img/logo.png"));
      //  painter.setPen(QColor(135, 184, 147));
     //painter.setPen(QColor(170, 212, 193));

        painter.drawRect(0,3000,9600,500);
        painter.fillRect(0,3000,9600,500,QColor(186, 213, 173));
        painter.setPen(Qt::darkGreen);

        painter.setFont(QFont("Comic Sans MS", 11));
        painter.drawText(100,3300,"CIN");
        painter.drawText(900,3300,"Nom");
        painter.drawText(2400,3300,"Prenom");
        painter.drawText(3500,3300,"Email");
        painter.drawText(5500,3300,"Phone");
        painter.drawText(7000,3300,"Date");
        painter.drawText(8500,3300,"Heure");


        painter.setPen(QColor(47, 62,150));
     QString date =   "Generated on "  +   QDate::currentDate().toString("dd/MM/yyyy");
     painter.drawText(6700,2800,date);
    // painter.drawText(6500,13500,date);



        QSqlQuery query;
        query.prepare("select idrdv,nom,prenom,email,phone,to_char(daterdv,'DD/MM/RRRR'),heure from RENDEZVOUS");
        query.exec();
        while (query.next())
        {
            painter.setPen(Qt::darkMagenta);
            painter.drawText(100,i,query.value(0).toString());
            painter.setPen(Qt::black);
            painter.drawText(900,i,query.value(1).toString());
            painter.drawText(2400,i,query.value(2).toString());
            painter.drawText(3500,i,query.value(3).toString());
            painter.drawText(5500,i,query.value(4).toString());
            painter.drawText(7000,i,query.value(5).toString());

            painter.setPen(QColor(47, 62,150));
            painter.drawText(8500,i,query.value(6).toString());



           i = i + 500;
        }
        int reponse = QMessageBox::question(this, "Génerer PDF", "PDF Enregistré! -Voulez-Vous Affichez Le PDF ?", QMessageBox::Yes |  QMessageBox::No);
            if (reponse == QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl::fromLocalFile("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/rdv.pdf"));
                painter.end();
            }
            if (reponse == QMessageBox::No)
            {
                 painter.end();
            }


}





void gestionrendezvu::on_stats_clicked()
{
    QSqlQuery query;
    query.exec("SELECT to_char(daterdv,'DD/MM/RRRR') FROM RENDEZVOUS");

    QMap<QString, int> counts;
    int total = 0;

    while (query.next()) {
        QString type = query.value(0).toString();
        if (counts.contains(type)) {
            counts[type]++;
        } else {
            counts[type] = 1;
        }
        total++;
    }

    QPieSeries *series = new QPieSeries();
    for (auto it = counts.begin(); it != counts.end(); it++) {
        double percentage = 100 * it.value() / total;
        series->append(it.key() + QString(" (%1%)").arg(percentage, 0, 'f', 1), it.value());
    }

    QChart *chart = new QChart();
    chart->setTheme(QChart::ChartThemeDark);

    chart->addSeries(series);
    chart->setTitle("Statistiques pour l'attribut \"date viste\"");
    series->setPieSize(0.7);
    series->setLabelsVisible();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog *dialog = new QDialog();
    dialog->resize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    dialog->setLayout(layout);

    chart->setParent(chartView);
    dialog->exec();
}






void gestionrendezvu::on_calendarWidget_clicked(const QDate &date)
{
  ui->tableView_2->setModel(supp.afficherc(ui->calendarWidget));
  ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



}


void gestionrendezvu::on_pushButton_6_clicked()
{
    QSqlQuery query;
    query.exec("SELECT daterdv FROM RENDEZVOUS");

    QMap<QString, int> counts;
    int total = 0;

    while (query.next()) {
        QString date = query.value(0).toDate().toString("yyyy-MM-dd");
        if (counts.contains(date)) {
            counts[date]++;
        } else {
            counts[date] = 1;
        }
        total++;
    }
/*
   QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Nombre de rendez-vous");
    for (auto it = counts.begin(); it != counts.end(); it++) {
        *set << it.value();
    }

    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques pour l'attribut \"date viste\"");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setLabelsAngle(-45);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog *dialog = new QDialog();
    dialog->resize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    dialog->setLayout(layout);

    dialog->exec();*/
}



void gestionrendezvu::on_employees_clicked()
{
    application * a = new application(current_id,current_user);
  a->setGeometry(geometry());
  a -> show();
  gestionrendezvu::close();
}

void gestionrendezvu::on_clients_clicked()
{
    interfaceclient * a = new interfaceclient(current_id,current_user);
  a->setGeometry(geometry());
  a -> show();
  gestionrendezvu::close();
}

void gestionrendezvu::on_stations_clicked()
{
    gestionBorne * a = new gestionBorne(current_id,current_user);
  a->setGeometry(geometry());
  a -> show();
  gestionrendezvu::close();
}

void gestionrendezvu::on_products_clicked()
{

    inteface_produit * a = new inteface_produit(current_id,current_user);
  a->setGeometry(geometry());
  a -> show();
  gestionrendezvu::close();
}

void gestionrendezvu::on_logout_clicked()
{
    QString username = ui->username->text();

    QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "E-charge", username + " vouleuz vous deconnecte ?",
                                   QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes) {
      // qDebug() << username + "%1 a été déconnecté(e) avec succes!";
      QMessageBox::information(nullptr, QObject::tr("E-charge"),
                                            QObject::tr(" %1 a été déconnecté(e) avec succes!\n"
                                                            "Taper sur cancel pour fermer.").arg(username), QMessageBox::Cancel);


        MainWindow *w = new MainWindow();
       w->show();
      gestionrendezvu::close();

     } else {
       qDebug() << "Yes was *not* clicked";
     }
}
