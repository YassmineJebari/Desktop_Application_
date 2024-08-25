#include "notifications.h"
#include <QSystemTrayIcon>
#include<QString>
notifications::notifications()
{

}
void notifications::notifications_ajouterClient()
{

    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;

    notifyIcon->setIcon(QIcon("D:/bureau/mail/img/valider.png"));
    notifyIcon->show();
    notifyIcon->showMessage("Gestion des Clients","Nouveau client ajouté ",QSystemTrayIcon::Information,15000);

}

void notifications::notifications_supprimerClient(){
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;

    notifyIcon->setIcon(QIcon("D:/bureau/mail/img/supprimer.jpg"));
    notifyIcon->show();
    notifyIcon->showMessage("Gestion des Clients","Client Supprimé",QSystemTrayIcon::Information,15000);
}

void notifications::notifications_modifierClient(){
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;

    notifyIcon->setIcon(QIcon("D:/bureau/mail/img/modifier.jpg"));
    notifyIcon->show();
    notifyIcon->showMessage("Gestion des Clients","Client est modifié",QSystemTrayIcon::Information,15000);

}
