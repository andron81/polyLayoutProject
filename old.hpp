#pragma once
#include "VectorDraw_pch.hpp"
#include <QMenuBar>

class MainWindow : public QMainWindow {
	Q_OBJECT
	QMainWindow *	m_main_window;
	typedef void (MainWindow::*funcactionType)();
	struct itemsMenuType {QString itemsText; funcactionType action ;};
public slots:
	void actOpen() {qDebug()<<"Open";}
	void actSave() {qDebug()<<"Save";}
	void actSaveAs() {qDebug()<<"SaveAs";}
	void actExit() {qDebug()<<"Exit";}
public: 
	QVector<itemsMenuType> items = {
		{"Открыть",  	 &MainWindow::actOpen},
		{"Сохранить",	 &MainWindow::actSave},
		{"Сохранить как",&MainWindow::actSaveAs},
		{"Выход",	 &MainWindow::actExit}
	};
	MainWindow( QWidget * p_parent = nullptr, int argc=0, char * argv[] = nullptr )
		: QMainWindow( p_parent ), m_main_window(this){
		this->setWindowState(Qt::WindowMaximized); //Maximizes the window
		auto addRootItemMenu { [=](QString menuText, QMenuBar * MB, QVector<itemsMenuType> items ){
				QMenu * menuL0 = new QMenu(menuText);
				for (auto i : items){				
				//menuL0->addAction(i.itemsText, this, (this->*(i.action))());	
				//menuL0->addAction(i.itemsText, this, &MainWindow::actOpen);	
				menuL0->addAction(i.itemsText, this, i.action);
				MB->addMenu(menuL0);
				}
			} 
		};

		QMenuBar * mainMenu = new QMenuBar;
		addRootItemMenu ("Файл", mainMenu, items);
		m_main_window->setMenuBar(mainMenu);
   
		QWidget * p_widget_central = new QWidget( this );
		this->setCentralWidget( p_widget_central );
		p_widget_central->setMaximumWidth( 100 );
		//p_widget_central->setMaximumHeight( 200 );

		QVBoxLayout * p_layout_central = new QVBoxLayout ( p_widget_central );
		p_layout_central->setContentsMargins( 0/*left*/, 0/*top*/, 4/*right*/, 4/*bottom*/ );

		QPushButton *button1 = new QPushButton(QIcon("images/up_svgrepo_com.svg"));
		QPushButton *button2 = new QPushButton(QIcon("images/up_svgrepo_com.svg"));


				button1->setFixedHeight( 32 );
				button1->setFlat( true );
				button1->setCheckable(true);
				button1->setAutoExclusive(true);

				//button1->setIcon( QIcon( icon ) );
				button2->setFixedHeight( 32 );
				button2->setFlat( true );
				button2->setCheckable(true);
				button2->setAutoExclusive(true);

				//button2->setIcon( QIcon( icon ) );


		p_layout_central->addWidget(button1);
		p_layout_central->addWidget(button2);

		p_layout_central->setSpacing( 0 );
		p_layout_central->setContentsMargins( 0/*left*/, 0/*top*/, 4/*right*/, 4/*bottom*/ );




				
	}
}; // class MainWindow
