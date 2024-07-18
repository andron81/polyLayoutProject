#include "MainWindow.hpp"
#include "AppSettings.hpp"
#include "View.hpp"
#include "Canvas.hpp"
#include "Items.hpp"
#include <QJsonArray>


void MainWindow::closeEvent(QCloseEvent * e) {
		Confirm();
	}


EditBlock& MainWindow::geteditBlk(){
	return editBlk;	
}
View * MainWindow::getView(){
	return view;
}
void MainWindow::rotateButtonClick(){
	QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
	view->getCanvas()->edit();
	if (itemTmp->rotation()+90>270) itemTmp->setRotation(0); 
		else
	itemTmp->setRotation(itemTmp->rotation()+90);
}	
void MainWindow::lineSizeEditChanged() {
	QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();	
	if (itemTmp) {
		view->getCanvas()->edit();
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
	view->getCanvas()->edit();
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
		mode = getSettings()->getValue("md").toBool();
		labelCoord = new QLabel;
		statusBar()->addWidget(labelCoord);
		editBlk.widgetmodifyAct->setEnabled(false);
		QStringList cmdline_args = QCoreApplication::arguments();
		setWindowState(Qt::WindowMaximized);
		resize( 1280, 720 );
		setGeometry( (*settings)["geometry"].toRect() );
		QMenuBar * mainMenu = new QMenuBar;
		if (mode)
		addRootItemMenu ("Файл", mainMenu, filemenuItemsMode1C);
		else 
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
		
		currentDir = QDir::currentPath()+"/"+ getSettings()->getValue("currentDir").toString();		
		QString fn = getSettings()->getValue("fn").toString();
		
		
		if (fn!="") Open(fn);
			
		
		
	}
	
	ToolType MainWindow::getTool() {
		return currentActiveTool;
	}
	AppSettings	* MainWindow::getSettings(){
		return settings;
	}
	void MainWindow::Export(QString fn=""){	
		QStringList fileNames;
		 if (fn==""){
			QFileDialog dialog(this);
			QStringList filters;
			filters << "png file (*.png)";	
			dialog.setAcceptMode (QFileDialog :: AcceptSave); 		 
			dialog.setNameFilters(filters);		 			
			if (dialog.exec()) fileNames = dialog.selectedFiles(); else return;
			view->save_to_image( fileNames.back() ,"");
			
		 } else 
			view->save_to_image( currentDir+"/"+fn+".png" ,"");		 
	}
	void MainWindow::actExport(){
		Export();
	}
	void MainWindow::actNew() {
		view->getCanvas()->clear();
	}
	
	
		void MainWindow::saveFile(bool dialog=false) {
		view->getCanvas()->select(false);
			
		if ( !dialog || fileName.isEmpty() ) {
			QString filename = QFileDialog::getSaveFileName( nullptr, "Сохранить", currentDir, "Векторный файл (*.vct)" );
			if ( filename.isEmpty() ) return;			
			refreshFileInfo(filename);	
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
		   QFile jsonFile(currentDir +"/"+fileName+".vct");
			jsonFile.open(QFile::WriteOnly);
			jsonFile.write(doc.toJson());					
	}
	
	
	void MainWindow::Confirm(){
	if (view->getCanvas()->getisEdit()) {	
	QMessageBox box;
		box.setIcon(QMessageBox::Question);
		box.setWindowTitle("Вопрос");
		box.setText("Сохранить изменения ?");
		box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
		QAbstractButton * buttonY = box.button(QMessageBox::Yes);
		buttonY->setText("Да");
		QAbstractButton *buttonN = box.button(QMessageBox::No);
		buttonN->setText("Нет");
		box.exec();
		if (box.clickedButton() == buttonY) {
				saveFile(true);
				if (mode) Export(fileName);
				view->getCanvas()->edit(false);
				QApplication::quit();
		
		}
		else if (box.clickedButton() == buttonN) {}
	}	
		
		
	}
	
	void MainWindow::Open(QString fn="") {
			view->getCanvas()->select(false);
		editBlk.setVisible(EditBlockVisible::changeLength);
		if (fn!="") fileName=currentDir+"/"+fn+".vct";
			else
		fileName = QFileDialog::getOpenFileName( nullptr, "Открыть", currentDir, "Vector Draw file (*.vct)" );
		if ( fileName.isEmpty() ) return;
		refreshFileInfo(fileName);
		view->getCanvas()->clear();			
		QString val;
		QFile file;
      file.setFileName(currentDir+"/"+fileName+".vct");
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      val = file.readAll();
      file.close();      	  		  
	  itemOperations::fillCanvas(this,scene,QJsonDocument::fromJson(val.toUtf8()));	
	  }
	
	void MainWindow::actOpen() {
		Open();
	}
	void MainWindow::refreshFileInfo(QString filename){
		QFileInfo fi = QFileInfo(filename);
			currentDir = fi.dir().path() ;
			fileName   = fi.baseName();	
			setWindowTitle(QFileInfo(QCoreApplication::applicationName()).baseName()
			+" "+ currentDir +"/"+fileName+"."+fi.suffix()
			);
	}

	
	void MainWindow::actSave() {
		saveFile(true);
	}
	void MainWindow::actSaveAs() {				
		saveFile();
		
		
	}
	void MainWindow::actExit() {
		Confirm();		
	}
	void MainWindow::acttool_edit() {
		currentActiveTool = ToolType::edit;
	}
	void MainWindow::acttool_size() {
		view->getCanvas()->select(false);
		currentActiveTool = ToolType::size;
	}
	void MainWindow::acttool_dotted_line() {
		view->getCanvas()->select(false);
		currentActiveTool = ToolType::line_dashed;
	}
	void MainWindow::acttool_solid_line() {
		view->getCanvas()->select(false);
		currentActiveTool = ToolType::line_solid;
	}
	void MainWindow::acttool_remove() {
		if (view->getCanvas()->getCurrentItem()) {
		view->getCanvas()->eraseCurrentItem();
		view->getCanvas()->edit();	
		}
	}
	void MainWindow::acttool_text() {
		view->getCanvas()->select(false);
		currentActiveTool = ToolType::text;
	}

	void MainWindow::actarrow_left() {
		QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
		itemOperations::move(itemTmp, 0);
		view->getCanvas()->edit();
	}
	void MainWindow::actarrow_right() {
		QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
		itemOperations::move(itemTmp, 1);
		view->getCanvas()->edit();
	}
	void MainWindow::actarrow_down() {
		QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();
		itemOperations::move(itemTmp, 2);
		view->getCanvas()->edit();
		}	
	
	void MainWindow::actarrow_up() {
		QGraphicsItem* itemTmp = view->getCanvas()->getCurrentItem();	
		itemOperations::move(itemTmp, 3);
		view->getCanvas()->edit();
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
	
