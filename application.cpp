#include "application.h"
#include "ui_application.h"
#include "employe.h"
#include "login.h"
#include "mainwindow.h"
#include "chatbox.h"
#include "gestionborne.h"
#include "gestionrendezvu.h"

#include "interfaceclient.h"
#include "inteface_produit.h"


#include <QString>
#include <QMessageBox>
#include <QDebug>



#include <QPrinter>
//#include<QTextStream>
#include<QGraphicsView>
#include<QtPrintSupport/QPrintDialog>
#include<QPdfWriter>
#include<QUrlQuery>
#include <QDesktopServices>
//#include <QTextDocument>

#include <QSqlQuery>
#include <QtCharts>
#include<QChartView>
#include <QPieSeries>
#include<QPieSlice>



using namespace chatting;


application::application(QString currentid,QString currentuser,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::application)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  //  ui->tableView->setModel(suppem.afficher());
    ui->CIN->setValidator(new QIntValidator(0, 999999, this)); //Control de saisie, 8 chiffres juste pour le CIN!
    ui->SALAIRE->setValidator(new QIntValidator(0, 999999, this));

    ui->PHONE->setValidator(new QIntValidator(0, 99999999, this));


    mail_regex = QRegExp("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
    // cin_regex=QRegExp("[0-9]{8}$");
    chaine_regex = QRegExp("[a-zA-Z ]{2,20}$");
    tel_regex=QRegExp("[0-9]{8}$");
    salaire_regex = QRegExp("[0-9]+$");


    //permet recherche et tri
        proxy = new QSortFilterProxyModel();
        //definir la source (tableau original)
        proxy->setSourceModel(suppem.afficher());
       //pour la recherche
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxy->setFilterKeyColumn(-1); // chercher dans tout le tableau (-1) ou donner le numero de la colone
        //faire tri
        proxy->setDynamicSortFilter(true);
        ui->tableView->setSortingEnabled(true);
        //remplissage tableau avec le masque
        ui->tableView->setModel(proxy);

        current_id=  currentid;
        current_user=currentuser;
//setcurrent_user(current_user);
ui->username->setText(current_user);

//ui->CIN->setText(current_id);
   // ui->rechercheemploye->setShortcut(QKeySequence(tr("return")));

}



//affichage + metier tri
void application::show_tables()
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //permet recherche et tri
        proxy = new QSortFilterProxyModel();
        //definir la source (tableau original)
        proxy->setSourceModel(suppem.afficher());
       //pour la recherche
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxy->setFilterKeyColumn(-1); // chercher dans tout le tableau (-1) ou donner le numero de la colone
        //faire tri
        proxy->setDynamicSortFilter(true);
        ui->tableView->setSortingEnabled(true);
        //remplissage tableau avec le masque
        ui->tableView->setModel(proxy);

}



/*void application::setcurrent_user(QString current_user)
{
  //ui->username->setText(current_user);


}*/

application::~application()
{
    delete ui;
}



void application::on_tableView_clicked(const QModelIndex &index)
{
    selected = ui->tableView->model()->data(index).toInt();
}

void application::on_add_clicked()
{

    ui->labelemail->setText("");
    ui->labelCin->setText("");
    ui->labelphone->setText("");
    ui->labelnom->setText("");
    ui->labelprenom->setText("");
    ui->labelsalaire->setText("");
    ui->labelrole->setText("");

    int idemploye = ui->CIN->text().toInt();
    QString nom = ui->NOM->text();
    QString prenom = ui->PRENOM->text();
    QString email = ui->EMAIL->text();
    QString phone = ui->PHONE->text();
    int salaire = ui->SALAIRE->text().toFloat();
    QString role = ui->comboBoxrole->currentText();


    bool mail_verif = mail_regex.exactMatch(ui->EMAIL->text());
    bool cin_verif = salaire_regex.exactMatch(ui->CIN->text());
    bool nom_verif = chaine_regex.exactMatch(ui->NOM->text());
    bool prenom_verif = chaine_regex.exactMatch(ui->PRENOM->text());
    bool telephone_verif = tel_regex.exactMatch(ui->PHONE->text());
    bool salaire_verif = salaire_regex.exactMatch(ui->SALAIRE->text());

    QString verifrole=ui->comboBoxrole->currentText();


    if (cin_verif && nom_verif && prenom_verif && telephone_verif && salaire_verif && mail_verif && verifrole != "Role" )
    {

        Employe E(idemploye, nom, prenom, email, phone, salaire, role);

        bool chercher = E.chercherexistid(idemploye);

         if (!chercher){
        bool test = E.ajouter();


         QString current_user = nom + " " + prenom;
         l.sign_up(current_user,QString::number(idemploye),email);

        if (test)
        {

  show_tables();

            QMessageBox::information(nullptr, QObject::tr("ok"),
                                     QObject::tr("ajouter.\n"
                                                 "Click Cancel to exit."),
                                     QMessageBox::Cancel);

            ui->CIN->setText("");
            ui->NOM->setText("");
            ui->PRENOM->setText("");
            ui->PHONE->setText("");
            ui->EMAIL->setText("");
            ui->SALAIRE->setText("");
            ui->comboBoxrole->setCurrentText("Role");

        }

         }else {


                 ui->labelCin->setText("cin already exist in table");
                 ui->labelCin->setStyleSheet("color: orange");


}}
    else
    {
        if (!mail_verif)
        {

            ui->labelemail->setText("Email doesnt meet requirements");
            ui->labelemail->setStyleSheet("color :red");
        }

        bool chercherem = E.chercherexistemail(email);
        if (chercherem)
        {
            ui->labelemail->setText("email already exist in table");
            ui->labelemail->setStyleSheet("color: orange");
        }

        if (!cin_verif)
        {

            ui->labelCin->setText("Only numbers are acceptable");
            ui->labelCin->setStyleSheet("color: red");
        }

        if (!telephone_verif)
        {

            ui->labelphone->setText("phone need to be 8 numbers");
            ui->labelphone->setStyleSheet("color: red");
        }

        if (!nom_verif)
        {

            ui->labelnom->setText("need at least 2 characters and 20 max");
            ui->labelnom->setStyleSheet("color: red");
        }

        if (!prenom_verif)
        {
            ui->labelprenom->setText("need at least 2 characters and 20 max");
            ui->labelprenom->setStyleSheet("color: red");
        }

        if (!salaire_verif)
        {

            ui->labelsalaire->setText("Only numbers are acceptable");
            ui->labelsalaire->setStyleSheet("color: red");
        }

        QString verifrole=ui->comboBoxrole->currentText();

        if ( verifrole == "Role" )
        {

            ui->labelrole->setText("need at least 2 characters and 20 max");
            ui->labelrole->setStyleSheet("color: red");
        }

        QMessageBox::warning(nullptr, QObject::tr("not ok "),
                             QObject::tr("ajout failed \n"
                                         "Click Cancel to exit."),
                             QMessageBox::Cancel);
    }







}

void application::on_deleteEmp_clicked()
{

    ui->labelCin->setText("");



    //int idemploye=ui->CIN->text().toInt();
   // QString email = ui->EMAIL->text();

 // bool cherche=E.chercherexistid(idemploye);
     //if(cherche){
  // l.delete_account(email);*/
    bool test = suppem.supprimer(selected);

    if (test)
    {

     //   ui->tableView->setModel(suppem.afficher());
          show_tables();


        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("supprimer.\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);



    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("not ok "),
                              QObject::tr("supprimer failed.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
     /* }
      else{

          ui->labelCin->setText("idnumber does not exist");
          ui->labelCin->setStyleSheet("color: orange");


      }*/
}

void application::on_tableView_activated(const QModelIndex &index)
{
    QString val = ui->tableView->model()->data(index).toString();
    QSqlQuery query;
    query.prepare("select * from EMPLOYE where  idemploye='" + val + "' ");
    if (query.exec())
    {

        while (query.next())
        {

            ui->CIN->setText(query.value(0).toString());
            ui->NOM->setText(query.value(1).toString());
            ui->PRENOM->setText(query.value(2).toString());
            ui->EMAIL->setText(query.value(3).toString());
            ui->PHONE->setText(query.value(4).toString());
            ui->SALAIRE->setText(query.value(5).toString());
           // ui->ROLE->setText(query.value(6).toString());


        ui->comboBoxrole->setCurrentText(query.value(6).toString());

        }
    }
}

void application::on_update_clicked()
{

    ui->labelemail->setText("");
    ui->labelCin->setText("");
    ui->labelphone->setText("");
    ui->labelnom->setText("");
    ui->labelprenom->setText("");
    ui->labelsalaire->setText("");
    ui->labelrole->setText("");

    int idemploye = ui->CIN->text().toInt();
    QString nom = ui->NOM->text();
    QString prenom = ui->PRENOM->text();
    QString email = ui->EMAIL->text();
    QString phone = ui->PHONE->text();
    int salaire = ui->SALAIRE->text().toFloat();
    QString role = ui->comboBoxrole->currentText();


    bool mail_verif = mail_regex.exactMatch(ui->EMAIL->text());
    bool cin_verif = salaire_regex.exactMatch(ui->CIN->text());
    bool nom_verif = chaine_regex.exactMatch(ui->NOM->text());
    bool prenom_verif = chaine_regex.exactMatch(ui->PRENOM->text());
    bool telephone_verif = tel_regex.exactMatch(ui->PHONE->text());
    bool salaire_verif = salaire_regex.exactMatch(ui->SALAIRE->text());

    QString verifrole=ui->comboBoxrole->currentText();


    if (cin_verif && nom_verif && prenom_verif && telephone_verif && salaire_verif && mail_verif && verifrole != "Role")
    {

        QString current_user = nom + " " + prenom;
        l.updateuseremploye(current_user,email);
        bool test = suppem.update(idemploye, nom, prenom, email, phone, salaire, role);






        if (test)
        {



  show_tables();

            QMessageBox::information(nullptr, QObject::tr("ok"),
                                     QObject::tr("update.\n"
                                                 "Click Cancel to exit."),
                                     QMessageBox::Cancel);

            ui->CIN->setText("");
            ui->NOM->setText("");
            ui->PRENOM->setText("");
            ui->PHONE->setText("");
            ui->EMAIL->setText("");
            ui->SALAIRE->setText("");
            ui->comboBoxrole->setCurrentText("Role");

        }
    }


    else
    {
        if (!mail_verif)
        {

            ui->labelemail->setText("Email doesnt meet requirements");
            ui->labelemail->setStyleSheet("color :red");
        }

      /*  bool chercher = E.chercherexistid(idemploye);
        if (chercher)
        {
            ui->labelCin->setText("cin already exist in table");
            ui->labelCin->setStyleSheet("color: red");
        }
        bool chercherem = E.chercherexistemail(email);
        if (chercherem)
        {
            ui->labelemail->setText("email already exist in table");
            ui->labelemail->setStyleSheet("color: red");
        }
*/
        if (!cin_verif)
        {

            ui->labelCin->setText("Only numbers are acceptable");
            ui->labelCin->setStyleSheet("color: red");
        }

        if (!telephone_verif)
        {

            ui->labelphone->setText("phone need to be 8 numbers");
            ui->labelphone->setStyleSheet("color: red");
        }

        if (!nom_verif)
        {

            ui->labelnom->setText("need at least 2 characters and 20 max");
            ui->labelnom->setStyleSheet("color: red");
        }

        if (!prenom_verif)
        {
            ui->labelprenom->setText("need at least 2 characters and 20 max");
            ui->labelprenom->setStyleSheet("color: red");
        }

        if (!salaire_verif)
        {

            ui->labelsalaire->setText("Only numbers are acceptable");
            ui->labelsalaire->setStyleSheet("color: red");
        }

        if ( verifrole == "Role" )
        {

            ui->labelrole->setText("Change Role option");
            ui->labelrole->setStyleSheet("color: red");
        }

        QMessageBox::warning(nullptr, QObject::tr("not ok "),
                             QObject::tr("update failed  \n"
                                         "Click Cancel to exit."),
                             QMessageBox::Cancel);
    }
}





void application::on_logout_clicked()
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
   application::close();

  } else {
    qDebug() << "Yes was *not* clicked";
  }

/* QMessageBox::information(nullptr, QObject::tr("E-charge"),
                                   QObject::tr(" %1 a été déconnecté(e) avec succes!\n"
                                                   "Taper sur cancel pour fermer.").arg(username), QMessageBox::Cancel);*/


}

void application::on_chat_clicked()
{

   // QString username = ui->username->text();
    QDialog *chat = new chatbox(getcurrent_user(),this);
    // QDialog *chat = new chatbox(this);

      chat->exec();



}


void application::on_linerechercheemp_textChanged(const QString &arg1)
{

    proxy->setFilterFixedString(arg1);

}

void application::on_pdf_clicked()
{

  QPdfWriter pdf("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/employes.pdf");
    QPainter painter(&pdf);






  //  painter.fillRect(0,0,9600 ,13650,QColor(248, 246, 231));
     painter.fillRect(0,0,9600 ,13650,QColor(245, 246, 226));


   int i = 4000;

   painter.fillRect(100,100,6000,1900,QColor(217, 224, 197));

        painter.setPen(QColor(16, 127, 54));
        painter.setFont(QFont("Comic Sans MS", 30));
        painter.drawText(700,1100,"Liste Des Employes ");
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
        painter.drawText(6600,3300,"Phone");
        painter.drawText(7800,3300,"Salaire");
        painter.drawText(8600,3300,"Role");


        painter.setPen(QColor(47, 62,150));
     QString date =   "Generated on "  +   QDate::currentDate().toString("dd/MM/yyyy");
     painter.drawText(6700,2800,date);
    // painter.drawText(6500,13500,date);



        QSqlQuery query;
        query.prepare("select * from EMPLOYE");
        query.exec();
        while (query.next())
        {
            painter.setPen(Qt::darkMagenta);
            painter.drawText(100,i,query.value(0).toString());
            painter.setPen(Qt::black);
            painter.drawText(900,i,query.value(1).toString());
            painter.drawText(2400,i,query.value(2).toString());
            painter.drawText(3500,i,query.value(3).toString());
            painter.drawText(6600,i,query.value(4).toString());
            painter.drawText(7900,i,query.value(5).toString());

            painter.setPen(QColor(47, 62,150));
            painter.drawText(8550,i,query.value(6).toString());



           i = i + 500;
        }
        int reponse = QMessageBox::question(this, "Génerer PDF", "PDF Enregistré! -Voulez-Vous Affichez Le PDF ?", QMessageBox::Yes |  QMessageBox::No);
            if (reponse == QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl::fromLocalFile("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/employes.pdf"));
                painter.end();
            }
            if (reponse == QMessageBox::No)
            {
                 painter.end();
            }

}





void application::on_statemploye_clicked()
{
 /* stats *w=new stats();
           w->stat();
          // w->show();

          w->exec();*/





    QSqlQuery query;
       query.exec("SELECT Role FROM Employe");

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



    QPieSeries *series=new QPieSeries();
    series->setHoleSize(0.35);

    for (auto it = counts.begin(); it != counts.end(); it++) {
        double percentage = 100 * it.value() / total;
     QPieSlice *slice  =   series->append(it.key() + QString(" (%1%)").arg(percentage, 0, 'f', 1), it.value());

     slice->setExploded();
      slice->setLabelVisible();
    }


               // slice->setPen(QPen(Qt::darkRed, 2));
              //  slice->setBrush(Qt::black);





   QChart *chart=new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTitle("Nombre de role des Employes");
    chart->setTheme(QChart::ChartThemeDark);
    chart->setBackgroundVisible(false);
    chart->setBackgroundBrush(Qt::white) ;
/*
    QChartView *chartview=new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setParent(ui->stat);
    chartview->setMinimumSize(700,600);
    chartview->setMaximumSize(700,600);*/


                 chart->legend()->show();
                 QChartView *chartView = new QChartView(chart);
                 chartView->setRenderHint(QPainter::Antialiasing);
                 ui->verticalLayout->addWidget(chartView);
                   chartView->update();




       QDialog *dialog = new QDialog();
       dialog->resize(800, 600);
       QVBoxLayout *layout = new QVBoxLayout();
       layout->addWidget(chartView);
       dialog->setLayout(layout);
       dialog->setStyleSheet("background-color: black;");

       chart->setParent(chartView);
       dialog->exec();




}

void application::on_clients_clicked()
{
    interfaceclient * a = new interfaceclient(current_id,current_user);
  a->setGeometry(geometry());
  a -> show();
 application::close();
}

void application::on_products_clicked()
{
    inteface_produit * a = new inteface_produit(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  application::close();
}

void application::on_stations_clicked()
{
    gestionBorne * a = new gestionBorne(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  application::close();
}

void application::on_rdv_clicked()
{
    gestionrendezvu * a = new gestionrendezvu(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  application::close();
}
