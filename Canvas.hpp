#pragma once
class View;

class Canvas : public QGraphicsRectItem {
	bool 					isMouseInsideCanvas = false;	
	View* 					view;
	QGraphicsItem*		currentItem = nullptr;			

public:
	Canvas(View* );
	
	bool isMouseInside();
	QPointF getTopLeft() const  ; 

	QPointF getBottomRight() const  ;
	
	QSize getSize() const  ;	
	View* getView() const ;
	auto itemCast();
	void setSize( const QSize & size ); 
	void mouseLeaveEvent();
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	toolType getTool();	
}; // class view_canvas
