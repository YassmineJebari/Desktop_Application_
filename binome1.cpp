#include "binome1.h"
#include "ui_binome1.h"
#include <QSqlQuery>
binome1::binome1(QString b,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::binome1)
{
    ui->setupUi(this);
    chercher(b);

}

binome1::~binome1()
{
    delete ui;
}
void binome1 :: chercher(QString idp)
{

        QSqlQuery query;
     query.prepare("select * from PRODUITS where idproduit='"+idp+"' ");

     query.exec();
     if(!query.next())
      return ;
     else
     {
                  ui->label_id->setText(query.value(0).toString());
                  ui->label_nom->setText(query.value(1).toString());
                  ui->label_type->setText(query.value(2).toString());
                  ui->label_quantite->setText(query.value(3).toString());
                  ui->label_dispo->setText(query.value(4).toString());
                  ui->label_prix->setText(query.value(5).toString());
     }
}
