#include "Canvas.hpp"	
	Canvas::Canvas() {
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
		innerCanvas=false;
		qDebug()<<"Canvas::mouseLeaveEvent()";
	}

	
	void Canvas::mouseMoveEvent(){
		innerCanvas=true;
		qDebug()<<"Canvas::mouseMoveEvent()";
	}
	
	bool Canvas::isinner() {return innerCanvas;}
	
