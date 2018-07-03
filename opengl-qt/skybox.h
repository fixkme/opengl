#ifndef SKYBOX_H
#define SKYBOX_H

#include <QVector>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

QT_FORWARD_DECLARE_CLASS(GLWidget)

class SkyBox: public QOpenGLFunctions
{
public:
    SkyBox();
    SkyBox(QOpenGLShaderProgram *program, QOpenGLFunctions *gf);
    ~SkyBox();
    void init();
    void draw();
    void setData(glm::mat4 view, glm::mat4 projection);
private:
    QOpenGLFunctions *gf;
    QOpenGLShaderProgram *program;
    QVector<const GLchar*> faceIamges;
    GLuint textureID;
    QOpenGLBuffer vbo;

    glm::mat4 view;
    glm::mat4 projection;
};

#endif // SKYBOX_H
