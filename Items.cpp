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

 QPointF myline::findObjectNearBy(QPointF currentMouseCord, View * view, myline * myln = nullptr){
	QList<QGraphicsItem *> 			itemList = view->items();	
	int 							sz=itemList.size();	
	bool 							loca; //location of line (vert or hori)				
	QLineF 							line;
	bool 							locaLine;
	qint8 							minDistance=100;
	QPointF 						res;		
		for (qsizetype i = 0; i < sz; i++) { 
			QGraphicsItem* item=itemList.at(i);		
			if (item->type()==600 && item!=myln) {	// is item line ?
			qDebug()<<item;
			QGraphicsLineItem * tmpLine = static_cast<QGraphicsLineItem *>( item );	
				QLineF linecoord = tmpLine->line();
					loca = (linecoord.x1()==linecoord.x2());
					if (loca && minDistance>=abs(currentMouseCord.x()-linecoord.x1()) && ((currentMouseCord.x()+10>=linecoord.x1() && currentMouseCord.x()<=linecoord.x1()) || (currentMouseCord.x()-10<=linecoord.x1() && currentMouseCord.x()>=linecoord.x1())) ) {
						minDistance = abs(currentMouseCord.x()-linecoord.x1()); 
						line=linecoord; 
						locaLine=loca;
						} 
					if (!loca && abs(currentMouseCord.y()-linecoord.y1()) && ((currentMouseCord.y()+10>=linecoord.y1() && currentMouseCord.y()<=linecoord.y1()) || (currentMouseCord.y()-10<=linecoord.y1() && currentMouseCord.y()>=linecoord.y1())) )  {						
						minDistance = abs(currentMouseCord.y()-linecoord.y1()); 
						line=linecoord; locaLine=loca; }				
			}				
		}
		
	if (minDistance==100) res=QPointF(currentMouseCord.x(),currentMouseCord.y()); else
	if (locaLine) res= QPointF(line.x1(),currentMouseCord.y() ); else res= QPointF(currentMouseCord.x(),line.y1() );
	return res;
}
