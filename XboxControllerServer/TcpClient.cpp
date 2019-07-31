#include "TcpClient.h"

static inline QByteArray IntToArray(qint32 source);
using namespace std::chrono_literals;
TcpClient::TcpClient(QString Ip, int Port) : QObject()
{
	tcpClientConfig.Ip = Ip;
	tcpClientConfig.Port = Port;
	Transmitter = new QTcpSocket(this);
	QByteArray *buffer = new QByteArray();
	qint32 *transmitterSize = new qint32(0);
	inBuffers.insert(Transmitter, buffer);
	clientSizes.insert(Transmitter, transmitterSize);
	connect(Transmitter, SIGNAL(readyRead()), this, SLOT(tcpResponse()));

}

void TcpClient::connectToHost(void)
{
	if (readWriteMutex.tryLock(10000))
	{
		Transmitter->connectToHost(tcpClientConfig.Ip, tcpClientConfig.Port);
		if (Transmitter->waitForConnected())
		{
			emit deviceStateUpdate(true);
		}
	}
	readWriteMutex.unlock();
}

void TcpClient::writeData(QByteArray data)
{
	//Start a clock
	if (Transmitter->state() == QAbstractSocket::ConnectedState)
	{
		Transmitter->write(IntToArray(data.size())); //write size of data
		Transmitter->write(data); //write the data itself
		if (Transmitter->waitForBytesWritten())
		{
			//Do Nothing For Now
		}
	}
	else 
	{
		return connectToHost();
	}
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
	if (Transmitter->bytesAvailable() > 0)
	{
		QByteArray *buffer = inBuffers.value(Transmitter);
		qint32 *s = clientSizes.value(Transmitter);
		qint32 size = buffer->size();
		buffer->append(Transmitter->readAll());
		QByteArray data = buffer->mid(0, buffer->length());
		QString response = QString::fromStdString(buffer->toStdString());
		buffer->clear();
		emit transactionComplete(response);
	}
}
qint32 ArrayToInt(QByteArray source)
{
	qint32 temp;
	QDataStream data(&source, QIODevice::ReadWrite);
	data >> temp;
	return temp;
}
