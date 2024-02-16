#pragma once
enum types { e_line_solid = QGraphicsItem::UserType + 1, e_line_dashed, e_text, e_size };

struct item_base {
	item_base();
	virtual int type() const = 0;
	virtual QJsonObject to_JSON() const = 0;
	QPen get_pen(qint8 style , qint8 width);
}; 

class MyQGraphicsItem : public QGraphicsItem {
	public:
			virtual void changefirstPointCoord(QPointF) = 0;
};

class myline final: public item_base,public MyQGraphicsItem { 
	qint8 				mode;
	QPointF 			firstPoint;
	public:	
	enum { Type = 600 };
	QJsonObject to_JSON() const;
	//myline(QLineF, int ,int, QWidget);
	myline(QPointF);
	QRectF boundingRect() const override;
	void changefirstPointCoord(QPointF);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	int type() const override { return Type; }
};
