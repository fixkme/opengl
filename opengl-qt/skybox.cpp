#include "skybox.h"
#include "stb_image.h"
#include <qdebug.h>

SkyBox::SkyBox()
{
    faceIamges.push_back(":/textures/right.jpg");
    faceIamges.push_back(":/textures/left.jpg");
    faceIamges.push_back(":/textures/top.jpg");
    faceIamges.push_back(":/textures/bottom.jpg");
    faceIamges.push_back(":/textures/back.jpg");
    faceIamges.push_back(":/textures/front.jpg");
    //this->init();
}

SkyBox::SkyBox(QOpenGLShaderProgram *program, QOpenGLFunctions *gf)
{
    faceIamges.push_back(":/textures/right.jpg");
    faceIamges.push_back(":/textures/left.jpg");
    faceIamges.push_back(":/textures/top.jpg");
    faceIamges.push_back(":/textures/bottom.jpg");
    faceIamges.push_back(":/textures/back.jpg");
    faceIamges.push_back(":/textures/front.jpg");
    this->program = program;
    this->gf = gf;

    this->init();
}

SkyBox::~SkyBox()
{
    this->vbo.destroy();
}

void SkyBox::init()
{
    //vertex
    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    this->vbo.create();
    if (!this->vbo.bind()) exit(-3);
    this->vbo.allocate(skyboxVertices, sizeof(skyboxVertices));
    this->program->enableAttributeArray(0);
    this->program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));

    //textures
    int width,height,nrComponents;
    unsigned char* image;
    this->gf->glGenTextures(1, &textureID);
    this->gf->glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
    for(int i = 0; i < faceIamges.size(); i++)
    {
        image = stbi_load(faceIamges[i], &width, &height, &nrComponents, 0);
        this->gf->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    this->gf->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->gf->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    this->gf->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    this->gf->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    this->gf->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    this->gf->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    qDebug()<<"skybox program ID: "<<this->program->programId()
           <<" vbo id "<<vbo.bufferId()<<" sky texture id "<<this->textureID;
}

void SkyBox::draw()
{
    this->program->bind();
    this->gf->glDepthFunc(GL_LEQUAL);
    this->gf->glUniformMatrix4fv(program->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(this->view));
    this->gf->glUniformMatrix4fv(program->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(this->projection));

    this->vbo.bind();
    this->program->enableAttributeArray(0);
    this->program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    this->gf->glActiveTexture(GL_TEXTURE0);
    this->gf->glUniform1i(program->uniformLocation("skybox"), 0);
    this->gf->glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
    this->gf->glDrawArrays(GL_TRIANGLES, 0, 36);
    this->gf->glDepthFunc(GL_LESS);
}

void SkyBox::setData(glm::mat4 view, glm::mat4 projection)
{
    this->view = view;
    this->projection = projection;
}



