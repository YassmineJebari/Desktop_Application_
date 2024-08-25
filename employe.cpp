#include "employe.h"

Employe::Employe()
{

}

Employe::Employe(int idemploye,QString nom,QString prenom,QString email,QString phone,int salaire,QString role)
{
     this->idemploye=idemploye;
     this->nom=nom;
     this->prenom=prenom;
     this->email=email;
     this->phone=phone;
     this->salaire=salaire;
     this->role=role;

}
bool Employe::ajouter()
{
    QSqlQuery query;

            query.prepare("INSERT INTO EMPLOYE (idemploye, nom, prenom,email, phone, salaire, role) "
                          "VALUES (:idemploye, :nom, :prenom,:email, :phone, :salaire, :role)");
            query.bindValue(":idemploye",idemploye);
            query.bindValue(":nom", nom);
            query.bindValue(":prenom", prenom);
            query.bindValue(":email", email);
            query.bindValue(":phone", phone);
            query.bindValue(":salaire", salaire);
            query.bindValue(":role", role);




      return query.exec();
}
QSqlQueryModel * Employe::afficher()
{
    QSqlQueryModel * Model= new QSqlQueryModel;
         Model -> setQuery("select * from EMPLOYE");
         Model -> setHeaderData(0,Qt::Horizontal,QObject::tr("idemploye"));
         Model -> setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
         Model -> setHeaderData(2,Qt::Horizontal,QObject::tr("prenom"));
         Model -> setHeaderData(3,Qt::Horizontal,QObject::tr("email"));
         Model -> setHeaderData(4,Qt::Horizontal,QObject::tr("phone"));
         Model -> setHeaderData(5,Qt::Horizontal,QObject::tr("salaire"));
         Model -> setHeaderData(6,Qt::Horizontal,QObject::tr("role"));
         return Model;
}
bool Employe::supprimer(int idemploye)
{
    QSqlQuery query;
    QString res = QString::number(idemploye);
    query.prepare("Delete from EMPLOYE where idemploye=:idemploye");
    query.bindValue(":idemploye",res);
    return query.exec();
}



bool Employe::update(int idemploye,QString nom,QString prenom,QString email,QString phone,int salaire,QString role)
    {
        QSqlQuery query;
        QString res = QString::number(idemploye);
        QString res2 = QString::number(salaire);
        query.prepare("UPDATE EMPLOYE SET nom='"+nom+"', prenom='"+prenom+"', email='"+email+"' ,  phone='"+phone+"', salaire='"+res2+"', role='"+role+"' WHERE idemploye='"+res+"' ;");

        return query.exec();
    }

   bool Employe::chercherexistid(int idemploye){
        QString res = QString::number(idemploye);
        QSqlQuery query;
     query.prepare("select * from EMPLOYE where idemploye='"+res+"' ");


         query.exec();
         if(!query.next())return false;
         else return  true;

   }

   bool Employe::chercherexistemail(QString email){

        QSqlQuery query;
     query.prepare("select * from EMPLOYE where email='"+email+"' ");


         query.exec();
         if(!query.next())return false;
         else return  true;

   }








