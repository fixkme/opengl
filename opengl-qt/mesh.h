#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

#include "qdebug.h"


struct Vertex {  //顶点
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture { //文理
    GLuint id;
    string type;
    aiString path;
};

struct Material // 表示材质属性
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    vector<Texture> textures;
};

class Mesh : public QOpenGLFunctions
{
public:
    Mesh(QOpenGLFunctions *gf, QOpenGLShaderProgram *p, vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
    ~Mesh();
    //渲染mesh
    void Draw();
    void setShaderProgram(QOpenGLShaderProgram *program);
    QOpenGLShaderProgram * getShaderProgram();
    void destroy();

//private:
    // 初始化VAO,VBO,EBO
    void setupMesh();

    QOpenGLFunctions *gf;
    QOpenGLShaderProgram *program;
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    //GLuint VAO, VBO, EBO;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

};

#endif // MESH_H
