#include "mesh.h"

// 构造函数
Mesh::Mesh(QOpenGLFunctions *gf, QOpenGLShaderProgram *p, vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{

    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->gf = gf;
    this->program = p;
    // 初始化VAO,VBO,EBO
    this->setupMesh();
}

//析构函数
Mesh::~Mesh()
{
    //qDebug()<<"distruct !!!"<<arrayBuf.isCreated();
}

void Mesh::destroy()
{
    this->arrayBuf.destroy();
    this->indexBuf.destroy();
}

//渲染mesh
void Mesh::Draw()
{
    //qDebug()<<"mesh draw"<<program->programId();
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;

    this->program->setUniformValue(this->program->uniformLocation("viewPos"),
                                   0.f,1.f,3.f);
    this->program->setUniformValue(this->program->uniformLocation("light.direction"),
                                   0.f,10.f,-3.f);
    this->program->setUniformValue(this->program->uniformLocation("light.ambient"),
                                   0.5f, 0.5f, 0.5f);
    this->program->setUniformValue(this->program->uniformLocation("light.diffuse"),
                                   0.5f, 0.5f, 0.5f);
    this->program->setUniformValue(this->program->uniformLocation("light.specular"),
                                   1.0f, 1.0f, 1.0f);
    this->program->setUniformValue(this->program->uniformLocation("material.shininess"),
                                   32.0f);
    for (GLuint i = 0; i < this->textures.size(); i++)
    {
        gf->glActiveTexture(GL_TEXTURE0 + i);
        // 检索纹理序列号 (N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->textures[i].type;
        if (name == "texture_diffuse")
            ss << diffuseNr++; // 将GLuin输入到string stream
        else if (name == "texture_specular")
            ss << specularNr++; // 将GLuin输入到string stream
        number = ss.str();

        // 绑定纹理
        gf->glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        // 向着色器传递纹理
        gf->glUniform1i(gf->glGetUniformLocation(this->program->programId(), (name + number).c_str()), i);
    }

    // 绘制Mesh
    this->arrayBuf.bind();
    this->indexBuf.bind();
    // 顶点位置
    this->program->enableAttributeArray(0);
    this->program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
    // 法向量
    this->program->enableAttributeArray(1);
    this->program->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));
    // 纹理坐标
    this->program->enableAttributeArray(2);
    this->program->setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, texCoords), 2, sizeof(Vertex));
    gf->glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    //gf->glDrawArrays(GL_TRIANGLES, 0,  this->indices.size());
}

// 初始化VAO,VBO,EBO
void Mesh::setupMesh()
{
    //qDebug()<<"mesh program"<<this->program->programId();
    // 创建VAO,VBO,EBO
    //initializeOpenGLFunctions();
    this->arrayBuf = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->indexBuf = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    this->arrayBuf.create();
    this->indexBuf.create();
    this->program->bind();
    if (!arrayBuf.isCreated() || !indexBuf.isCreated())
        exit(-2);
    // 往VBO加载数据
    if (!this->arrayBuf.bind()) exit(-3);
    this->arrayBuf.allocate(&vertices[0], this->vertices.size() * sizeof(Vertex));
    if (!this->indexBuf.bind()) exit(-3);
    this->indexBuf.allocate(&indices[0], this->indices.size() * sizeof(GLuint));
    // 设置顶点属性
    // 顶点位置
    this->program->enableAttributeArray(0);
    this->program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
    // 法向量
    this->program->enableAttributeArray(1);
    this->program->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));
    // 纹理坐标
    this->program->enableAttributeArray(2);
    this->program->setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, texCoords), 2, sizeof(Vertex));
    //qDebug()<<"array buffer id"<<arrayBuf.bufferId()<<" "<<"index buffer id"<<indexBuf.bufferId();
}

void Mesh::setShaderProgram(QOpenGLShaderProgram *program)
{
    this->program = program;
}

QOpenGLShaderProgram * Mesh::getShaderProgram()
{
    return this->program;
}
