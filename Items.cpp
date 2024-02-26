#include "Items.hpp"
#include "View.hpp"
#include "AppSettings.hpp"
	item_base::item_base(MainWindow* mw_):mw(mw_) {	}

	QPen item_base::getPen(toolType style , qint8 width , qint64 color ){
	QPen pen;
		switch (style) {
			case toolType::line_solid: pen.setStyle(Qt::SolidLine);break;
			case toolType::line_dashed: pen.setStyle(Qt::DashLine);break;
		}
		pen.setWidth(width);
		pen.setColor(color);
		return pen;
	};

	Myline::Myline(MainWindow* mw_,QPointF firstPoint_):mode(0),item_base(mw_),firstPoint(firstPoint_){
		
		}	
	void Myline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
		
		switch  ( mode ) {
			case  0:
				painter->drawEllipse(firstPoint.x(),firstPoint.y() ,5,5);
				pen = getPen(mw->getTool(),
							 mw->getSettings()->getValue("lineWidth").toInt(),
							 mw->getSettings()->getValue("lineColorDefault").toString().toInt(0, 16));
			break;
			case  1:			
				painter->drawEllipse(secondPoint.x(),secondPoint.y() ,5,5);
				painter->setPen(pen);
				painter->drawLine(firstPoint.x(),firstPoint.y() ,secondPoint.x(),secondPoint.y());
				

			break;
			case  2:
				painter->setPen(pen);
				painter->drawLine(firstPoint.x(),firstPoint.y() ,secondPoint.x(),secondPoint.y());	
		  
			break;

			
		}
	};	
    QRectF Myline::boundingRect() const{        
	        return QRectF(0, 0,1800,2800);
	}
	void Myline::changeMode() {        
			if (mode<3) mode++;
			update();
	}
	qint8 Myline::getMode(){return mode;}
	void Myline::changefirstPointCoord(QPointF point_) {
	if (mode==0) firstPoint=point_;
	update();
	}
	void Myline::changesecondPointCoord(QPointF point_) {
	if (mode==1) secondPoint=point_;
	update();
	}
	QJsonObject Myline::to_JSON() const {
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
QLineF Myline::line() {return {firstPoint,secondPoint};}


//////////////////////////
	
	QPointF Myline::findObjectNearBy(QPointF secondPoint_) {
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
				Myline * tmpLine = static_cast<Myline *>( item );				
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
		void Myline::changePoints(QPointF cursorCoord){			
			cursorCoord=findObjectNearBy(cursorCoord);					
			if (mode==0) changefirstPointCoord(cursorCoord);
				else
			if (mode==1) changesecondPointCoord(cursorCoord);
		}

		Size::Size(MainWindow* mw, QPointF firstPoint_):Myline(mw , firstPoint_){

		}	


