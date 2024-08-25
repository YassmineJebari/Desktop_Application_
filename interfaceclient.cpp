#include "interfaceclient.h"
#include "ui_interfaceclient.h"
#include "client.h"
#include "dialog.h"
#include "smtp.h"
#include "notifications.h"
#include <QMessageBox>
#include <QIntValidator>



#include <QPrinter>
#include <QPainter>
#include<QTextStream>
#include<QGraphicsView>
#include<QtPrintSupport/QPrintDialog>
#include<QPdfWriter>
#include<QUrlQuery>
#include <QDesktopServices>
#include <QTextDocument>
#include <QDate>


#include <QtCharts>
#include<QChartView>
#include <QPieSeries>
#include<QPieSlice>
// les gestions
#include "mainwindow.h"
#include "application.h"
#include "inteface_produit.h"
#include "gestionborne.h"
#include "gestionrendezvu.h"



interfaceclient::interfaceclient(QString currentid,QString currentuser,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::interfaceclient)
{
    ui->setupUi(this);
    current_id=  currentid;
    current_user=currentuser;
    ui->username->setText(current_user);


       ui->tableView->setModel(c.afficher());
    show_tables();
     ui->lineEdit_cin->setValidator(new QIntValidator(0, 999999, this)) ;
      ui->lineEdit_phone->setValidator(new QIntValidator(0,99999999, this)) ;
      email_regex= QRegExp("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
     cin_regex=QRegExp("[0-9]{6}$");
      chaine_regex=QRegExp("[a-zA-Z ]{4,20}$");
     tel_regex=QRegExp("[0-9]{8}$");
     int ret=A.connect_arduino(); // lancer la connexion à arduino
       switch(ret){
       case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
           break;
       case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
          break;
       case(-1):qDebug() << "arduino is not available";
       }
       //QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
        //le slot update_label suite à la reception du signal readyRead (reception des données).
      //   A.write_to_arduino("2");
         QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(notifer()));
}

interfaceclient::~interfaceclient()
{
    delete ui;
    A.close_arduino();
}

void interfaceclient::on_add_clicked()
{
    //rep_donneeee

    ui->label_cin->setText("");
     ui->label_phone->setText("");
     ui->label_nom->setText("");
      ui->label_prenom->setText("");
      ui->label_email->setText("");
ui->label_region->setText("");

    int cin=ui->lineEdit_cin->text().toInt();
    QString nom=ui->lineEdit_nom->text();
    QString prenom=ui->lineEdit_prenom->text();
     int phone=ui->lineEdit_phone->text().toInt();
    QString email=ui->lineEdit_email->text();
 QString region=ui->lineEdit_region->text();


     bool mail_verif = email_regex.exactMatch(ui->lineEdit_email->text());
     bool cin_verif=cin_regex.exactMatch(ui->lineEdit_cin->text());
     bool nom_verif=chaine_regex.exactMatch(ui->lineEdit_nom->text());
     bool prenom_verif=chaine_regex.exactMatch(ui->lineEdit_prenom->text());
     bool phone_verif=tel_regex.exactMatch(ui->lineEdit_phone->text());
bool region_verif=chaine_regex.exactMatch(ui->lineEdit_region->text());
     if(cin_verif && nom_verif && prenom_verif && phone_verif  && mail_verif && region_verif )
     {


         client c(cin,nom,prenom,email,phone,region);

bool chercher=c.chercherexistid(cin);
         if(!chercher)
         {
     bool test=c.ajouter();



     if(test){

         ui->tableView->setModel(c.afficher());
show_tables();
         notifications n;
   n.notifications_ajouterClient();
        QMessageBox::information(nullptr, QObject::tr("ok"),
                            QObject::tr("add successful.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
        ui->lineEdit_cin->setText("");
        ui->lineEdit_nom->setText("");
         ui->lineEdit_prenom->setText("");
         ui->lineEdit_email->setText("");
          ui->lineEdit_phone->setText("");
          ui->lineEdit_region->setText("");
          Smtp* smtp = new Smtp("yassmine.jebari@esprit.tn","hvytcvruoxaftlss", "smtp.gmail.com", 465);

          smtp->sendMail(ui->lineEdit_nom->text(),email , "Client(e) ajouté(e)" ,"Cher(e) client(e)"+nom+" "+prenom+",\n Bienvenue chez notre station de charge electrique\n");

        }


         }else {

                 ui->label_cin->setText("cin already exist in table");
                 ui->label_cin->setStyleSheet("color: red");



}}

     else
     {



         if (!cin_verif){

             ui->label_cin->setText("Only 8 numbers are acceptable");
             ui->label_cin->setStyleSheet("color: red");


         }

         if (!phone_verif){
             ui->label_phone->setText("Only numbers are acceptable");
             ui->label_phone->setStyleSheet("color: red");
         }

         if (!nom_verif){

             ui->label_nom->setText("need at least 2 characters");
             ui->label_nom->setStyleSheet("color: red");

         }

         if (!prenom_verif){
             ui->label_prenom->setText("need at least 2 characters");
             ui->label_prenom->setStyleSheet("color: red");
         }
         if (!region_verif){
             ui->label_region->setText("need at least 5 characters");
             ui->label_region->setStyleSheet("color: red");
         }
         if (!mail_verif){

             ui->label_email->setText("Email doesnt meet requirements");
                 ui->label_email->setStyleSheet("color :red");





                           QMessageBox::warning(nullptr, QObject::tr("not ok "),
                                       QObject::tr("ajout failed \n"
                                                   "Click Cancel to exit."), QMessageBox::Cancel);

     }



}
}

void interfaceclient::on_delete_2_clicked()
{
   // int cin=ui->lineEdit_cin->text().toInt();
    bool test=c.supprimer(selected);
    if(test)
    {
        ui->tableView->setModel(c.afficher());
        show_tables();
        notifications n;
  n.notifications_supprimerClient();

        QMessageBox::information(nullptr, QObject::tr("ok"),
                            QObject::tr("delete successful.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);

        }
            else
                QMessageBox::critical(nullptr, QObject::tr("not ok "),
                            QObject::tr("delete failed.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);



}

void interfaceclient::on_pushButton_5_clicked()
{
    ui->label_cin->setText("");
     ui->label_phone->setText("");
     ui->label_nom->setText("");
      ui->label_prenom->setText("");
      ui->label_email->setText("");
ui->label_region->setText("");

    int cin=ui->lineEdit_cin->text().toInt();
    QString nom=ui->lineEdit_nom->text();
    QString prenom=ui->lineEdit_prenom->text();
    int phone=ui->lineEdit_phone->text().toInt();
    QString email=ui->lineEdit_email->text();
    QString region=ui->lineEdit_region->text();

    bool mail_verif = email_regex.exactMatch(ui->lineEdit_email->text());
   // bool cin_verif=cin_regex.exactMatch(ui->lineEdit_cin->text());
    bool nom_verif=chaine_regex.exactMatch(ui->lineEdit_nom->text());
    bool prenom_verif=chaine_regex.exactMatch(ui->lineEdit_prenom->text());
    bool phone_verif=tel_regex.exactMatch(ui->lineEdit_phone->text());
    bool region_verif=chaine_regex.exactMatch(ui->lineEdit_region->text());

    if(nom_verif && prenom_verif && mail_verif && region_verif && phone_verif)
    {


        client c(cin,nom,prenom,email,phone,region);



    bool test=c.modifier(cin);



    if(test){

        ui->tableView->setModel(c.afficher());
        show_tables();
        notifications n;
  n.notifications_modifierClient();
  Smtp* smtp = new Smtp("yassmine.jebari@esprit.tn","hvytcvruoxaftlss", "smtp.gmail.com", 465);

  smtp->sendMail(ui->lineEdit_nom->text(),email , "Client(e) modifié(e)" ,"Cher(e) client(e)"+c.getNom()+" "+c.getPreom()+",\n Nous avons changé vos informations pour quelques raisons de securité\n CIN : "+c.getCin()+"\n Nom : "+nom+" \n Prenom : "+prenom+" \n Email : "+email+"\n Phone : "+phone+" \n Region : "+region+"\n Cordialement ");
       QMessageBox::information(nullptr, QObject::tr("ok"),
                           QObject::tr("update successful.\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
       ui->lineEdit_cin->setText("");
       ui->lineEdit_nom->setText("");
        ui->lineEdit_prenom->setText("");
        ui->lineEdit_email->setText("");
         ui->lineEdit_phone->setText("");
          ui->lineEdit_region->setText("");


       }


    }


    else
    {





        if (!phone_verif){
            ui->label_phone->setText("Only numbers are acceptable");
            ui->label_phone->setStyleSheet("color: red");
        }

        if (!nom_verif){

            ui->label_nom->setText("need at least 2 characters");
            ui->label_nom->setStyleSheet("color: red");

        }

        if (!prenom_verif){
            ui->label_prenom->setText("need at least 2 characters");
            ui->label_prenom->setStyleSheet("color: red");
        }
        if (!region_verif){
            ui->label_region->setText("need at least 5 characters");
            ui->label_region->setStyleSheet("color: red");
        }
        if (!mail_verif){

            ui->label_email->setText("Email doesnt meet requirements");
                ui->label_email->setStyleSheet("color :red");





                          QMessageBox::warning(nullptr, QObject::tr("not ok "),
                                      QObject::tr("update failed \n"
                                                  "Click Cancel to exit."), QMessageBox::Cancel);

    }



}


}

void interfaceclient::on_tableView_clicked(const QModelIndex &index)
{
    selected=ui->tableView->model()->data(index).toInt();



}

void interfaceclient::on_tableView_activated(const QModelIndex &index)
{
    QString cin=ui->tableView->model()->data(index).toString();
            QSqlQuery query;
         query.prepare("select * from CLIENTS where  cin='"+cin+"' ");
        if(query.exec()){

            while(query.next()){


                ui->lineEdit_cin->setText(query.value(0).toString());
                ui->lineEdit_nom->setText(query.value(1).toString());
                ui->lineEdit_prenom->setText(query.value(2).toString());
                ui->lineEdit_email->setText(query.value(3).toString());
                ui->lineEdit_phone->setText(query.value(4).toString());
                ui->lineEdit_region->setText(query.value(5).toString());
    QString val=ui->tableView->model()->data(index).toString();
    QString nom=query.value(1).toString();
    const char * p= nom.toStdString().c_str();

    A.write_to_arduino(p);
           }

        }}

void interfaceclient::on_mail_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();

    QString email_recipient = select->selectedRows(3).value(0).data().toString();
    QString nom_recipient = select->selectedRows(1).value(0).data().toString();
    QString prenom_recipient = select->selectedRows(2).value(0).data().toString();

    QDialog *d=new dialog(email_recipient,nom_recipient,prenom_recipient,this);
    d->show();
    d->exec();
}
//pdf
void interfaceclient::on_pushButton_6_clicked()
{
    QPdfWriter pdf("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/client.pdf");
        QPainter painter(&pdf);






      //  painter.fillRect(0,0,9600 ,13650,QColor(248, 246, 231));
         painter.fillRect(0,0,9600 ,13650,QColor(245, 246, 226));


       int i = 4000;

       painter.fillRect(100,100,6000,1900,QColor(217, 224, 197));

            painter.setPen(QColor(16, 127, 54));
            painter.setFont(QFont("Comic Sans MS", 30));
            painter.drawText(700,1100,"Liste Des Clients");
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
            painter.drawText(1200,3300,"Nom");
            painter.drawText(2400,3300,"Prenom");
            painter.drawText(3500,3300,"Email");
            painter.drawText(6600,3300,"Phone");
             painter.drawText(7500,3300,"Region");


            painter.setPen(QColor(47, 62,150));
         QString date =   "Generated on "  +   QDate::currentDate().toString("dd/MM/yyyy");
         painter.drawText(6700,2800,date);
        // painter.drawText(6500,13500,date);



            QSqlQuery query;
            query.prepare("select * from CLIENTS");
            query.exec();
            while (query.next())
            {
                painter.setPen(Qt::darkMagenta);
                painter.drawText(100,i,query.value(0).toString());
                painter.setPen(Qt::black);
                painter.drawText(1200,i,query.value(1).toString());
                painter.drawText(2400,i,query.value(2).toString());
                painter.drawText(3500,i,query.value(3).toString());
                painter.drawText(6600,i,query.value(4).toString());
                painter.drawText(7500,i,query.value(5).toString());





               i = i + 500;
            }
            int reponse = QMessageBox::question(this, "Génerer PDF", "PDF Enregistré! -Voulez-Vous Affichez Le PDF ?", QMessageBox::Yes |  QMessageBox::No);
                if (reponse == QMessageBox::Yes)
                {
                    QDesktopServices::openUrl(QUrl::fromLocalFile("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/client.pdf"));
                    painter.end();
                }
                if (reponse == QMessageBox::No)
                {
                     painter.end();
                }








}
//tri recherche
void interfaceclient::show_tables()
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //permet recherche et tri
        proxy = new QSortFilterProxyModel();
        //definir la source (tableau original)
        proxy->setSourceModel(c.afficher());
       //pour la recherche
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxy->setFilterKeyColumn(-1); // chercher dans tout le tableau (-1) ou donner le numero de la colone
        //faire tri
        proxy->setDynamicSortFilter(true);
        ui->tableView->setSortingEnabled(true);
        //remplissage tableau avec le masque
        ui->tableView->setModel(proxy);

}

void interfaceclient::on_lineEdit_5_textChanged(const QString &arg1)
{
    proxy->setFilterFixedString(arg1);
}

void interfaceclient::on_stat_clicked()
{
    /* stats *w=new stats();
              w->stat();
             // w->show();
             w->exec();*/





       QSqlQuery query;
          query.exec("SELECT region FROM clients");

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
       chart->setTitle("Pourcentage des clients par region");
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
void interfaceclient:: update_label()
{

    data=A.read_from_arduino();


                 // data=A.getserial()->readLine();
                  data_text += QString::fromStdString(data.toStdString());
                    if (data_text.endsWith("\r\n"))
                    {data_text = data_text.trimmed();
                        qDebug()<<"Data Received: " <<data_text;
                        QSqlQuery query;
                                         query.exec("select * from Clients where CIN='"+data_text+"'");
                                         {
                                     while(query.next()){

                                        ui->lineEdit_cin->setText(query.value(0).toString());
                                                 ui->lineEdit_nom->setText(query.value(1).toString());
                                                 ui->lineEdit_prenom->setText(query.value(2).toString());
                                                 ui->lineEdit_email->setText(query.value(3).toString());
                                                 ui->lineEdit_phone->setText(query.value(4).toString());
                                                 ui->lineEdit_region->setText(query.value(5).toString());


                                            }}

                    }


}



void interfaceclient::notifer()
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
   /* if(A.getserial()->canReadLine())
        {
            data=A.getserial()->readLine();
            data.chop(2);
            qDebug()<<"Data Received: " <<data;
            QSqlQuery query;
                             query.exec("select * from Clients where CIN='"+data+"'");
                             {
                        if(query.next()){

                          b= new binome1(data,this);
                          b->show();

                                }}


             }*/
}
void interfaceclient::onDialogFinished(int x){
    delete b;
        b = nullptr;

}



void interfaceclient::on_pushButton_7_clicked()
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
      interfaceclient::close();

     } else {
       qDebug() << "Yes was *not* clicked";
     }
}

void interfaceclient::on_employees_clicked()
{
    application * a = new application(current_id,current_user);
  a->setGeometry(geometry());
  a -> show();
  interfaceclient::close();


}

void interfaceclient::on_products_clicked()
{
    inteface_produit * a = new inteface_produit(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  interfaceclient::close();
}

void interfaceclient::on_stations_clicked()
{
    gestionBorne * a = new gestionBorne(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  interfaceclient::close();
}

void interfaceclient::on_rdv_clicked()
{
    gestionrendezvu * a = new gestionrendezvu(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  interfaceclient::close();
}
