#include "Items.hpp"

item_base::item_base(){};

QPen item_base::get_pen(qint8 style , qint8 width){
	QPen pen;
		switch (style) {
			case 1: pen.setStyle(Qt::SolidLine);break;
			case 2: pen.setStyle(Qt::DashLine);break;
			case 3: pen.setStyle(Qt::DotLine);break;
			case 4: pen.setStyle(Qt::DashDotLine);break;						
		}
		pen.setWidth(width);
		return pen;
};
//myline::myline(QLineF coord, int style,int width , QGraphicsLineItem* parent_):	
//	QGraphicsLineItem(coord,parent_){
//	setPen(get_pen(style , width));	
//};
	//QPointF myline::getMouseCoord(){}
	myline::myline(QPointF firstPoint_):mode(0),item_base(),firstPoint(firstPoint_){}	
	void myline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {		
	qDebug()<<mode	;
		switch  ( mode ) {
			case  0:	
				
				painter->drawEllipse(firstPoint.x(),firstPoint.y() ,5,5);
				
			break;

			
		}
	};	
    QRectF myline::boundingRect() const{        
	        return QRectF(0, 0,1800,2800);
	}

void myline::changefirstPointCoord(QPointF point_) {
	//qDebug()<<"before: "<<firstPoint<< " "<<mode;
	if (mode==0) firstPoint=point_;
	update();
	//qDebug()<<"after: "<<firstPoint<< " "<<mode;
	
}
QJsonObject myline::to_JSON() const {
	QJsonObject json;
	/*QLineF line=this->line();
			json["type"] = type();
			json["style"] = static_cast<int>(pen().style());
			json["width"] = pen().width();
			json["x1"]=line.x1();
			json["y1"]=line.y1();
			json["x2"]=line.x2();
			json["y2"]=line.y2();*/
		return json;
};
