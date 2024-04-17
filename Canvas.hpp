#pragma once

#include "AppSettings.hpp"
class View;


class Canvas : public QGraphicsRectItem {
	bool 					isMouseInsideCanvas = false;
	bool 					isMouseHold = false;
	View* 					view;
	QGraphicsItem*			currentItem = nullptr;
	QPointF					currentItemPoint;
	

public:
	Canvas(View* );
	
	bool isMouseInside();
	QPointF getTopLeft() const  ; 
	void clear();
	QPointF getBottomRight() const  ;
	void eraseCurrentItem();
	QSize getSize() const  ;	
	View* getView() const ;
	void setSize( const QSize & size ); 
	void mouseLeaveEvent();
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent();
	void FindNearbyItem(const QPointF&); 
	void select(bool flag);
	QGraphicsItem* getCurrentItem();
	ToolType getTool();
}; // class view_canvas
