#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDockWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    this->initUI();
    this->m_glWidget = new GLWidget(this);
    this->setCentralWidget(this->m_glWidget);
    this->m_glWidget->setFocus();
    this->m_glWidget->setInfoWidget(this->infoWidget);
    this->m_glWidget->setObjWidget(objWidget);
}

MainWindow::~MainWindow()
{
    delete this->infoWidget;
    delete this->infoDock;
    delete this->objWidget;
    delete this->objectsDock;
    delete this->m_glWidget;
    delete ui;
}

void MainWindow::initUI()
{
    setFixedSize(1024, 636);
    setWindowTitle("openGL");
    //文件菜单
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("&Save");
    fileMenu->addAction("&Exit", this, &QWidget::close);

    //帮助菜单
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(tr("&Usage"));

    //dock
    infoDock = new QDockWidget(tr("hierarchy"),this);
    infoDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    infoDock->setMinimumWidth(240);
    addDockWidget(Qt::RightDockWidgetArea,infoDock);

    objectsDock = new QDockWidget(tr("inspector"),this);
    objectsDock->setFeatures(QDockWidget::DockWidgetMovable);
    objectsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    objectsDock->setMinimumWidth(240);
    addDockWidget(Qt::RightDockWidgetArea,objectsDock);

    //infowidget
    this->infoWidget = new InfoWidget(this);
    infoDock->setWidget(infoWidget);

    //objwidget
    this->objWidget = new ObjWidget(this);
    objectsDock->setWidget(objWidget);

}
