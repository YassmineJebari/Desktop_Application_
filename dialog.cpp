#include "dialog.h"
#include "ui_dialog.h"
#include "smtp.h"
dialog::dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog)
{
    ui->setupUi(this);
}

dialog::dialog(QString e,QString n,QString s,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog)
{
    QString contenu="Contenu";
    ui->setupUi(this);


    ui->uname->setText("yassmine.jebari@esprit.tn");
    ui->passwd->setText("hvytcvruoxaftlss");
    ui->passwd->setEchoMode(QLineEdit::Password);
    ui->recipient->setText(e);
    ui->recipient_n->setText(n);
    ui->recipient_s->setText(s);

    ui->message->setText("Cher client "+s+" "+n+",\n\n"+contenu+"\n\n"+"Cordialement,\n");

}

dialog::~dialog()
{
    delete ui;
}

void dialog::on_envoyer_dialog_clicked()
{
    Smtp* smtp = new Smtp(ui->uname->text(), ui->passwd->text(), "smtp.gmail.com", 465);

    smtp->sendMail(ui->uname->text(), ui->recipient->text() , ui->subjectLineEdit->text() ,ui->message->toPlainText());

    /*if(status == "Message sent")
        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ), tr( "Message sent!\n\n" ) );*/

}


void dialog::on_annuler_mail_clicked()
{

}
