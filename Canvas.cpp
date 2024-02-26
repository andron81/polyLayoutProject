#include "MainWindow.hpp"
#include "Canvas.hpp"
#include "View.hpp"
#include "Items.hpp"

	
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
	/*toolType Canvas::getTool(){
	MainWindow*      		MW 		   = static_cast<MainWindow*>(getView()->parent()->parent());
	return MW->getTool() ;
	}*/
	
	void Canvas::mouseMoveEvent(QMouseEvent *event){
						isMouseInsideCanvas=true;
		QPointF 		cursorCoord = QPointF(getView()->mapToScene( event->pos() ).x(),
											 getView()->mapToScene( event->pos() ).y());
		if (!currentItem) {
		qDebug()<<"Yes here"			;
			switch (getView()->getTool()) {
				case toolType::line_solid: case toolType::line_dashed:	{
					 currentItem = new Myline(static_cast<MainWindow*>(getView()->parent()->parent()),
										 QPointF(cursorCoord.x(),cursorCoord.y()));							
				break;
				}
				case toolType::size:{
					currentItem = new Size(static_cast<MainWindow*>(getView()->parent()->parent()),
										 QPointF(cursorCoord.x(),cursorCoord.y()));							
				break;	
				}				
			}
		getView()->scene()->addItem(currentItem);		
		}
		else { 
			switch (getView()->getTool()) {
				case toolType::line_solid: case toolType::line_dashed:	{
					Myline* aLine	= static_cast<Myline*>(currentItem);
					aLine->changePoints(cursorCoord);
					break;
				}
				case toolType::size: {
					Size* aSize	= static_cast<Size*>(currentItem);
					aSize->changePoints(cursorCoord);
					break;
				}
			}	
											
		}	
	}
	
	bool Canvas::isMouseInside() {return isMouseInsideCanvas;}
	
	View* Canvas::getView() const  {return view;}
	
	void Canvas::mousePressEvent(QMouseEvent * event) {
			qDebug()<<"mousePressEvent";		
			switch (getView()->getTool()) {
				case toolType::line_solid:
				case toolType::line_dashed:{
					QPointF 		mouseCoord = QPointF(getView()->mapToScene( event->pos() ).x(),
														 getView()->mapToScene( event->pos() ).y());	
					Myline* line	= static_cast<Myline*>(currentItem);						
					line->changeMode();
					line->changesecondPointCoord(mouseCoord);
							
					if (line->getMode()>1) currentItem=nullptr;					
					break;
				}
				case toolType::size: {break;} 
			}	
		}

		
	
	
