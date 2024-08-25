#include "inteface_produit.h"
#include "ui_inteface_produit.h"
#include "produits.h"
#include "gestionborne.h"
#include <QString>
#include <QLineEdit>
#include<QMessageBox>
#include <QIntValidator>
#include <QSqlQuery>
#include "calculatrice.h"
#include <QPixmap>
#include <QUrl>
#include <QImage>
#include <QPainter>
#include <QDesktopServices>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QPieSeries>
#include <QtCharts>
#include "mainwindow.h"
#include"qrcode.h"
#include<qrcode.h>
#include <QtSvg/QSvgRenderer>
#include <QtSvg/QtSvg>
#include<QFile>
#include<QFileDialog>
#include <iostream>
#include <fstream>
#include <QtSvg/qsvgrenderer.h>
#include <QPrinter>
#include<QTextStream>
#include<QGraphicsView>
#include<QtPrintSupport/QPrintDialog>
#include<QPdfWriter>
#include<QUrlQuery>
#include <QDesktopServices>

#include "interfaceclient.h"
#include "gestionrendezvu.h"

using namespace qrcodegen;





inteface_produit::inteface_produit(QString currentid,QString currentuser,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inteface_produit)
{




    ui->setupUi(this);
    current_id=  currentid;
    current_user=currentuser;
    ui->username->setText(current_user);
     QPixmap pix("C:/Users/21627/Desktop/projet C++/test_sarra/img/logo");

    ui->tableView->setModel(Etmp.afficher());
    show_tables();
    ui->lineEdit_CIN->setValidator(new QIntValidator(0, 999999, this)) ;
    ui->lineEdit_idproduit->setValidator(new QIntValidator(0, 999999, this)) ;
    ui->lineEdit_quantite->setValidator(new QIntValidator(0, 99, this)) ;
    ui->lineEdit_prix->setValidator(new QIntValidator(0, 999, this)) ;
    //mail_regex=QRegExp("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
   // cin_regex=QRegExp("[0-9]{8}$");
    chaine_regex=QRegExp("[a-zA-Z ]{2,20}$");
   // tel_regex=QRegExp("[0-9]{8}$");
    salaire_regex=QRegExp("[0-9]+$");
    //horaire_regex=QRegExp("^([0-1]?[0-9]|2[0-3]):[0-5][0-9]$");
//recherche
connect(ui->le_rech, SIGNAL(textChanged(const QString &)), this, SLOT(on_lineEdit_recherche_textChanged(const QString &arg1)));
int ret=A.connect_arduino(); // lancer la connexion à arduino
      switch(ret){
      case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
          break;
      case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
         break;
      case(-1):qDebug() << "arduino is not available";
      }
       // QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
       //le slot update_label suite à la reception du signal readyRead (reception des données).
        QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(notifer()));

}

inteface_produit::~inteface_produit()
{
    delete ui;
    A.close_arduino();
}
// metier qrcode
void inteface_produit::on_pb_qr_clicked()
{
    if(ui->tableView->currentIndex().row()==-1)
            QMessageBox::information(nullptr, QObject::tr("Suppression"),
                                     QObject::tr("Veuillez Choisir un produit du Tableau.\n"
                                                 "Click Ok to exit."), QMessageBox::Ok);
        else
        {




    QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
    if (selection.isEmpty())
      return;

    int row = selection.at(0).row();


    QStringList attributs;
    for (int col = 0; col < ui->tableView->model()->columnCount(); col++) {
      attributs << ui->tableView->model()->data(ui->tableView->model()->index(row, col)).toString();
    }



    const QrCode qr = QrCode::encodeText(attributs.join(",").toStdString().c_str(), QrCode::Ecc::HIGH);




    std::ofstream myfile;
    myfile.open("qrcode.svg");
    myfile << qr.toSvgString(0);
    myfile.close();




    QSvgRenderer svgRenderer(QString("qrcode.svg"));
    QPixmap pix(QSize(150, 150));
    QPainter pixPainter(&pix);
    svgRenderer.render(&pixPainter);
    ui->qrcode->setPixmap(pix);

        }
}



void inteface_produit::show_tables()
{
ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //permet recherche et tri
        proxy = new QSortFilterProxyModel();
        //definir la source (tableau original)
        proxy->setSourceModel(p.afficher());
       //pour la recherche
       // proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        //proxy->setFilterKeyColumn(-1); // chercher dans tout le tableau (-1) ou donner le numero de la colone

           //tri dynamique
        QSortFilterProxyModel *proxy=new QSortFilterProxyModel(this);
           proxy->setDynamicSortFilter(true);
           proxy->setSourceModel(Etmp.afficher());
            ui->tableView->setModel(proxy);
            ui->tableView->setSortingEnabled(true);



        //remplissage tableau avec le masque
        ui->tableView->setModel(proxy);

}


void inteface_produit::on_add_clicked()
{
    ui->labelidproduit->setText("");
    ui->labelnom->setText("");
     ui->labeltype->setText("");
     ui->labelquantite->setText("");
      ui->labeldisponibilite->setText("");
      ui->labelprix->setText("");
      ui->labelcin->setText("");



      QString nom=ui->lineEdit_nom->text();
      QString type=ui->lineEdit_type->text();
      //QString disponibilite=ui->lineEdit_disponibilite->text();
      int  quantite=ui->lineEdit_quantite->text().toInt();
      float prix=ui->lineEdit_prix->text().toInt();
      int idproduit=ui->lineEdit_idproduit->text().toInt();
       int CIN=ui->lineEdit_CIN->text().toInt();
      QString disponibilite = ui->comboBox->currentText();

      bool idproduit_verif=salaire_regex.exactMatch(ui->lineEdit_idproduit->text());
      bool nom_verif=chaine_regex.exactMatch(ui->lineEdit_nom->text());
      bool type_verif=chaine_regex.exactMatch(ui->lineEdit_type->text());
      bool prix_verif=salaire_regex.exactMatch(ui->lineEdit_prix->text());
      bool quantite_verif=salaire_regex.exactMatch(ui->lineEdit_quantite->text());
      //bool disponibilite_verif=chaine_regex.exactMatch(ui->lineEdit_disponibilite->text());
       bool CIN_verif=salaire_regex.exactMatch(ui->lineEdit_CIN->text());

      if(idproduit_verif && nom_verif && type_verif && prix_verif && quantite_verif   && disponibilite!="Disponibilite"&& CIN_verif)
      {



bool cher=Etmp.chercherexistcin(CIN);
bool chercher=Etmp.chercherexistid(idproduit);

      Produits p(idproduit,current_id,CIN,nom,type,quantite,disponibilite,prix);




      if( !chercher)
      {
          if(cher){
        bool test=p.ajouter();

        ui->tableView->setModel(Etmp.afficher());
        show_tables();
      if(test){
          QMessageBox::information(nullptr, QObject::tr("ok"),
                      QObject::tr("ajouter.\n"
                                  "Click Cancel to exit."), QMessageBox::Cancel);


          ui->lineEdit_idproduit->setText("");
          ui->lineEdit_nom->setText("");
           ui->lineEdit_type->setText("");
           ui->lineEdit_quantite->setText("");
            //ui->lineEdit_disponibilite->setText("");
            ui->lineEdit_prix->setText("");
            ui->lineEdit_CIN->setText("");
            ui->comboBox->setCurrentText("disponibilite");

      }
      }else {


          if(!cher)
          {
          ui->labelcin->setText("cin  doesnt exist in table");
          ui->labelcin->setStyleSheet("color: red");}



      }}else { if(chercher )
          {
      ui->labelidproduit->setText("id already exist in table");
                ui->labelidproduit->setStyleSheet("color: red"); } }

         }
      else
      {




          if (!idproduit_verif){

              ui->labelidproduit->setText("Only numbers are acceptable");
              ui->labelidproduit->setStyleSheet("color: red");


          }
          if (!CIN_verif){

              ui->labelcin->setText("Only numbers  and caracters are acceptable");
              ui->labelcin->setStyleSheet("color: red");


          }

          if (!quantite_verif){

              ui->labelquantite->setText("Only numbers are acceptable");
              ui->labelquantite->setStyleSheet("color: red");

          }

          if (!nom_verif){

              ui->labelnom->setText("need at least 2 characters");
              ui->labelnom->setStyleSheet("color: red");

          }

          if (!type_verif){
              ui->labeltype->setText("need at least 2 characters ans only caracters ");
              ui->labeltype->setStyleSheet("color: red");
          }

          if (!prix_verif){

              ui->labelprix->setText("Only numbers are acceptable");
              ui->labelprix->setStyleSheet("color: red");
                }
         /* if (!disponibilite_verif){

              ui->labeldisponibilite->setText("answer yes or no");
              ui->labeldisponibilite->setStyleSheet("color: red");
                }*/




          QMessageBox::critical(nullptr, QObject::tr("not ok "),
                      QObject::tr("ajout failed.\n"
                                  "Click Cancel to exit."), QMessageBox::Cancel);
      }
}
//button supprimer
void inteface_produit::on_delete_2_clicked()
{
   // int idproduit=ui->lineEdit_idproduit->text().toInt();
       //int numBorne=ui->lineEdit->text().toInt();
   bool test=Etmp.supprimer(selected);
   if(test){
       ui->tableView->setModel(Etmp.afficher());
       show_tables();
       QMessageBox::information(nullptr, QObject::tr("ok"),
                   QObject::tr("supprimer.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);
   }
   else
   {
       QMessageBox::critical(nullptr, QObject::tr("not ok "),
                   QObject::tr("suppression failed.\n"
                               "Click Cancel to exit."), QMessageBox::Cancel);
   }
}
//button modifier

void inteface_produit::on_modifier_clicked()
{
    ui->labelidproduit->setText("");
    ui->labelnom->setText("");
     ui->labeltype->setText("");
     ui->labelquantite->setText("");
      ui->labeldisponibilite->setText("");
      ui->labelprix->setText("");
      ui->labelcin->setText("");



       int idproduit=ui->lineEdit_idproduit->text().toInt();
       QString nom=ui->lineEdit_nom->text();
       QString type=ui->lineEdit_type->text();
       int quantite=ui->lineEdit_quantite->text().toInt();
      // QString disponibilite=ui->lineEdit_disponibilite->text();
      float  prix=ui->lineEdit_prix->text().toInt();
       int CIN=ui->lineEdit_CIN->text().toInt();
        QString disponibilite = ui->comboBox->currentText();


      bool idproduit_verif=salaire_regex.exactMatch(ui->lineEdit_idproduit->text());
      bool nom_verif=chaine_regex.exactMatch(ui->lineEdit_nom->text());
      bool type_verif=chaine_regex.exactMatch(ui->lineEdit_type->text());
      bool prix_verif=salaire_regex.exactMatch(ui->lineEdit_prix->text());
      bool quantite_verif=salaire_regex.exactMatch(ui->lineEdit_quantite->text());
     // bool disponibilite_verif=chaine_regex.exactMatch(ui->lineEdit_disponibilite->text());
       bool CIN_verif=salaire_regex.exactMatch(ui->lineEdit_CIN->text());

      if(idproduit_verif && nom_verif && type_verif && prix_verif && quantite_verif && disponibilite!="Disponibilite" && CIN_verif )
      {
       Produits p(idproduit,current_id,CIN,nom,type,quantite,disponibilite,prix);

       bool test=p.modifier_prod(idproduit);
           if(test)
          {

               ui->tableView->setModel(p.afficher());
               show_tables();

               QMessageBox::information(nullptr, QObject::tr("ok"),
                           QObject::tr("Modification avec succes.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);

             /*  ui->lineEdit_idproduit->clear();
               ui->lineEdit_nom->clear();
               ui->lineEdit_type->clear();

               ui->lineEdit_quantite->clear();
               ui->lineEdit_disponibilite->clear();
               ui->lineEdit_prix->clear();*/


               ui->lineEdit_idproduit->setText("");
               ui->lineEdit_nom->setText("");
                ui->lineEdit_type->setText("");
                ui->lineEdit_quantite->setText("");
                 //ui->lineEdit_disponibilite->setText("");
                 ui->lineEdit_prix->setText("");
                 ui->lineEdit_CIN->setText("");
        ui->comboBox->setCurrentText("disponibilite");


       }}
           else
      {
              bool cher=Etmp.chercherexistcin(CIN);
                   if(cher)
                   {
                       ui->labelcin->setText("cin  doesnt exist in table");
                       ui->labelidproduit->setStyleSheet("color: red");

                   }

                   if (!CIN_verif){

                       ui->labelcin->setText("Only numbers  and caracters are acceptable");
                       ui->labelcin->setStyleSheet("color: red");


                   }




          bool chercher=Etmp.chercherexistid(idproduit);
          if(chercher)
          {
              ui->labelidproduit->setText("id already exist in table");
              ui->labelidproduit->setStyleSheet("color: red");

          }


          if (!idproduit_verif){

              ui->labelidproduit->setText("Only numbers are acceptable");
              ui->labelidproduit->setStyleSheet("color: red");


          }

          if (!quantite_verif){

              ui->labelquantite->setText("Only numbers are acceptable");
              ui->labelquantite->setStyleSheet("color: red");

          }

          if (!nom_verif){

              ui->labelnom->setText("only 2 characters  at least are available ");
              ui->labelnom->setStyleSheet("color: red");

          }

          if (!type_verif){
              ui->labeltype->setText("need at least 2 characters and only caracters ");
              ui->labeltype->setStyleSheet("color: red");
          }

          if (!prix_verif){

              ui->labelprix->setText("Only numbers are acceptable");
              ui->labelprix->setStyleSheet("color: red");
                }
          /*if (!disponibilite_verif){

              ui->labeldisponibilite->setText("answer yes or no");
              ui->labeldisponibilite->setStyleSheet("color: red");
                }
*/
               QMessageBox::critical(nullptr, QObject::tr("not ok"),
                           QObject::tr("Modification echoué.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
}  }


void inteface_produit::on_tableView_clicked(const QModelIndex &index)
{
     selected=ui->tableView->model()->data(index).toInt();
}

void inteface_produit::on_tableView_activated(const QModelIndex &index)
{
    QString val=ui->tableView->model()->data(index).toString();
        QSqlQuery query;
     query.prepare("select * from PRODUITS where  idproduit='"+val+"' ");
    if(query.exec()){

        while(query.next()){

            ui->lineEdit_idproduit->setText(query.value(0).toString());
            ui->lineEdit_nom->setText(query.value(1).toString());
            ui->lineEdit_type->setText(query.value(2).toString());
            ui->lineEdit_quantite->setText(query.value(3).toString());
            //ui->lineEdit_disponibilite->setText(query.value(4).toString());
            ui->lineEdit_prix->setText(query.value(5).toString());
             ui->lineEdit_CIN->setText(query.value(7).toString());

  ui->comboBox->setCurrentText(query.value(4).toString());

            QString val=ui->tableView->model()->data(index).toString();
                int prix=query.value(5).toInt();
                QString res=QString::number(prix);
               const char * p= res.toStdString().c_str();
//float prix=ui->lineEdit_prix->text().toInt();
                A.write_to_arduino(p);
    }
}
}
//calculatrice

void inteface_produit::on_calcul_clicked()
{
    calc=new Calculatrice(this);
    calc->show();
}

/*void inteface_produit::on_pushButton_Recherche_clicked()
{
    Produits p;


               QString choix=ui->comboBox_chercher->currentText();

               if (choix=="idproduit")
               {
                   QString idproduit=ui->le_rech->text();
                   ui->tableView->setModel(p.rechercheridproduit(idproduit));
               }
               if (choix=="nom")
               {
                   QString nom=ui->le_rech->text();
                   ui->tableView->setModel(p.recherchernom(nom));
               }
               if (choix=="disponibilite")
               {
                   QString disponibilite=ui->le_rech->text();
                   ui->tableView->setModel(p.rechercherdisponibilite(disponibilite));
               }
               if (choix=="type")
               {
                   QString type=ui->le_rech->text();
                   ui->tableView->setModel(p.recherchertype(type));
               }
}

void inteface_produit::on_pushButton_trie_clicked()
{
    Produits p;
            QString choix=ui->comboBox_tri->currentText();
            if (choix=="nom")
            {
                //ui->tab_materiel->setModel(p.trie_NOM());
                ui->tableView->setModel(p.afficher());
                bool test=p.trie_NOM();
                if (test)
                {

                    ui->tableView->setModel(p.trie_NOM());
                    QMessageBox::information(nullptr,QObject::tr("ok"),
                                             QObject::tr("tri Nom effectué \n"
                                                         "Click Cancel to exist ."),QMessageBox::Cancel);

                }
                else
                      QMessageBox::critical(nullptr, QObject::tr("non"),
                                  QObject::tr("tri Nom failed.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
            }
            if (choix=="prix")
            {
                ui->tableView->setModel(p.trie_PRIX());
                ui->tableView->setModel(p.afficher());
                bool test=p.trie_PRIX();
                if (test)
                {

                    ui->tableView->setModel(p.trie_PRIX());
                    QMessageBox::information(nullptr,QObject::tr("ok"),
                                             QObject::tr("tri prix effectué \n"
                                                         "Click Cancel to exist ."),QMessageBox::Cancel);

                }
                else
                      QMessageBox::critical(nullptr, QObject::tr("non"),
                                  QObject::tr("tri prix failed.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
            }
            if (choix=="idproduit")
            {
                ui->tableView->setModel(p.trie_ID());
                ui->tableView->setModel(p.afficher());
                bool test=p.trie_ID();
                if (test)
                {

               ui->tableView->setModel(p.trie_ID());
                    QMessageBox::information(nullptr,QObject::tr("ok"),
                                             QObject::tr("tri id effectué \n"
                                                         "Click Cancel to exist ."),QMessageBox::Cancel);

                }
                else
                      QMessageBox::critical(nullptr, QObject::tr("non"),
                                  QObject::tr("tri id failed.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
            }
}

void inteface_produit::on_pdf1_clicked()
{
    QPrinter printer;
           printer.setOutputFormat(QPrinter::PdfFormat);
           printer.setOutputFileName("produit.pdf");
           QPainter painter;
           if (! painter.begin(&printer)) { // failed to open file
               qWarning("failed to open file, is it writable?");
               return ;
           }


          QString idproduit=ui->lineEdit_idproduit->text();
          QString nom=ui->lineEdit_nom->text();
          QString type=ui->lineEdit_type->text();
          QString quantite=ui->lineEdit_quantite->text();
          QString prix=ui->lineEdit_prix->text();
          QString disponibilite =ui->lineEdit_disponibilite->text();
          painter.setFont(QFont("Time New Roman", 25));
          painter.drawText(400,100," PRODUIT");
          painter.setPen(Qt::black);

          painter.setFont(QFont("Time New Roman", 9));
           painter.setPen(QColor(135, 184, 147));
          painter.drawText(10,300,"Identifiant:");
          painter.setPen(Qt::black);
          painter.drawText(150,300,idproduit);
          painter.drawText(10,350,"Nom:");
          painter.drawText(150,350,nom);
          painter.drawText(10,400,"Type:");
          painter.drawText(150,400,type);
          painter.drawText(10,450,"quantite:");
          painter.drawText(150,450,quantite);
          painter.drawText(10,500,"disponibilite :");
          painter.drawText(150,500,disponibilite);
          painter.drawText(10,550,"Prix:");
          painter.drawText(150,550,prix);
          QImage img("C:/Users/21627/Desktop/projet C++/test_sarra/logo");
          QPoint imageCoordinates(650,30);
          painter.drawImage(imageCoordinates,img);
          painter.setPen(QColor(47, 62,150));

       QString date =   "Generated on "  +   QDate::currentDate().toString("dd/MM/yyyy");
       painter.drawText(6700,2800,date);

                 int reponse = QMessageBox::question(this, "Génerer PDF", "<PDF Enregistré>...Vous Voulez Affichez Le PDF ?", QMessageBox::Yes |  QMessageBox::No);
                     if (reponse == QMessageBox::Yes)
                     {
                         QDesktopServices::openUrl(QUrl::fromLocalFile("C:\\Users\\21627\\Desktop\\projet C++\\build-test-Desktop_Qt_5_9_9_MinGW_32bit-Debug\\produit.pdf"));

                         painter.end();
                     }
                     if (reponse == QMessageBox::No)
                     {
                         painter.end();
                     }
                     painter.end();
}*/
// metier pdf
void inteface_produit::on_pdf_clicked()
{
    QPdfWriter pdf("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/listeproduits.pdf");
      QPainter painter(&pdf);






    //  painter.fillRect(0,0,9600 ,13650,QColor(248, 246, 231));
       painter.fillRect(0,0,9600 ,13650,QColor(245, 246, 226));


     int i = 4000;

     painter.fillRect(100,100,6000,1900,QColor(217, 224, 197));

          painter.setPen(QColor(16, 127, 54));
          painter.setFont(QFont("Comic Sans MS", 30));
          painter.drawText(700,1100,"Liste Des Produits ");
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
          painter.drawText(100,3300,"ID");
          painter.drawText(900,3300,"Nom");
          painter.drawText(2400,3300,"Type");
          painter.drawText(4000,3300,"Quantite");
          painter.drawText(5000,3300,"Disponibilite");
          painter.drawText(6600,3300,"Prix");
          painter.drawText(7200,3300,"IDEMPLOYE");
         painter.drawText(8550,3300,"CIN");

          painter.setPen(QColor(47, 62,150));
       QString date =   "Generated on "  +   QDate::currentDate().toString("dd/MM/yyyy");
       painter.drawText(6700,2800,date);
      // painter.drawText(6500,13500,date);



          QSqlQuery query;
          query.prepare("select * from PRODUITS");
          query.exec();
          while (query.next())
          {
              painter.setPen(Qt::darkMagenta);
              painter.drawText(100,i,query.value(0).toString());
              painter.setPen(Qt::black);
              painter.drawText(900,i,query.value(1).toString());
              painter.drawText(2400,i,query.value(2).toString());
              painter.drawText(4000,i,query.value(3).toString());
              painter.drawText(5000,i,query.value(4).toString());
              painter.drawText(6600,i,query.value(5).toString());

              painter.setPen(QColor(47, 62,150));
              painter.drawText(7700,i,query.value(6).toString());
              painter.drawText(8550,i,query.value(7).toString());


             i = i + 500;
          }
          int reponse = QMessageBox::question(this, "Génerer PDF", "PDF Enregistré! -Voulez-Vous Affichez Le PDF ?", QMessageBox::Yes |  QMessageBox::No);
              if (reponse == QMessageBox::Yes)
              {
                  QDesktopServices::openUrl(QUrl::fromLocalFile("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/listeproduits.pdf"));
                  painter.end();
              }
              if (reponse == QMessageBox::No)
              {
                   painter.end();
              }

}
//recherche dynamique
void inteface_produit::on_le_rech_textChanged(const QString &arg1)
{

    for (int row = 0; row < ui->tableView->model()->rowCount(); ++row) {
            bool match = false;
            for (int col = 0; col < ui->tableView->model()->columnCount(); ++col) {
                QModelIndex index = ui->tableView->model()->index(row, col);
                QString cellText = ui->tableView->model()->data(index, Qt::DisplayRole).toString();
                if (cellText.contains(arg1, Qt::CaseInsensitive)) {
                    match = true;
                    break;
                }
            }
            ui->tableView->setRowHidden(row, !match);
        }
}
//metier stat
void inteface_produit::on_stat_clicked()
{




    QSqlQuery query;
       query.exec("SELECT type FROM PRODUITS");

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
    chart->setTitle("Statistiques pour l'attribut \"type de produit\"");
    chart->setTheme(QChart::ChartThemeDark);
    chart->setBackgroundVisible(false);
    chart->setBackgroundBrush(Qt:: white) ;



                 chart->legend()->show();
                 QChartView *chartView = new QChartView(chart);
                 chartView->setRenderHint(QPainter::Antialiasing);
                 //ui->verticalLayout->addWidget(chartView);
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


void inteface_produit:: update_label()
{

    data=A.read_from_arduino();


                 // data=A.getserial()->readLine();
                  data_text += QString::fromStdString(data.toStdString());
                    if (data_text.endsWith("\r\n"))
                    {data_text = data_text.trimmed();
                        qDebug()<<"Data Received: " <<data_text;
                        QSqlQuery query;
                                         query.exec("select * from PRODUITS where idproduit='"+data_text+"'");
                                         {
                                     while(query.next()){

                                                 ui->lineEdit_idproduit->setText(query.value(0).toString());
                                                 ui->lineEdit_nom->setText(query.value(1).toString());
                                                 ui->lineEdit_type->setText(query.value(2).toString());
                                                 ui->lineEdit_quantite->setText(query.value(3).toString());
                                                 //ui->lineEdit_disponibilite->setText(query.value(4).toString());
                                                 ui->lineEdit_prix->setText(query.value(5).toString());


                                            }}

                    }


}



void inteface_produit::notifer()
{
   /* data=A.getserial()->readLine();
     //data=A.read_from_arduino();
     data_text += QString::fromStdString(data.toStdString());
       if (data_text.endsWith("\r\n"))
       {data_text = data_text.trimmed();
           qDebug()<<"Data Received: " <<data_text;
           QSqlQuery query;
                            query.exec("select * from Clients where CIN='"+data_text+"'");
                            {
                       if(query.next()){
                         b= new binome1(data_text,this);
                         b->show();
                               }}
       }*/
    if(A.getserial()->canReadLine())
        {
            data=A.getserial()->readLine();
            data.chop(2);
            qDebug()<<"Data Received: " <<data;
            QSqlQuery query;
                             query.exec("select * from PRODUITS where idproduit='"+data+"'");
                             {
                        if(query.next()){

                          b= new binome1(data,this);
                          b->show();

                                }}


             }
}
void inteface_produit::onDialogFinished(int x){
    delete b;
        b = nullptr;

}

void inteface_produit::on_clients_clicked()
{
    interfaceclient * a = new interfaceclient(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  inteface_produit::close();
}

void inteface_produit::on_employees_clicked()
{
    application * a = new application(current_id,current_user);
  a->setGeometry(geometry());
  a -> show();
  inteface_produit::close();
}

void inteface_produit::on_stations_clicked()
{
    gestionBorne * a = new gestionBorne(current_id,current_user);
  a->setGeometry(geometry());
  a -> show();
  inteface_produit::close();
}

void inteface_produit::on_lougout_clicked()
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
      inteface_produit::close();

     } else {
       qDebug() << "Yes was *not* clicked";
     }
}

void inteface_produit::on_rdv_clicked()
{
    gestionrendezvu * a = new gestionrendezvu(current_id,current_user);
  a->setGeometry(geometry());
  a -> show();
  inteface_produit::close();
}
