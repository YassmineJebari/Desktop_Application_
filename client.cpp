#include "client.h"
#include <QtDebug>
#include <QSqlQuery>
client::client()
{

}

client::client(int cin, QString nom,QString prenom,QString email,int phone,QString region)
{
this->cin=cin;
    this->nom=nom;
    this->prenom=prenom;
    this->email=email;
    this->phone=phone;
    this->region=region;
}
bool client::ajouter()
{
    QSqlQuery query;

    QString res=QString::number(cin);
    QString ph=QString::number(phone);
    query.prepare("INSERT INTO Clients (cin, nom,prenom,email,phone,region) values (:cin, :nom, :prenom, :email, :phone, :region) ");
    query.bindValue(":cin",res);
    query.bindValue(":nom",nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":email",email);
    query.bindValue(":phone",ph);
    query.bindValue(":region",region);
    return  query.exec();

}
QSqlQueryModel* client::afficher()
{
 QSqlQueryModel* model=new QSqlQueryModel();
 model->setQuery("SELECT* FROM clients");
       model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
       model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
       model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
       model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
       model->setHeaderData(4, Qt::Horizontal, QObject::tr("PHONE"));
       model->setHeaderData(5, Qt::Horizontal, QObject::tr("REGION"));


 return  model;
}
QSqlQueryModel* client::afficherArd()
{
 QSqlQueryModel* model=new QSqlQueryModel();
 model->setQuery("SELECT nom prenom FROM clients");

       model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
       model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));



 return  model;
}
bool client :: supprimer(int cin)
{
    QSqlQuery query;
    QString res=QString::number(cin);
    query.prepare("delete from clients where cin = :cin");
    query.bindValue(":cin",res);
    return query.exec();

}
bool client::modifier(int cin)
{
    QSqlQuery query;
    QString res=QString::number(cin);
    QString res1=QString::number(phone);

    query.prepare("UPDATE  clients set nom=:nom,prenom=:prenom,phone=:phone,email=:email, region=:region where cin=:cin");
    query.bindValue(":cin",res);
    query.bindValue(":nom",nom);
    query.bindValue(":prenom",prenom);

    query.bindValue(":phone",res1);
    query.bindValue(":email",email);
    query.bindValue(":region",region);


    return query.exec();
}
bool client::chercherexistid(int cin){
     QString res = QString::number(cin);
     QSqlQuery query;
  query.prepare("select * from clients where cin='"+res+"' ");


      query.exec();
      if(!query.next())return false;
      else return  true;

}

