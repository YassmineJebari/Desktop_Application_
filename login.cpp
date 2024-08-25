#include "login.h"

login::login()
{

}

int login::hash(QString pwd)
{
    std::hash<std::string> hash;
    return hash(pwd.toStdString());
}


bool login::sign_in(QString uname,QString pwd)
{
    QSqlQuery qry;
    qry.prepare("SELECT * FROM AUTHENTIFICATION WHERE EMAIL=:EMAIL AND PWD=:PWD");
    qry.bindValue(":EMAIL",uname);
    qry.bindValue(":PWD",this->hash(pwd));


    return qry.exec() && qry.next();

}





bool login::sign_up(QString uname,QString pwd,QString email)
{
    QSqlQuery qry;
    qry.prepare("INSERT INTO AUTHENTIFICATION (USERNAME,PWD,email)"
                "VALUES (:USERNAME,:PWD,:email)");
    qry.bindValue(":USERNAME",uname);
    qry.bindValue(":PWD",this->hash(pwd));
    qry.bindValue(":email",email);

    return qry.exec();
}



QString login::fetch_user(QString uname)
{
    QSqlQuery qry;
     QString pref;
    qry.prepare("SELECT * FROM AUTHENTIFICATION WHERE EMAIL=:EMAIL");
    qry.bindValue(":EMAIL",uname);

    qry.first();
    qry.exec();

    while(qry.next())
    {
        pref=qry.value(2).toString();

    }
    return pref;
}

QString login::fetch_id(QString uname)
{
    QSqlQuery qry;
     QString pref;
    qry.prepare("SELECT * FROM EMPLOYE WHERE EMAIL=:EMAIL");
    qry.bindValue(":EMAIL",uname);

    qry.first();
    qry.exec();

    while(qry.next())
    {
        pref=qry.value(0).toString();

    }
    return pref;
}



bool login::updateuseremploye(QString nom,QString email)
    {
        QSqlQuery qry;
        //qry.prepare("UPDATE AUTHENTIFICATION SET USERNAME='"+nom+"' WHERE email='"+email+"' ;");
        //qry.prepare("UPDATE AUTHENTIFICATION SET USERNAME=:USERNAME,email=:newemail WHERE (email=:email)");
        qry.prepare("UPDATE AUTHENTIFICATION SET USERNAME=:USERNAME WHERE (email=:email)");

        qry.bindValue(":USERNAME",nom);
          qry.bindValue(":email",email);
         // qry.bindValue(":newemail",newemail);



           return qry.exec();
    }


bool login::delete_account(QString uname)
{
    QSqlQuery qry;
    qry.prepare("Delete from AUTHENTIFICATION where email = :email");
    qry.bindValue(":email",uname);
    return qry.exec();
}




bool login::modifier_pwd(QString EMAIL,QString new_mdp)
{
    QSqlQuery qry;
    qry.prepare("UPDATE AUTHENTIFICATION SET PWD=:PWD WHERE EMAIL=:EMAIL");
    qry.bindValue(":EMAIL",EMAIL);
    qry.bindValue(":PWD",this->hash(new_mdp));

    return qry.exec();
}


bool login::chercheremail(QString EMAIL){
    QSqlQuery query;
 query.prepare("select * from AUTHENTIFICATION where EMAIL='"+EMAIL+"' ");
     query.exec();
     if(!query.next())return false;
     else return  true;
}




