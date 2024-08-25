#include "deuxiemeinterface.h"
#include "ui_deuxiemeinterface.h"
#include "historique.h"
#include "borne.h"
#include <QFile>
#include<QTextStream>
#include<QFileDialog>
#include <QMessageBox>
#include<QDir>
#include <QTimer>
#include <QDateTime>
#include <cmath>
#include<cstdlib>
#include <QtCharts>
#include<QChartView>
#include <QPieSeries>
#include<QPieSlice>
deuxiemeinterface::deuxiemeinterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deuxiemeinterface)
{
    ui->setupUi(this);
    ui->lineEdit_2->setValidator(new QIntValidator(0,100,this));
    statistique();
}
deuxiemeinterface::deuxiemeinterface(QString b,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deuxiemeinterface)
{
    ui->setupUi(this);
    ui->lineEdit->setText(b);
    ui->lineEdit_2->setValidator(new QIntValidator(0,100,this));
    statistique();
}


deuxiemeinterface::~deuxiemeinterface()
{
    tab.clear();
    delete ui;
}

void deuxiemeinterface::on_readfile_clicked()
{
   QString filter="(*.txt);;";
   QDir currentdir= QDir::current();
   QString filename=QFileDialog::getOpenFileName(this,"open a file",currentdir.path(),filter);
   QFile file(filename);
   if(!file.open(QFile::ReadOnly | QFile::Text)){
       QMessageBox::warning(this,"title","file not open");
   }
   QTextStream in(&file);
   QString text=in.readAll();
   ui->plainTextEdit->setPlainText(text);
   file.close();
}

bool deuxiemeinterface::verifierint(int x,int y){

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
        ui->label_4->setText("Veuillez entrer que des nombre positives");
            return false;}
    else if(x && x>0 && !y){
        ui->label_4->setText("Veuillez entrer que des nombre");
        ui->label_4->setStyleSheet("QLineEdit {color :red;}");
        ui->label_3->setText("");
            return false;}
    else if(x && x<1 && !y){
        ui->label_4->setText("Veuillez entrer que des nombre");
        ui->label_4->setStyleSheet("QLineEdit {color :red;}");
        ui->label_3->setText("Veuillez entrer que des nombre positives");
            return false;}
    else if(x && y && x<1 && y<1){
        ui->label_4->setText("Veuillez entrer que des nombre positives");
        ui->label_4->setStyleSheet("QLineEdit {color :red;}");
        ui->label_3->setText("Veuillez entrer que des nombre positives");
            return false;}

    else {
        ui->label_4->setText("");
        ui->label_3->setText("");
        return true;}
}

bool deuxiemeinterface::cherchermain(int numborne){
     QString res = QString::number(numborne);
     QSqlQuery query;
  query.prepare("select * from BORNES where NUMBORNE='"+res+"' ");

  query.exec();
  if(!query.next())return false;
  else {
      intensiteBorne=query.value("INTENSITEBORNE").toInt();
      etatBorne=query.value("ETATBORNE").toString();
      return true;}
}

bool deuxiemeinterface::chercherhistorique(int numborne){
     QString res = QString::number(numborne);
     QSqlQuery query;
  query.prepare("select * from HISTORIQUE where NUMBORNE='"+res+"' ");

  query.exec();
  if(!query.next())return false;
  else return true;
}

void deuxiemeinterface::on_activer_clicked()
{
bool x=verifierint(ui->lineEdit->text().toInt(),ui->lineEdit_3->text().toInt());
if(x){
  bool chercher=cherchermain(ui->lineEdit->text().toInt());
  if(chercher && etatBorne=="fonctionelle"){


      bool chercher2=chercherhistorique(ui->lineEdit->text().toInt());
      if(!chercher2){

    Historique h(ui->lineEdit->text().toInt(),ui->lineEdit_3->text().toInt(),ui->lineEdit_2->text().toInt());
    h.ajouter();
    QTimer *timer=new QTimer(this);
    timer->setSingleShot(true);
    float x=h.calculertemps(intensiteBorne);
    int heure,minute,seconde;
    heure=static_cast<int>(x);
    minute=static_cast<int>((x-heure) * 60);
    seconde=static_cast<int>((((x-heure) * 60)-minute)*60);
    /*div_t resultat=div(static_cast<int>(x*60),60);
    heure=static_cast<int>(x)+resultat.quot;
    minute=resultat.rem;*/
    int compteur;
    //compteur=static_cast<int>(x * 3600 * 1000);
    compteur=(minute*60*1000)+(heure*3600*1000)+(seconde*1000);
    timer->start(10000);
    ui->lineEdit_4->setText(QString::number(heure)+" heures "+QString::number(minute)+" minutes "+QString::number(seconde)+" seconde ");
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    tab.insert(ui->lineEdit->text().toInt(),timer);
    int x1=calculerplace();
    if(x1==0){
    A.write_to_arduino("0");//A.write_to_arduino("2");
    }
    else{
    A.write_to_arduino("1");//A.write_to_arduino("2");
    }
      }//chercherhistorique
      else{ui->label_3->setText("La borne est activee pour le moment");}
  }//cherchermain
  else if(!chercher){ui->label_3->setText("La borne n'existe pas");}
  else if(etatBorne=="en panne"){ui->label_3->setText("La borne est en panne");}
}//verifierint
}//onactiveclicked



void deuxiemeinterface::onTimerTimeout(){

    QMap<int, QTimer*>::iterator it;
    it=tab.begin();
    while(it != tab.end()) {
        int num = it.key();
        QTimer *timer = it.value();
        if (!timer->isActive()) {
            ecrirefichier(num);
            historique.supprimer(num);
            it = tab.erase(it);
        }
        else {it++;}
    }
}


void deuxiemeinterface::ecrirefichier(int n){
    QString res = QString::number(n);
    QSqlQuery query;
    query.prepare("select * from HISTORIQUE where NUMBORNE=:NUMBORNE ");
    query.bindValue(":NUMBORNE",n);
    if(query.exec()){
        while(query.next()){
        int numborne=query.value("NUMBORNE").toInt();
        int capacite=query.value("CAPACITE").toInt();
        int pourcentage=query.value("POURCENTAGE").toInt();
        QString date=query.value("DATEDEBUT").toString();
        QString fmt="yyyy-MM-ddThh:mm:ss";
        QDateTime dt=QDateTime::fromString(date,fmt);
        QString timeStr=dt.toString("yyyy-MM-dd")+".txt";
        QFile file(timeStr);
        if(!file.open(QFile::Append | QFile::Text)){
            QMessageBox::warning(this,"title","file not open");
        }
        QTextStream out(&file);
        out<<"Borne : "<<numborne<<" Date : "<<date<<" Capacite : "<<capacite<<" Pourcentage : "<<pourcentage<<endl;
        file.flush();
        file.close();

        }
    }
}


void deuxiemeinterface::statistique(){
    int fonct=0,panne=0;
    QSqlQuery query;
    query.prepare("select * from BORNES");
    if(query.exec()){
       while(query.next()){
           QString etat=query.value("ETATBORNE").toString();
           if(etat=="fonctionelle") fonct++;
           else panne++;
       }
    }
    QString fonct_string = QString::number(fonct);
    QString panne_string = QString::number(panne);

    QPieSeries *series=new QPieSeries();
    series->setHoleSize(0.35);
    series->append("Fonctionelle :  "+fonct_string,fonct)->setColor("#023020");
    series->append("En panne :  "+panne_string,panne)->setColor("#306844");

    QChart *chart=new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("L'etats des bornes");
  //  chart->setTheme(QChart::ChartThemeDark);
   chart->setTitleBrush(QBrush(QColor("black")));

    chart->setBackgroundBrush(QBrush(QColor("white")));

    chart->setBackgroundVisible(true);


    QChartView *chartview=new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setParent(ui->frame_3);
    chartview->setMinimumSize(400,450);
    chartview->setMaximumSize(500,450);


}
int deuxiemeinterface::calculerplace(){
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


