#include "TcpClient.h"

static inline QByteArray IntToArray(qint32 source);
using namespace std::chrono_literals;
TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
	Transmitter = new QTcpSocket(this);
	QByteArray *buffer = new QByteArray();
	qint32 *transmitterSize = new qint32(0);
	inBuffers.insert(Transmitter, buffer);
	clientSizes.insert(Transmitter, transmitterSize);
	connect(Transmitter, SIGNAL(readyRead()), this, SLOT(tcpResponse()));

}

bool TcpClient::connectToHost(QString host, int port)
{
	Transmitter->connectToHost(host, port);
	return Transmitter->waitForConnected();
}

bool TcpClient::writeData(QByteArray data)
{
	//Start a clock
	

	if (Transmitter->state() == QAbstractSocket::ConnectedState)
	{
		if (readWriteMutex.tryLock(3000))
		{
			Transmitter->write(IntToArray(data.size())); //write size of data
			Transmitter->write(data); //write the data itself
			return Transmitter->waitForBytesWritten();
		}
	}
	else {
		return connectToHost("192.168.91.112", 1000);
	}

	readWriteMutex.unlock();
	std::this_thread::sleep_for(2ms);

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
	

	//while (Transmitter->bytesAvailable() > 0)
	//{
	//	buffer->append(Transmitter->readAll());
	//	while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
	//	{
	//		if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
	//		{
	//			size = ArrayToInt(buffer->mid(0, 4));
	//			*s = size;
	//			buffer->remove(0, 4);
	//		}
	//		if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
	//		{
	//			QByteArray data = buffer->mid(0, size);
	//			buffer->remove(0, size);
	//			size = 0;
	//			*s = size;
	//			QString response = QString::fromUtf8(data);
	//			emit transactionComplete(response);
	//		}
	//	}
	//}

	if (Transmitter->bytesAvailable() > 0)
	{
		//QMutexLocker locker(&);
		if (readWriteMutex.tryLock(3000))
		{
			QByteArray *buffer = inBuffers.value(Transmitter);
			qint32 *s = clientSizes.value(Transmitter);
			qint32 size = buffer->size();
			buffer->append(Transmitter->readAll());
			/*
			TODO: FIX ME
			*/
			QByteArray data = buffer->mid(0, buffer->length());
			QString response = QString::fromStdString(buffer->toStdString());
			buffer->clear();
			emit transactionComplete(response);
		}
	}
	readWriteMutex.unlock();
	std::this_thread::sleep_for(2ms);
}
qint32 ArrayToInt(QByteArray source)
{
	qint32 temp;
	QDataStream data(&source, QIODevice::ReadWrite);
	data >> temp;
	return temp;
}
