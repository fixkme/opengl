
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "Model.h"
#include "qdebug.h"


// 构造函数
Model::Model(string path, QOpenGLFunctions *gf, QOpenGLShaderProgram *program)
{
    loaded = false;
    this->path = path;
    this->gf = gf;
    this->program = program;
    this->loadModel(this->path);
    //this->start();
}

void Model::destroy()
{
    for (GLuint i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].destroy();
    }
}

void Model::run()
{
    this->loadModel(this->path);
}

// 渲染模型
void Model::Draw()
{
    if (!loaded)
        return;
    for (GLuint i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].Draw();
    }
}

// 加载模型
void Model::loadModel(string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // 是否加载成功
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }

    this->directory = path.substr(0, path.find_last_of('/'));
    //qDebug()<<path.c_str()<<" --- "<<directory.c_str();
    this->processNode(scene->mRootNode, scene);
    this->loaded = true;
    qDebug()<<path.c_str()<<"load complete!";
}

//处理节点
void Model::processNode(aiNode* node, const aiScene* scene) {
    //处理节点的所有网格信息

    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene));
    }
    //对所有的子节点做相同处理
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

//处理网格
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    // 获得顶点属性
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        // 顶点坐标
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
        //vertex.position = vector;
        // 法向量
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;
        //vertex.normal = vector;
        // 纹理坐标
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }//vbo_________________________
    // 获得顶点索引EBO
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }//ebo ________________________
/*    for (int i=0; i < vertices.size(); i++){
        Vertex vertex = vertices[i];
        qDebug()<<i<<": "<<vertex.position.x<<" "<<vertex.position.y<<" "<<vertex.position.z<<" "
               <<vertex.normal.x<<" "<<vertex.normal.y<<" "<<vertex.normal.z<<" "
               <<vertex.texCoords.x<<" "<<vertex.texCoords.y<<" ";
    }
    for (int i=0; i<indices.size(); i++)
    {
        qDebug()<<indices[i];
    }
*/    // 获得材质（纹理）
    if (mesh->mMaterialIndex >= 0)
    {
        //cout <<mesh->mName.C_Str()<< "有材质" << endl;
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. Diffuse maps
        vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. Specular maps
        vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    else
    {
        qDebug() << mesh->mName.C_Str() << "no textures.";
    }
    //qDebug()<<"processMeshed "<<"vertices num "<<vertices.size()<<" indices num "<<indices.size();
    Mesh myMesh(this->gf, this->program, vertices, indices, textures);
    //qDebug()<<"processMesh";
    return myMesh;
}

// 加载文理
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        GLboolean skip = false;
        for (GLuint j = 0; j < textures_loaded.size(); j++)
        {
            if (textures_loaded[j].path == str)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // 如果纹理没有被加载过，加载之
            Texture texture;
            texture.id = this->TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            this->textures_loaded.push_back(texture);  // 添加到纹理列表 textures
        }
    }
    return textures;
}

GLint Model::TextureFromFile(const char* path, string directory)
{
    //Generate texture ID and load texture data
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    this->gf->glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        this->gf->glBindTexture(GL_TEXTURE_2D, textureID);
        this->gf->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        this->gf->glGenerateMipmap(GL_TEXTURE_2D);

        this->gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        this->gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        this->gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        this->gf->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        qDebug()<<"loaded texture fail: "<< path;
        stbi_image_free(data);
    }

    return textureID;
}


