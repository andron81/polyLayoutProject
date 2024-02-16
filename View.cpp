#include "View.hpp"
#include "MainWindow.hpp"
toolType View::getTool() {
	MainWindow * mw = static_cast<MainWindow *>(parent()->parent()->parent()); 
	return mw->getTool();
}