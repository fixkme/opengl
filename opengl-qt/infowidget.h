#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Ui {
class InfoWidget;
}

class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = 0);
    ~InfoWidget();
public slots:
    void setInfo(glm::vec3 pos, glm::vec3 angles, glm::vec3 scale, QString name);

private:
    Ui::InfoWidget *ui;
};

#endif // INFOWIDGET_H
