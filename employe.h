#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Employe
{
private:
    int idemploye,salaire;
    QString nom,prenom,email,role,phone;
public:
    Employe();
    Employe(int,QString,QString,QString,QString,int,QString);


    int getidemploye(){return idemploye;}
    QString getnom(){return nom;}
    QString getprenom(){return prenom;}
    QString getemail(){return email;}
    QString getphone(){return phone;}
    int getsalaire(){return salaire;}
    QString getrole(){return role;}


      void setidemploye(int idemploye){ this->idemploye=idemploye;}
      void setnom(QString nom){ this->nom=nom;}
      void setprenom(QString prenom){ this->prenom=prenom;}
      void setemail(QString email){ this->email=email;}
      void setphone(QString phone){ this->phone=phone;}
      void setsalaire(int salaire){ this->salaire=salaire;}
      void setrole(QString role){ this->role=role;}


         bool ajouter();
         QSqlQueryModel * afficher();
         bool supprimer(int);
         bool update(int,QString ,QString ,QString ,QString ,int ,QString );
         bool chercherexistid(int);
         bool chercherexistemail(QString);





};

#endif // EMPLOYE_H
