	#include "AppSettings.hpp"
	
	AppSettings::AppSettings( )
		: QSettings( QFileInfo( QCoreApplication::applicationFilePath() ).baseName() + ".ini", QSettings::IniFormat ){
			Q_ASSERT( QFile( fileName()).exists() );
			beginGroup( "MainWindow" );
			QStringList keys = allKeys(); 
			for ( const auto& i : keys )
				params[i] = value(i);
			endGroup();
		}

	QVariant AppSettings::operator[](QString key) {				
		return params[key];		
	}
	QVariant AppSettings::getValue(QString key){
		return params[key];
	}

	void AppSettings::save() {
		beginGroup( "MainWindow" );

		endGroup();
	}