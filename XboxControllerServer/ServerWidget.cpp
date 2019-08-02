#include "ServerWidget.h"

ServerWidget::ServerWidget(QWidget *parent)
	: QWidget(parent)
{

	serverCentralLayout = new QVBoxLayout();

	//Top GroupBox
	gamepad = new QGroupBox("Gamepad");
	gamepadLayout = new QHBoxLayout();
	gamepad->setLayout(gamepadLayout);

	//Left Pane Box
	buttons = new QGroupBox("Buttons");
	buttonsLayout = new QFormLayout();
	buttons->setLayout(buttonsLayout);

	//Right Pane Box
	trigger_Analog = new QGroupBox("Triggers and Analog");
	trigger_AnalogLayout = new  QFormLayout();
	trigger_Analog->setLayout(trigger_AnalogLayout);

	//Middle Log groupBox
	logGroupBox = new QGroupBox("Log");
	logFrameLayout = new QVBoxLayout();
	logGroupBox->setLayout(logFrameLayout);

	//Bottom control and status groupbox
	controls_status = new QGroupBox();
	controls_statusLayout = new QGridLayout();
	controls_status->setLayout(controls_statusLayout);

	serverMode = new QGroupBox("Mode");
	serverModeLayout = new QVBoxLayout();
	serverMode->setLayout(serverModeLayout);

	controlServer = new QGroupBox("Server Control");
	controlServerLayout = new QHBoxLayout();
	controlServer->setLayout(controlServerLayout);


	//Labels and line Edit
	labelA = new QLabel("A");
	labelB = new QLabel("B");
	labelX = new QLabel("X");
	labelY = new QLabel("Y");
	labelLeftTrigger = new QLabel("Left Trigger");
	labelRightAnalog = new QLabel("Right Analog");
	labelLeftAnalog = new QLabel("Left Analog");
	labelRightTrigger = new QLabel("Right Trigger");
	ipLabel = new QLabel("IP");
	portLabel = new QLabel("Port");

	lineEditA = new QLineEdit();
	lineEditB = new QLineEdit();
	lineEditX = new QLineEdit();
	lineEditY = new QLineEdit();
	lineEditLeftTrigger = new QLineEdit();
	lineEditRightAnalog = new QLineEdit();
	lineEditLeftAnalog = new QLineEdit();
	lineEditRightTrigger = new QLineEdit();
	ipLineEdit = new QLineEdit();
	portLineEdit = new QLineEdit();
	ipLineEdit->setText("192.168.91.112");
	portLineEdit->setText("1000");

	//
	logBox = new QPlainTextEdit();
	logBox->setReadOnly(true);
	StartServer = new QPushButton("Start Server");
	StopServer = new QPushButton("Stop Server");

	//serverMode
	serverModeOptions = new QComboBox();
	QStringList items;
	items << QMetaEnum::fromType<options>().valueToKey(Server)
		<< QMetaEnum::fromType<options>().valueToKey(MCU)
		<< QMetaEnum::fromType<options>().valueToKey(Debug);

	serverModeOptions->addItems(items);
	ServerConnected = new QLabel("Disconnected");
	ServerConnected->setAlignment(Qt::AlignCenter);
	ServerConnected->setStyleSheet("QLabel { background-color : red; }");

	gamepadLayout->addWidget(buttons);
	gamepadLayout->addWidget(trigger_Analog);
	gamepadLayout->addWidget(logGroupBox);

	buttonsLayout->setWidget(0, QFormLayout::LabelRole, labelA);
	buttonsLayout->setWidget(0, QFormLayout::FieldRole, lineEditA);

	buttonsLayout->setWidget(1, QFormLayout::LabelRole, labelB);
	buttonsLayout->setWidget(1, QFormLayout::FieldRole, lineEditB);

	buttonsLayout->setWidget(2, QFormLayout::LabelRole, labelX);
	buttonsLayout->setWidget(2, QFormLayout::FieldRole, lineEditX);

	buttonsLayout->setWidget(3, QFormLayout::LabelRole, labelY);
	buttonsLayout->setWidget(3, QFormLayout::FieldRole, lineEditY);

	trigger_AnalogLayout->setWidget(0, QFormLayout::LabelRole, labelLeftTrigger);
	trigger_AnalogLayout->setWidget(0, QFormLayout::FieldRole, lineEditLeftTrigger);

	trigger_AnalogLayout->setWidget(1, QFormLayout::LabelRole, labelRightTrigger);
	trigger_AnalogLayout->setWidget(1, QFormLayout::FieldRole, lineEditRightTrigger);

	trigger_AnalogLayout->setWidget(2, QFormLayout::LabelRole, labelLeftAnalog);
	trigger_AnalogLayout->setWidget(2, QFormLayout::FieldRole, lineEditLeftAnalog);

	trigger_AnalogLayout->setWidget(3, QFormLayout::LabelRole, labelRightAnalog);
	trigger_AnalogLayout->setWidget(3, QFormLayout::FieldRole, lineEditRightAnalog);

	controlServerLayout->addWidget(StartServer);
	controlServerLayout->addWidget(StopServer);
	serverModeLayout->addWidget(serverModeOptions);
	serverModeLayout->addWidget(ServerConnected);

	logFrameLayout->addWidget(logBox);
	controls_statusLayout->addWidget(ipLabel, 0, 0);
	controls_statusLayout->addWidget(ipLineEdit, 0, 1);
	controls_statusLayout->addWidget(portLabel, 0, 2);
	controls_statusLayout->addWidget(portLineEdit, 0, 3);
	controls_statusLayout->addWidget(controlServer, 1, 0, 1, 2);
	controls_statusLayout->addWidget(serverMode, 1, 2, 1, 2);

	serverCentralLayout->addWidget(gamepad);
	serverCentralLayout->addWidget(controls_status);

	setLayout(serverCentralLayout);

}

int ServerWidget::currentMode(void)
{
	QString selectedText = serverModeOptions->itemText(serverModeOptions->currentIndex());
	auto metaEnum = QMetaEnum::fromType<options>();
	return (options)metaEnum.keyToValue(selectedText.toLocal8Bit().data());
}
