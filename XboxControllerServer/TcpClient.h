#pragma once

#include <QTcpSocket>
#include <QNetworkSession>
#include <QDataStream>
#include <QTimer>
#include <QMutex>
#include <thread>

class TcpClient : public QObject
{
	Q_OBJECT

public:
	explicit TcpClient(QObject *parent = 0);

public slots:
	bool connectToHost(QString host, int port);
	bool writeData(QByteArray data);
	void tcpResponse(void);
signals:
	void transactionComplete(QString data);

private:
	//TCP Transmitter and Receiver
	QTcpSocket *Transmitter;
	QTcpSocket *Receiver;
	//QDataStream inBuffer;
	QString currentResponse;
	QHash<QTcpSocket*, QByteArray*> inBuffers;
	QHash<QTcpSocket*, qint32 *> clientSizes;
	QNetworkSession *networkSession;
	QMutex readWriteMutex;

};
qint32 ArrayToInt(QByteArray source);