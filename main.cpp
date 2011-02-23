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
    case 4:
        t.setMaxConnections(QString(argv[3]).toUInt());
    case 3:
        bindhost = QHostAddress(argv[1]);
        bindport = QString(argv[2]).toUInt();
        break;
    case 2:
        bindport = QString(argv[1]).toUInt();
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
