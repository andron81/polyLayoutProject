#pragma once
class View;
class MyQGraphicsItem;
class Canvas : public QGraphicsRectItem {
	bool 					innerCanvas = false;	
	View* 					view;
	MyQGraphicsItem*		currentItem = nullptr;			

public:
	Canvas(View* );
	

	QPointF getTopLeft() const  ; 

	QPointF getBottomRight() const  ;
	
	QSize getSize() const  ;	
	View* getView() const ;
	auto itemCast();
	void setSize( const QSize & size ); 
	void mouseLeaveEvent();
	void mouseMoveEvent(QMouseEvent *);
	bool isinner();
}; // class view_canvas
