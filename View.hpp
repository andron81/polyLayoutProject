#pragma once
#include "Canvas.hpp"



class View : public QGraphicsView {
	const int			scalefactorMin		= 1;
	const int			scalefactorMax		= 200;
	const int			scalefactorDiv		= 10;
	int					scalefactor			= scalefactorDiv;
	Canvas * 			canvas;
	QGraphicsItem* 		currentItem			=nullptr;
	public:
	//toolType getTool() {return static_cast<MainWindow>(mainWin)->getTool();}
	toolType getTool() ;
	bool isMouseInsideCanvas(QPointF 	mouseCoord) {
		
		return (mouseCoord.x()>=canvas->getTopLeft().x() && mouseCoord.x()<=canvas->getTopLeft().x()+canvas->getSize().width() ) &&
			   (mouseCoord.y()>=canvas->getTopLeft().y() && mouseCoord.y()<=canvas->getTopLeft().y()+canvas->getSize().height());
		
	}
	View( QGraphicsScene * p_scene, QWidget * p_parent )
		: QGraphicsView( p_scene, p_parent ),
		  canvas(new Canvas(this))
	{


	
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
		setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );

		canvas->setSize(QSize(1000,700));
		scene()->addItem( canvas );

		
		connect( scene(), &QGraphicsScene::focusItemChanged, this,
			[&]( QGraphicsItem * p_new, QGraphicsItem * p_old, Qt::FocusReason reason ) {
					//update_zoom();
		//		if (get_m_painter().get_tool_e()==tool_e::edit) {
		//		update_zoom();
		//		}
			} );

		//update_zoom();
	};
	void mouseMoveEvent(QMouseEvent *event) override{
		QPointF 		mouseCoord = {mapToScene( event->pos() ).x(),mapToScene( event->pos() ).y()};
		QSize			canvasSize = canvas->getSize(); 
		QPointF 		canvasTopLeft = canvas->getTopLeft();
		if (isMouseInsideCanvas(mouseCoord))
				canvas->mouseMoveEvent(event); 
		 else if (canvas->isMouseInside()) 
				canvas->mouseLeaveEvent(); 
	}
	void mousePressEvent(QMouseEvent *event) override{
		if (canvas->isMouseInside())
			canvas->mousePressEvent(event); 
	}		
	void update_zoom() {
		QTransform tform;
		qreal scale = qreal(scalefactor) / scalefactorDiv;
		tform.scale( scale, scale );
		setTransform( tform );
	}
	
		void wheelEvent( QWheelEvent * p_event ) override {
		//QGraphicsView::wheelEvent( p_event ); // I don't need default QGraphicsView wheel behaviour
		if ( p_event->modifiers() & Qt::ControlModifier ) {
			scalefactor += p_event->angleDelta().y() > 0 ? 1 : -1;
			scalefactor = std::clamp( scalefactor, scalefactorMin, scalefactorMax );
			update_zoom();
		}
	}


	
};