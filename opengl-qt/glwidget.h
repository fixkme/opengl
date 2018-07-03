#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTime>
#include <QTimer>
#include <QVector>
#include <QVector2D>
#include <QOpenGLContext>
#include <QTreeWidgetItem>

#include "camera.h"
#include "light.h"
#include "skybox.h"
#include "model.h"
#include "gameobject.h"
#include "infowidget.h"
#include "objwidget.h"


class GLWidget : public QOpenGLWidget , public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void setClearColor(const QColor &color);
    void printContextInformation();
    void getProgram(const QString vs, const QString fs, QOpenGLShaderProgram &p);
    void setInfoWidget(InfoWidget *w);
    void setObjWidget(ObjWidget *w);
    void connectInfoPanal(GameObject *obj, InfoWidget *target);

    InfoWidget *infoWidget;
    ObjWidget *objWidget;
    GameObject *curShowInfoObj;

    Camera camera;
    Light light;
    SkyBox *skybox;

    glm::mat4 projection;
    glm::mat4 view;

    //逻辑数据
    glm::vec3 dir;
    float speed;

public slots:
    void changeObjInfo(QTreeWidgetItem *item, int column);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent  *event) override;
    void keyReleaseEvent(QKeyEvent  *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent * event) override;

private:
    void updateFrame();
    void doInput();

    Model *ground;
    QOpenGLShaderProgram *ground_shader;
    QOpenGLShaderProgram *skybox_shader;
    QOpenGLShaderProgram *program;
    QVector<Model*> models;
    QVector<GameObject*> gameObjects;

    QColor clearColor;
    QVector2D lastCursorPos;
    QTime time;
    QTimer timer;
    float deltaTime;
    float lastTime;
    bool pressRightStart;
    bool pressMidStart;
    bool keys[256];
    bool pressControl;
};

#endif // GLWIDGET_H
