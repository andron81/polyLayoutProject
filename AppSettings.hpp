#pragma once
#include <QSettings>
class AppSettings :public QSettings {
	QMap<QString,QVariant> 			params;
	public:
	AppSettings();
	QVariant operator[](QString);
	QVariant getValue(QString);
	void save();
}; // class settings


