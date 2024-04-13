#pragma once
	#include "AppSettings.hpp"
class View;


struct point_and_QGraphicsItem{
	QGraphicsItem* item;
	QPointF point;
	QLineF firstCoord;	
};

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
