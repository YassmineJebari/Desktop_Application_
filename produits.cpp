#include "produits.h"
#include <QtDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
Produits::Produits()
{
/*idproduit=0;
nom="";
type="";
quantite=0;
disponibilite="";
prix=0;*/


}
 Produits::Produits(  int idproduit,QString idemploye,int ID_CLIENT ,QString nom,QString type,int quantite,QString disponibilite,float prix)
 {   this->idproduit=idproduit;
     this->idemploye=idemploye;
     this->ID_CLIENT=ID_CLIENT;
     this->nom=nom;
     this->type=type;
     this->quantite=quantite;
     this->disponibilite=disponibilite;
     this->prix=prix;
 }
int  Produits::getidproduit()
{ return idproduit;

}
QString  Produits::getnom()
{
    return  nom;
}
QString Produits::gettype()
{ return  type;

}
int  Produits::getquantite()
{
    return  quantite;
}
QString Produits:: getdisponibilite()
{
    return  disponibilite;
}
float Produits:: getprix()
{   return prix;

}
void  Produits::setidproduit(int idproduit)
{
   this->idproduit=idproduit;
}
void  Produits::setnom(QString nom)
{this->nom=nom;

}
void Produits:: settype(QString type )
{   this->type=type;

}
void  Produits::setquantite(int quantite)
{
    this->quantite=quantite;
}
void  Produits::setdiponibilite(QString disponibilite)
{
    this->disponibilite=disponibilite;
}
void  Produits::setprix(float prix ){
    this->prix=prix;
}
bool Produits::ajouter()
{

            QSqlQuery   query;
            QString res = QString::number(idproduit);
            QString res1 = QString::number(quantite);
            QString res2 = QString::number(prix);
         query.prepare("insert into PRODUITS (idproduit,ID_EMPLOYE,ID_CLIENT,nom,type,quantite,disponibilite,prix)""values(:idproduit,:ID_EMPLOYE,:ID_CLIENT,:nom,:type,:quantite,:disponibilite,:prix)");
    query.bindValue(":idproduit",res);
    query.bindValue(":quantite",res1);
    query.bindValue(":prix",res2);
    query.bindValue(":type",type);
    query.bindValue(":nom",nom);
    query.bindValue(":disponibilite",disponibilite);
         query.bindValue(":ID_EMPLOYE",idemploye);
              query.bindValue(":ID_CLIENT",ID_CLIENT);
    return query.exec();





}
 bool  Produits::supprimer (int idproduit)
 {


     QSqlQuery query;
         QString res = QString::number(idproduit);
         query.prepare("delete from PRODUITS where IDPRODUIT=:IDPRODUIT");
         query.bindValue(":IDPRODUIT",res);
         return query.exec();

 }

 QSqlQueryModel *Produits::afficher()
 {
 QSqlQueryModel *model = new QSqlQueryModel();
 model->setQuery("SELECT* FROM produits ");
 model->setHeaderData(0, Qt::Horizontal, QObject::tr("idproduit"));
 model->setHeaderData(1, Qt::Horizontal,QObject:: tr("nom"));
 model->setHeaderData(2, Qt::Horizontal,QObject:: tr("type"));
 model->setHeaderData(3, Qt::Horizontal,QObject:: tr("quantite"));
 model->setHeaderData(4, Qt::Horizontal,QObject:: tr("disponibilite"));
 model->setHeaderData(5, Qt::Horizontal,QObject:: tr("prix"));
 model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID_EMPLOYE"));
 model->setHeaderData(7, Qt::Horizontal, QObject::tr("ID_CLIENT"));
 return model;
}
 /*bool Produits::modifier_prod(int idproduit)
 {
     QSqlQuery query;
     QString res= QString::number(idproduit);
      QString res1= QString::number(quantite);
      QString res2= QString::number(prix);
       QString res3= QString::number(ID_CLIENT);
     query.prepare("update PRODUITS SET nom=:nom,type=:type,quantite=:quantite,disponibilite=:disponibilite,prix=:prix,ID_CLIENT=:ID_CLIENT WHERE idproduit=:idproduit,");
     query.bindValue(":idproduit",res);
     query.bindValue(":nom", nom);
     query.bindValue(":type", type);
     query.bindValue(":quantite", res1);
     query.bindValue(":disponibilite", disponibilite);
     query.bindValue(":prix", res2);
      query.bindValue(":ID_CLIENT",ID_CLIENT);


     return    query.exec();
 }*/
 bool Produits::modifier_prod(int idproduit)
 {
     QSqlQuery query;
     QString res= QString::number(idproduit);
      QString res1= QString::number(quantite);
      QString res2= QString::number(prix);
     query.prepare("update PRODUITS SET nom=:nom,type=:type,quantite=:quantite,disponibilite=:disponibilite,prix=:prix,ID_CLIENT=:ID_CLIENT WHERE idproduit=:idproduit");
     query.bindValue(":idproduit",res);
     query.bindValue(":nom", nom);
     query.bindValue(":type", type);
     query.bindValue(":quantite", res1);
     query.bindValue(":disponibilite", disponibilite);
     query.bindValue(":prix", res2);
     query.bindValue(":ID_CLIENT",ID_CLIENT);

     return    query.exec();
 }

 bool Produits ::chercherexistid(int idproduit){
      QString res = QString::number(idproduit);
      QSqlQuery query;
   query.prepare("select * from PRODUITS where idproduit='"+res+"' ");


       query.exec();
       if(!query.next())return false;
       else return  true;

 }

 bool Produits ::chercherexistcin(int CIN){
      QString res = QString::number(CIN);
      QSqlQuery query;
   query.prepare("select * from CLIENTS where CIN='"+res+"' ");


       query.exec();
       if(!query.next())return false;
       else return  true;




 }




 QSqlQueryModel * Produits::trie_ID()
 {
     QSqlQueryModel * model= new QSqlQueryModel();

           model->setQuery("SELECT * FROM PRODUITS ORDER BY idproduit ");
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("idproduit"));
           model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
           model->setHeaderData(2,Qt::Horizontal,QObject::tr("type"));
           model->setHeaderData(3,Qt::Horizontal,QObject::tr("quantite"));
           model->setHeaderData(4,Qt::Horizontal,QObject::tr("disponibilite"));
           model->setHeaderData(5,Qt::Horizontal,QObject::tr("prix"));

     return model;
 }
 QSqlQueryModel * Produits::trie_NOM()
 {
     QSqlQueryModel * model= new QSqlQueryModel();

           model->setQuery("SELECT * FROM PRODUITS ORDER BY nom ");
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("idproduit"));
           model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
           model->setHeaderData(2,Qt::Horizontal,QObject::tr("type"));
           model->setHeaderData(3,Qt::Horizontal,QObject::tr("quantite"));
           model->setHeaderData(4,Qt::Horizontal,QObject::tr("disponibilite"));
           model->setHeaderData(5,Qt::Horizontal,QObject::tr("prix"));

     return model;
 }
 QSqlQueryModel * Produits::trie_PRIX()
 {
     QSqlQueryModel * model= new QSqlQueryModel();

           model->setQuery("SELECT * FROM PRODUITS ORDER BY prix ");
           model->setHeaderData(0,Qt::Horizontal,QObject::tr("idproduit"));
           model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
           model->setHeaderData(2,Qt::Horizontal,QObject::tr("type"));
           model->setHeaderData(3,Qt::Horizontal,QObject::tr("quantite"));
           model->setHeaderData(4,Qt::Horizontal,QObject::tr("disponibilite"));
           model->setHeaderData(5,Qt::Horizontal,QObject::tr("prix"));

     return model;
 }





 QSqlQueryModel* Produits::rechercheridproduit(QString idd )
     {

     QSqlQueryModel * model= new QSqlQueryModel();

             model->setQuery("select * from PRODUITS where idproduit ='"+idd+"' ");
             model->setHeaderData(0, Qt::Horizontal, QObject::tr("idproduit"));
             model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
             model->setHeaderData(2, Qt::Horizontal, QObject::tr("type"));
             model->setHeaderData(3, Qt::Horizontal, QObject::tr("quantite"));
             model->setHeaderData(4, Qt::Horizontal, QObject::tr("disponibilite"));
             model->setHeaderData(5, Qt::Horizontal, QObject::tr("prix"));

             return model;
     }
 QSqlQueryModel* Produits::recherchernom(QString nom)
     {

     QSqlQueryModel * model= new QSqlQueryModel();

             model->setQuery("select * from PRODUITS where nom ='"+nom+"' ");
             model->setHeaderData(0, Qt::Horizontal, QObject::tr("idproduit"));
             model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
             model->setHeaderData(2, Qt::Horizontal, QObject::tr("type"));
             model->setHeaderData(3, Qt::Horizontal, QObject::tr("quantite"));
             model->setHeaderData(4, Qt::Horizontal, QObject::tr("disponibilite"));
             model->setHeaderData(5, Qt::Horizontal, QObject::tr("prix"));

             return model;
     }
 QSqlQueryModel* Produits::recherchertype(QString type)
     {

     QSqlQueryModel * model= new QSqlQueryModel();

             model->setQuery("select * from PRODUITS where type ='"+type+"' ");
             model->setHeaderData(0, Qt::Horizontal, QObject::tr("idproduit"));
             model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
             model->setHeaderData(2, Qt::Horizontal, QObject::tr("type"));
             model->setHeaderData(3, Qt::Horizontal, QObject::tr("quantite"));
             model->setHeaderData(4, Qt::Horizontal, QObject::tr("disponibilite"));
             model->setHeaderData(5, Qt::Horizontal, QObject::tr("prix"));

             return model;
     }

 QSqlQueryModel* Produits::rechercherdisponibilite(QString disponibilite)
     {

     QSqlQueryModel * model= new QSqlQueryModel();

             model->setQuery("select * from PRODUITS where disponibilite ='"+disponibilite+"' ");
             model->setHeaderData(0, Qt::Horizontal, QObject::tr("idproduit"));
             model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
             model->setHeaderData(2, Qt::Horizontal, QObject::tr("type"));
             model->setHeaderData(3, Qt::Horizontal, QObject::tr("quantite"));
             model->setHeaderData(4, Qt::Horizontal, QObject::tr("disponibilite"));
             model->setHeaderData(5, Qt::Horizontal, QObject::tr("prix"));

             return model;
     }
 int Produits::statistique(QString type)
 {
     QSqlQuery query;
     query.prepare("select count(*) from PRODUITS where type=:type ");
     query.bindValue(":type",type);
     query.exec();

     int count =-1;

             while(query.next())
                     {
                         count = query.value(0).toInt() ;
                         return count;

                     }

     return count;

 }
 /*bool P::chercherexistemail(QString email){

      QSqlQuery query;
   query.prepare("select * from EMPLOYE where email='"+email+"' ");


       query.exec();
       if(!query.next())return false;
       else return  true;

 }


*/





