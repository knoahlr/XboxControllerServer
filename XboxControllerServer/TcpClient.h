#pragma once

#include <QTcpSocket>
#include <QNetworkSession>
#include <QDataStream>
#include <QTimer>

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
	QDataStream inBuffer;
	QString currentResponse;
	QNetworkSession *networkSession;


};