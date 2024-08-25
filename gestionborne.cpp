#include "gestionborne.h"
#include "ui_gestionborne.h"
#include "borne.h"
#include <QString>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QString>
#include <QPixmap>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QTextTable>
#include <QPrinter>
#include <QTableWidget>
#include <QTableWidgetItem>
#include<QDate>
#include<QDebug>
#include <QSortFilterProxyModel>
#include "mainwindow.h"
#include "application.h"
#include "interfaceclient.h"
#include "inteface_produit.h"
#include "gestionrendezvu.h"

gestionBorne::gestionBorne(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gestionBorne)
{
    ui->setupUi(this);
    ui->tableView->setModel(Etmp.afficher());
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->lineEdit->setValidator(new QIntValidator(0, 999999, this)) ;

    //recherche
    connect(ui->lineEdit_recherche, SIGNAL(textChanged(const QString &)), this, SLOT(on_lineEdit_recherche_textChanged(const QString &arg1)));
    //tri
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(Etmp.afficher());
    ui->tableView->setModel(m);
    ui->tableView->setSortingEnabled(true);

    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(notifer()));
     int x=calculerplace();
     if(x==0){
     A.write_to_arduino("0");//A.write_to_arduino("2");
     }
     else{
     A.write_to_arduino("1");//A.write_to_arduino("2");
     }
}
void gestionBorne::show_tables()
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //permet recherche et tri
        QSortFilterProxyModel *proxy = new QSortFilterProxyModel();
        //definir la source (tableau original)
        proxy->setSourceModel(Etmp.afficher());
        proxy->setDynamicSortFilter(true);
        ui->tableView->setModel(proxy);
        ui->tableView->setSortingEnabled(true);

}

gestionBorne::gestionBorne(QString currentid,QString currentuser,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gestionBorne)
{
    ui->setupUi(this);
    current_id=  currentid;
    current_user=currentuser;
    ui->username->setText(current_user);
    ui->tableView->setModel(Etmp.afficher());
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //recherche
    connect(ui->lineEdit_recherche, SIGNAL(textChanged(const QString &)), this, SLOT(on_lineEdit_recherche_textChanged(const QString &arg1)));
    //tri
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(Etmp.afficher());
    ui->tableView->setModel(m);
    ui->tableView->setSortingEnabled(true);

    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(notifer()));
     int x=calculerplace();
     if(x==0){
     A.write_to_arduino("0");//A.write_to_arduino("2");
     }
     else{
     A.write_to_arduino("1");//A.write_to_arduino("2");
     }
}
void gestionBorne::notifer(){
    if(A.getserial()->canReadLine())
    {
        data=A.getserial()->readLine();
        data.chop(2);
        qDebug()<<"Data Received: " <<data;
         int distance = data.split(':')[1].toInt();
         if(distance<=24 && distance>8){
             ui->pushButton_2->setStyleSheet("border-color:red;");
             A.close_arduino();
         }
    }
}

gestionBorne::~gestionBorne()
{
    delete ui;
    A.close_arduino();
}



//chercher une borne dans la bd par numero
bool gestionBorne::cherchermain(int numborne){
     QString res = QString::number(numborne);
     QSqlQuery query;
  query.prepare("select * from BORNES where NUMBORNE='"+res+"' ");

  query.exec();
  if(!query.next())return false;
  else return true;
}





//verifier que les donnes entrer sont des entiers
bool gestionBorne::verifierint(int x,int y){

    if(!x && !y){
      ui->label_3->setText("Veuillez entrer que des nombre");
      ui->label_4->setText("Veuillez entrer que des nombre");
      ui->label_3->setStyleSheet("QLineEdit {color :red;}");
      ui->label_4->setStyleSheet("QLineEdit {color :red;}");
            return false;}
    else if(!x && y && y>0){
        ui->label_3->setText("Veuillez entrer que des nombre");
        ui->label_3->setStyleSheet("QLineEdit {color :red;}");
        ui->label_4->setText("");
            return false;}
    else if(!x && y && y<1){
        ui->label_3->setText("Veuillez entrer que des nombre");
        ui->label_3->setStyleSheet("QLineEdit {color :red;}");
        ui->label_4->setText("must enter positive numbers");
            return false;}
    else if(x && x>0 && !y){
        ui->label_4->setText("Veuillez entrer que des nombre");
        ui->label_4->setStyleSheet("QLineEdit {color :red;}");
        ui->label_3->setText("");
            return false;}
    else if(x && x<1 && !y){
        ui->label_4->setText("Veuillez entrer que des nombre");
        ui->label_4->setStyleSheet("QLineEdit {color :red;}");
        ui->label_3->setText("must enter positive numbers");
            return false;}
    else if(x && y && x<1 && y<1){
        ui->label_4->setText("Veuillez entrer que des nombre positives");
        ui->label_4->setStyleSheet("QLineEdit {color :red;}");
        ui->label_3->setText("Veuillez entrer que des nombre");
            return false;}

    else {
        ui->label_4->setText("");
        ui->label_3->setText("");
        return true;}
}





//ajout
void gestionBorne::on_add_clicked()
{
    /*ui->lineEdit->setValidator(new QIntValidator(0,1000,this));
    if(!ui->lineEdit->hasAcceptableInput()){QMessageBox::critical(nullptr, QObject::tr("not int "),
                                                           QObject::tr("not int.\n"
                                                          "Click Cancel to exit."), QMessageBox::Cancel);}*/
    //int ID_EMPLOYEE=1;
    int numBorne=ui->lineEdit->text().toInt();
    int intensiteBorne=ui->lineEdit_2->text().toInt();
    bool verif=verifierint(numBorne,intensiteBorne);
    if(verif){
     QString etatBorne;
     if (ui->radioButton->isChecked()){etatBorne="en panne";}
     if (ui->radioButton_2->isChecked()){etatBorne="fonctionelle";}
     QString imageBorne=image;

    Borne b(current_id,numBorne,intensiteBorne,imageBorne,etatBorne);
    bool chercher=b.chercher(numBorne);
    if(!chercher){
    bool test=b.ajouter();
    if(test){
        ui->tableView->setModel(Etmp.afficher());
        show_tables();
        QMessageBox::information(nullptr, QObject::tr("ok"),
                    QObject::tr("ajouter.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        int x=calculerplace();
        if(x==0){
        A.write_to_arduino("0");//A.write_to_arduino("2");
        }
        else{
        A.write_to_arduino("1");//A.write_to_arduino("2");
        }
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("not ok "),
                    QObject::tr("ajout failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
    }
    }
    else{
        ui->label_3->setText("Borne existe");
    }
    }
}


//supression
void gestionBorne::on_update_clicked()
{
    int numBorne=ui->lineEdit->text().toInt();
    if(numBorne){
        ui->label_3->setText("");
        ui->label_4->setText("");

        bool cherche=cherchermain(numBorne);
        if(cherche){
bool test=Etmp.supprimer(numBorne);
if(test){
    ui->tableView->setModel(Etmp.afficher());
    show_tables();
    QMessageBox::information(nullptr, QObject::tr("ok"),
                QObject::tr("supprimer.\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
    int x=calculerplace();
    if(x==0){
    A.write_to_arduino("0");//A.write_to_arduino("2");
    }
    else{
    A.write_to_arduino("1");//A.write_to_arduino("2");
    }
}
else
{
    QMessageBox::critical(nullptr, QObject::tr("not ok "),
                QObject::tr("suppression failed.\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
}
        }
        else{ui->label_3->setText("Le numero n'existe pas");}
    }
    else{
        ui->label_3->setText("Veuillez entrer que des nombre");
        ui->label_4->setText("");
        ui->label_3->setStyleSheet("QLineEdit {color :red;}");
    }
}




//modifier
void gestionBorne::on_delete_2_clicked()
{
    int numBorne=ui->lineEdit->text().toInt();
    int intensiteBorne=ui->lineEdit_2->text().toInt();
    bool verif=verifierint(numBorne,intensiteBorne);
    if(verif){
    QString etatBorne;
    if (ui->radioButton->isChecked()){etatBorne="en panne";}
    if (ui->radioButton_2->isChecked()){etatBorne="fonctionelle";}
    QString imageBorne=image;
    bool cherche=cherchermain(numBorne);
    if(cherche){
bool test=Etmp.update(numBorne,intensiteBorne,etatBorne,imageBorne);
if(test){
    ui->tableView->setModel(Etmp.afficher());
    show_tables();
    QMessageBox::information(nullptr, QObject::tr("ok"),
                QObject::tr("update.\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
    int x=calculerplace();
    if(x==0){
    A.write_to_arduino("0");//A.write_to_arduino("2");
    }
    else{
    A.write_to_arduino("1");//A.write_to_arduino("2");
    }
}
else
{
    QMessageBox::critical(nullptr, QObject::tr("not ok "),
                QObject::tr("update failed.\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
}
    }
    else{ui->label_3->setText("Le numero n'existe pas");}


}
}





//insertion image
void gestionBorne::on_pushButton_5_clicked()
{
    QString filter="(*.jpg);;(*.png);;(*.jpeg);;";
   QString filename=QFileDialog::getOpenFileName(this,"open a file","C:/Bureau",filter);
   //ui->label->setText(filename);
   QString path=filename;
  QPixmap pic(path);
   ui->label_2->setPixmap(pic.scaled(130,130,Qt::KeepAspectRatio));
   image=filename;
}




//recuperation des donnes
void gestionBorne::on_tableView_activated(const QModelIndex &index)
{
    QString val=ui->tableView->model()->data(index).toString();
    QSqlQuery query;
 query.prepare("select * from BORNES where  NUMBORNE=:NUMBORNE ");
 query.bindValue(":NUMBORNE",val);
if(query.exec()){

    while(query.next()){
        ui->lineEdit->setText(query.value(0).toString());
        ui->lineEdit_2->setText(query.value(4).toString());

        if(query.value(2).toString()=="en panne"){ui->radioButton->setChecked(true);}
        if(query.value(2).toString()=="fonctionelle"){ui->radioButton_2->setChecked(true);}

        QString path=query.value(1).toString();
       QPixmap pic(path);
        ui->label_2->setPixmap(pic.scaled(130,130,Qt::KeepAspectRatio));
        image=path;
}
}
}




//recherche
void gestionBorne::on_lineEdit_recherche_textChanged(const QString &arg1)
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


/*
//tri
void MainWindow::tri()
{

    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(Etmp.afficher());
    ui->tableView->setModel(m);
    ui->tableView->setSortingEnabled(true);
}
*/
//exportation pdf
void gestionBorne::on_pdf_clicked()
{
    //saveTableViewToPdf(ui->tableView,"donnés_relatives_aux_bornes","mybornes.pdf");
    QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);

        printer.setOutputFileName("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/mybornes.pdf");

        QTableView* tableView = ui->tableView; // replace with the name of your QTableView

        // create a QTextDocument with the contents of the QTableView
        QTextDocument doc;
        QTextCursor cursor(&doc);
        doc.setPageSize(printer.pageRect().size());
           doc.setDocumentMargin(10.0);

        // insert the title in the middle
            QTextBlockFormat blockFormat;
            blockFormat.setAlignment(Qt::AlignCenter);
            cursor.insertBlock(blockFormat);
            QTextCharFormat titleFormat;
            titleFormat.setFontWeight(QFont::Bold);
            titleFormat.setFontPointSize(14);
            cursor.insertText("Donnés relatives aux bornes", titleFormat);

            // insert the image in the top left corner
            QImage image("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/img/logo.png");
            if (!image.isNull()) {
                QPixmap pixmap = QPixmap::fromImage(image);
                if (!pixmap.isNull()) {
                    pixmap = pixmap.scaled(QSize(200, 50), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    QTextImageFormat imageFormat;
                    imageFormat.setWidth(pixmap.width());
                    imageFormat.setHeight(pixmap.height());
                    imageFormat.setName("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/mybornes.pdf");
                    cursor.movePosition(QTextCursor::Start);
                    cursor.insertImage(imageFormat);
                    doc.addResource(QTextDocument::ImageResource, QUrl("D:/Users/lenovo/Documents/QT/integfinal/2a6-e-charging/PDF/mybornes.pdf"), QVariant(pixmap));
                }
            }

            // insert the table
            //cursor.insertBlock();
            QTextBlockFormat tableBlockFormat;
                tableBlockFormat.setAlignment(Qt::AlignCenter);
                cursor.insertBlock(tableBlockFormat);
        //cursor.insertText("Table View Data\n\n");
        int rows = tableView->model()->rowCount();
        int columns = tableView->model()->columnCount();
        QTableWidget table(rows + 1, columns);
        table.setHorizontalHeaderLabels(QStringList() << "Numero" << "Image" << "Etat" << "Date" << "Intensité" << "Id empl"); // replace with your column headers
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                table.setItem(row + 1, column, new QTableWidgetItem(tableView->model()->data(tableView->model()->index(row, column)).toString()));
            }
        }
        QTextTableFormat tableFormat;
        tableFormat.setHeaderRowCount(1);
        QTextTable* textTable = cursor.insertTable(rows + 1, columns, tableFormat);

        for (int column = 0; column < columns; column++) {
            QTextTableCell headerCell = textTable->cellAt(0, column);
            QTextCursor headerCursor = headerCell.firstCursorPosition();
            headerCursor.insertText(table.horizontalHeaderItem(column)->text());
        }

        QTextCharFormat cellFormat;
        cellFormat.setFont(QFont("Helvetica", 10, QFont::Bold));
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                QTextTableCell cell = textTable->cellAt(row + 1, column);
                cell.setFormat(cellFormat);
                cell.firstCursorPosition().insertText(tableView->model()->data(tableView->model()->index(row, column)).toString());
            }
        }
        //la date ou le pdf a ete generer
        cursor.movePosition(QTextCursor::PreviousBlock);
            QTextBlockFormat dateBlockFormat;
            dateBlockFormat.setAlignment(Qt::AlignRight);
            cursor.insertBlock(dateBlockFormat);
            QTextCharFormat dateFormat;
            dateFormat.setFontPointSize(8);
            cursor.insertText("Generated on " + QDate::currentDate().toString("dd/MM/yyyy"), dateFormat);

        // print the QTextDocument to the PDF printer
        QPainter painter(&printer);
        painter.fillRect(0,0,575 ,13650,QColor(11,156,49,100));
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        doc.setPageSize(printer.pageRect().size());
        doc.drawContents(&painter);
}

// ouvrire deuxieme interface
void gestionBorne::on_historique_clicked()
{
  dinter = new deuxiemeinterface(this);
  dinter->show();
}

void gestionBorne::on_pushButton_2_clicked()
{
    ui->pushButton_2->setStyleSheet("border-color:transparent;");
    capt=new Capteurultrason(this);
    connect(capt, SIGNAL(finished(int)), this, SLOT(onDialogFinished(int)));
    capt->show();
}

void gestionBorne::onDialogFinished(int x){
    delete capt;
        capt = nullptr;
        int ret=A.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }
         QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(notifer()));
}
int gestionBorne::calculerplace(){
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


void gestionBorne::on_logout_clicked()
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
      gestionBorne::close();

     } else {
       qDebug() << "Yes was *not* clicked";
     }
}

void gestionBorne::on_employees_clicked()
{
    application * a = new application(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  gestionBorne::close();
}

void gestionBorne::on_clients_clicked()
{
    interfaceclient * a = new interfaceclient(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  gestionBorne::close();
}

void gestionBorne::on_products_clicked()
{
    inteface_produit * a = new inteface_produit(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  gestionBorne::close();
}

void gestionBorne::on_rdv_clicked()
{
    gestionrendezvu * a = new gestionrendezvu(current_id,current_user);
a->setGeometry(geometry());
  a -> show();
  gestionBorne::close();
}
