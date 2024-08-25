#ifndef INTERFACECLIENT_H
#define INTERFACECLIENT_H


#include <QWidget>
#include "client.h"
#include "arduino.h"
#include "binome1.h"
#include "application.h"
#include <QSortFilterProxyModel>
namespace Ui {
class interfaceclient;
}

class interfaceclient : public QWidget
{
    Q_OBJECT

public:
    explicit interfaceclient(QWidget *parent = nullptr);
    ~interfaceclient();

    explicit interfaceclient(QString,QString ,QWidget *parent = nullptr);



private slots :
    void on_add_clicked();

    void on_delete_2_clicked();

    void on_pushButton_5_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_activated(const QModelIndex &index);
  //  void on_add_clicked2();

   void on_mail_clicked();

   void on_pushButton_6_clicked();

void show_tables();

void on_lineEdit_5_textChanged(const QString &arg1);

void on_stat_clicked();
void update_label();
void notifer();
void onDialogFinished(int x);

void on_employees_clicked();

void on_pushButton_7_clicked();

void on_products_clicked();

void on_stations_clicked();

void on_rdv_clicked();

private:
    Ui::interfaceclient *ui;
    client c;
        QRegExp cin_regex,chaine_regex,tel_regex,email_regex;

        int selected =0;
      QSortFilterProxyModel *proxy;
      QString current_user;
      QString current_id;

      QByteArray data; // variable contenant les données reçues
      QByteArray datafinal; // variable contenant les données reçues
      QString data_text;
      Arduino A; // objet temporaire


      binome1 *b;

};

#endif // INTERFACECLIENT_H
