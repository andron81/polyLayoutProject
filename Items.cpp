#include "Items.hpp"
#include "View.hpp"
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
			qDebug()<<mode<<" "<<secondPoint.x() << ","<<secondPoint.y()			;		
		switch  ( mode ) {
			case  0:					
				painter->drawEllipse(firstPoint.x(),firstPoint.y() ,5,5);				
			break;
			case  1:
				painter->drawLine(firstPoint.x(),firstPoint.y() ,secondPoint.x(),secondPoint.y());
				painter->drawEllipse(secondPoint.x(),secondPoint.y() ,5,5);
			break;
			case  2:
				painter->drawLine(firstPoint.x(),firstPoint.y() ,secondPoint.x(),secondPoint.y());				
			break;

			
		}
	};	
    QRectF myline::boundingRect() const{        
	        return QRectF(0, 0,1800,2800);
	}
	void myline::changeMode() {        
			mode++;
			update();
	}
	qint8 myline::getMode(){return mode;}
	void myline::changefirstPointCoord(QPointF point_) {
	if (mode==0) firstPoint=point_;
	update();
	}
	void myline::changesecondPointCoord(QPointF point_) {
	if (mode==1) secondPoint=point_;
	update();
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
QLineF myline::line() {return {firstPoint,secondPoint};}


//////////////////////////
	
	QPointF myline::findObjectNearBy(QPointF secondPoint_) {
		int 							step = 10;
		QPointF 						result = secondPoint_;
		bool 							loca; //location of line (vert or hori)		
		QList<QGraphicsItem *> 			itemList = scene()->views()[0]->items();
		int 							sz=itemList.size();			
		if (abs(firstPoint.x()-result.x()) < abs(firstPoint.y() - 
			result.y())) {result.setX(firstPoint.x()); loca=true; }
			else {result.setY(firstPoint.y()); loca=false; }		
		/////
		for (qsizetype i = 1; i < sz; i++) { 
			QGraphicsItem* item=itemList.at(i);							
			if (item->type()==600) {							
				myline * tmpLine = static_cast<myline *>( item );				
				QLineF linecoord = tmpLine->line();
				linecoord.setP1(linecoord.p1() +item->pos());
				linecoord.setP2(linecoord.p2() +item->pos());
				qreal minX = std::min(linecoord.x1(),linecoord.x2());
				qreal minY = std::min(linecoord.y1(),linecoord.y2());
				qreal maxX = std::max(linecoord.x1(),linecoord.x2());
				qreal maxY = std::max(linecoord.y1(),linecoord.y2());
				if (secondPoint_.x()+step >=linecoord.x1() && secondPoint_.x()<=linecoord.x1() && !loca && (linecoord.x1()==linecoord.x2() ) 
					&& secondPoint_.y()>=minY
					&& secondPoint_.y()<=maxY  )  
					{					
					result.setX(linecoord.x1()); break;
					} else
				if (secondPoint_.x()-step <=linecoord.x1() && secondPoint_.x()>=linecoord.x1() && !loca && (linecoord.x1()==linecoord.x2()) 
					&& secondPoint_.y()>=minY
					&& secondPoint_.y()<=maxY)  
					{					
					result.setX(linecoord.x1()); break;
					} 
					else
				if (secondPoint_.y()+step >=linecoord.y1() && secondPoint_.y()<=linecoord.y1() && loca && (linecoord.y1()==linecoord.y2())  
					&& secondPoint_.x()>=minX
					&& secondPoint_.x()<=maxX) 
					{					
					result.setY(linecoord.y1());break;
					} 
					else
				if (secondPoint_.y()-step <=linecoord.y1() && secondPoint_.y()>=linecoord.y1() && loca && (linecoord.y1()==linecoord.y2()) 
					&& secondPoint_.x()>=minX
					&& secondPoint_.x()<=maxX)  
					{					
					result.setY(linecoord.y1()); break;}
					} 
				} //for
				if (!(result.x()==firstPoint.x() || result.y()==firstPoint.y()))  result = secondPoint_;			
				return result;
		}


