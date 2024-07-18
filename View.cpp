#include "View.hpp"
#include "MainWindow.hpp"
#include "Canvas.hpp"
#include "AppSettings.hpp"



void View::save_to_image(const QString & filename, const QString ext){
	Canvas * canvas = getCanvas();
		QRectF rect(canvas->getTopLeft().x(), canvas->getTopLeft().y(), scene()->width(), scene()->height());
		QImage image(canvas->getSize(), QImage::Format_ARGB32_Premultiplied);
		QPainter painter(&image);
		scene()->render(&painter,rect, QRectF(-canvas->getSize().width()
		,-canvas->getSize().height(), scene()->width(), scene()->height()));				
		QByteArray ba = ext.toLocal8Bit();
		const char *ext_c = ba.data();		
		image.save(filename,ext_c);	
	
	
}
ToolType View::getTool() {	
	//mw = static_cast<MainWindow *>(parent()->parent());
	return mw->getTool();
}
	bool View::isMouseInsideCanvas(QPointF 	mouseCoord) {
		
		return (mouseCoord.x()>=canvas->getTopLeft().x() && mouseCoord.x()<=canvas->getTopLeft().x()+canvas->getSize().width() ) &&
			   (mouseCoord.y()>=canvas->getTopLeft().y() && mouseCoord.y()<=canvas->getTopLeft().y()+canvas->getSize().height());
		
	}						

	View::View( QGraphicsScene * p_scene, QWidget * p_parent )
		: QGraphicsView( p_scene, p_parent ),
		  canvas(new Canvas(this)){		
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
		setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );
		scene()->addItem( canvas );
		mw=static_cast<MainWindow *>(p_parent->parent()->parent());
		QString CanvSZ = mw->getSettings()->getValue("cs").toString();		
		
		QStringList cs = CanvSZ.split("x");
		canvas->setSize({cs[0].toInt(),cs[1].toInt()});
		connect( scene(), &QGraphicsScene::focusItemChanged, this,
			[&]( QGraphicsItem * p_new, QGraphicsItem * p_old, Qt::FocusReason reason ) {
			} );
	};
	void View::mouseReleaseEvent(QMouseEvent *event) {
		
		canvas->mouseReleaseEvent();
	}
	void View::mouseMoveEvent(QMouseEvent *event) {
		QPointF 		mouseCoord = {mapToScene( event->pos() ).x(),mapToScene( event->pos() ).y()};
		QSize			canvasSize = canvas->getSize(); 
		QPointF 		canvasTopLeft = canvas->getTopLeft();
		if (isMouseInsideCanvas(mouseCoord))
				canvas->mouseMoveEvent(event); 
		 else if (canvas->isMouseInside()) 
				canvas->mouseLeaveEvent(); 
	}
	void View::mousePressEvent(QMouseEvent *event) {
		if (canvas->isMouseInside())
			canvas->mousePressEvent(event); 
	}		
	void View::update_zoom() {
		QTransform tform;
		qreal scale = qreal(scalefactor) / scalefactorDiv;
		tform.scale( scale, scale );
		setTransform( tform );
	}
	Canvas * View::getCanvas(){			
		return canvas;
	}
	void View::wheelEvent( QWheelEvent * p_event )  {
		//QGraphicsView::wheelEvent( p_event ); // I don't need default QGraphicsView wheel behaviour
		if ( p_event->modifiers() & Qt::ControlModifier ) {
			scalefactor += p_event->angleDelta().y() > 0 ? 1 : -1;
			scalefactor = std::clamp( scalefactor, scalefactorMin, scalefactorMax );
			update_zoom();
		}
	}
