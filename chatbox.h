#ifndef CHATBOX_H
#define CHATBOX_H

#include <QDialog>
#include <QString>
#include <QDateTime>



QT_BEGIN_NAMESPACE
namespace Ui {
class chatbox;
}
QT_END_NAMESPACE
class QTcpSocket;
namespace chatting{
class chatbox : public QDialog
{
    Q_OBJECT

public:
    explicit chatbox(QWidget *parent = nullptr);
    explicit chatbox(QString ,QWidget *parent = nullptr);

    ~chatbox();



private slots:
    void on_send_clicked();

    void on_bind_clicked();

   // void sockt_recv_data();
   // void socket_disconnect();
private:
    Ui::chatbox *ui;
    QTcpSocket *nSocket;

       QString nickname;
};
}
#endif // CHATBOX_H
