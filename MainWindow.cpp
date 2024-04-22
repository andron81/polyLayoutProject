#include "MainWindow.hpp"
#include "AppSettings.hpp"
#include "View.hpp"
#include "Canvas.hpp"
#include "Items.hpp"




EditBlock& MainWindow::geteditBlk(){
	return editBlk;	
}
View * MainWindow::getView(){
	return view;
}
void MainWindow::rotateButtonClick(){
	QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
	itemTmp->setRotation(itemTmp->rotation()+90);
}	
void MainWindow::lineSizeEditChanged() {
	QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();	
	if (itemTmp) {
	if (itemTmp->type()==600)
	static_cast<Myline*>(itemTmp)->changeLength(editBlk.lineSizeEdit->text().toDouble());
	else 
	if (itemTmp->type()==602){
	static_cast<Text*>(itemTmp)->changeSize(editBlk.lineSizeEdit->text().toDouble());	
	}
	if (itemTmp->type()==603){
	static_cast<Size*>(itemTmp)->changeLength(editBlk.lineSizeEdit->text().toDouble());	
	}
	}

}
void MainWindow::lineTextEditChanged() {
	QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
	static_cast<Text*>(itemTmp)->setPlainText(editBlk.lineTextEdit->text());
}
void MainWindow::setCoordLabel(QString txt){
	labelCoord->setText(  txt);	
}
MainWindow::MainWindow()
		:settings(new AppSettings), 
		 QMainWindow( nullptr ), 
		 editBlk({new QWidget( nullptr ),nullptr,new QLineEdit( nullptr ),new QLabel,
		 new QLineEdit( nullptr ),new QLabel("Текст"),
				new QPushButton("Вращать")
		 })
		 {
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
		labelCoord = new QLabel;
		statusBar()->addWidget(labelCoord);
		editBlk.widgetmodifyAct->setEnabled(false);
		QStringList cmdline_args = QCoreApplication::arguments();
		setWindowState(Qt::WindowMaximized);
		resize( 1280, 720 );
		setGeometry( (*settings)["geometry"].toRect() );
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
		editBlk.setVisible(EditBlockVisible::none);
		//editBlk.lineEdit->setFixedWidth( 70 );
		statusBar()->addWidget(editBlk.lineSizeLabel1);		
		statusBar()->addWidget(editBlk.lineSizeEdit);
		statusBar()->addWidget(editBlk.lineTextLabel2);
		statusBar()->addWidget(editBlk.lineTextEdit);
		editBlk.lineSizeEdit->setFixedWidth( 90 );
		editBlk.lineTextEdit->setFixedWidth( 90 );
		//connect(button, &QPushButton::clicked, this , &MainWindow::actOpen) 
		connect(  editBlk.lineSizeEdit, &QLineEdit::editingFinished, this, &MainWindow::lineSizeEditChanged );
		connect(  editBlk.lineTextEdit, &QLineEdit::editingFinished, this, &MainWindow::lineTextEditChanged );
		connect(  editBlk.rotateButton, &QPushButton::clicked, this, &MainWindow::rotateButtonClick );

		statusBar()->addWidget(editBlk.rotateButton);
		

		layoutTools->addStretch( 0 );	
	//picture zone:
		QWidget * widgetView = new QWidget(widget_main);
		QHBoxLayout * layoutView = new QHBoxLayout(widgetView );
		scene = new QGraphicsScene( -1000/*x*/, -1000/*y*/, 2000/*w*/, 2000/*h*/, widgetView );
		view = new View( scene, widgetView );		
		layoutMain->addWidget( view );
		
		
		
		view->setFocus();
	}
	
	ToolType MainWindow::getTool() {
		return currentActiveTool;
	}
	AppSettings	* MainWindow::getSettings(){
		return settings;
	}
	void MainWindow::actExport(){

		QFileDialog dialog(this);
		 QStringList filters;
		 filters << "png file (*.png)";	
		 dialog.setAcceptMode (QFileDialog :: AcceptSave); 		 
		 dialog.setNameFilters(filters);		 
		 QStringList fileNames;
		 if (dialog.exec()) fileNames = dialog.selectedFiles(); else return;		 
		view->save_to_image( fileNames.back() , dialog.selectedMimeTypeFilter());


	}
	void MainWindow::actNew() {
		view->getCanvas()->clear();
	}
	void MainWindow::actOpen() {
		view->getCanvas()->select(false);
		editBlk.setVisible(EditBlockVisible::changeLength);
	}
	void MainWindow::actSave() {
		view->getCanvas()->select(false);
		
		if ( fileName.isEmpty() ) {
			QString filename = QFileDialog::getSaveFileName( nullptr, "Сохранить", ".", "Векторный файл (*.vct)" );
			if ( filename.isEmpty() ) return;
			fileName = filename;
		}
		
		QList<QGraphicsItem *> 			itemList = view->items();		
		QJsonObject m_currentJsonObject 		 = QJsonObject(); 
		QJsonArray textsArray 					 = m_currentJsonObject["texts"].toArray();
		

		for(auto it = itemList.begin(); it != itemList.end()-1; it++){
			QJsonObject js = itemOperations::to_JSON(*it);
			if (!js.isEmpty() ) textsArray.append(js);
		}
		m_currentJsonObject["texts"]=textsArray;
		
		QJsonDocument doc = QJsonDocument(m_currentJsonObject);
		   QFile jsonFile(fileName);
			jsonFile.open(QFile::WriteOnly);
			jsonFile.write(doc.toJson());	
	}
	void MainWindow::actSaveAs() {				
		view->getCanvas()->select(false);
		qDebug()<<"SaveAs";
	}
	void MainWindow::actExit() {
		qDebug()<<"Exit";
	}
	void MainWindow::acttool_edit() {
		currentActiveTool = ToolType::edit;
		qDebug()<<"tool_edit";
	}
	void MainWindow::acttool_size() {
		view->getCanvas()->select(false);
		currentActiveTool = ToolType::size;
		qDebug()<<"tool_size";
	}
	void MainWindow::acttool_dotted_line() {
		view->getCanvas()->select(false);
		currentActiveTool = ToolType::line_dashed;
		qDebug()<<"tool_dotted_line";
	}
	void MainWindow::acttool_solid_line() {
		view->getCanvas()->select(false);
		currentActiveTool = ToolType::line_solid;
		qDebug()<<"tool_solid_line";
	}
	void MainWindow::acttool_remove() {
		view->getCanvas()->eraseCurrentItem();	
	}
	void MainWindow::acttool_text() {
		view->getCanvas()->select(false);
		currentActiveTool = ToolType::text;
		qDebug()<<"acttool_text";
	}

	void MainWindow::actarrow_left() {
		QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
		itemOperations::move(itemTmp, 0);
	}
	void MainWindow::actarrow_right() {
		QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
		itemOperations::move(itemTmp, 1);
	}
	void MainWindow::actarrow_down() {
		QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
		itemOperations::move(itemTmp, 2);
		}	
	
	void MainWindow::actarrow_up() {
		QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
		itemOperations::move(itemTmp, 3);

	}

	void EditBlock::setVisible(EditBlockVisible flag) {
			
		switch (flag) {
			case EditBlockVisible::none: 
				widgetmodifyAct->setEnabled(false);			
				lineSizeEdit->setVisible(false);
				lineSizeLabel1->setVisible(false);
				lineTextEdit->setVisible(false);
				lineTextLabel2->setVisible(false);
				rotateButton->setVisible(false);
			break;
			case EditBlockVisible::changeLength :
			qDebug()<<"changeLength";				
				setVisible(EditBlockVisible::none);
				widgetmodifyAct->setEnabled(true);	
				lineSizeEdit->setVisible(true);
				lineSizeLabel1->setVisible(true);
				lineSizeLabel1->setText("Размер (мм.): ");				
				
			break;
			case EditBlockVisible::changeText :	
				setVisible(EditBlockVisible::none);
				widgetmodifyAct->setEnabled(true);				
				lineSizeEdit->setVisible(true);
				lineSizeLabel1->setVisible(true);
				lineSizeLabel1->setText("Размер текста: ");			
				
				lineTextEdit->setVisible(true);
				lineTextLabel2->setVisible(true);;
				rotateButton->setVisible(true);
			
			break;

		}		
	}
	
