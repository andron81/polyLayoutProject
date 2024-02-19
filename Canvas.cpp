#include "Canvas.hpp"
#include "View.hpp"
#include "Items.hpp"
#include "MainWindow.hpp"
	
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
		getView()->getTool();		
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
		//setRect( , 0, 100, 100 );
	}

	void Canvas::mouseLeaveEvent(){
		isMouseInsideCanvas=false;
		if (currentItem) {
		getView()->scene()->removeItem(currentItem); 
		delete(currentItem);
		currentItem=nullptr;
		}	
	}
	toolType Canvas::getTool(){
	MainWindow*      		MW 		   = static_cast<MainWindow*>(getView()->parent()->parent());
	return MW->getTool() ;
	}
	
	void Canvas::mouseMoveEvent(QMouseEvent *event){
						isMouseInsideCanvas=true;
		QPointF 		cursorCoord = QPointF(getView()->mapToScene( event->pos() ).x(),
											 getView()->mapToScene( event->pos() ).y());												 
		qDebug()<<"mouseMoveEvent";
		switch (getTool()) {
			case toolType::line_solid:
				{
					
				if (!currentItem) {			
					currentItem = new myline(QPointF(cursorCoord.x(),cursorCoord.y()));							
					getView()->scene()->addItem(currentItem);
				}			
				else { 
					myline* line	= static_cast<myline*>(currentItem);
					qDebug()<<"before "<<cursorCoord;
					cursorCoord = myline::findObjectNearBy(cursorCoord,getView(),static_cast<myline*>(currentItem));
					qDebug()<<"before "<<cursorCoord;
					if (line->getMode()==0) line->changefirstPointCoord(cursorCoord);
						else
					if (line->getMode()==1) line->changesecondPointCoord(cursorCoord);			
				}	
			}	
		}	
	}
	
	bool Canvas::isMouseInside() {return isMouseInsideCanvas;}
	
	View* Canvas::getView() const  {return view;}
	
	void Canvas::mousePressEvent(QMouseEvent * event) {
			qDebug()<<"mousePressEvent";		
			switch (getTool()) {
				case toolType::line_solid:{
					QPointF 		mouseCoord = QPointF(getView()->mapToScene( event->pos() ).x(),
														 getView()->mapToScene( event->pos() ).y());	
					myline* line	= static_cast<myline*>(currentItem);						
					line->changeMode();
					line->changesecondPointCoord(mouseCoord);				
					if (line->getMode()>1) currentItem=nullptr;					
					break;
				}			
			}	
		}

		
	
	
