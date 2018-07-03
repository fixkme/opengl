#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>

#include "glwidget.h"
#include "infowidget.h"
#include "objwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initUI();

    Ui::MainWindow *ui;
    GLWidget *m_glWidget;

    InfoWidget *infoWidget;
    ObjWidget *objWidget;
    QDockWidget *infoDock;
    QDockWidget *objectsDock;
};

#endif // MAINWINDOW_H
