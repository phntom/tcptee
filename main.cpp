#include <QtCore/QCoreApplication>
#include <QTcpServer>
#include "tcptee.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    quint16 bindport = 0;
    QHostAddress bindhost = QHostAddress::Any;
    tcptee t;

    switch (argc) {
    case 3:
        t.setMaxConnections(QString(argv[2]).toUInt());
    case 2:
        bindhost = QHostAddress(argv[0]);
        bindport = QString(argv[1]).toUInt();
        break;
    case 1:
        bindport = QString(argv[0]).toUInt();
        break;
    default:
        std::cout << "Usage: tcptee [bindport]" << std::endl
                  << "       tcptee [bindhost] [bindport]" << std::endl
                  << "       tcptee [bindhost] [bindport] [max connections]"
                  << std::endl;
        return 0;
    }

    t.listen(bindhost, bindport);

    return a.exec();
}
