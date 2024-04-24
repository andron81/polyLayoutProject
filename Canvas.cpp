#include "MainWindow.hpp"
#include "Canvas.hpp"
#include "View.hpp"
#include "Items.hpp"
#include "math.h"

	
	Canvas::Canvas(View* view_ ):view(view_)  {
		setFlag( QGraphicsItem::ItemStacksBehindParent );
		setPen( Qt::NoPen );
		setBrush( QColor( 255, 255, 255 ) );
	
		QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect;
		effect->setBlurRadius( 32.f );
		effect->setOffset( 0 );
		effect->setColor( QColor( 64, 64, 64 ) );
		setGraphicsEffect( effect );
		setFiltersChildEvents(true);				
	}
	
	QPointF Canvas::getTopLeft() const  {
		return rect().topLeft();
	}
	
	QPointF Canvas::getBottomRight() const {
		return rect().bottomRight();
	}
	
	QSize Canvas::getSize() const{
		return rect().size().toSize();
	}

	void Canvas::setSize( const QSize & size ) {
		int w = size.width();
		int h = size.height();
		setRect( -w / 2, -h / 2, w, h );
	}

	void Canvas::mouseLeaveEvent(){
		isMouseInsideCanvas=false;
		if (currentItem && getTool()!=ToolType::edit) 
			eraseCurrentItem();
	}

	void Canvas::eraseCurrentItem(){
		getView()->scene()->removeItem(currentItem); 
		delete(currentItem);
		currentItem=nullptr; 
	}
	void Canvas::clear(){
		QList<QGraphicsItem *> itemList = view->items();
		for(auto it = itemList.begin(); it != itemList.end()-1; ++it){			 
			view->scene()->removeItem( *it );
			delete (*it);
		}	
	}		
	
	void Canvas::mouseMoveEvent(QMouseEvent *event){
						isMouseInsideCanvas=true;
			QPointF 		cursorCoord = QPointF(getView()->mapToScene( event->pos() ).x(),
											 getView()->mapToScene( event->pos() ).y());
		MainWindow* MW = static_cast<MainWindow*>(getView()->parent()->parent());
		MW->setCoordLabel(QString::number(cursorCoord.x()) + ":"+ QString::number(cursorCoord.y()));
		switch (getTool()){
		case ToolType::line_solid: case ToolType::line_dashed:	{
			if (!currentItem) {
					currentItem = new Myline(MW, QPointF(cursorCoord.x(),cursorCoord.y()));
					getView()->scene()->addItem(currentItem);
			}
			else static_cast<Myline*>(currentItem)->changePoints(cursorCoord);
		break;
		}
		case ToolType::edit:	{
			if (isMouseHold && currentItem) {
			
				if (currentItem->type()==600)
				static_cast<Myline*>(currentItem)->drag(cursorCoord, currentItemPoint);
				if (currentItem->type()==603)
				static_cast<Size*>(currentItem)->drag(cursorCoord, currentItemPoint);
				if (currentItem->type()==602)
				static_cast<Text*>(currentItem)->drag(cursorCoord);
			}	
			break;
		}		
		case ToolType::size:	{
			if (!currentItem) {
				
					currentItem = new Size(MW,
										 QPointF(cursorCoord.x(),cursorCoord.y()));
					getView()->scene()->addItem(currentItem);	
					
					
			}
			else 
					static_cast<Size*>(currentItem)->changePoints(cursorCoord);
		break;
			}
		case ToolType::text:	{
			
			if (!currentItem) {
				
					currentItem = new Text(
										 MW,
										 QPointF(cursorCoord.x(),cursorCoord.y())
										  );
					
					getView()->scene()->addItem(currentItem);					 
					

			}
			else
			static_cast<Text*>(currentItem)->changePoints(cursorCoord);
			
	
		break;		
			}
		
		}	
	}
	
	bool Canvas::isMouseInside() {return isMouseInsideCanvas;}
	
	View* Canvas::getView() const  {return view;}

	void Canvas::select(bool flag) {
		QColor tmpColor;
		if (currentItem && getTool()==ToolType::edit) {
			MainWindow* MW = static_cast<MainWindow*>(getView()->parent()->parent());
			if (flag) tmpColor=QColor(MW->getSettings()->getValue("lineColorSelected").toString().toInt(0, 16));
				else 
					  tmpColor=QColor(MW->getSettings()->getValue("lineColorDefault").toString().toInt(0, 16));
			itemOperations::setColor(currentItem , tmpColor);
			if (!flag)	{
			currentItem =nullptr;
			MW->geteditBlk().setVisible(EditBlockVisible::none );
			}
		}
	}
	ToolType Canvas::getTool(){return getView()->getTool();}
	void Canvas::mouseReleaseEvent(){
		isMouseHold=false;
	}
	void Canvas::mousePressEvent(QMouseEvent * event) {
		isMouseHold=true;
		QPointF 		mouseCoord = QPointF(getView()->mapToScene( event->pos() ).x(),
											 getView()->mapToScene( event->pos() ).y());	
			switch (getTool()) {
				case ToolType::line_solid:
				case ToolType::line_dashed:{
					Myline* line	= static_cast<Myline*>(currentItem);						
					line->changeMode();
					line->changesecondPointCoord(mouseCoord,false);							
					if (line->getMode()>1) {currentItem=nullptr;}
					break;
				}
				case ToolType::size: {					
					Size* size	= static_cast<Size*>(currentItem);						
					size->changeMode();
					switch (size->getMode()){
					case 1: size->changesecondPointCoord(mouseCoord); break;
					case 2: size->changelastPointCoord(mouseCoord); break;		
					case 3: currentItem=nullptr;  break;		
					}
					break;
				}
				case ToolType::text: {
					currentItem=nullptr;	
					break;
				}
				case ToolType::edit: {
					select(false);
					FindNearbyItem(mouseCoord);
					if (currentItem) select(true);																	
					break;
				}
								
			}
		}

	void Canvas::FindNearbyItem(const QPointF& mouseCoord) {
				QList<QGraphicsItem *> itemList = view->items();
				int sz=itemList.size();	
				bool loca; //location of line (vert or hori)				
				QLineF line;
				bool locaLine;
				qint8 minDistance=100;			
			QGraphicsItem * result{nullptr}; 
			QLineF linecoord;
			
		for (qsizetype i = 0; i < sz; i++) { 
			QGraphicsItem* item=itemList.at(i);	
			if (item->type()==static_cast<int>(ToolType::text)) {	// is item a txt ?			
				Text* tmpText = static_cast<Text*>( item );
				qDebug()<<"x"<<(qSin(tmpText->rotation())*((tmpText->getAllPoints()).firstPoint).x());
				qDebug()<<"y"<<(qCos(tmpText->rotation())*((tmpText->getAllPoints()).firstPoint).y());
				
				QRectF r = tmpText->mapToScene(tmpText->boundingRect()).boundingRect();
				QSizeF sz = r.size();
				qreal w = sz.width();
				qreal h = sz.height();
					if (mouseCoord.x()>tmpText->pos().x() && mouseCoord.x()<tmpText->pos().x()+w &&
						mouseCoord.y()>tmpText->pos().y() && mouseCoord.y()<tmpText->pos().y()+h
					) {result = item;
						minDistance=-1;}
	
				
			} else
			if (item->type()==static_cast<int>(ToolType::line_solid )) {	// is item a line ?
			 Myline* tmpLine = static_cast<Myline *>( item );	
				linecoord = tmpLine->line();
				linecoord.setP1(linecoord.p1() +item->pos());
				linecoord.setP2(linecoord.p2() +item->pos());
					loca = (linecoord.x1()==linecoord.x2());
					if (loca && minDistance>=abs(mouseCoord.x()-linecoord.x1()) && ((mouseCoord.x()+10>=linecoord.x1() && mouseCoord.x()<=linecoord.x1()) || (mouseCoord.x()-10<=linecoord.x1() && mouseCoord.x()>=linecoord.x1())) ) {
						minDistance = abs(mouseCoord.x()-linecoord.x1()); 
						result= item;						
						locaLine=loca;
						} 
					if (!loca && abs(mouseCoord.y()-linecoord.y1()) && ((mouseCoord.y()+10>=linecoord.y1() && mouseCoord.y()<=linecoord.y1()) || (mouseCoord.y()-10<=linecoord.y1() && mouseCoord.y()>=linecoord.y1())) )  {						
						minDistance = abs(mouseCoord.y()-linecoord.y1());
						result = item;						 
						locaLine=loca; 
						}				
			} else
			if (item->type()==static_cast<int>(ToolType::size)) {	// is item a size element ?	
				Size * tmpSize = static_cast<Size *>( item );	
					QLineF linecoord = tmpSize->get_main_line();
						loca = (linecoord.x1()==linecoord.x2());
						
					if (loca && minDistance>=abs(mouseCoord.x()-linecoord.x1()) && ((mouseCoord.x()+10>=linecoord.x1() && mouseCoord.x()<=linecoord.x1()) || (mouseCoord.x()-10<=linecoord.x1() && mouseCoord.x()>=linecoord.x1())) ) {
						minDistance = abs(mouseCoord.x()-linecoord.x1()); 
						result = item;
						}
						else 
					if (!loca && minDistance>=abs(mouseCoord.y()-linecoord.y1()) && ((mouseCoord.y()+10>=linecoord.y1() && mouseCoord.y()<=linecoord.y1()) || (mouseCoord.y()-10<=linecoord.y1() && mouseCoord.y()>=linecoord.y1())) ){
						minDistance = abs(mouseCoord.y()-linecoord.y1()); 
						result = item;
						locaLine=loca;
	
						}
					
			}		
				
	}
	if (minDistance==100) currentItem = nullptr; else {currentItem = result; 	currentItemPoint = mouseCoord; }	
	}		
	
	QGraphicsItem* Canvas::	getCurrentItem() {
		return  currentItem;		
	}
