#pragma once
#include <QDebug>

#include <QApplication>
#include <QScreen>
#include <QFileInfo>
#include <QSettings>

#include <QMessageBox>
#include <QMainWindow>
#include <QWidget>
#include <QDockWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QPrinter>
#include <QFileDialog>

#include <QVBoxLayout>

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

#include <QUndoStack>
#include <QUndoView>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsDropShadowEffect>

#include <QJsonObject>
#include <QJsonDocument>

#include <QWheelEvent>
#include <QIntValidator>

enum class ToolType { none, edit, line_solid = 600, line_dashed = 601, text = 602, size = 603};
enum class EditBlockVisible { none, changeLength, changeText};
