#include "historique.h"

Historique::Historique()
{

}
Historique::Historique(int NUMBORNE,int CAPACITE ,int POURCENTAGE)
{
    this->NUMBORNE=NUMBORNE;
    this->CAPACITE=CAPACITE;
    this->POURCENTAGE=POURCENTAGE;
}

bool Historique::ajouter(){
    QSqlQuery query;
    QString res = QString::number(NUMBORNE);
    QString res1 = QString::number(CAPACITE);
    QString res2 = QString::number(POURCENTAGE);
 query.prepare("insert into HISTORIQUE (NUMBORNE,CAPACITE,POURCENTAGE)""values(:NUMBORNE,:CAPACITE,:POURCENTAGE)");
query.bindValue(":NUMBORNE",res);
query.bindValue(":CAPACITE",res1);
query.bindValue(":POURCENTAGE",res2);
return query.exec();
}

bool Historique::supprimer(int NUMBORNE){
    QSqlQuery query;
    QString res=QString::number(NUMBORNE);
     query.prepare("Delete from HISTORIQUE where NUMBORNE =:NUMBORNE");
     query.bindValue(":NUMBORNE",res);
     return query.exec();
}
float Historique::calculertemps(int n){
    return (float)((float)CAPACITE * (1 - (float)(POURCENTAGE / 100)) / n);
}
