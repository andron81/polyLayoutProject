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
		innerCanvas=false;
		if (currentItem) {
		getView()->scene()->removeItem(currentItem); 
		delete(currentItem);
		currentItem=nullptr;
		}	
	}
		
	
	void Canvas::mouseMoveEvent(QMouseEvent *event){
						innerCanvas=true;
		QPointF 		mouseCoord = QPointF(getView()->mapToScene( event->pos() ).x(),
											 getView()->mapToScene( event->pos() ).y());												 
		auto      		MW 		   = static_cast<MainWindow*>(getView()->parent()->parent());
		toolType tool = MW->getTool() ;
		if (tool == toolType::line_dashed){
		}
		if (tool == toolType::line_solid){
			if (!currentItem) {			
				currentItem = new myline(QPointF(mouseCoord.x(),mouseCoord.y()));		
				getView()->scene()->addItem(currentItem);
			}			
			else { 
			currentItem->changefirstPointCoord(mouseCoord);			
			}
		}	
	}
	
	bool Canvas::isinner() {return innerCanvas;}
	
	View* Canvas::getView() const  {return view;}
	
