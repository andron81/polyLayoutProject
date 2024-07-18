	#include "AppSettings.hpp"
	
	AppSettings::AppSettings( )
		: QSettings( QFileInfo( QCoreApplication::applicationFilePath() ).baseName() + ".ini", QSettings::IniFormat ){
		parseCmdLn.setApplicationDescription("myApp");
		parseCmdLn.addHelpOption();
		parseCmdLn.addVersionOption();
		// options list:
		for (auto i = outputOptions.begin(); i != outputOptions.end(); i++)
			parseCmdLn.addOption(i.value());
		parseCmdLn.process(QCoreApplication::arguments());

		QMap<QString, QCommandLineOption>::const_iterator it = outputOptions.find("c");
			Q_ASSERT( QFile( fileName()).exists() );
			beginGroup( "MainWindow" );
			QStringList keys = allKeys(); 
		for ( const auto& i : keys )
			params[i] = value(i);
		endGroup();
		}
	QString AppSettings::getParamCmdLn(QString paramName){
		
	QMap<QString, QCommandLineOption>::const_iterator it = outputOptions.find(paramName);
	
	if(it != outputOptions.end()) {
		if (it.value().valueName()=="") {return QString::number(parseCmdLn.isSet(it.value()));} 
			else {
				return parseCmdLn.value(it.value());
			} 
	}
	else return ""; 
		
	}
	QVariant AppSettings::operator[](QString key) {	
	QString tmp = getParamCmdLn(key);
	if (tmp!="") return tmp; else
		return params[key];		
	}
	QVariant AppSettings::getValue(QString key){
	QString tmp = getParamCmdLn(key);
	if (tmp!="") return tmp; else
		return params[key];		

	}

	void AppSettings::save() {
		beginGroup( "MainWindow" );

		endGroup();
	}