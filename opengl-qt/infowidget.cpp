#include "infowidget.h"
#include "ui_infowidget.h"
#include <QString>

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
    setWindowTitle("transform");
    ui->posX->setText(QString::number(1.10));
}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::setInfo(glm::vec3 pos, glm::vec3 angles, glm::vec3 scale, QString name)
{
    ui->objName->setText(name);
    ui->posX->setText(QString::number(pos.x));
    ui->posY->setText(QString::number(pos.y));
    ui->posZ->setText(QString::number(pos.z));

    ui->angleX->setText(QString::number(angles.x));
    ui->angleY->setText(QString::number(angles.y));
    ui->angleZ->setText(QString::number(angles.z));

    ui->scaleX->setText(QString::number(scale.x));
    ui->scaleY->setText(QString::number(scale.y));
    ui->scaleZ->setText(QString::number(scale.z));

}
