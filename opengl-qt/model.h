#ifndef MODEL_H
#define MODEL_H

#include <QThread>
#include <QString>
#include "Mesh.h"
#include "stb_image.h"

class Model : public QThread
{
    //Q_OBJECT

public:
    Model(string path, QOpenGLFunctions *gf, QOpenGLShaderProgram *program);
    void Draw();
    glm::mat4 tranform;
    QOpenGLShaderProgram *getShaderProgram(){ return this->program; }
    bool isLoaded(){ return this->loaded; }
    void destroy();
    void run() override;

//private:

    string path;
    bool loaded;
    /*  模型数据  */
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;
    QOpenGLShaderProgram *program;
    QOpenGLFunctions *gf;

    /*  私有成员函数   */
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    GLint TextureFromFile(const char* path, string directory);
};

#endif // MODEL_H
