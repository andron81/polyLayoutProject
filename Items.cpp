#include "Items.hpp"
#include "View.hpp"
#include "AppSettings.hpp"
#include <QJsonArray>
	item_base::item_base(MainWindow* mw_):mw(mw_) {	}
	
MainWindow*	item_base::getmw(){
	return mw;
}	
QPen item_base::getPen(){
	return pen;
}

void Text::changeSize(qreal sz){
	fontSize = sz;
	setFont(QFont("Arial", sz, QFont::Normal));
	update();
}
qreal Text::getSize(){
	return fontSize ;
}
void itemOperations::fillCanvas(MainWindow * mw, QGraphicsScene * scene, QJsonDocument doc){					
			QJsonObject obj = doc.object();			
			QJsonArray items = obj["texts"].toArray();
			for (auto v : items) {
				QJsonObject element = v.toObject();
					switch (element["type"].toInt())	 {
						case 602: {
					Text* itm = new Text(mw, QPointF(element["x"].toInt(),element["y"].toInt()));
					itm->setPlainText( element["text"].toString() );
					itm->setRotation(element["a"].toInt());
					itm->changeSize(element["s"].toInt());
					scene->addItem(itm);	
						
						break;
						}
						/* case 6: {
					vd::items::myline* itm = new vd::items::myline(
					this,
					element["x1"].toInt(),
					element["y1"].toInt(),
					element["x2"].toInt(),
					element["y2"].toInt(), 
					element["style"].toInt(), 
					element["width"].toInt()
					);
					m_view->scene()->addItem(itm);
						break;
						}

						case -500: {
					vd::items::size* itm = new vd::items::size(
					this, 
					element["x1"].toInt(), 
					element["y1"].toInt(),
					element["x2"].toInt(), 
					element["y2"].toInt(),
					element["x3"].toInt(), 
					element["y3"].toInt(),
					QLine(element["main_line_x1"].toInt(),
						  element["main_line_y1"].toInt(),
						  element["main_line_x2"].toInt(),
						  element["main_line_y2"].toInt()), 
					QPen());
					m_view->scene()->addItem(itm);
						break;
					} */
							
					}
			}	
			

	
}
_3points Myline::getAllPoints(){	
	return {firstPoint,secondPoint};
}
_3points Text::getAllPoints(){	
	return {firstPoint};
}
_3points Size::getAllPoints(){	
	return {firstPoint,secondPoint,lastPoint};
}
QJsonObject  itemOperations::to_JSON(QGraphicsItem * item){
	QJsonObject jsonResult;
	_3points itemPoints;
	 int type = item->type();
		switch (type){
			case 600:{				
				Myline* line = static_cast<Myline*>(item);
				itemPoints = line->getAllPoints();
				jsonResult["type"] = type;
				jsonResult["style"] = static_cast<int>(line->getPen().style());
				jsonResult["width"] = line->getPen().width();
				jsonResult["x1"]=itemPoints.firstPoint.x();
				jsonResult["y1"]=itemPoints.firstPoint.y();
				jsonResult["x2"]=itemPoints.secondPoint.x();
				jsonResult["y2"]=itemPoints.secondPoint.y();				
			break;
			}
			case 602:{
				Text* txt = static_cast<Text*>(item);
				itemPoints = txt->getAllPoints();				 		
				jsonResult["type"] = type;
				jsonResult["x"] = itemPoints.firstPoint.x();
				jsonResult["y"] = itemPoints.firstPoint.y();
				jsonResult["a"] = txt->rotation() ; //angle
				jsonResult["s"] = txt->getSize() ; //font size 
				jsonResult["text"] = txt->toPlainText();
			break;
			}
			case 603:{
				Size* sz = static_cast<Size*>(item);
				itemPoints = sz->getAllPoints();				 		
				jsonResult["type"] = type;
				jsonResult["x1"] = itemPoints.firstPoint.x();
				jsonResult["y1"] = itemPoints.firstPoint.y();
				jsonResult["x2"] = itemPoints.secondPoint.x();
				jsonResult["y2"] = itemPoints.secondPoint.y();
				jsonResult["x3"] = itemPoints.lastPoint.x();
				jsonResult["y4"] = itemPoints.lastPoint.y();

			break;
			}
		}
	return jsonResult;	
}
	

void itemOperations::setColor(QGraphicsItem * item , QColor Color) {
	switch (item->type()){
			case 600:{
				Myline* line = static_cast<Myline*>(item);
				line->setColor(Color);
				line->getmw()->geteditBlk().lineSizeEdit->setText(QString::number(line->getLength()));
				line->getmw()->geteditBlk().setVisible(EditBlockVisible::changeLength );				
			break;
			}
			case 603:{
				Size* sz = static_cast<Size*>(item);
				sz->setColor(Color);
				sz->getmw()->geteditBlk().lineSizeEdit->setText(QString::number(sz->getLength()));
				sz->getmw()->geteditBlk().setVisible(EditBlockVisible::changeLength );
			break;
			}
			case 602:{
				Text* txt = static_cast<Text*>(item);
				txt->setColor(Color);
				txt->getmw()->geteditBlk().lineTextEdit->setText(txt->toPlainText());
				txt->getmw()->geteditBlk().lineSizeEdit->setText(QString::number(txt->font().pointSize()));
				txt->getmw()->geteditBlk().setVisible(EditBlockVisible::changeText );
			break;							
			}
		}
	}
	void Myline::moveRight(){
		qint8 step = mw->getSettings()->getValue("moveStep").toInt();
		firstPoint.setX(firstPoint.x()+step);
		secondPoint.setX(secondPoint.x()+step);
			update();
	}
	void Myline::moveLeft(){
		qint8 step = mw->getSettings()->getValue("moveStep").toInt();
		firstPoint.setX(firstPoint.x()-step);
		secondPoint.setX(secondPoint.x()-step);
		update();
	}
	
	void Myline::moveUp(){
		qint8 step = mw->getSettings()->getValue("moveStep").toInt();
		firstPoint.setY(firstPoint.y()+step);
		secondPoint.setY(secondPoint.y()+step);
			update();
	}
	void Myline::moveDown(){		
		qint8 step = mw->getSettings()->getValue("moveStep").toInt();		
		firstPoint.setY(firstPoint.y()	-step);
		secondPoint.setY(secondPoint.y()-step);
			update();
	}

	

	
	void itemOperations::move(QGraphicsItem * item, int movement){
		switch (item->type()){
			case 600:{
			Myline* line = static_cast<Myline*>(item);
			if (movement==0) line->moveLeft(); else
			if (movement==1) line->moveRight(); else
			if (movement==2) line->moveUp(); else			
			if (movement==3) line->moveDown(); 
			break;
			}
			case 602:{
			Text* text = static_cast<Text*>(item);
			if (movement==0) text->moveLeft(); else
			if (movement==1) text->moveRight(); else
			if (movement==2) text->moveUp(); else			
			if (movement==3) text->moveDown(); 
			break;
			}
			case 603:{
			Size* sz = static_cast<Size*>(item);
			if (movement==0) sz->moveLeft(); else
			if (movement==1) sz->moveRight(); else
			if (movement==2) sz->moveUp(); else			
			if (movement==3) sz->moveDown(); 
			break;
			}
				
		}
		
	}

	QPen item_base::getPen(ToolType style , qint8 width , qint64 color ){
	QPen pen;
		switch (style) {
			case ToolType::line_solid: pen.setStyle(Qt::SolidLine);break;
			case ToolType::line_dashed: pen.setStyle(Qt::DashLine);break;
		}
		pen.setWidth(width);
		pen.setColor(color);
		return pen;
	};

	Myline::Myline(MainWindow* mw_,QPointF firstPoint_):mode(0),item_base(mw_),firstPoint(firstPoint_),
	style(style){
		
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
	
	void Myline::changesecondPointCoord(QPointF point_ , bool alwaysChange) {
	if (mode==1 || alwaysChange) secondPoint=point_;
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
		
			if (abs(firstPoint.x()-result.x()) < abs(firstPoint.y() - result.y())) 
				{ result.setX(firstPoint.x()); loca=true; }
			else 
				{result.setY(firstPoint.y()); loca=false; }

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
		QLineF Size::get_main_line() {return main_line;}
		void Myline::changeLength(double len) {
			
			if (isHoriLine()) 
				changesecondPointCoord(QPointF(firstPoint.x()+len,secondPoint.y()) , true);				
			
			else 
				changesecondPointCoord(QPointF(firstPoint.x(),secondPoint.y()+len) , true);				
			
		}
		void Myline::drag(QPointF mouseCord , QPointF& 	currentItemPoint){
				qreal deltaX=currentItemPoint.x()-mouseCord.x();
				qreal deltaY=currentItemPoint.y()-mouseCord.y();;
					firstPoint =  {firstPoint.x() -deltaX,firstPoint.y() -deltaY };
					secondPoint = {secondPoint.x()-deltaX,secondPoint.y()-deltaY };
								currentItemPoint = mouseCord;
			update();
			
		}
		
		void Size::drag(QPointF& mouseCord , QPointF& 	currentItemPoint){			
				qreal deltaX=currentItemPoint.x()-mouseCord.x();
				qreal deltaY=currentItemPoint.y()-mouseCord.y();;
					firstPoint =  {firstPoint.x() -deltaX,firstPoint.y() -deltaY };
					secondPoint = {secondPoint.x()-deltaX,secondPoint.y()-deltaY };
					lastPoint = {lastPoint.x()-deltaX,lastPoint.y()-deltaY };				
								currentItemPoint = mouseCord;
			update();			
		}
		void Text::drag(QPointF& mouseCord ){			
			setPos(mouseCord);
					firstPoint =  mouseCord;

			update();			
		}
		
		void Size::changeLength(double len) {
			
			if (isHoriLine()) 
				changesecondPointCoord(QPointF(firstPoint.x()+len,secondPoint.y()) , true);				
			
			else 
				changesecondPointCoord(QPointF(firstPoint.x(),secondPoint.y()+len) , true);				
			
		}		
		void Myline::changePoints(QPointF cursorCoord){			
			cursorCoord=findObjectNearBy(cursorCoord);					
			if (mode==0) changefirstPointCoord(cursorCoord);
				else
			if (mode==1) changesecondPointCoord(cursorCoord);
		}

		Size::Size(MainWindow* mw, QPointF firstPoint_):Myline(mw , firstPoint_){			
		}	
		
		void Myline::swapPoint(){
		QPointF tmp;
		tmp=secondPoint;
		secondPoint = firstPoint;
		firstPoint = tmp;
		}
		void Myline::sortPoint() {
		if ((firstPoint.y()==secondPoint.y() and firstPoint.x()>secondPoint.x()) || (firstPoint.x()==secondPoint.x() 
			and firstPoint.y()>secondPoint.y()) ||
			(firstPoint.y()>secondPoint.y() && firstPoint.x()>secondPoint.x())
			) 
		
			swapPoint(); 
		}

		
		
		void Size::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
		
		switch  ( mode ) {
			case  0:
				painter->drawEllipse(firstPoint.x(),firstPoint.y() ,5,5);
				pen = getPen(mw->getTool(),
							 mw->getSettings()->getValue("lineWidth").toInt(),
							 mw->getSettings()->getValue("lineColorDefault").toString().toInt(0, 16));
			break;
			case  1:						
				painter->drawEllipse(firstPoint.x(),firstPoint.y() ,5,5);
				painter->drawEllipse(secondPoint.x(),secondPoint.y() ,5,5);
				//painter->setPen(pen);
				//painter->drawLine(firstPoint.x(),firstPoint.y() ,secondPoint.x(),secondPoint.y());
			break;
			case  2:case  3:
				sortPoint() ;
				if (mode<3) {
					painter->drawEllipse(firstPoint.x(),firstPoint.y() ,5,5);
					painter->drawEllipse(secondPoint.x(),secondPoint.y() ,5,5);	
					painter->drawEllipse(lastPoint.x(),lastPoint.y() ,5,5);
				}
				painter->setPen(pen);
				if (firstPoint.y()==secondPoint.y() ) {
					if (abs(secondPoint.x()-firstPoint.x())<40) {
						painter->drawLine(firstPoint.x()-15,lastPoint.y(),secondPoint.x()+15,lastPoint.y()); 
						main_line=QLineF(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());//set mainline 
						painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()-8,lastPoint.y()+8);
						painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()-8,lastPoint.y()-8);				
						painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()+8,lastPoint.y()-8);
						painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()+8,lastPoint.y()+8);
					}
					else {
						painter->drawLine(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y()); //!
						main_line=QLineF(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());//set mainline 
						painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()+8,lastPoint.y()+8);
						painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()+8,lastPoint.y()-8);				
						painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()-8,lastPoint.y()+8);
						painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()-8,lastPoint.y()-8);
					}
						if (lastPoint.y()>secondPoint.y())
							painter->drawText(QPoint(firstPoint.x()+abs(secondPoint.x()-firstPoint.x())/2,lastPoint.y()	+15),QString::number(abs(secondPoint.x()-firstPoint.x())));
								else 
							painter->drawText(QPoint(firstPoint.x()+abs(secondPoint.x()-firstPoint.x())/2,lastPoint.y()	-15),QString::number(abs(secondPoint.x()-firstPoint.x())));							
							painter->drawLine(firstPoint.x(),secondPoint.y(),firstPoint.x(),lastPoint.y());
							painter->drawLine(secondPoint.x(),secondPoint.y(),secondPoint.x(),lastPoint.y());
					
				} //firstPoint.y()==secondPoint.y()
					else 
				if (firstPoint.x()==secondPoint.x()) {				
				if (abs(firstPoint.y()-secondPoint.y())<40 && firstPoint.y()<secondPoint.y()) {
				painter->drawLine(lastPoint.x(),firstPoint.y()-15,lastPoint.x(),secondPoint.y()+15); //!
				main_line=QLineF(lastPoint.x(),firstPoint.y(),lastPoint.x(),secondPoint.y());//set mainline 
				painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()-8,firstPoint.y()-8);
				painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()+8,firstPoint.y()-8);
				painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()-8,secondPoint.y()+8);
				painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()+8,secondPoint.y()+8);
				} else {
					painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x(),secondPoint.y());   //!
					main_line=QLineF(lastPoint.x(),firstPoint.y(),lastPoint.x(),secondPoint.y());//set mainline  
				painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()+8,firstPoint.y()+8);
				painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()-8,firstPoint.y()+8);
				painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()+8,secondPoint.y()-8);
				painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()-8,secondPoint.y()-8);
				}
				

				if (lastPoint.x()>secondPoint.x())
				painter->drawText(QPoint(lastPoint.x()+15,firstPoint.y()+abs(secondPoint.y()-firstPoint.y())/2),QString::number(abs(secondPoint.y()-firstPoint.y())));
				else 
					painter->drawText(QPoint(lastPoint.x()-25,firstPoint.y()+abs(secondPoint.y()-firstPoint.y())/2),QString::number(abs(secondPoint.y()-firstPoint.y())));
					painter->drawLine(firstPoint.x(),firstPoint.y(),lastPoint.x(),firstPoint.y());
					painter->drawLine(secondPoint.x(),secondPoint.y(),lastPoint.x(),secondPoint.y());
				}
				else {
						if (lastPoint.x()>=firstPoint.x() && lastPoint.x()<=secondPoint.x() && lastPoint.y()>=firstPoint.y() && lastPoint.y()<=secondPoint.y()) {
						
						 if (abs(secondPoint.x()-firstPoint.x())>40){							 
							painter->drawLine(firstPoint.x(),firstPoint.y(),firstPoint.x(),lastPoint.y()+15);
							painter->drawLine(secondPoint.x(),secondPoint.y(),secondPoint.x(),lastPoint.y()-15);
							painter->drawLine(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());
							main_line=QLineF(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());//set mainline 
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()+8,lastPoint.y()-8);
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()+8,lastPoint.y()+8);							
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()-8,lastPoint.y()-8);
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()-8,lastPoint.y()+8);
							painter->drawText(QPoint(firstPoint.x()+abs(secondPoint.x()-firstPoint.x())/2,lastPoint.y()	+15),QString::number(abs(secondPoint.x()-firstPoint.x())));
							}
							else{	
							painter->drawLine(firstPoint.x(),firstPoint.y(),firstPoint.x(),lastPoint.y()+15);
							painter->drawLine(secondPoint.x(),secondPoint.y(),secondPoint.x(),lastPoint.y()-15);
							painter->drawLine(firstPoint.x()-15,lastPoint.y(),secondPoint.x()+15,lastPoint.y());
							main_line=QLineF(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());//set mainline 
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()-8,lastPoint.y()+8);
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()-8,lastPoint.y()-8);							
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()+8,lastPoint.y()+8);
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()+8,lastPoint.y()-8);
							painter->drawText(QPoint(firstPoint.x()+abs(secondPoint.x()-firstPoint.x())/2,lastPoint.y()	+15),QString::number(abs(secondPoint.x()-firstPoint.x())));								
							}
						}
						else
						if (lastPoint.x()>=firstPoint.x() && lastPoint.x()>=secondPoint.x() && lastPoint.y()>=firstPoint.y() && lastPoint.y()<=secondPoint.y()) {
							if (abs(secondPoint.x()-firstPoint.x())>40){
							painter->drawLine(firstPoint.x(),firstPoint.y(),lastPoint.x(),firstPoint.y());
							painter->drawLine(secondPoint.x(),secondPoint.y(),lastPoint.x(),secondPoint.y());
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x(),secondPoint.y());
							main_line=QLineF(lastPoint.x(),firstPoint.y(),lastPoint.x(),secondPoint.y());//set mainline 
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()-8,firstPoint.y()+8);
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()+8,firstPoint.y()+8);
							
							painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()-8,secondPoint.y()-8);
							painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()+8,secondPoint.y()-8);
							painter->drawText(QPoint(lastPoint.x()+5,firstPoint.y()+abs(secondPoint.y()-firstPoint.y())/2),QString::number(abs(secondPoint.y()-firstPoint.y())));
							}
							else {
							painter->drawLine(firstPoint.x(),firstPoint.y(),lastPoint.x(),firstPoint.y());
							painter->drawLine(secondPoint.x(),secondPoint.y(),lastPoint.x(),secondPoint.y());
							painter->drawLine(lastPoint.x(),firstPoint.y()-15,lastPoint.x(),secondPoint.y()+15);
							main_line=QLineF(lastPoint.x(),firstPoint.y(),lastPoint.x(),secondPoint.y());//set mainline 
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()-8,firstPoint.y()-8);
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()+8,firstPoint.y()-8);
							
							painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()-8,secondPoint.y()+8);
							painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()+8,secondPoint.y()+8);
							painter->drawText(QPoint(lastPoint.x(),firstPoint.y()+abs(secondPoint.y()-firstPoint.y())/2),QString::number(abs(secondPoint.y()-firstPoint.y())));
							}
							
							}
					else
							if (lastPoint.x()<firstPoint.x() && lastPoint.x()<secondPoint.x() && lastPoint.y()>=firstPoint.y() && lastPoint.y()<=secondPoint.y()) {
							if (abs(secondPoint.x()-firstPoint.x())>40){
							painter->drawLine(firstPoint.x(),firstPoint.y(),lastPoint.x(),firstPoint.y());
							painter->drawLine(secondPoint.x(),secondPoint.y(),lastPoint.x(),secondPoint.y());
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x(),secondPoint.y());
							main_line=QLineF(lastPoint.x(),firstPoint.y(),lastPoint.x(),secondPoint.y());
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()-8,firstPoint.y()+8);
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()+8,firstPoint.y()+8);
							
							painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()-8,secondPoint.y()-8);
							painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()+8,secondPoint.y()-8);
							painter->drawText(QPoint(lastPoint.x()+5,firstPoint.y()+abs(secondPoint.y()-firstPoint.y())/2),QString::number(abs(secondPoint.y()-firstPoint.y())));
							}
							else {
							painter->drawLine(firstPoint.x(),firstPoint.y(),lastPoint.x(),firstPoint.y());
							painter->drawLine(secondPoint.x(),secondPoint.y(),lastPoint.x(),secondPoint.y());
							painter->drawLine(lastPoint.x(),firstPoint.y()-15,lastPoint.x(),secondPoint.y()+15);
							main_line=QLineF(lastPoint.x(),firstPoint.y(),lastPoint.x(),secondPoint.y());
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()-8,firstPoint.y()-8);
							painter->drawLine(lastPoint.x(),firstPoint.y(),lastPoint.x()+8,firstPoint.y()-8);
							
							painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()-8,secondPoint.y()+8);
							painter->drawLine(lastPoint.x(),secondPoint.y(),lastPoint.x()+8,secondPoint.y()+8);
							painter->drawText(QPoint(lastPoint.x(),firstPoint.y()+abs(secondPoint.y()-firstPoint.y())/2),QString::number(abs(secondPoint.y()-firstPoint.y())));
							}
							
							}
					else
							if ((lastPoint.y()>firstPoint.y() && lastPoint.y()>secondPoint.y()) ) {
							if (abs(secondPoint.x()-firstPoint.x())>40){
							painter->drawLine(firstPoint.x(),firstPoint.y(),firstPoint.x(),lastPoint.y()+8);
							painter->drawLine(secondPoint.x(),secondPoint.y(),secondPoint.x(),lastPoint.y()+8);
							
							painter->drawLine(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());
							main_line=QLineF(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()+8,lastPoint.y()+8);
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()+8,lastPoint.y()-8);
							
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()-8,lastPoint.y()+8);
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()-8,lastPoint.y()-8);
					
							painter->drawText(QPoint(firstPoint.x()+abs(secondPoint.x()-firstPoint.x())/2,lastPoint.y()+13),QString::number(abs(secondPoint.x()-firstPoint.x())));
							}
							else {

							painter->drawLine(firstPoint.x(),firstPoint.y(),firstPoint.x(),lastPoint.y()+8);
							painter->drawLine(secondPoint.x(),secondPoint.y(),secondPoint.x(),lastPoint.y()+8);
							
							painter->drawLine(firstPoint.x()-15,lastPoint.y(),secondPoint.x()+15,lastPoint.y());
							main_line=QLineF(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()-8,lastPoint.y()+8);
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()-8,lastPoint.y()-8);
							
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()+8,lastPoint.y()+8);
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()+8,lastPoint.y()-8);
					
							painter->drawText(QPoint(firstPoint.x()+abs(secondPoint.x()-firstPoint.x())/2,lastPoint.y()+13),QString::number(abs(secondPoint.x()-firstPoint.x())));


							}
							
							}
							else
							if ((lastPoint.y()<firstPoint.y() && lastPoint.y()<secondPoint.y()) ) {
							if (abs(secondPoint.x()-firstPoint.x())>40){
							painter->drawLine(firstPoint.x(),firstPoint.y(),firstPoint.x(),lastPoint.y()-8);
							painter->drawLine(secondPoint.x(),secondPoint.y(),secondPoint.x(),lastPoint.y()-8);
							
							painter->drawLine(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());
							main_line=QLineF(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()+8,lastPoint.y()+8);
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()+8,lastPoint.y()-8);
							
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()-8,lastPoint.y()+8);
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()-8,lastPoint.y()-8);
							painter->drawText(QPoint(firstPoint.x()+abs(secondPoint.x()-firstPoint.x())/2,lastPoint.y()-8),QString::number(abs(secondPoint.x()-firstPoint.x())));
							}
							else {
							painter->drawLine(firstPoint.x(),firstPoint.y(),firstPoint.x(),lastPoint.y()-8);
							painter->drawLine(secondPoint.x(),secondPoint.y(),secondPoint.x(),lastPoint.y()-8);

							painter->drawLine(firstPoint.x()-15,lastPoint.y(),secondPoint.x()+15,lastPoint.y());
							main_line=QLineF(firstPoint.x(),lastPoint.y(),secondPoint.x(),lastPoint.y());
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()-8,lastPoint.y()+8);
							painter->drawLine(firstPoint.x(),lastPoint.y(),firstPoint.x()-8,lastPoint.y()-8);
							
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()+8,lastPoint.y()+8);
							painter->drawLine(secondPoint.x(),lastPoint.y(),secondPoint.x()+8,lastPoint.y()-8);

							painter->drawText(QPoint(firstPoint.x()+abs(secondPoint.x()-firstPoint.x())/2,lastPoint.y()-8),QString::number(abs(secondPoint.x()-firstPoint.x())));
							}
							
							}							
							
							
		
				
				}
				
					
			break;
			
		}
	};
		void Size::changePoints(QPointF cursorCoord){								
			if (mode==0) {
				cursorCoord=findObjectNearBy(cursorCoord);		
				firstPoint = cursorCoord;
			}
				else
			if (mode==1) {
				cursorCoord=findObjectNearBy(cursorCoord);
				secondPoint = cursorCoord;
			}	else
			if (mode>1) {	
				lastPoint=cursorCoord;
			}
			update();
			
		}
		
			
	QPointF Size::findObjectNearBy(QPointF Point_) {
		int 							step = 10;
		QPointF 						result = Point_;
		bool 							loca; //location of line (vert or hori)		
		QList<QGraphicsItem *> 			itemList = scene()->views()[0]->items();
		int 							sz=itemList.size();
			//qDebug()<<"firstPoint="<<firstPoint<<" Point_="<<Point_;
			//if (abs(firstPoint.x()-result.x()) < abs(firstPoint.y() - result.y())) 				
			//		loca=true;
			//else 				
			//		loca=false; 
		
			
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
				if (Point_.x()+step >=linecoord.x1() && Point_.x()<=linecoord.x1() //&& !loca 
					&& (linecoord.x1()==linecoord.x2() ) 
					&& Point_.y()>=minY
					&& Point_.y()<=maxY  )  
					{					
					result.setX(linecoord.x1()); break;
					} else
				if (Point_.x()-step <=linecoord.x1() && Point_.x()>=linecoord.x1() //&& !loca 
					&& (linecoord.x1()==linecoord.x2()) 
					&& Point_.y()>=minY
					&& Point_.y()<=maxY)  
					{					
					result.setX(linecoord.x1());break;
					} 
					else
				if (Point_.y()+step >=linecoord.y1() && Point_.y()<=linecoord.y1() //&& loca 
					&& (linecoord.y1()==linecoord.y2())  
					&& Point_.x()>=minX
					&& Point_.x()<=maxX) 
					{					
					result.setY(linecoord.y1());break;
					} 
					else
				if (Point_.y()-step <=linecoord.y1() && Point_.y()>=linecoord.y1() //&& loca 
					&& (linecoord.y1()==linecoord.y2()) 
					&& Point_.x()>=minX
					&& Point_.x()<=maxX)  
					{					
					result.setY(linecoord.y1()); break;}
					} 
				} //for
				//if (!(result.x()==firstPoint.x() || result.y()==firstPoint.y()))  
					//	result = Point_;			
								//qDebug()<<"firstPoint2="<<firstPoint<<" Point_="<<Point_;
				return result;
		}

	void Size::changeMode() {        
			if (mode<4) mode++;	
			update();
	}
	void Size::changelastPointCoord(QPointF Point_){
			lastPoint=Point_;
			update();
	}
	
	Text::Text(MainWindow* mw_, QPointF firstPoint_):	
			item_base(mw_){				
				setDefaultTextColor(pen.color() );
			setPos( firstPoint_ );
			setFont( QFont( "Arial", 72) );
			setPlainText( "Текст" );
		
	}
	
	void Text::changePoints(QPointF cursorCoord){
			firstPoint=cursorCoord;
			setPos( cursorCoord );
			
	}
	
	void Size::moveLeft(){
			firstPoint={firstPoint.x()-1,firstPoint.y()};
			secondPoint={secondPoint.x()-1,secondPoint.y()};
			lastPoint={lastPoint.x()-1,lastPoint.y()};
			update();
			
	}
	void Size::moveRight(){
			firstPoint = {firstPoint.x()+1,firstPoint.y()};
			secondPoint = {secondPoint.x()+1,secondPoint.y()};
			lastPoint = {lastPoint.x()+1,lastPoint.y()};
			update();
	}
	void Size::moveUp(){
			firstPoint = {firstPoint.x(),firstPoint.y()+1};
			secondPoint = {secondPoint.x(),secondPoint.y()+1};
			lastPoint = {lastPoint.x(),lastPoint.y()+1};	
			update();
	}
	void Size::moveDown(){
			firstPoint = {firstPoint.x(),firstPoint.y()-1};
			secondPoint = {secondPoint.x(),secondPoint.y()-1};
			lastPoint = {lastPoint.x(),lastPoint.y()-1};	
			update();
	}
	void Text::moveLeft(){
		
		setPos( pos().x()-1,pos().y() );
		firstPoint = pos();
	}
	void Text::moveUp(){
		setPos( pos().x(),pos().y()+1 );
		firstPoint = pos();
	}
	void Text::moveDown(){
		setPos( pos().x(),pos().y()-1 );	
		firstPoint = pos();
	}
	void Text::moveRight(){
		setPos( pos().x()+1,pos().y() );
		firstPoint = pos();
	}
	int Text::type() const{
		return Type;
	}
	QJsonObject Text::to_JSON() const {
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
	
	bool Myline::isHoriLine() const  {
		return (firstPoint.y()==secondPoint.y());		
	}
	qreal Myline::getLength() const {
		if (isHoriLine()) return abs(firstPoint.x() - secondPoint.x());
			else return abs(firstPoint.y() - secondPoint.y());
	}
	
		bool Size::isHoriLine() const  {
		return main_line.y1()==main_line.y1();		
	}
	qreal Size::getLength() const {
		if (isHoriLine()) return abs(main_line.x1() - main_line.x2());
			else return abs(main_line.y1() - main_line.y2());
	}
	
	void Size::setColor(QColor clr_) {
		pen.setColor(clr_);update(); 
	}
	void Myline::setColor(QColor clr_) {pen.setColor(clr_);update();}
	void Text::setColor(QColor clr_) {setDefaultTextColor(clr_);pen.setColor(clr_);}
	
	void Text::mousePressEvent(QGraphicsSceneMouseEvent *event){
		
		//setDefaultTextColor(mw->getSettings()->getValue("lineColorDefault").toString().toInt(0, 16));		
	}


