#pragma once

#include "libqtoufi_global.hpp"
#include <QObject>
#include <QTcpServer>

#include "net/simpletcpsocket.hpp"

namespace ltoufi {

class LIBQTOUFI_EXPORT SimpleTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit SimpleTcpServer(QObject* parent = nullptr);
    explicit SimpleTcpServer(const QHostAddress& host, quint16 port, QObject* parent = nullptr);
    virtual ~SimpleTcpServer();

    bool listen(const QHostAddress& host = QHostAddress::Any, quint16 port = 0);
    inline void close() { mServer->close(); };

    inline bool isListening() const { return mServer->isListening(); }
    inline QHostAddress serverAddress() const { return mServer->serverAddress(); }
    inline quint16 serverPort() const { return mServer->serverPort(); }
    inline QAbstractSocket::SocketError errorCode() const { return mServer->serverError(); }
    inline QString errorString() const { return mServer->errorString(); }

    inline bool hasPendingConnections() const { return mServer->hasPendingConnections(); }
    SimpleTcpSocket* nextPendingConnection();

signals:
    void acceptError(QAbstractSocket::SocketError);
    void newConnection();

private:
    QTcpServer* const mServer;
};


}
