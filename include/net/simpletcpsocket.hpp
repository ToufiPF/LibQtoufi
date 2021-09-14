#pragma once

#include "libqtoufi_global.hpp"
#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QHostAddress>

namespace ltoufi {

class LIBQTOUFI_EXPORT SimpleTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit SimpleTcpSocket(QObject* parent = nullptr);
    explicit SimpleTcpSocket(QTcpSocket* underling, QObject* parent = nullptr);
    virtual ~SimpleTcpSocket();

    inline QHostAddress localAddress() const { return mSocket->localAddress(); }
    inline quint16 localPort() const { return mSocket->localPort(); }
    inline QHostAddress peerAddress() const { return mSocket->peerAddress(); }
    inline quint16 peerPort() const { return mSocket->peerPort(); }

    inline bool isListening() const { return mSocket->state() == QTcpSocket::ConnectedState; }
    inline QAbstractSocket::SocketError errorCode() const { return mSocket->error(); }
    inline QString errorString() const { return mSocket->errorString(); }

signals:
    void connected();
    void disconnected();
    void bytesWritten(qint64);
    void errorOccured(QAbstractSocket::SocketError);

    void dataReceived(const QByteArray& data);

public slots:
    void connectToHost(QHostAddress host, quint16 port); // non-blocking, emit connected signal
    void disconnectFromHost(); // non-blocking, emit disconnected signal

    void sendData(const QByteArray& data); // non-blocking, emit bytesWritten signal

public:// DEPRECATED, may fail on Windows
    bool waitForConnection(QHostAddress host, quint16 port, int timeoutMs = 10000); // blocking, max 10s
    bool waitForDisconnection(int timeoutMs = 10000); // blocking, max 10s
    bool waitForDataSent(const QByteArray& data, int timeoutMs = 10000); // blocking, max 10s

private slots:
    void onReadyRead();

private:
    QTcpSocket* const mSocket;
    QByteArray mReceived; // need a buffer to store data until it's completely received
    qsizetype mExpected; // need to store the expected amount of data
};

}
