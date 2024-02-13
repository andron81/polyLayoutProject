#pragma once

class Canvas : public QGraphicsRectItem {
	bool innerCanvas = false;
public:
	Canvas();
	

	QPointF getTopLeft() const  ; 

	QPointF getBottomRight() const  ;
	
	QSize getSize() const  ;	

	void setSize( const QSize & size ); 
	void mouseLeaveEvent();
	void mouseMoveEvent();
	bool isinner();
}; // class view_canvas
