#pragma once
#include "MainWindow.hpp"
#include <variant>
//enum types { e_line_solid = QGraphicsItem::UserType + 1, e_line_dashed, e_text, e_size };

class itemOperations {
	public:
	static void setColor(QGraphicsItem * , QColor);
	static void move(QGraphicsItem *, int);
	static QJsonObject to_JSON(QGraphicsItem *);
	static void fillCanvas(MainWindow * , QGraphicsScene * , QJsonDocument);	
};

struct _3points{
	QPointF 					firstPoint;
	QPointF 					secondPoint;
	QPointF 					lastPoint;	
};

struct item_base {
	
	MainWindow* mw;
	QPen pen;
	item_base(MainWindow* mw_);
	virtual int type() const = 0;	
	QPen getPen(ToolType , qint8 , qint64 );
	QPen getPen();
	MainWindow*	getmw();	
}; 
class View;
class Myline : public item_base,public QGraphicsItem {
	ToolType style;
	protected:
	qint8 						mode;
	QPointF 					firstPoint;
	QPointF 					secondPoint;
	public:	
	enum { Type = 600 };
	QJsonObject to_JSON() const;
	void swapPoint();
	void sortPoint();
	Myline(MainWindow* mw, QPointF);
	QRectF boundingRect() const override;
	void changefirstPointCoord(QPointF);
	void changesecondPointCoord(QPointF, bool alwaysChange = false);
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void changePoints(QPointF);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void changeMode();
	void changeLength(double);
	void drag(QPointF, QPointF&);
	void setColorItem(QColor);
	QLineF line();
	bool isHoriLine() const;
	void setColor(QColor);
	qreal getLength() const;
	int type() const override { return Type; }
	//point_and_QGraphicsItem findObjectNearBy(QPointF);
	QPointF findObjectNearBy(QPointF);
	qint8 getMode();
	_3points getAllPoints();
	void setItemPen(qint64 , qint8 , qint64);
};

class Size : public Myline {
	QPointF 					lastPoint;
	QLineF 						main_line;
	public:
	enum { Type = 603 };
	Size(MainWindow* mw, QPointF);	
	void changePoints(QPointF);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	QPointF findObjectNearBy(QPointF);
	void changelastPointCoord(QPointF);
	void changeLength(double);	
	void changeMode();
	QLineF get_main_line();
	void setColor(QColor);
	bool isHoriLine() const;
	qreal getLength() const;
	int type() const override { return Type; }
	void drag(QPointF& , QPointF&);
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	_3points getAllPoints();
};

class Text: public item_base, public QGraphicsTextItem {	
	QPointF 					firstPoint;
	qreal 						fontSize = 72;
public:
	enum { Type = 602 };
	Text(MainWindow* , QPointF );
	void changePoints(QPointF cursorCoord);	
	QJsonObject to_JSON() const;
	int type() const;
	void changeSize(qreal);
	qreal getSize();
	void setColor(QColor);
	void drag(QPointF&);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override ;
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	_3points getAllPoints();	
}; // class text
	
