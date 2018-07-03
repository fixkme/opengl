

#include "Mesh.h"

// 构造函数
Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// 初始化VAO,VBO,EBO
	this->setupMesh();
}

//析构函数
Mesh::~Mesh()
{

}

//渲染mesh
void Mesh::Draw()
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
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
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		// 向着色器传递纹理
		glUniform1i(glGetUniformLocation(shader.getProgram(), (name + number).c_str()), i);
	}

	//glUniform1f(glGetUniformLocation(shader.getProgram(), "material.shininess"), 16.0f);

	// 绘制Mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	glBindVertexArray(0);

}

// 初始化VAO,VBO,EBO
void Mesh::setupMesh()
{
	// 创建VAO,VBO,EBO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	// 往VBO加载数据
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
	// 往EBO加载数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// 设置顶点属性
	// 顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// 法向量
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// 纹理坐标
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

void Mesh::setShader(Shader shader)
{
	this->shader = shader;
}
Shader Mesh::getShader()
{
	return shader;
}


