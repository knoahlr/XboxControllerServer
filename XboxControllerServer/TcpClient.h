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
	explicit TcpClient(QString Ip, int Port);
	struct Config {
		QString Ip;
		int Port;
	};
	Config tcpClientConfig;

public slots:
	void connectToHost(void);
	void writeData(QByteArray data);
	void tcpResponse(void);

signals:
	void transactionComplete(QString data);
	void deviceStateUpdate(bool connected);

private:
	//TCP Transmitter and Receiver
	QTcpSocket *Transmitter;
	QTcpSocket *Receiver;
	QString currentResponse;
	QHash<QTcpSocket*, QByteArray*> inBuffers;
	QHash<QTcpSocket*, qint32 *> clientSizes;
	QNetworkSession *networkSession;
	QMutex readWriteMutex;

};
qint32 ArrayToInt(QByteArray source);