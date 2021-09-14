#include "net/simpletcpserver.hpp"

#include <QByteArray>
#include <QDataStream>
#include <QHash>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QDebug>

namespace ltoufi {

SimpleTcpServer::SimpleTcpServer(QObject *parent) :
    QObject(parent),
    mServer(new QTcpServer(this))
{
    connect(mServer, &QTcpServer::acceptError, this, &SimpleTcpServer::acceptError);
    connect(mServer, &QTcpServer::newConnection, this, &SimpleTcpServer::newConnection);
}

SimpleTcpServer::SimpleTcpServer(const QHostAddress &host, quint16 port, QObject *parent) :
    SimpleTcpServer(parent)
{
    listen(host, port);
}

SimpleTcpServer::~SimpleTcpServer() {
    close();
}


bool SimpleTcpServer::listen(const QHostAddress &host, quint16 port) {
    bool listening = mServer->listen(host, port);
    if (!listening)
        qDebug() << "Failed to listen to " << host << ":" << port;
    return listening;
}

SimpleTcpSocket* SimpleTcpServer::nextPendingConnection() {
    if (mServer->hasPendingConnections())
        return new SimpleTcpSocket(mServer->nextPendingConnection(), this);
    return nullptr;
}

}
