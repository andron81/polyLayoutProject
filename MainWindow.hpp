#pragma once
#include "VectorDraw_pch.hpp"
#include "View.hpp"



struct editBlock{
	QWidget* 					widgetmodifyAct;
	QVBoxLayout*	    		layoutmodifyAct;
	QLineEdit* 					lineEdit;
	QLabel* 					editlabelText;
	
	void setVisible(bool flag);
};

class MainWindow : public QMainWindow {
	Q_OBJECT	
	QGraphicsScene *			scene;
	View *						view;
	toolType 					currentActiveTool		=	toolType::none;
	typedef void (MainWindow::*funcactionType)();
	struct itemsMenuType {QString itemsText; funcactionType action ;};
	struct itemsButtonType {QString descr; QString iconfName; funcactionType action ; bool is_toggled;};
	editBlock editBlk ;
	
public slots:
	void actOpen();
	void actSave();
	void actSaveAs();
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
public: 
	toolType getTool();
	const QVector<itemsMenuType> filemenuItems = {
		{"Открыть",  	 &MainWindow::actOpen},
		{"Сохранить",	 &MainWindow::actSave},
		{"Сохранить как",&MainWindow::actSaveAs},
		{"Выход",	 &MainWindow::actExit}
	};
	const QVector<itemsButtonType> toolButtons = {
		{"Редактировать", "tool_edit.svg", 	 &MainWindow::acttool_edit,true},
		{"Размер",	 "tool_size.svg",&MainWindow::acttool_size,true},
		{"Сплошная линия","tool_line_solid.svg",&MainWindow::acttool_dotted_line,true},
		{"Пунктирная линия","tool_line_dashed.svg"	 ,&MainWindow::acttool_solid_line,true},		
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
