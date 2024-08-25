#include <QCoreApplication>
#include "dumessengerserver.h"

using namespace chatting;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
DuMessengerServer Server;
if(!Server.startServer(3333)){
qDebug()<<"error:" << Server.errorString();
return 1;
}
qDebug() << "Server started ...";
    return a.exec();
}
