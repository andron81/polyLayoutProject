#pragma once

class MainWindow;
class Canvas;
class Scene;

class View : public QGraphicsView {
	const int			scalefactorMin		= 1;
	const int			scalefactorMax		= 200;
	const int			scalefactorDiv		= 10;
	int					scalefactor			= scalefactorDiv;
	MainWindow * 		mw					=nullptr;
	Canvas * 			canvas;
	public:
	//toolType getTool() {return static_cast<MainWindow>(mainWin)->getTool();}
	ToolType getTool() ;
	bool isMouseInsideCanvas(QPointF 	mouseCoord);
	View( QGraphicsScene * p_scene, QWidget * p_parent );
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void update_zoom();
	void wheelEvent( QWheelEvent * p_event ) override ;
	Canvas * getCanvas();
};