#ifndef TCPTEE_H
#define TCPTEE_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#define MIN_CONNECTIONS 2

class tcptee : public QObject
{
    Q_OBJECT
public:
    explicit tcptee(QObject *parent = 0);
    void listen(QHostAddress host, quint16 port);
    void setMaxConnections(quint8 newMaxConnections);

protected:
    QTcpServer tcpServer;
    quint8 maxConnections;
    bool boolBuffer;
    QByteArray barBuffer;

    void disconnectFromAll();


protected slots:
    void newConnection ();
    void readyRead ();
    void disconnected ();
    void error ( QAbstractSocket::SocketError socketError );

signals:


};

#endif // TCPTEE_H
