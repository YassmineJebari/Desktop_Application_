#include "borne.h"

Borne::Borne(){

}

    Borne::Borne(QString idEmployee,int numBorne,int intensiteBorne,QString imageBorne,QString etatBorne){
        this->idEmployee=idEmployee;
        this->numBorne=numBorne;
        this->intensiteBorne=intensiteBorne;
        this->imageBorne=imageBorne;
        this->etatBorne=etatBorne;
    }

    bool Borne::ajouter(){
        QSqlQuery query;
        QString res = QString::number(numBorne);
        QString res1 = QString::number(intensiteBorne);
     query.prepare("insert into BORNES (ID_EMPLOYE,NUMBORNE,INTENSITEBORNE,IMAGEBORNE,ETATBORNE)""values(:ID_EMPLOYE,:NUMBORNE,:INTENSITEBORNE,:IMAGEBORNE,:ETATBORNE)");
query.bindValue(":ID_EMPLOYE",idEmployee);
query.bindValue(":NUMBORNE",res);
query.bindValue(":INTENSITEBORNE",res1);
query.bindValue(":ETATBORNE",etatBorne);
query.bindValue(":IMAGEBORNE",imageBorne);
return query.exec();
    }
    bool Borne::supprimer(int numBorne){
        QSqlQuery query;
        QString res=QString::number(numBorne);
         query.prepare("Delete from BORNES where NUMBORNE =:NUMBORNE");
         query.bindValue(":NUMBORNE",res);
         return query.exec();
    }
    QSqlQueryModel * Borne::afficher(){
        QSqlQueryModel * model=new QSqlQueryModel();
        model->setQuery("select * from BORNES");
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("NUMBORNE"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("IMAGEBORNE"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("ETATBORNE"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("DATECREATION"));
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("INTENSITEBORNE"));
        model->setHeaderData(5,Qt::Horizontal,QObject::tr("ID_EMPLOYE"));



        return model;
    }

    bool Borne::update(int NUMBORNE,int INTENSITEBORNE,QString ETATBORNE,QString IMAGEBORNE)
     {
         QSqlQuery query;
         QString res = QString::number(NUMBORNE);
         QString res1 = QString::number(INTENSITEBORNE);
         query.prepare("UPDATE BORNES SET INTENSITEBORNE=:INTENSITEBORNE, ETATBORNE=:ETATBORNE, IMAGEBORNE=:IMAGEBORNE WHERE NUMBORNE=:NUMBORNE ;");
         query.bindValue(":NUMBORNE",res);
         query.bindValue(":INTENSITEBORNE",res1);
         query.bindValue(":ETATBORNE",ETATBORNE);
         query.bindValue(":IMAGEBORNE",IMAGEBORNE);
         return query.exec();
         /*QSqlQuery query;
         QString res = QString::number(NUMBORNE);
         QString res1 = QString::number(INTENSITEBORNE);
         query.prepare("UPDATE BORNES SET INTENSITEBORNE='"+res1+"', ETATBORNE='"+ETATBORNE+"', IMAGEBORNE='"+IMAGEBORNE+"' WHERE NUMBORNE='"+res+"' ;");
         query.bindValue(":NUMBORNE",res);
         query.bindValue(":INTENSITEBORNE",res1);
         query.bindValue(":ETATBORNE",ETATBORNE);
         query.bindValue(":IMAGEBORNE",IMAGEBORNE);
         return query.exec();*/
     }

    bool Borne::chercher(int numborne){
         QString res = QString::number(numborne);
         QSqlQuery query;
      query.prepare("select * from BORNES where NUMBORNE=:NUMBORNE ");
query.bindValue(":NUMBORNE",res);
      query.exec();
      if(!query.next())return false;
      else return true;
    }



