#pragma once
#include <QSettings>
#include <QCommandLineParser>
class AppSettings :public QSettings {
	QMap<QString,QVariant> 			params;
	QCommandLineParser 				parseCmdLn;
	const QMap<QString, QCommandLineOption> outputOptions = {
		{"fn" 		, QCommandLineOption(QStringList() << "f" << "fn", "file name" ,"file")},
		{"cs" 		, QCommandLineOption(QStringList() << "c" << "cs", "canvassize","canvassize")},
		{"mode" 		, QCommandLineOption(QStringList() << "m" << "mode"	  )}
		
	};
	public:
	AppSettings();
	QString getParamCmdLn(QString);
	QVariant operator[](QString);
	QVariant getValue(QString);
	void save();
}; // class settings


