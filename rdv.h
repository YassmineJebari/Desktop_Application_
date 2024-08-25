#ifndef RDV_H
#define RDV_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include <QDate>


#include <QCalendarWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTableView>

class rdv
{
private:
    int idrdv,phone;
    QString nom,prenom,email,heure,id_employe;
    QDate daterdv;


public:
    rdv();
    rdv(int,QString,QString,QString,int,QDate,QString,QString);

    int getidrdv(){return idrdv;}
    QString getnom(){return nom;}
    QString getprenom(){return prenom;}
    QString getemail(){return email;}
    int getphone(){return phone;}
    QDate getdate(){return daterdv;}
    QString getheure(){return heure;}
    QString getid_employe(){return id_employe;}


    void setidrdv(int idrdv){this->idrdv=idrdv;}
    void setnom(QString nom){this->nom=nom;}
    void setprenom(QString prenom){this->prenom=prenom;}
    void setemail(QString email){this->email=email;}
    void setphone(int phone){this->phone=phone;}
    void setDate(QDate daterdv){this->daterdv=daterdv;}
    void setheure(QString heure){this->heure=heure;}
    void setid_employe(QString id_employe){this->id_employe=id_employe;}



    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool update(int,QString,QString,QString,int,QDate,QString);
    bool chercherexistid(int);

    QSqlQueryModel * afficherc(QCalendarWidget* calendarWidget);
            QList<QDate> recupererDatesDeLaBaseDeDonnees(QCalendarWidget* calendarWidget);
             bool ajouterc(QCalendarWidget* calendarWidget,QString qle);
             bool supprimerc(QCalendarWidget* calendarWidget);
               void recupererDatesDeLaBaseDeDonnees();

};

#endif // RDV_H
