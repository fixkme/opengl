#ifndef OBJWIDGET_H
#define OBJWIDGET_H

#include <QWidget>
#include <QVector>
#include <QTreeWidget>
#include "gameobject.h"

namespace Ui {
class ObjWidget;
}

class ObjWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObjWidget(QWidget *parent = 0);
    ~ObjWidget();
    QTreeWidget *getTreeWidegt() { return this->treeWidget; }
public slots:
    void addGameObject(GameObject *obj);

signals:
    //void itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::ObjWidget *ui;
    QTreeWidget *treeWidget;

    QVector<GameObject *> gameObjects;
};

#endif // OBJWIDGET_H
