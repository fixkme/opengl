#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "stb_image.h"

class Model
{
public:
	Model(GLchar *path, Shader shader);
	void Draw();
	glm::mat4 tranform;
	Shader getShader(){ return this->shader; }

private:
	/*  模型数据  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;
	Shader shader;

	/*  私有成员函数   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint TextureFromFile(const char* path, string directory);
};

#endif