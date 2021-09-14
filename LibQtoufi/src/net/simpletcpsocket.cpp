#include "net/simpletcpsocket.hpp"

#include <QByteArray>
#include <QObject>
#include <QTcpSocket>

#include <QDebug>

constexpr qsizetype BYTES_IN_SIZE_T = sizeof (qsizetype);

qsizetype arrayToSize_t(const QByteArray& array) {
    if (array.length() < BYTES_IN_SIZE_T)
        return 0;

    qsizetype result = 0;
    for (unsigned i = BYTES_IN_SIZE_T - 1 ; i-- > 0 ; ) {
        result |= 0xFF & array.at(i);
        result <<= 8;
    }
    return result;
}
QByteArray size_tToArray(qsizetype x) {
    QByteArray array(BYTES_IN_SIZE_T, '\0');

    for (unsigned i = 0 ; i < BYTES_IN_SIZE_T ; ++i) {
        array[i] = x;
        x >>= 8;
    }

    return array;
}

namespace ltoufi {

SimpleTcpSocket::SimpleTcpSocket(QObject* parent) :
    SimpleTcpSocket(nullptr, parent)
{
}
SimpleTcpSocket::SimpleTcpSocket(QTcpSocket *underling, QObject *parent) :
    QObject(parent),
    mSocket(underling == nullptr ? new QTcpSocket() : underling),
    mReceived(),
    mExpected(0)
{
    mSocket->setParent(this);
    // transmit basic signals
    connect(mSocket, &QTcpSocket::connected, this, &SimpleTcpSocket::connected);
    connect(mSocket, &QTcpSocket::disconnected, this, &SimpleTcpSocket::disconnected);
    connect(mSocket, &QTcpSocket::bytesWritten, this, &SimpleTcpSocket::bytesWritten);
    connect(mSocket, &QTcpSocket::errorOccurred, this, &SimpleTcpSocket::errorOccured);

    connect(mSocket, &QTcpSocket::readyRead, this, &SimpleTcpSocket::onReadyRead);
}

SimpleTcpSocket::~SimpleTcpSocket() {
    // this is parent of mSocket -> Qt will delete it
}

void SimpleTcpSocket::connectToHost(QHostAddress host, quint16 port) {
    mSocket->connectToHost(host, port);
}


void SimpleTcpSocket::disconnectFromHost() {
    mSocket->disconnectFromHost();
}

void SimpleTcpSocket::sendData(const QByteArray &data) {
    if(mSocket->state() == QAbstractSocket::ConnectedState) {
        //write size of data
        mSocket->write(size_tToArray(data.size()));
        //write the data itself
        mSocket->write(data);
    }
}

bool SimpleTcpSocket::waitForConnection(QHostAddress host, quint16 port, int timeoutMs) {
    mSocket->connectToHost(host, port);
    return mSocket->state() == QAbstractSocket::ConnectedState
            || mSocket->waitForConnected(timeoutMs);
}

bool SimpleTcpSocket::waitForDisconnection(int timeoutMs) {
    mSocket->disconnectFromHost();
    return mSocket->state() == QAbstractSocket::UnconnectedState
            || mSocket->waitForDisconnected(timeoutMs);
}

bool SimpleTcpSocket::waitForDataSent(const QByteArray &data, int timeoutMs) {
    sendData(data);
    return mSocket->waitForBytesWritten(timeoutMs);
}

void SimpleTcpSocket::onReadyRead() {
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if (socket == nullptr)
        return;

    while (socket->bytesAvailable() > 0) {
        mReceived.append(socket->readAll());

        // While we can process data, process it
        //(we may have received several small msgs, hence the loop)
        while ((mExpected == 0 && mReceived.size() >= BYTES_IN_SIZE_T)
               || (mExpected > 0 && mReceived.size() >= mExpected)) {
            //if we received the expected size of data
            if (mExpected == 0 && mReceived.size() >= BYTES_IN_SIZE_T) {
                mExpected = arrayToSize_t(mReceived.mid(0, BYTES_IN_SIZE_T));
                mReceived.remove(0, BYTES_IN_SIZE_T);
            }
            // If data is received completely
            if (mExpected > 0 && mReceived.size() >= mExpected) {
                QByteArray data = mReceived.mid(0, mExpected);
                mReceived.remove(0, mExpected);
                mExpected = 0;

                emit dataReceived(data);
            }
        }
    }
}

}
