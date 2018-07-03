#include "objwidget.h"
#include "ui_objwidget.h"
#include <QTreeWidgetItem>
#include <QRect>
#include "qdebug.h"

ObjWidget::ObjWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjWidget)
{
    ui->setupUi(this);
    this->treeWidget = ui->treeWidget;
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabel("Gameobjects");
}

ObjWidget::~ObjWidget()
{
    delete ui;
}

void ObjWidget::addGameObject(GameObject *obj)
{
    this->gameObjects.push_back(obj);
    QTreeWidgetItem *root = new QTreeWidgetItem();
    root->setText(0, obj->getName());
    ui->treeWidget->addTopLevelItem(root);

    for (int i=0; i< obj->getChilds().size(); i++){
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setText(0, obj->getChilds()[i]->getName());
        root->addChild(child);
    }
}
