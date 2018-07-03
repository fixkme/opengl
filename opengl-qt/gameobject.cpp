#include "gameobject.h"
#include "qdebug.h"
#include "glwidget.h"

class GLWidget;

GameObject::GameObject(QString name, Model *model, Transform local, GameObject *parent)
{
    this->foreVector = glm::vec3(0.0f, 0.0f, -1.0f);
    this->upVector = glm::vec3(0.f, 1.0f, 0.f);
    this->name = name;
    this->model = model;
    this->parent = parent;
    this->localTransform = local;

    if(this->parent == NULL){
     /*   float x = 0, y = 0, z = 0, num = 0;
        for (int i = 0; i < model->meshes.size(); i++){
            for (int j = 0; j < model->meshes[i].vertices.size(); j++){
                x += model->meshes[i].vertices[j].position.x;
                y += model->meshes[i].vertices[j].position.y;
                z += model->meshes[i].vertices[j].position.z;
                num ++;
            }
        }
        x = x / num; y = y / num; z = z / num;
        qDebug() << name.c_str() << ": " << x << " " << y << " " << z;
     */
        this->transform.position = glm::vec3(0.f);
        this->transform.rotation = glm::vec3(0.f);
        this->transform.scale = glm::vec3(1.f);

    }else{
        this->parent->addChild(this);
        this->transform.position = local.position + parent->transform.position;
        this->transform.rotation = local.rotation + parent->transform.rotation;
        this->transform.scale = local.scale * parent->transform.scale;
    }
    this->updateMat();
}

void GameObject::translate(float dx, float dy, float dz)
{
    this->localTransform.position.x += dx;
    this->localTransform.position.y += dy;
    this->localTransform.position.z += dz;
    if (this->parent == NULL)
        this->transform.position = this->localTransform.position;
    else{
        this->transform.position = localTransform.position + parent->transform.position;
    }
    this->updateMat();
}

void GameObject::translate(glm::vec3 dp)
{
    this->localTransform.position += dp;
    if (this->parent == NULL)
        this->transform.position = this->localTransform.position;
    else{
        this->transform.position = localTransform.position + parent->transform.position;
    }
    this->updateMat();
}

void GameObject::rotate(float angle, glm::vec3 aix)
{
    if (aix.x == 1){
        this->localTransform.rotation.x += angle;
    }else if (aix.y == 1){
        this->localTransform.rotation.y += angle;
    }else if (aix.z == 1){
        this->localTransform.rotation.z += angle;
    }
    if (this->parent == NULL){
        this->transform.rotation = this->localTransform.rotation;
    }else{
        this->transform.rotation = localTransform.rotation + parent->transform.rotation;
    }
    this->updateMat();
}

void GameObject::rotate(glm::vec3 angles)
{
    this->localTransform.rotation.x += angles.x;
    this->localTransform.rotation.y += angles.y;
    this->localTransform.rotation.z += angles.z;
    if (this->parent == NULL){
        this->transform.rotation = this->localTransform.rotation;
    }else{
        this->transform.rotation = localTransform.rotation + parent->transform.rotation;
    }
    this->updateMat();
}

void GameObject::scale(float sx, float sy, float sz)
{
    this->localTransform.scale = glm::vec3(sx, sy, sz);

    if (this->parent ==NULL){
        this->transform.scale = this->localTransform.scale;
    }else{
        this->transform.scale = localTransform.scale * parent->transform.scale;
    }
    this->updateMat();
}

void GameObject::scale(float s)
{
    this->localTransform.scale = glm::vec3(s);

    if (this->parent ==NULL){
        this->transform.scale = this->localTransform.scale;
    }else{
        this->transform.scale = localTransform.scale * parent->transform.scale;
    }
    this->updateMat();
}

void GameObject::updateMat()
{
    if (this->parent == NULL){
        this->modelMat =  glm::mat4();
        this->modelMat = glm::translate(this->modelMat, glm::vec3(0.f, -0.9f, 0.f));
    }else {
        this->modelMat = this->parent->modelMat;
    }
    this->modelMat = glm::translate(this->modelMat, this->localTransform.position);
    this->modelMat = glm::rotate(this->modelMat,
                                 glm::radians(this->localTransform.rotation.x),
                                 glm::vec3(1.f, 0.f,0.f));
    this->modelMat = glm::rotate(this->modelMat,
                                 glm::radians(this->localTransform.rotation.y),
                                 glm::vec3(0.f, 1.f,0.f));
    this->modelMat = glm::rotate(this->modelMat,
                                 glm::radians(this->localTransform.rotation.z),
                                 glm::vec3(0.f, 0.f,1.f));
    this->modelMat = glm::scale(this->modelMat, this->localTransform.scale);

    for (int i = 0; i < this->childs.size(); i++)
    {
        //this->childs[i]->modelMat = this->modelMat;
        this->childs[i]->updateMat();
    }
    emit valueChanged(this->localTransform.position, localTransform.rotation, localTransform.scale
                      , this->name);
}

void GameObject::Draw(glm::mat4 projection, glm::mat4 view)
{
    this->model->program->bind();
    this->model->gf->glUniformMatrix4fv(model->program->uniformLocation("model"),
                       1, GL_FALSE, glm::value_ptr(this->modelMat));
    this->model->gf->glUniformMatrix4fv(model->program->uniformLocation("projection"),
                       1, GL_FALSE, glm::value_ptr(projection));
    this->model->gf->glUniformMatrix4fv(model->program->uniformLocation("view"),
                       1, GL_FALSE, glm::value_ptr(view));
    this->model->Draw();
    //for (int i = 0; i < this->childs.size(); i++)
        //this->childs[i]->Draw(projection, view);
}

void GameObject::addChild(GameObject *child)
{
    this->childs.push_back(child);
}

void GameObject::setParent(GameObject *parent, Transform local)
{
    this->parent = parent;
    this->parent->addChild(this);
    this->localTransform = local;
}



