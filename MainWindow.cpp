#include "MainWindow.hpp"

MainWindow::MainWindow()
		:QMainWindow( nullptr ), 
		 editBlk({new QWidget( nullptr ),nullptr,new QLineEdit( nullptr ),new QLabel("Размер (мм.):")})
		 {
		QStringList cmdline_args = QCoreApplication::arguments();
		//qDebug()<<cmdline_args[0];
		setWindowState(Qt::WindowMaximized);
		resize( 1280, 720 );
		//	hs="100x100";	
			setGeometry( QRect(0,23,2560,1377)); 
			//move( screen()->geometry().center() - frameGeometry().center() );

	
	auto addRootItemMenu { [=](QString menuText, QMenuBar * MB, QVector<itemsMenuType> items ){
		QMenu * menuL0 = new QMenu(menuText);
		for (auto i : items){				
		menuL0->addAction(i.itemsText, this, i.action);
		MB->addMenu(menuL0);
							}
			} 
		};			
	auto addButtons { [=](QVBoxLayout * p_lay, QVector<itemsButtonType> items ,QWidget * parent=nullptr){
		for (auto i : items){				
		QPushButton *button = new QPushButton(QIcon("images/"+i.iconfName),"",parent);
		button->setFixedHeight( 70 );
		button->setFixedWidth( 70 );
		button->setFlat( true );
		button->setToolTip( i.descr);
		button->setCheckable(i.is_toggled);
		button->setAutoExclusive(i.is_toggled);
		p_lay->addWidget(button);				
		connect( button, &QPushButton::clicked, this , i.action );
							}
			} 
		};
		QMenuBar * mainMenu = new QMenuBar;
		addRootItemMenu ("Файл", mainMenu, filemenuItems);
		setMenuBar(mainMenu);
	//main widget & main layer
		QWidget * widget_main = new QWidget( this );
		setCentralWidget( widget_main );
		QHBoxLayout * layoutMain = new QHBoxLayout ( widget_main );
	//left tools zone
		QWidget * widget_left = new QWidget( widget_main );
		QVBoxLayout * layoutTools = new QVBoxLayout ( widget_left );
		layoutMain->addWidget(widget_left);
		widget_left->setFixedWidth( 90 );
		layoutMain->setSpacing( 0 );		
		layoutTools->setContentsMargins( 10, 10,0,0);
		layoutMain->setContentsMargins( 0, 0,0,0);	
		addButtons(layoutTools,toolButtons);		
		editBlk.layoutmodifyAct = new QVBoxLayout ( editBlk.widgetmodifyAct );
		addButtons(editBlk.layoutmodifyAct,modifyactButtons,editBlk.widgetmodifyAct);		
		editBlk.layoutmodifyAct->setContentsMargins( 0, 0,0,0);
		layoutTools->addWidget(editBlk.widgetmodifyAct);
		editBlk.setVisible(false);
		editBlk.lineEdit->setFixedWidth( 70 );
		layoutTools->addWidget(editBlk.editlabelText);
		layoutTools->addWidget(editBlk.lineEdit);
		layoutTools->addStretch( 0 );	
	//picture zone:
		QWidget * widgetView = new QWidget(widget_main);
		QHBoxLayout * layoutView = new QHBoxLayout(widgetView );
		scene = new QGraphicsScene( -1000/*x*/, -1000/*y*/, 2000/*w*/, 2000/*h*/, widgetView );
		view = new View( scene, widgetView );		
		layoutMain->addWidget( view );
		view->setFocus();
	}
	toolType MainWindow::getTool() {
		return currentActiveTool;
	}
	void MainWindow::actOpen() {
		editBlk.setVisible(false);
	}
	void MainWindow::actSave() {
		editBlk.setVisible(true);
	}
	void MainWindow::actSaveAs() {
		qDebug()<<"SaveAs";
	}
	void MainWindow::actExit() {
		qDebug()<<"Exit";
	}
	void MainWindow::acttool_edit() {
		currentActiveTool = toolType::edit;
		qDebug()<<"tool_edit";
	}
	void MainWindow::acttool_size() {
		currentActiveTool = toolType::size;
		qDebug()<<"tool_size";
	}
	void MainWindow::acttool_dotted_line() {
		currentActiveTool = toolType::line_dashed;
		qDebug()<<"tool_dotted_line";
	}
	void MainWindow::acttool_solid_line() {
		currentActiveTool = toolType::line_solid;
		qDebug()<<"tool_solid_line";
	}
	void MainWindow::acttool_remove() {
		qDebug()<<"acttool_remove";
	}
	void MainWindow::acttool_text() {
		qDebug()<<"acttool_text";
	}

	void MainWindow::actarrow_left() {
		qDebug()<<"actarrow_left";
	}
	void MainWindow::actarrow_right() {
		qDebug()<<"actarrow_right";
	}
	void MainWindow::actarrow_up() {
		qDebug()<<"actarrow_up";}
	void MainWindow::actarrow_down() {
		qDebug()<<"actarrow_down";
		}	
	
	
	void editBlock::setVisible(bool flag) {
		widgetmodifyAct->setEnabled(flag);
		lineEdit->setVisible(flag);
		editlabelText->setVisible(flag);
	}