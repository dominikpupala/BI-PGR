//----------------------------------------------------------------------------------------
/**
 * \file       Objects.cpp
 * \author     Dominik Pupala
 * \date       2021/06/05
 * \brief      Source file for objects.
 *
 *  Source file containing declarations for objects used in application.
 *
*/
//----------------------------------------------------------------------------------------

#include "Curve.h"
#include "Objects.h"
#include "SkyboxData.h"
#include "PyramidGenerator.h"
#include "SpectateParameters.h"

#include <iostream>

Pyramid GeneratedPyramid, StonePyramid, QuartzPyramid;
LandScape Desert;
MeshData Sky;
Flora Aloe, Cactus0, Cactus1;
Rock Rock0, Rock1;
Plane InfiniteTexture, Billboard;
Car Player, Police;

Camera Spectate = Camera(glm::vec3(0.0f, 1.0f, 3.0f));
CameraSystem CameraManager;

void transformUniforms(Shader& shader, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
{
	glm::mat4 PVM = projection * view * model;
	glm::mat4 modelRotationMatrix = glm::mat4(
		model[0],
		model[1],
		model[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelRotationMatrix));

	shader.SetUniformMatrix4fv("pvmMatrix", 1, GL_FALSE, glm::value_ptr(PVM));
	shader.SetUniformMatrix4fv("viewMatrix", 1, GL_FALSE, glm::value_ptr(view));
	shader.SetUniformMatrix4fv("modelMatrix", 1, GL_FALSE, glm::value_ptr(model));
	shader.SetUniformMatrix4fv("normalMatrix", 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void materialUniforms(Shader& shader, const MeshData& object)
{
	shader.SetUniform3fv("material.Diffuse", 1, glm::value_ptr(object.Diffuse));
	shader.SetUniform3fv("material.Ambient", 1, glm::value_ptr(object.Ambient));
	shader.SetUniform3fv("material.Specular", 1, glm::value_ptr(object.Specular));
	shader.SetUniform1f("material.Shininess", object.Shininess);

	if (object.Texture != 0)
	{
		shader.SetUniform1i("texUse", 1);
		shader.SetUniform1i("texSampler", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, object.Texture);
	}
	else
	{
		shader.SetUniform1i("texUse", 0);
	}
}

void lightUniforms(Shader& shader, const Camera& camera)
{
	shader.SetUniform3fv("cameraPosition", 1, glm::value_ptr(camera.GetPosition()));

	shader.SetUniform3f("sunlight.Diffuse", 1.0f, 1.0f, 0.3f);
	shader.SetUniform3f("sunlight.Ambient", 0.13f, 0.13f, 0.13f);
	shader.SetUniform3f("sunlight.Specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("sunlight.Position", 13.873f, 35.399f, -21.242f);

	shader.SetUniform3f("spotlight.Diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("spotlight.Ambient", 0.13f, 0.13f, 0.13f);
	shader.SetUniform3f("spotlight.Specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("spotlight.Position", 4.47f, 2.69f, 1.2f);
	shader.SetUniform3f("spotlight.Direction", 3.0f, 1.3f, -1.0f);
	shader.SetUniform1f("spotlight.CutoffInn", 0.91f);
	shader.SetUniform1f("spotlight.CutoffOut", 0.82f);

	shader.SetUniform3f("pointlight.Diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("pointlight.Ambient", 0.13f, 0.13f, 0.13f);
	shader.SetUniform3f("pointlight.Specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("pointlight.Position", 0.0f, 10.0f, -13.0f);
	shader.SetUniform1f("pointlight.Constant", 0.05f);
	shader.SetUniform1f("pointlight.Linear", 0.09f);
	shader.SetUniform1f("pointlight.Quadratic", 0.0032f);
}

void fogUniforms(Shader& shader)
{
	shader.SetUniform3f("fog.Color", 1.0f, 0.85f, 0.75f);
	shader.SetUniform1f("fog.Density", 0.07f + 0.03f * cos(CameraManager.CurrentTime / 3));
	shader.SetUniform1f("fog.Gradient", 0.5f + 0.75f * sin(CameraManager.CurrentTime / 3));
}

void setBuffers(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, const std::vector<GLuint>& sequence, MeshData& outObject)
{
	outObject.VB = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	outObject.EB = new ElementBuffer(&indices[0], indices.size());

	outObject.VBL = new VertexBufferLayout();

	for (size_t i = 0; i < sequence.size(); ++i)
		outObject.VBL->Push<float>(sequence[i]);

	outObject.VAO = new VertexArray();
	outObject.VAO->AddBuffer(*outObject.VB, *outObject.VBL);
}

void loadMeshGeometry(const aiMesh& mesh, MeshData& outObject)
{
	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;

	for (size_t i = 0; i < mesh.mNumVertices; ++i)
	{
		vertices.push_back(mesh.mVertices[i].x);
		vertices.push_back(mesh.mVertices[i].y);
		vertices.push_back(mesh.mVertices[i].z);

		if (mesh.HasNormals())
		{
			vertices.push_back(mesh.mNormals[i].x);
			vertices.push_back(mesh.mNormals[i].y);
			vertices.push_back(mesh.mNormals[i].z);
		}

		if (mesh.HasTextureCoords(0))
		{
			vertices.push_back(mesh.mTextureCoords[0][i].x);
			vertices.push_back(mesh.mTextureCoords[0][i].y);
		}
		else
		{
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}

	for (size_t i = 0; i < mesh.mNumFaces; ++i)
	{
		indices.push_back(mesh.mFaces[i].mIndices[0]);
		indices.push_back(mesh.mFaces[i].mIndices[1]);
		indices.push_back(mesh.mFaces[i].mIndices[2]);
	}

	setBuffers(vertices, indices, { 3, 3, 2 }, outObject);
}

void loadMeshMaterial(const aiMaterial& material, const std::string& path, MeshData& outObject)
{
	aiColor4D color;
	aiString name;
	ai_real shininess, strength;
	uint32_t max;

	material.Get(AI_MATKEY_NAME, name);

	if (aiGetMaterialColor(&material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
		outObject.Diffuse = glm::vec3(color.r, color.g, color.b);

	if (aiGetMaterialColor(&material, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS)
		outObject.Ambient = glm::vec3(color.r, color.g, color.b);

	if (aiGetMaterialColor(&material, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS)
		outObject.Specular = glm::vec3(color.r, color.g, color.b);

	if (aiGetMaterialFloatArray(&material, AI_MATKEY_SHININESS, &shininess, &(max = 1)) != AI_SUCCESS)
		shininess = 1.0f;

	if (aiGetMaterialFloatArray(&material, AI_MATKEY_SHININESS_STRENGTH, &strength, &(max = 1)) != AI_SUCCESS)
		strength = 1.0f;

	outObject.Shininess = shininess * strength;
	outObject.Texture = 0;

	if (material.GetTextureCount(aiTextureType_DIFFUSE) < 1)
		return;

	aiString temp;
	aiReturn texFound = material.GetTexture(aiTextureType_DIFFUSE, 0, &temp);
	std::string textureName = temp.data;

	size_t found = path.find_last_of("/\\");

	if (found != std::string::npos)
		textureName.insert(0, path.substr(0, found + 1));

	outObject.Texture = pgr::createTexture(textureName);
}

bool loadMesh(const std::string& path, MeshData& outObject)
{
	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);
	const aiScene* scn = importer.ReadFile(path.c_str(), 0 | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (scn == NULL || scn->mNumMeshes != 1)
		return false;

	loadMeshGeometry(*scn->mMeshes[0], outObject);
	loadMeshMaterial(*scn->mMaterials[scn->mMeshes[0]->mMaterialIndex], path, outObject);

	return true;
}

void drawLights(Shader& shader)
{
	shader.Bind();
	lightUniforms(shader, *CameraManager.Current);
}

void drawFog(Shader& shader)
{
	shader.Bind();
	fogUniforms(shader);
}

void initSky()
{
	std::string path = "data/skybox";
	Sky.VB = new VertexBuffer(&SkyboxVertices[0], SkyboxVertices.size() * sizeof(float));

	Sky.VBL = new VertexBufferLayout();
	Sky.VBL->Push<float>(3);

	Sky.VAO = new VertexArray();
	Sky.VAO->AddBuffer(*Sky.VB, *Sky.VBL);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &Sky.Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Sky.Texture);

	GLuint targets[] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < SkyboxSuffix.size(); ++i)
	{
		std::string texture = path + "_" + SkyboxSuffix[i] + ".png";
		std::cout << "Loading cube map texture: " << texture << std::endl;

		if (!pgr::loadTexImage2D(texture, targets[i]))
			pgr::dieWithError("Skybox cube map loading failed!");
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void drawSky(const glm::mat4& projection, const glm::mat4& view, Shader& shader)
{
	glDepthMask(GL_FALSE);

	glm::mat4 v = glm::mat4(glm::mat3(view));

	shader.Bind();
	shader.SetUniformMatrix4fv("viewMatrix", 1, GL_FALSE, glm::value_ptr(v));
	shader.SetUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, glm::value_ptr(projection));

	Sky.VAO->Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Sky.Texture);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
}

void initGeneratedPyramid()
{
	PyramidData p = PyramidGenerator::Generate(30);

	GeneratedPyramid.Position = glm::vec3(0.0f, 0.0f, -15.0f);
	GeneratedPyramid.Scale = glm::vec3(5.0f);

	GeneratedPyramid.Animate = false;

	GeneratedPyramid.Diffuse = glm::vec3(1.0f, 0.55f, 0.172f);
	GeneratedPyramid.Ambient = glm::vec3(0.251208007f);
	GeneratedPyramid.Specular = glm::vec3(0.700483024f);
	GeneratedPyramid.Shininess = 3.82f;

	setBuffers(p.Vertices, p.Triangles, { 3, 3 }, GeneratedPyramid);
}

void drawGeneratedPyramid(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer, float elapsedTime)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, GeneratedPyramid.Position + glm::vec3(0.0f, GeneratedPyramid.Scale.y, 0.0f));
	model = glm::scale(model, GeneratedPyramid.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, GeneratedPyramid);
	shader.SetUniform1f("alpha", GeneratedPyramid.Animate ? 0.5f * sin(elapsedTime) : 0.0f);

	renderer.Draw(*GeneratedPyramid.VAO, *GeneratedPyramid.EB, shader, GL_TRIANGLES);
}

void clickGeneratedPyramid()
{
	GeneratedPyramid.Animate = !GeneratedPyramid.Animate;
}

void initStonePyramid()
{
	PyramidData p = PyramidGenerator::Generate(6);

	StonePyramid.Position = glm::vec3(14.0f, 0.0f, 0.0f);
	StonePyramid.Scale = glm::vec3(3.0f, 2.0f, 3.0f);

	StonePyramid.Animate = false;
	StonePyramid.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	StonePyramid.Ambient = glm::vec3(0.215f, 0.2345f, 0.215f);
	StonePyramid.Specular = glm::vec3(0.633f, 0.727811f, 0.633f);

	StonePyramid.Shininess = 76.8f;

	setBuffers(p.Vertices, p.Triangles, { 3, 3 }, StonePyramid);
}

void drawStonePyramid(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, StonePyramid.Position + glm::vec3(0.0f, StonePyramid.Scale.y, 0.0f));
	model = glm::scale(model, StonePyramid.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, StonePyramid);

	renderer.Draw(*StonePyramid.VAO, *StonePyramid.EB, shader, GL_TRIANGLES);
}

void initQuartzPyramid()
{
	PyramidData p = PyramidGenerator::Generate(80);

	QuartzPyramid.Position = glm::vec3(-2.0f, 0.0f, 17.0f);
	QuartzPyramid.Scale = glm::vec3(7.0f);

	QuartzPyramid.Animate = false;
	QuartzPyramid.Diffuse = glm::vec3(1.0f, 0.829f, 0.829f);
	QuartzPyramid.Ambient = glm::vec3(0.25f, 0.20725f, 0.20725f);
	QuartzPyramid.Specular = glm::vec3(0.296648f, 0.296648f, 0.296648f);

	QuartzPyramid.Shininess = 11.264f;

	setBuffers(p.Vertices, p.Triangles, { 3, 3 }, QuartzPyramid);
}

void drawQuartzPyramid(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, QuartzPyramid.Position + glm::vec3(0.0f, QuartzPyramid.Scale.y, 0.0f));
	model = glm::scale(model, QuartzPyramid.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, QuartzPyramid);

	renderer.Draw(*QuartzPyramid.VAO, *QuartzPyramid.EB, shader, GL_TRIANGLES);
}

void initDesert()
{
	Desert.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Desert.Scale = glm::vec3(30.0f);

	if (!loadMesh("data/desert_test.obj", Desert))
	{
		std::cout << "initializing desert has failed!" << std::endl;
		return;
	}
}

void drawDesert(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Desert.Position + glm::vec3(0.0f, Desert.Scale.y / 10.0f, 0.0f));
	model = glm::scale(model, Desert.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, Desert);

	renderer.Draw(*Desert.VAO, *Desert.EB, shader, GL_TRIANGLES);
}

void initAloe()
{
	Aloe.Position = glm::vec3(-14.5f, 0.0f, -1.0f);
	Aloe.Scale = glm::vec3(0.35f);

	if (!loadMesh("data/aloe.obj", Aloe))
	{
		std::cout << "initializing aloe has failed!" << std::endl;
		return;
	}
}

void drawAloe(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Aloe.Position + glm::vec3(0.0f, Aloe.Scale.y / 2.0f, 0.0f));
	model = glm::scale(model, Aloe.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, Aloe);

	renderer.Draw(*Aloe.VAO, *Aloe.EB, shader, GL_TRIANGLES);
}

void initCactus0()
{
	Cactus0.Position = glm::vec3(-22.0f, 0.0f, -1.0f);
	Cactus0.Scale = glm::vec3(0.5f);

	if (!loadMesh("data/cactus00.obj", Cactus0))
	{
		std::cout << "initializing cactus has failed!" << std::endl;
		return;
	}
}

void drawCactus0(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Cactus0.Position + glm::vec3(0.0f, Cactus0.Scale.y / 2.0f, 0.0f));
	model = glm::scale(model, Cactus0.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, Cactus0);

	renderer.Draw(*Cactus0.VAO, *Cactus0.EB, shader, GL_TRIANGLES);
}

void initCactus1()
{
	Cactus1.Position = glm::vec3(-22.0f, 0.0f, 5.0f);
	Cactus1.Scale = glm::vec3(0.25f);

	if (!loadMesh("data/cactus01.obj", Cactus1))
	{
		std::cout << "initializing cactus has failed!" << std::endl;
		return;
	}
}

void drawCactus1(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Cactus1.Position + glm::vec3(0.0f, Cactus1.Scale.y, 0.0f));
	model = glm::scale(model, Cactus1.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, Cactus1);

	renderer.Draw(*Cactus1.VAO, *Cactus1.EB, shader, GL_TRIANGLES);
}

void initRock0()
{
	Rock0.Position = glm::vec3(-15.0f, 0.0f, 5.0f);
	Rock0.Scale = glm::vec3(0.5f);

	if (!loadMesh("data/rock00.obj", Rock0))
	{
		std::cout << "initializing rock0 has failed!" << std::endl;
		return;
	}

	Rock0.Index = 0;
	Rock0.Diffuses =
	{
		Rock0.Diffuse,
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	};
	Rock0.Diffuse = Rock0.Diffuses[0];
}

void drawRock0(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Rock0.Position + glm::vec3(0.0f, Rock0.Scale.y / 2.0f, 0.0f));
	model = glm::scale(model, Rock0.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, Rock0);

	renderer.Draw(*Rock0.VAO, *Rock0.EB, shader, GL_TRIANGLES);
}

void clickRock0()
{
	Rock0.Diffuse = Rock0.Diffuses[++Rock0.Index % Rock0.Diffuses.size()];
}

void initRock1()
{
	Rock1.Position = glm::vec3(-18.0f, 0.0f, 2.0f);
	Rock1.Scale = glm::vec3(0.8f);

	if (!loadMesh("data/rock01.obj", Rock1))
	{
		std::cout << "initializing rock1 has failed!" << std::endl;
		return;
	}

	Rock1.Diffuse = glm::vec3(0.8f, 0.0f, 0.6f);
}

void drawRock1(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Rock1.Position + glm::vec3(0.0f, Rock1.Scale.y / 2.0f, 0.0f));
	model = glm::scale(model, Rock1.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, Rock1);

	renderer.Draw(*Rock1.VAO, *Rock1.EB, shader, GL_TRIANGLES);

	glDisable(GL_BLEND);
}

void initInfiniteTexture()
{
	InfiniteTexture.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	InfiniteTexture.Scale = glm::vec3(1.0f);

	InfiniteTexture.Animate = false;

	if (!loadMesh("data/plane.obj", InfiniteTexture))
	{
		std::cout << "initializing infinite texture has failed!" << std::endl;
		return;
	}

	InfiniteTexture.Diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
}

void drawInfiniteTexture(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer, float elapsedTime)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, InfiniteTexture.Position + glm::vec3(0.0f, 0.02f, 0.0f));
	model = glm::scale(model, InfiniteTexture.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, InfiniteTexture);
	shader.SetUniform1f("time", InfiniteTexture.Animate ? elapsedTime : 0);

	renderer.Draw(*InfiniteTexture.VAO, *InfiniteTexture.EB, shader, GL_TRIANGLES);
}

void clickInfiniteTexture()
{
	InfiniteTexture.Animate = !InfiniteTexture.Animate;
}

void initBillboard()
{
	Billboard.Position = glm::vec3(-8.0f, 0.0f, -12.0f);
	Billboard.Scale = glm::vec3(0.7f);

	Billboard.Animate = true;

	if (!loadMesh("data/billboard.obj", Billboard))
	{
		std::cout << "initializing billboard texture has failed!" << std::endl;
		return;
	}
}

void drawBillboard(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer, float elapsedTime)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Billboard.Position + glm::vec3(0.0f, Billboard.Scale.y, 0.0f));
	model = glm::scale(model, Billboard.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, Billboard);
	shader.SetUniform1f("time", Billboard.Animate ? elapsedTime : 0);

	renderer.Draw(*Billboard.VAO, *Billboard.EB, shader, GL_TRIANGLES);
}

void initPlayer()
{
	Player.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Player.Scale = glm::vec3(0.5f);

	Player.Yaw = 0.0f;
	Player.Pitch = 0.0f;
	Player.Speed = 0.0f;

	if (!loadMesh("data/car.obj", Player))
	{
		std::cout << "initializing player car has failed!" << std::endl;
		return;
	}
}

void drawPlayer(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Player.Position + glm::vec3(0.0f, -0.05f + Player.Scale.y / 2.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-Player.Yaw - 180.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, Player.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, Player);

	renderer.Draw(*Player.VAO, *Player.EB, shader, GL_TRIANGLES);
}

void increaseSpeedPlayer()
{
	Player.Speed = std::min(Player.Speed + 1.5f * Car::SPEED, 4.8f);
}

void decreaseSpeedPlayer()
{
	Player.Speed = std::max(Player.Speed - Car::SPEED, -1.3f);
}

void turnLeftPlayer()
{
	if (Player.Speed == 0.0f)
		return;

	if (Player.Speed < 0.0f)
	{
		Player.Yaw += Car::STEER;
		if (Player.Yaw > 360.0f)
			Player.Yaw -= 360.0f;
	}
	else
	{
		Player.Yaw -= Car::STEER;
		if (Player.Yaw < 0.0f)
			Player.Yaw += 360.0f;
	}
}

void turnRightPlayer()
{
	if (Player.Speed == 0.0f)
		return;

	if (Player.Speed < 0.0f)
	{
		Player.Yaw -= Car::STEER;
		if (Player.Yaw < 0.0f)
			Player.Yaw += 360.0f;
	}
	else
	{
		Player.Yaw += Car::STEER;
		if (Player.Yaw > 360.0f)
			Player.Yaw -= 360.0f;
	}
}

void updatePlayer(float elapsedTime)
{
	movePlayer(elapsedTime);

	Player.Direction = glm::normalize(glm::vec3(
		cos(glm::radians(Player.Yaw)) * cos(glm::radians(Player.Pitch)),
		sin(glm::radians(Player.Pitch)),
		sin(glm::radians(Player.Yaw)) * cos(glm::radians(Player.Pitch))
	));

	// decceleration
	if (Player.Speed != 0.0f)
	{
		Player.Speed += Player.Speed > 0 ? -0.06f : 0.06f;

		if (abs(Player.Speed) < 0.03f)
			Player.Speed = 0;
	}

	Player.Cam.SetPosition(glm::vec3(0.0f, 0.375f, 0.0f) + Player.Position);
	Player.Cam.SetDirection(Player.Direction);
}

void movePlayer(float elapsedTime)
{
	glm::vec3 temp = Player.Position;
	float timeDelta = elapsedTime - Player.CurrentTime;
	Player.CurrentTime = elapsedTime;
	Player.Position += timeDelta * Player.Speed * Player.Direction;

	if (Player.Position.x < -13.0f || Player.Position.x > 25.0f ||
		Player.Position.y < -0.029f || Player.Position.y > 0.029f ||
		Player.Position.z < -9.5f || Player.Position.z > 9.0f ||
		glm::distance(Player.Position, StonePyramid.Position) <= 0.65f + 3.0f)
	{
		Player.Position = temp;
		Player.Speed = 0.0f;
	}
}

void switchToPlayer()
{
	CameraManager.CanLook = false;
	CameraManager.CanMove = false;
	CameraManager.IsStatic = false;
	CameraManager.SwitchTo(&Player.Cam);
}

void initPolice()
{
	Police.Scale = glm::vec3(0.5f);

	Police.Yaw = 0.0f;
	Police.Pitch = 0.0f;
	Police.Speed = 0.0f;

	if (!loadMesh("data/police.obj", Police))
	{
		std::cout << "initializing police car has failed!" << std::endl;
		return;
	}
}

void drawPolice(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = Curve::AlignObject(Police.Position, Police.Direction);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	model = glm::scale(model, Police.Scale);

	shader.Bind();
	transformUniforms(shader, projection, view, model);
	materialUniforms(shader, Police);

	renderer.Draw(*Police.VAO, *Police.EB, shader, GL_TRIANGLES);
}

void updatePolice(float elapsedTime)
{
	float timeDelta = elapsedTime - Police.CurrentTime;
	Police.CurrentTime = elapsedTime;

	static std::vector<glm::vec3> curve =
	{
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
	};

	static float speed = 0.3f;
	static glm::mat4 base = Curve::BasisMatrix(15.0f);
	static glm::vec3 origin = glm::vec3(-18.0f, -0.05f + Player.Scale.y / 2.0f, 2.0f);

	Police.Position = origin + Curve::EvalCurve(curve, base, Police.CurrentTime * speed);
	Police.Direction = glm::normalize(Curve::EvalCurveDerivate(curve, base, Police.CurrentTime * speed));

	Police.Cam.SetPosition(glm::vec3(0.0f, 0.132f, 0.0f) + Police.Position);
	Police.Cam.SetDirection(Police.Direction);
}

void switchToPolice()
{
	CameraManager.CanLook = false;
	CameraManager.CanMove = false;
	CameraManager.IsStatic = false;
	CameraManager.SwitchTo(&Police.Cam);
}

void updateSpectate(float elapsedTime)
{
	float timeDelta = elapsedTime - Police.CurrentTime;
	CameraManager.CurrentTime = elapsedTime;

	static std::vector<glm::vec3> curve = SPECTATE_CONTROL_POINTS;

	static float speed = SPECTATE_SPEED;
	static glm::mat4 base = Curve::BasisMatrix(SPECTATE_CR_PARAMETER);
	static glm::vec3 origin = SPECTATE_ORIGIN;

	Spectate.SetPosition(origin + Curve::EvalCurve(curve, base, CameraManager.CurrentTime * speed));
	Spectate.SetDirection(glm::normalize(Curve::EvalCurveDerivate(curve, base, CameraManager.CurrentTime * speed)));
}

void switchToSpectate()
{
	CameraManager.CanLook = false;
	CameraManager.CanMove = false;
	CameraManager.IsStatic = false;
	CameraManager.SwitchTo(&Spectate);
}