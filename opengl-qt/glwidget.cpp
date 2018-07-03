
#include "glwidget.h"
#include <math.h>
#include <QMouseEvent>


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      clearColor(Qt::black),
      camera(glm::vec3(0, 10.0, 10.0)),
      skybox(0)
{
    this->grabKeyboard();
    this->setFocus();
    //this->setMouseTracking(true);
    this->time.start();

    this->curShowInfoObj = NULL;

    this->pressMidStart = false;
    this->pressRightStart = false;
    this->pressControl = false;
    for (int i=0; i<256; i++)
        this->keys[i] = false;
    this->dir = glm::vec3(0.0f, 0.0f, -1.0f);
    this->speed = 8.0f;
    this->lastTime = time.elapsed() / 1000.f;
}

GLWidget::~GLWidget()
{
    makeCurrent();
    ground->destroy();
    for (int i=0; i<models.size(); i++){
        models[i]->destroy();
    }
    delete skybox_shader;
    delete ground_shader;
    delete program;

    delete skybox;
    delete ground;
    for (int i=0; i<models.size(); i++){
        delete models[i];
    }
    for (int i=0; i<gameObjects.size(); i++){
        delete gameObjects[i];
    }
    doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void GLWidget::initializeGL()
{
    bool r = true;
    initializeOpenGLFunctions();
    printContextInformation();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //shaderProgram
    this->skybox_shader = new QOpenGLShaderProgram;
    r = skybox_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/skybox.vs");
    r = skybox_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/skybox.fs");
    if (!skybox_shader->link() || !r){
        skybox_shader->log();
        exit(-1);
    }
    this->ground_shader = new QOpenGLShaderProgram;
    r= ground_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/texture_vertex.vs");
    r= ground_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/fshader.fs");
    if (!ground_shader->link() || !r){
        ground_shader->log();
        exit(-1);
    }
    this->program = new QOpenGLShaderProgram;
    r = program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/texture_vertex.vs");
    r = program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/directional_light.fs");
    if (!program->link() || !r){
        program->log();
        exit(-1);
    }
    //model
    skybox = new SkyBox(this->skybox_shader, context()->functions());
    ground = new Model("D:/Lixin/Qt/Projects/qtopengl/models/t.obj", context()->functions(), ground_shader);
    models.push_back(new Model("D:/Lixin/Qt/Projects/qtopengl/models/body.obj", context()->functions(), program));
    models.push_back(new Model("D:/Lixin/Qt/Projects/qtopengl/models/wFL.obj", context()->functions(), program));
    models.push_back(new Model("D:/Lixin/Qt/Projects/qtopengl/models/wFR.obj", context()->functions(), program));
    models.push_back(new Model("D:/Lixin/Qt/Projects/qtopengl/models/wBL.obj", context()->functions(), program));
    models.push_back(new Model("D:/Lixin/Qt/Projects/qtopengl/models/wBR.obj", context()->functions(), program));

    //gameObject
    //body
    Transform local = {glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f)};
    gameObjects.push_back(new GameObject("car", models[0], local, NULL));
    //wFL
    local = {glm::vec3(-0.75f, 0.85f, -1.3f), glm::vec3(0.f), glm::vec3(1.f)};
    gameObjects.push_back(new GameObject("frontLeftWheel", models[1], local, gameObjects[0]));
    //wFR
    local = {glm::vec3(0.75f, 0.85f, -1.3f), glm::vec3(0.f), glm::vec3(1.f)};
    gameObjects.push_back(new GameObject("frontRightWheel", models[2], local, gameObjects[0]));
    //wBL
    local = {glm::vec3(-0.75f, 0.85f, 1.55f), glm::vec3(0.f), glm::vec3(1.f)};
    gameObjects.push_back(new GameObject("backLeftWheel", models[3], local, gameObjects[0]));
    //wBR
    local = {glm::vec3(0.75f,0.85f,1.55f), glm::vec3(0.f), glm::vec3(1.f)};
    gameObjects.push_back(new GameObject("backRightWheel", models[4], local, gameObjects[0]));

    this->connectInfoPanal(gameObjects[0], this->infoWidget);
    this->connect(objWidget->getTreeWidegt(), SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
                  this, SLOT(changeObjInfo(QTreeWidgetItem*,int)));
    this->objWidget->addGameObject(gameObjects[0]);
    //timer
    connect(&timer, &QTimer::timeout, this, &GLWidget::updateFrame);
    timer.start(16);
}

void GLWidget::paintGL()
{
    glm::vec3 bodyPos = gameObjects[0]->localTransform.position;
    this->deltaTime = (time.elapsed() / 1000.f - this->lastTime);
    this->lastTime = time.elapsed() / 1000.f;

    this->doInput();
    //qDebug()<<deltaTime;
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //projection, view
    this->projection = glm::perspective(this->camera.Zoom,(GLfloat)this->width() / (GLfloat)this->height(),0.1f, 100.0f);

    //view = this->camera.GetViewMatrix();
    glm::mat4 m;
    m = glm::translate(m, glm::vec3(0.f));
    glm::vec3 cameraUp = glm::vec3(bodyPos.x,bodyPos.y+10.f,bodyPos.z);
    glm::vec3 cameraForward =glm::vec3(bodyPos.x,bodyPos.y ,bodyPos.z + 10.f);
    glm::vec3 cameraRight =glm::vec3(bodyPos.x + 10.f, bodyPos.y,bodyPos.z);
    glm::mat4 *viewM = new glm::mat4[4];
    viewM[0] = glm::lookAt(cameraUp,bodyPos,glm::vec3(-1.0f, 0.0f, 0.0f)); //1,0,0  顶视图  0,1,0 测、正视图,
    viewM[1] = glm::lookAt(cameraForward,bodyPos,glm::vec3(0.0f, 1.0f, 0.0f));
    viewM[2] = glm::lookAt(cameraRight,bodyPos,glm::vec3(0.0f, 1.0f, 0.0f));
    viewM[3] = view;

    for(int j = 0;j<4;j++)
    {
        if(j==3)
            glViewport(212,0,570,636);
        else
            glViewport(0,212*j,212,212);
        //ground
        this->ground_shader->bind();
        glUniformMatrix4fv(ground_shader->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(ground_shader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewM[j]));
        glUniformMatrix4fv(ground_shader->uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(m));
        if (ground->isLoaded()) ground->Draw();

        //gameObject
        for (int i=0; i < gameObjects.size(); i++){
            gameObjects[i]->Draw(projection, viewM[j]);
        }

        //draw skybox
        glm::mat4 v= glm::mat4(glm::mat3(viewM[j]));
        skybox->setData(v, projection);
        skybox->draw();
    }

    delete viewM;
}
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::updateFrame()
{
    this->paintGL();
    update();
}

void GLWidget::getProgram(const QString vs, const QString fs, QOpenGLShaderProgram &p)
{
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!vshader->compileSourceFile(vs))
        vshader->log();
    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!fshader->compileSourceFile(fs))
        fshader->log();

    //shaderProgram
    p.addShader(vshader);
    p.addShader(fshader);
    if (!p.link())
        p.log();
}

void GLWidget::doInput()
{
    glm::vec3 bodyPos = gameObjects[0]->localTransform.position;
    if (this->keys[Qt::Key_Space]){

    }else if (this->keys[Qt::Key_V]){
        camera.Position = bodyPos;
        camera.Position.y = camera.Position.y + 3.f;
        view = glm::lookAt(camera.Position,
                    bodyPos+ dir * 8.f,
                    glm::vec3(0.0f, 1.0f, 0.0f)); //-1,0,0  顶视图  0,1,0 测、正视图
    }else if (this->pressControl){
        view = this->camera.GetViewMatrix();
    }
    else{
        bodyPos = gameObjects[0]->localTransform.position;
        camera.Position = bodyPos - dir * 8.f;
        camera.Position.y = bodyPos.y + 5.f;
        view = glm::lookAt(camera.Position, bodyPos, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if(this->keys[Qt::Key_W]){ //向前
        //this->camera.ProcessKeyboard(FORWARD, this->deltaTime);
        float wAngle = -120.f * speed * deltaTime;
        gameObjects[0]->translate(dir * speed  * deltaTime);
        gameObjects[3]->rotate(glm::vec3(wAngle, 0.f, 0.f));
        gameObjects[4]->rotate(glm::vec3(wAngle, 0.f, 0.f));
        if (gameObjects[1]->localTransform.rotation.y != 0){
            gameObjects[1]->localTransform.rotation.x = 0.f;
            gameObjects[2]->localTransform.rotation.x = 0.f;
        }
        if (gameObjects[1]->localTransform.rotation.y > 0){
            gameObjects[1]->rotate(glm::vec3(0.f, -1.f, 0.f));
            gameObjects[2]->rotate(glm::vec3(0.f, -1.f, 0.f));
        }else if(gameObjects[1]->localTransform.rotation.y < 0){
            gameObjects[1]->rotate(glm::vec3(0.f, 1.f, 0.f));
            gameObjects[2]->rotate(glm::vec3(0.f, 1.f, 0.f));
        }
        gameObjects[1]->rotate(glm::vec3(wAngle, 0.f, 0.f));
        gameObjects[2]->rotate(glm::vec3(wAngle, 0.f, 0.f));
    }
    if(this->keys[Qt::Key_S]){ //向后
        //this->camera.ProcessKeyboard(BACKWARD, this->deltaTime);
        float wAngle = 120.f * speed * deltaTime;
        gameObjects[0]->translate(-dir * speed  * deltaTime);
        gameObjects[3]->rotate(glm::vec3(wAngle, 0.f, 0.f));
        gameObjects[4]->rotate(glm::vec3(wAngle, 0.f, 0.f));

        if (gameObjects[1]->localTransform.rotation.y != 0){
            gameObjects[1]->localTransform.rotation.x = 0.f;
            gameObjects[2]->localTransform.rotation.x = 0.f;
        }
        if (gameObjects[1]->localTransform.rotation.y > 0){
            gameObjects[1]->rotate(glm::vec3(0.f, -1.f, 0.f));
            gameObjects[2]->rotate(glm::vec3(0.f, -1.f, 0.f));
        }else if(gameObjects[1]->localTransform.rotation.y < 0){
            gameObjects[1]->rotate(glm::vec3(0.f, 1.f, 0.f));
            gameObjects[2]->rotate(glm::vec3(0.f, 1.f, 0.f));
        }
        gameObjects[1]->rotate(glm::vec3(wAngle, 0.f, 0.f));
        gameObjects[2]->rotate(glm::vec3(wAngle, 0.f, 0.f));
    }
    if (this->keys[Qt::Key_A]){  //向左
        //this->camera.ProcessKeyboard(LEFT, this->deltaTime);
        gameObjects[0]->rotate(glm::vec3(0.f, 20.f *deltaTime, 0.f));
        if (gameObjects[1]->localTransform.rotation.y >= 45){

        }else{
            gameObjects[1]->localTransform.rotation.x = 0.f;
            gameObjects[2]->localTransform.rotation.x = 0.f;
            gameObjects[1]->rotate(glm::vec3(0.f, 5.f, 0.f));
            gameObjects[2]->rotate(glm::vec3(0.f, 5.f, 0.f));
        }

        float curAngle = gameObjects[0]->localTransform.rotation.y;
        glm::vec3 d;
        d.x = -sin(glm::radians(curAngle));
        d.z = -cos(glm::radians(curAngle));
        d.y = dir.y;
        dir = glm::normalize(d);
    }
    if(this->keys[Qt::Key_D]){  //向右
        //this->camera.ProcessKeyboard(RIGHT, this->deltaTime);
        gameObjects[0]->rotate(glm::vec3(0.f, -30.f *deltaTime, 0.f));
        if (gameObjects[1]->localTransform.rotation.y <= -45){

        }else{
            gameObjects[1]->localTransform.rotation.x = 0.f;
            gameObjects[2]->localTransform.rotation.x = 0.f;
            gameObjects[1]->rotate(glm::vec3(0.f, -5.f ,0.f));
            gameObjects[2]->rotate(glm::vec3(0.f, -5.f , 0.f));
        }
        float curAngle = gameObjects[0]->localTransform.rotation.y;
        glm::vec3 d;
        d.x = -sin(glm::radians(curAngle));
        d.z = -cos(glm::radians(curAngle));
        d.y = dir.y;
        dir = glm::normalize(d);
    }
}

void GLWidget::keyPressEvent(QKeyEvent  *event)
{
    //qDebug()<< event->key();
    if (event->key() == 16777249){
        this->pressControl = true;
    }
    else if (event->key() < 256){
        this->keys[event->key()] = true;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent  *event)
{
    if (event->key() == 16777249){
        this->pressControl = false;
    }
    else  if (event->key() < 256){
        this->keys[event->key()] = false;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton && pressControl){
        this->pressMidStart = true;
        this->lastCursorPos = QVector2D(event->localPos());
    }else if (event->button() == Qt::RightButton && pressControl){
        this->pressRightStart = true;
        this->lastCursorPos = QVector2D(event->localPos());
    }

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D move = QVector2D(event->localPos()) - this->lastCursorPos;
    this->lastCursorPos = QVector2D(event->localPos());
    if (this->pressMidStart){
        camera.Position.x += -move.x() * 0.04;
        camera.Position.y += move.y() * 0.04;
        //camera.updateCameraVectors();
    }
    else if (this->pressRightStart){
        camera.ProcessMouseMovement(move.x(), -move.y());
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * event)
{
    this->pressMidStart = false;
    this->pressRightStart = false;
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
    //qDebug()<< camera.Zoom;
    this->camera.ProcessMouseScroll(event->angleDelta().y());
}

void GLWidget::setInfoWidget(InfoWidget *w)
{
    this->infoWidget = w;
}
void GLWidget::setObjWidget(ObjWidget *w)
{
    this->objWidget = w;
}

void GLWidget::changeObjInfo(QTreeWidgetItem *item, int column)
{
    QString name = item->text(column);
    for (int i=0; i<this->gameObjects.size(); i++)
    {
        if (gameObjects[i]->getName() == name){
            this->connectInfoPanal(gameObjects[i], this->infoWidget);
        }
    }
}

void GLWidget::connectInfoPanal(GameObject *obj, InfoWidget *target)
{
    if (this->curShowInfoObj == obj)
        return ;
    if (this->curShowInfoObj != NULL){
        disconnect(curShowInfoObj, &GameObject::valueChanged, target, &InfoWidget::setInfo);
    }
    connect(obj, &GameObject::valueChanged, target, &InfoWidget::setInfo);
    this->curShowInfoObj = obj;
}

void GLWidget::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    // 获取版本信息
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // 获取 Profile 信息
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
#undef CASE

    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
