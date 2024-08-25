#ifndef LOGIN_H
#define LOGIN_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>


class login
{



public:
    login();

    int hash(QString);
    bool sign_up(QString,QString,QString);
    bool sign_in(QString,QString);

    bool delete_account(QString);
    bool updateuseremploye(QString,QString);
    bool modifier_pwd(QString,QString);
    bool chercheremail(QString);

    QString fetch_user(QString);
    QString fetch_id(QString);


    private:














};

#endif // LOGIN_H
