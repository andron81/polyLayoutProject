#pragma once
class MainWindow;

class Winda : public QDialog  {
	private:
    QLineEdit* 					m_edit;
	public:
	Winda(QWidget* parentWidget_ = 0 ):QDialog(parentWidget_){
		setModal(true);
		    QBoxLayout* layout = new QHBoxLayout;
    m_edit = new QLineEdit;
    layout->addWidget( m_edit );

    QPushButton* okBtn = new QPushButton( "OK" );
    connect( okBtn, SIGNAL( clicked() ), SLOT( accept() ) );
    layout->addWidget( okBtn );

    QPushButton* applyBtn = new QPushButton( "Apply" );
    connect( applyBtn, SIGNAL( clicked() ), SIGNAL( applied() ) );
    layout->addWidget( applyBtn);

    QPushButton* cancelBtn = new QPushButton( "Cancel" );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( reject() ) );
    layout->addWidget( cancelBtn );

    setLayout( layout );
		
	}



    ~Winda(){};

    QString getInput() const {return m_edit->text();}

signals:
    void accepted();


};
