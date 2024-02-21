#pragma once
enum types { e_line_solid = QGraphicsItem::UserType + 1, e_line_dashed, e_text, e_size };

	struct point_and_QGraphicsItem{
	QGraphicsItem* item;
	QPointF point;
	QLineF firstCoord;	
};


struct item_base {
	item_base();
	virtual int type() const = 0;
	virtual QJsonObject to_JSON() const = 0;
	QPen get_pen(qint8 style , qint8 width);
}; 
class View;
class myline final: public item_base,public QGraphicsItem { 
	qint8 				mode;
	QPointF 			firstPoint;
	QPointF 			secondPoint;
	public:	
	enum { Type = 600 };
	QJsonObject to_JSON() const;
	//myline(QLineF, int ,int, QWidget);
	myline(QPointF);
	QRectF boundingRect() const override;
	void changefirstPointCoord(QPointF);
	void changesecondPointCoord(QPointF);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void changeMode();
	QLineF line();
	int type() const override { return Type; }
	//point_and_QGraphicsItem findObjectNearBy(QPointF);
	QPointF findObjectNearBy(QPointF);
	qint8 getMode();

};
