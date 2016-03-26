#pragma once

#include "Shader.h"
#include "Mesh.h"
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
// GL Includes
#include "glew.h" // Contains all the necessery OpenGL includes
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "src/SOIL.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;


GLint TextureFromFile(const char* path, string directory);


class Model
{
public:
	/*  Functions   */
	Model(GLchar* path);
	void Draw(Shader shader);
private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;
	/*  Functions   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};









