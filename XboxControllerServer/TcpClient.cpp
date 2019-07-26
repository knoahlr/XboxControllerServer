#include "TcpClient.h"

static inline QByteArray IntToArray(qint32 source);
TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
	Transmitter = new QTcpSocket(this);
	connect(Transmitter, SIGNAL(QIODevice::readyRead), this, SLOT(tcpResponse));
}

bool TcpClient::connectToHost(QString host, int port)
{
	Transmitter->connectToHost(host, port);
	return Transmitter->waitForConnected();
}

bool TcpClient::writeData(QByteArray data)
{
	if (Transmitter->state() == QAbstractSocket::ConnectedState)
	{
		Transmitter->write(IntToArray(data.size())); //write size of data
		Transmitter->write(data); //write the data itself
		return Transmitter->waitForBytesWritten();
	}
	else
		return false;
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
	//Avoid use of cast, this is the Qt way to serialize objects
	QByteArray temp;
	QDataStream data(&temp, QIODevice::ReadWrite);
	data << source;
	return temp;
}
void TcpClient::tcpResponse(void) 
{
	inBuffer.startTransaction();
	QString response;
	inBuffer >> response;

	if (inBuffer.commitTransaction())
		emit transactionComplete(response);
		return;

}

//Transmitter = new QTcpSocket(this);
//inBuffer.setDevice(Transmitter);
//inBuffer.setVersion(QDataStream::Qt_4_0);
//connect(Transmitter, &QIODevice::readyRead, this, tcpResponseHandler);