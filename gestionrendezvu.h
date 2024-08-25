#ifndef GESTIONRENDEZVU_H
#define GESTIONRENDEZVU_H
#include "rdv.h"

#include <QSortFilterProxyModel>

#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QSqlError>
#include <QWidget>

namespace Ui {
class gestionrendezvu;
}

class gestionrendezvu : public QWidget
{
    Q_OBJECT

public:
    explicit gestionrendezvu(QWidget *parent = nullptr);


    explicit gestionrendezvu(QString,QString ,QWidget *parent = nullptr);

    ~gestionrendezvu();
    void show_tables();

private slots:
    void on_add_clicked();

    void on_update_clicked();

    void on_pushButton_5_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_envoyerSMS_clicked();
    void replyFinished(QNetworkReply* reply);



    void on_lineEdit_5_textChanged(const QString &arg1);

    void on_PDF_clicked();





    void on_stats_clicked();



    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_6_clicked();

    void on_employees_clicked();

    void on_clients_clicked();

    void on_stations_clicked();

    void on_products_clicked();

    void on_logout_clicked();

private:
    Ui::gestionrendezvu *ui;
    rdv supp;
    int selected=0;
    QString current_user;
    QString current_id;
        QRegExp cin_regex,chaine_regex,tel_regex,email_regex,salaire_regex,horaire_regex ;

         QSortFilterProxyModel *proxy;
};

#endif // GESTIONRENDEZVU_H
