#pragma once
#include "VectorDraw_pch.hpp"




class View;
class AppSettings;
struct EditBlock{	
	QWidget* 					widgetmodifyAct;
	QVBoxLayout*	    		layoutmodifyAct;
	QLineEdit* 					lineSizeEdit;
	QLabel* 					lineSizeLabel1;

	QLineEdit* 					lineTextEdit;
	QLabel* 					lineTextLabel2;
	QPushButton*				rotateButton;
	void setVisible(EditBlockVisible);
	void setlineSizeEdit(qreal);


};

class MainWindow : public QMainWindow {

	typedef void (MainWindow::*funcactionType)();
	struct itemsMenuType {QString itemsText; funcactionType action ;};
	struct itemsButtonType {QString descr; QString iconfName; funcactionType action ; bool is_toggled;};

	Q_OBJECT	
	AppSettings	*				settings;
	QGraphicsScene *			scene;
	View *						view;
	ToolType 					currentActiveTool		=	ToolType::none;
	EditBlock 					editBlk ;
	QLabel*						labelCoord;
	QString						fileName				=	"";
	QString						currentDir				= 	".";
	bool						mode					;
	
	void Open(QString);
	void Export(QString);
	void Confirm();
	void saveFile(bool dialog); 
	

public slots:
	void lineTextEditChanged();
	void lineSizeEditChanged();
	void rotateButtonClick();
	void actNew();
	void actOpen();
	void actSave();
	void actSaveAs();
	void actExport();	
	void actExit();
	void acttool_edit();
	void acttool_size();
	void acttool_dotted_line();
	void acttool_solid_line(); 
	void acttool_remove();
	void acttool_text();
	void actarrow_left();
	void actarrow_right();
	void actarrow_up();
	void actarrow_down();
protected:
void closeEvent(QCloseEvent * ) override; 
public:
	EditBlock& geteditBlk();
	View * getView();
	ToolType getTool();
	AppSettings	* getSettings();
	void setCoordLabel(QString);	
	void refreshFileInfo(QString);
	const QVector<itemsMenuType> filemenuItems = {
		{"Новый",  	 &MainWindow::actNew},
		{"Открыть",  	 &MainWindow::actOpen},
		{"Сохранить",	 &MainWindow::actSave},
		{"Сохранить как",&MainWindow::actSaveAs},
		{"Экспортировать как",&MainWindow::actExport},
		{"Выход",	 &MainWindow::actExit}
	};
	const QVector<itemsMenuType> filemenuItemsMode1C = {
		{"Выход",	 &MainWindow::actExit}
	};
	const QVector<itemsButtonType> toolButtons = {
		{"Редактировать", "tool_edit.svg", 	 &MainWindow::acttool_edit,true},
		{"Размер",	 "tool_size.svg",&MainWindow::acttool_size,true},
		{"Сплошная линия","tool_line_solid.svg",&MainWindow::acttool_solid_line,true},
		{"Пунктирная линия","tool_line_dashed.svg"	 ,&MainWindow::acttool_dotted_line,true},		
		{"Текст","tool_text.svg"	,&MainWindow::acttool_text,true}
	};
	
	const QVector<itemsButtonType> modifyactButtons = {
		{"Удалить",	"tool_remove.svg", &MainWindow::acttool_remove,false},
		{"Сдвиг влево", "left_svgrepo_com.svg",	 &MainWindow::actarrow_left,false},
		{"Сдвиг вправо", "right_svgrepo_com.svg",	 &MainWindow::actarrow_right,false},
		{"Сдвиг вверх","up_svgrepo_com.svg",	 &MainWindow::actarrow_up,false},
		{"Сдвиг вниз","down_svgrepo_com.svg"	 ,&MainWindow::actarrow_down,false}
	};
	MainWindow();

	
}; // class MainWindow
