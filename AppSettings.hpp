#pragma once
#include <QSettings>
#include <QCommandLineParser>
class AppSettings :public QSettings {
	QMap<QString,QVariant> 			params;
	QCommandLineParser 				parseCmdLn;
	const QMap<QString, QCommandLineOption> outputOptions = {
		{"fn" 		, QCommandLineOption(QStringList() << "f" << "fn", "file-option" ,"file-name-value")},
		{"cs" 		, QCommandLineOption(QStringList() << "c" << "cs"<<"canvassize", "canvas-size-option","canvas-size-value")},
		{"md" 		, QCommandLineOption(QStringList() << "m" << "md"<<"mode" , "mode")},
		{"currentDir", QCommandLineOption(QStringList() << "d" << "currentDir", "current-Dir-option", "currentDir-value")}
		
		
	};
	public:
	AppSettings();
	QString getParamCmdLn(QString);
	QVariant operator[](QString);
	QVariant getValue(QString);
	void save();
}; // class settings


