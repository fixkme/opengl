#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "model.h"
#include <QObject>
#include <QString>

struct Transform{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

class GameObject : public QObject
{
    Q_OBJECT

signals:
    void valueChanged(glm::vec3 pos, glm::vec3 angles, glm::vec3 scale, QString name);

public:
    GameObject(QString name, Model *model, Transform local, GameObject *parent = NULL);
    void addChild(GameObject *child);
    void setParent(GameObject *parent, Transform local={glm::vec3(0.f),glm::vec3(0.f),glm::vec3(1.f)});
    void translate(float dx, float dy, float dz);
    void translate(glm::vec3 dp);
    void rotate(float angle, glm::vec3 aix);
    void rotate(glm::vec3 angles);
    void scale(float sx, float sy, float sz);
    void scale(float s);
    void updateMat();
    void Draw(glm::mat4 projection, glm::mat4 view);

    Model *getModel() { return  this->model; }
    QString getName() {return this->name; }
    GameObject* getParent() { return this->parent; }
    QVector<GameObject *> getChilds() { return this->childs; }

    Transform transform;
    Transform localTransform;
    glm::vec3 foreVector;
    glm::vec3 upVector;
    glm::mat4 modelMat;

private:
    QString name;
    Model *model;
    GameObject *parent;
    QVector<GameObject *> childs;

};

#endif // GAMEOBJECT_H
