#include "tcptee.h"

tcptee::tcptee(QObject *parent) :
        QObject(parent),
        maxConnections(2),
        boolBuffer(false)
{
}

void tcptee::setMaxConnections(quint8 newMaxConnections) {
    if (!newMaxConnections) return;
    maxConnections = newMaxConnections;
    tcpServer.setMaxPendingConnections(maxConnections);
}

void tcptee::listen(QHostAddress host, quint16 port) {
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    tcpServer.listen(host, port);
}

void tcptee::newConnection() {
    if (!tcpServer.hasPendingConnections()) return;
    QTcpSocket *socket = tcpServer.nextPendingConnection();
    QList<QTcpSocket *> myClients = tcpServer.findChildren<QTcpSocket *>();
    if (myClients.count() > maxConnections) {
        socket->abort();
        socket->deleteLater();
        return;
    }
    connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(error(QAbstractSocket::SocketError)));
    if (boolBuffer && myClients.count() >= MIN_CONNECTIONS) {
        socket->write(barBuffer);
        barBuffer.clear();
    }
    boolBuffer = myClients.count() < MIN_CONNECTIONS;
}

void tcptee::readyRead() {
    QTcpSocket *socket = dynamic_cast<QTcpSocket *>(sender());
    if (!socket) return;
    if (boolBuffer) {
        barBuffer.append(socket->readAll());
    } else {
        QByteArray tmpBuffer = socket->readAll();
        foreach (QTcpSocket* target, tcpServer.findChildren<QTcpSocket *>()) {
            if (target == socket) continue;
            target->write(tmpBuffer);
        }
    }
}

void tcptee::disconnected() {
    sender()->deleteLater();
    disconnectFromAll();
}

void tcptee::error ( QAbstractSocket::SocketError ) {
    sender()->deleteLater();
    disconnectFromAll();
}

void tcptee::disconnectFromAll() {
    QList <QTcpSocket *> socketList = tcpServer.findChildren<QTcpSocket *>();
    if (socketList.count() <= MIN_CONNECTIONS) {
        boolBuffer = true;
        foreach (QTcpSocket *socket, socketList) {
            socket->disconnectFromHost();
        }
    }
}
