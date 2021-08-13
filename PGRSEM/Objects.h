//----------------------------------------------------------------------------------------
/**
 * \file       Objects.h
 * \author     Dominik Pupala
 * \date       2021/06/05
 * \brief      Header file for objects.
 *
 *  Source file containing definitions for objects used in application.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include "Shader.h"
#include "Renderer.h"
#include "MeshData.h"
#include "CameraSystem.h"

/// Struct that wrapps additional context for pyramid.
/**
  This struct contains data context for pyramid.
*/
struct Pyramid : public MeshData
{
	glm::vec3 Position;
	glm::vec3 Scale;

	bool Animate;
};
/// Struct that wrapps additional context for landscape.
/**
  This struct contains data context for landscape.
*/
struct LandScape : public MeshData
{
	glm::vec3 Position;
	glm::vec3 Scale;
};
/// Struct that wrapps additional context for flora.
/**
  This struct contains data context for flora.
*/
struct Flora : public MeshData
{
	glm::vec3 Position;
	glm::vec3 Scale;
};
/// Struct that wrapps additional context for rock.
/**
  This struct contains data context for rock.
*/
struct Rock : public MeshData
{
	glm::vec3 Position;
	glm::vec3 Scale;

	size_t Index;
	std::vector<glm::vec3> Diffuses;
};
/// Struct that wrapps additional context for plane.
/**
  This struct contains data context for plane.
*/
struct Plane : public MeshData
{
	glm::vec3 Position;
	glm::vec3 Scale;

	bool Animate;
};
/// Struct that wrapps additional context for car.
/**
  This struct contains data context for car.
*/
struct Car : public MeshData
{
	static constexpr float SPEED = 0.1f;
	static constexpr float STEER = 2.5f;

	glm::vec3 Direction;
	glm::vec3 Position;
	glm::vec3 Scale;

	Camera Cam = glm::vec3(0.0f);

	float Yaw;
	float Pitch;
	float Speed;
	float CurrentTime;
};
/// Transform uniform setup
/**
  Sets up transform matrices via uniforms.

  \param[in] shader			Target shader.
  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] model			Objects model matrix.
*/
void transformUniforms(Shader& shader, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
/// Material uniform setup
/**
  Sets up material properties of a object via uniforms.

  \param[in] shader		Target shader.
  \param[in] object		Source object.
*/
void materialUniforms(Shader& shader, const MeshData& object);
/// Light uniform setup
/**
  Sets up global light properties via uniforms.

  \param[in] shader		Target shader.
  \param[in] camera		Active camera.
*/
void lightUniforms(Shader& shader, const Camera& camera);
/// Fog uniform setup
/**
  Sets up global fog properties via uniforms.

  \param[in] shader		Target shader.
*/
void fogUniforms(Shader& shader);
/// Typical OpenGL context setup 
/**
  Sets up OpenGL context of a object the usual way.

  \param[in] vertices		Vertex data for target object.
  \param[in] indices		Face data for target object.
  \param[in] sequence		Sequence of sizes of the layout for the new target object.
  \param[out] outObject		Target object to be setup.
*/
void setBuffers(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, const std::vector<GLuint>& sequence, MeshData& outObject);
/// Load mesh from assimp context.
/**
  Loads geometry of the mesh from assimp context.

  \param[in] mesh			Assimp context.
  \param[out] outObject		Target object to be setup.
*/
void loadMeshGeometry(const aiMesh& mesh, MeshData& outObject);
/// Load material from assimp context.
/**
  Loads material of the object from assimp context.

  \param[in] material		Assimp context.
  \param[in] path			Path context.
  \param[out] outObject		Target object to be setup.
*/
void loadMeshMaterial(const aiMaterial& material, const std::string& path, MeshData& outObject);
/// Load object from wavefront file.
/**
  Loads object from the file using assimp.

  \param[in] path			Path context.
  \param[out] outObject		Target object to be setup.
*/
bool loadMesh(const std::string& path, MeshData& outObject);
/// Draw lights using shader.
/**
  Sets up the shader with lighting.

  \param[in] shader		Target shader.
*/
void drawLights(Shader& shader);
/// Draw fog using shader.
/**
  Sets up the shader with fog.

  \param[in] shader		Target shader.
*/
void drawFog(Shader& shader);
/// Initialize skybox.
/**
  Initializes skybox.
*/
void initSky();
/// Draw skybox.
/**
  Draws skybox.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
*/
void drawSky(const glm::mat4& projection, const glm::mat4& view, Shader& shader);
/// Initialize generated pyramid.
/**
  Initializes generated pyramid.
*/
void initGeneratedPyramid();
/// Draw generated pyramid.
/**
  Draws generated pyramid.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
  \param[in] elapsedTime	Time context.
*/
void drawGeneratedPyramid(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer, float elapsedTime);
/// Handle click on generated pyramid.
/**
  Handles click on generated pyramid.
*/
void clickGeneratedPyramid();
/// Initialize stone pyramid.
/**
  Initializes stone pyramid.
*/
void initStonePyramid();
/// Draw stone pyramid.
/**
  Draws stone pyramid.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawStonePyramid(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Initialize quartz pyramid.
/**
  Initializes quartz pyramid.
*/
void initQuartzPyramid();
/// Draw quartz pyramid.
/**
  Draws quartz pyramid.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawQuartzPyramid(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Initialize desert.
/**
  Initializes desert.
*/
void initDesert();
/// Draw desert.
/**
  Draws desert.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawDesert(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Initialize aloe.
/**
  Initializes aloe.
*/
void initAloe();
/// Draw aloe.
/**
  Draws aloe.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawAloe(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Initialize cactus0.
/**
  Initializes cactus0.
*/
void initCactus0();
/// Draw cactus0.
/**
  Draws cactus0.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawCactus0(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Initialize cactus1.
/**
  Initializes cactus1.
*/
void initCactus1();
/// Draw cactus1.
/**
  Draws cactus1.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawCactus1(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Initialize rock0.
/**
  Initializes rock0.
*/
void initRock0();
/// Draw rock0.
/**
  Draws rock0.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawRock0(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Handle click on rock0.
/**
  Handles click on rock0.
*/
void clickRock0();
/// Initialize rock1.
/**
  Initializes rock1.
*/
void initRock1();
/// Draw rock1.
/**
  Draws rock1.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawRock1(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Initialize infinite texture.
/**
  Initializes infinite texture.
*/
void initInfiniteTexture();
/// Draw infinite texture.
/**
  Draws infinite texture.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
  \param[in] elapsedTime	Time context.
*/
void drawInfiniteTexture(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer, float elapsedTime);
/// Handle click on infinite texture.
/**
  Handles click on infinite texture.
*/
void clickInfiniteTexture();
/// Initialize billboard.
/**
  Initializes billboard.
*/
void initBillboard();
/// Draw billboard.
/**
  Draws billboard.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
  \param[in] elapsedTime	Time context.
*/
void drawBillboard(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer, float elapsedTime);
/// Initialize player.
/**
  Initializes player.
*/
void initPlayer();
/// Draw player.
/**
  Draws player.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawPlayer(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Increase player's speed.
/**
  Increases player's speed.
*/
void increaseSpeedPlayer();
/// Decreases player's speed.
/**
  Decreases player's speed.
*/
void decreaseSpeedPlayer();
/// Turn player left.
/**
  Turns player left.
*/
void turnLeftPlayer();
/// Turn player right.
/**
  Turns player right.
*/
void turnRightPlayer();
/// Update player.
/**
  Updates player.

  \param[in] elapsedTime	Time context.
*/
void updatePlayer(float elapsedTime);
/// Move player.
/**
  Moves player, including collision and boundary checks.

  \param[in] elapsedTime	Time context.
*/
void movePlayer(float elapsedTime);
/// Switch to player.
/**
  Switches to player's camera.
*/
void switchToPlayer();
/// Initialize police.
/**
  Initializes police.
*/
void initPolice();
/// Draw police.
/**
  Draws police.

  \param[in] projection		Global projection matrix.
  \param[in] view			Global view matrix.
  \param[in] shader			Target shader.
  \param[in] renderer		Target renderer.
*/
void drawPolice(const glm::mat4& projection, const glm::mat4& view, Shader& shader, const Renderer& renderer);
/// Update police.
/**
  Updates police.

  \param[in] elapsedTime	Time context.
*/
void updatePolice(float elapsedTime);
/// Switch to police.
/**
  Switches to police's camera.
*/
void switchToPolice();
/// Update spectate.
/**
  Updates spectating camera.

  \param[in] elapsedTime	Time context.
*/
void updateSpectate(float elapsedTime);
/// Switch to specate.
/**
  Switches to spactating camera.
*/
void switchToSpectate();
