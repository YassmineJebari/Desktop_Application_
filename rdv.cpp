#include "rdv.h"

rdv::rdv()
{

}

rdv::rdv(int idrdv,QString nom,QString prenom,QString email,int phone,QDate daterdv,QString heure,QString id_employe)
{
     this->idrdv=idrdv;
     this->nom=nom;
     this->prenom=prenom;
     this->email=email;
     this->phone=phone;
     this->daterdv=daterdv;
     this->heure=heure;
    this->id_employe=id_employe;

}

bool rdv::ajouter()
{
    QSqlQuery query;

            query.prepare("INSERT INTO RENDEZVOUS (idrdv, nom, prenom,email, phone, daterdv, heure,id_employe) "
                          "VALUES (:idrdv, :nom, :prenom,:email, :phone, :daterdv, :heure, :id_employe)");
            query.bindValue(":idrdv",idrdv);
            query.bindValue(":nom", nom);
            query.bindValue(":prenom", prenom);
            query.bindValue(":email", email);
            query.bindValue(":phone", phone);
            query.bindValue(":daterdv", daterdv);
            query.bindValue(":heure", heure);
            query.bindValue(":id_employe", id_employe);




      return query.exec();
}

bool rdv::supprimer(int idrdv)
{
    QSqlQuery query;
    QString res = QString::number(idrdv);
    query.prepare("Delete from RENDEZVOUS where idrdv=:idrdv");
    query.bindValue(":idrdv",res);
    return query.exec();
}




QSqlQueryModel * rdv::afficher()
{
    QSqlQueryModel * Model= new QSqlQueryModel;
         Model -> setQuery("select idrdv,nom,prenom,email,phone,to_char(daterdv,'DD/MM/RRRR'),heure from RENDEZVOUS");
         Model -> setHeaderData(0,Qt::Horizontal,QObject::tr("idrdv"));
         Model -> setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
         Model -> setHeaderData(2,Qt::Horizontal,QObject::tr("prenom"));
         Model -> setHeaderData(3,Qt::Horizontal,QObject::tr("email"));
         Model -> setHeaderData(4,Qt::Horizontal,QObject::tr("phone"));
         Model -> setHeaderData(5,Qt::Horizontal,QObject::tr("daterdv"));
         Model -> setHeaderData(6,Qt::Horizontal,QObject::tr("heure"));
         return Model;
}

bool rdv::update(int idrdv,QString nom,QString prenom,QString email,int phone,QDate daterdv,QString heure)
    {
        QSqlQuery query;
        query.prepare("UPDATE RENDEZVOUS SET nom=:nom , prenom=:prenom , email=:email , phone=:phone , daterdv=:daterdv , heure=:heure WHERE idrdv=:idrdv");
            query.bindValue(":idrdv", idrdv);
            query.bindValue(":nom", nom);
            query.bindValue(":prenom", prenom);
            query.bindValue(":email", email );
            query.bindValue(":phone", phone);
            query.bindValue(":daterdv", daterdv);
            query.bindValue(":heure", heure);

        return query.exec();
    }

bool rdv::chercherexistid(int idrdv){
        QString res = QString::number(idrdv);
        QSqlQuery query;
     query.prepare("select * from RENDEZVOUS where idrdv='"+res+"' ");


         query.exec();
         if(!query.next())return false;
         else return  true;

   }




//CALENDRIER
QSqlQueryModel* rdv::afficherc(QCalendarWidget* calendarWidget)
{
    QDate selectedDate = calendarWidget->selectedDate();
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT heure FROM RENDEZVOUS WHERE daterdv = :selectedDate");
    query.bindValue(":selectedDate", selectedDate);

    if (query.exec()) {
        model->setQuery(query);
    } else {
        //qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
    }

    return model;
}




QList<QDate> rdv::recupererDatesDeLaBaseDeDonnees(QCalendarWidget *calendarWidget)
{
    QList<QDate> dates;
     QSqlQuery query("SELECT daterdv FROM RENDEZVOUS");
     while (query.next()) {
         QDate date = query.value(0).toDate();
         if (date.isValid()) {
             QTextCharFormat format;
             format.setBackground(Qt::darkGreen);
             calendarWidget->setDateTextFormat(date, format);
             dates.append(date);
         }
     }
     return dates;
}











void rdv::recupererDatesDeLaBaseDeDonnees()
{
    QSqlQuery query("SELECT daterdv FROM RENDEZVOUS");

    while (query.next()) {
        QDate date = query.value(0).toDate();
        if (date.isValid()) {
            QTextCharFormat format;
            format.setBackground(Qt::red);
          }
    }
}


