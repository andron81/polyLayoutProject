#pragma once
#include "MainWindow.hpp"
//enum types { e_line_solid = QGraphicsItem::UserType + 1, e_line_dashed, e_text, e_size };

	struct point_and_QGraphicsItem{
	QGraphicsItem* item;
	QPointF point;
	QLineF firstCoord;	
};


struct item_base {
	MainWindow* mw;
	QPen pen;
	item_base(MainWindow* mw_);
	virtual int type() const = 0;
	virtual QJsonObject to_JSON() const = 0;
	QPen getPen(toolType , qint8 , qint64 );
}; 
class View;
class Myline : public item_base,public QGraphicsItem { 
	qint8 						mode;
	QPointF 					firstPoint;
	QPointF 					secondPoint;
	public:	
	enum { Type = 600 };
	QJsonObject to_JSON() const;
	//myline(QLineF, int ,int, QWidget);
	Myline(MainWindow* mw, QPointF);
	QRectF boundingRect() const override;
	void changefirstPointCoord(QPointF);
	void changesecondPointCoord(QPointF);
	void changePoints(QPointF);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void changeMode();
	QLineF line();
	int type() const override { return Type; }
	//point_and_QGraphicsItem findObjectNearBy(QPointF);
	QPointF findObjectNearBy(QPointF);
	qint8 getMode();
};

class Size : public Myline {
	public:
	Size(MainWindow* mw, QPointF);	
};	
