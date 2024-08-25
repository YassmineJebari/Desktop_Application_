#ifndef APPLICATION_H
#define APPLICATION_H

#include <QWidget>
#include "employe.h"

#include "login.h"
#include <QDebug>
#include <QDialog>
#include <QSortFilterProxyModel>

#include "arduino.h"

namespace Ui {
class application;
}

class application : public QWidget
{
    Q_OBJECT

public:
    explicit application(QWidget *parent = nullptr);
    explicit application(QString ,QWidget *parent = nullptr);
    explicit application(QString,QString ,QWidget *parent = nullptr);


    ~application();
    QString getcurrent_user(){return current_user;}
    void setcurrent_user(QString current_user){this->current_user=current_user;}
    void show_tables();


private slots:
    void on_add_clicked();

    void on_deleteEmp_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_activated(const QModelIndex &index);

    void on_update_clicked();

    void on_logout_clicked();

    void on_chat_clicked();



    void on_linerechercheemp_textChanged(const QString &arg1);

    void on_pdf_clicked();





    void on_statemploye_clicked();

    void on_clients_clicked();

    void on_products_clicked();

    void on_stations_clicked();

    void on_rdv_clicked();

private:
    Ui::application *ui;
     Employe E;
     login l;
    Employe suppem;
    int selected=0;
    QRegExp cin_regex,chaine_regex,tel_regex,mail_regex,salaire_regex,horaire_regex ;
    QString current_user;
    QString current_id;

    QSortFilterProxyModel *proxy;


    QByteArray data; // variable contenant les données reçues
    QByteArray datafinal; // variable contenant les données reçues
    QString data_text;
    Arduino A; // objet temporaire


};

#endif // APPLICATION_H
