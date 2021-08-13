#include "CameraSystem.h"
#include "Objects.h"
#include "AppParameters.h"

// [1-3] Keys
#define KEY_1 43
#define KEY_2 236
#define KEY_3 154

glm::mat4 View; ///< Global view matrix
glm::mat4 Projection; ///< Global projection matrix

// Shaders
Shader* SkyboxShader;
Shader* ObjectShader;
Shader* PyramidShader;
Shader* InfiniteShader;
Shader* BillboardShader;
// Renderer
Renderer* CoreRenderer;

extern CameraSystem CameraManager; ///< Global app camera handler

/// Struct that wrapps application state context.
/**
  This struct contains data context for application state.
*/
struct AppState
{
	// logic state
	float ElapsedTime = 0.0f;

	// window state
	int Width = WIN_WIDTH;
	int Height = WIN_HEIGHT;

	// Input states
	enum { MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE, MOUSE_EVENT_COUNT };
	bool MouseMap[MOUSE_EVENT_COUNT];

	enum { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_EVENT_COUNT };
	bool KeyMap[KEY_EVENT_COUNT];

} AppState;
/// Evaluate clicked object.
/**
  Evaluates which object clicked was clicked.

  \param[in] x	Mouse position x.
  \param[in] y	Mouse position y.
*/
void setClickedObject(int x, int y)
{
	uint8_t clickedObjectID;
	glReadPixels(x, AppState.Height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &clickedObjectID);

	switch (clickedObjectID)
	{
	case 1:
		clickGeneratedPyramid();
		break;
	case 2:
		clickInfiniteTexture();
		break;
	case 3:
		clickRock0();
	default:
		break;
	}
}
/// Set time context.
/**
  Sets time context of the application.
*/
void setElapsedTime()
{
	AppState.ElapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
}
/// Set time context to the object.
/**
  Sets time context to the object.
*/
float setCurrentTime(float& objectTime)
{
	float temp = AppState.ElapsedTime - objectTime;
	objectTime = AppState.ElapsedTime;

	return temp;
}
/// Initialize application resources.
/**
  Initializes the application resources.
*/
void initialize()
{
	Renderer::Initialize(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), []() { glEnable(GL_DEPTH_TEST); glutSetCursor(GLUT_CURSOR_NONE); });

	// initialize objects
	initSky();
	initGeneratedPyramid();
	initInfiniteTexture();
	initQuartzPyramid();
	initStonePyramid();
	initBillboard();
	initDesert();
	initAloe();
	initRock0();
	initRock1();
	initPlayer();
	initPolice();
	initCactus0();
	initCactus1();

	// initializes shaders
	SkyboxShader = new Shader("skybox_shader.vert", "skybox_shader.frag");
	ObjectShader = new Shader("per_fragment_shader.vert", "per_fragment_shader.frag");
	PyramidShader = new Shader("per_fragment_shader_geometry.vert", "per_fragment_shader_geometry.frag");
	InfiniteShader = new Shader("per_fragment_shader_move.vert", "per_fragment_shader_move.frag");
	BillboardShader = new Shader("per_fragment_shader_billboard.vert", "per_fragment_shader_billboard.frag");

	// initializes renderer
	CoreRenderer = new Renderer();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}
/// Updates application.
/**
  Updates the application.
*/
void update()
{
	setElapsedTime();
	float timeDelta = setCurrentTime(CameraManager.CurrentTime);

	if (CameraManager.CanMove)
	{
		if (AppState.KeyMap[AppState::KEY_LEFT_ARROW])
			CameraManager.Current->ProcessPosition(Camera::Movement::LEFT, timeDelta);

		if (AppState.KeyMap[AppState::KEY_RIGHT_ARROW])
			CameraManager.Current->ProcessPosition(Camera::Movement::RIGHT, timeDelta);

		if (AppState.KeyMap[AppState::KEY_UP_ARROW])
			CameraManager.Current->ProcessPosition(Camera::Movement::FORWARD, timeDelta);

		if (AppState.KeyMap[AppState::KEY_DOWN_ARROW])
			CameraManager.Current->ProcessPosition(Camera::Movement::BACKWARD, timeDelta);
	}
	else if (!CameraManager.IsStatic)
	{
		if (AppState.KeyMap[AppState::KEY_LEFT_ARROW])
			turnLeftPlayer();

		if (AppState.KeyMap[AppState::KEY_RIGHT_ARROW])
			turnRightPlayer();

		if (AppState.KeyMap[AppState::KEY_UP_ARROW])
			increaseSpeedPlayer();

		if (AppState.KeyMap[AppState::KEY_DOWN_ARROW])
			decreaseSpeedPlayer();
	}

	// update dynamic objects
	updatePlayer(AppState.ElapsedTime);
	updatePolice(AppState.ElapsedTime);
	updateSpectate(AppState.ElapsedTime);
}
/// Draws application.
/**
  Draws the application.
*/
void draw()
{
	// draw lights
	drawLights(*ObjectShader);
	drawLights(*PyramidShader);
	drawLights(*InfiniteShader);
	drawLights(*BillboardShader);

	// draw fog
	drawFog(*ObjectShader);
	drawFog(*PyramidShader);
	drawFog(*InfiniteShader);
	drawFog(*BillboardShader);

	// draw skybox
	drawSky(Projection, View, *SkyboxShader);

	// draw objects
	drawQuartzPyramid(Projection, View, *ObjectShader, *CoreRenderer);
	drawStonePyramid(Projection, View, *ObjectShader, *CoreRenderer);
	drawBillboard(Projection, View, *BillboardShader, *CoreRenderer, AppState.ElapsedTime);
	drawCactus0(Projection, View, *ObjectShader, *CoreRenderer);
	drawCactus1(Projection, View, *ObjectShader, *CoreRenderer);
	drawDesert(Projection, View, *ObjectShader, *CoreRenderer);
	drawPlayer(Projection, View, *ObjectShader, *CoreRenderer);
	drawPolice(Projection, View, *ObjectShader, *CoreRenderer);
	drawAloe(Projection, View, *ObjectShader, *CoreRenderer);

	unsigned int id = 0;
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, (id++) + 1, -1);
	drawGeneratedPyramid(Projection, View, *PyramidShader, *CoreRenderer, AppState.ElapsedTime);
	glStencilFunc(GL_ALWAYS, (id++) + 1, -1);
	drawInfiniteTexture(Projection, View, *InfiniteShader, *CoreRenderer, AppState.ElapsedTime);
	glStencilFunc(GL_ALWAYS, (id++) + 1, -1);
	drawRock0(Projection, View, *ObjectShader, *CoreRenderer);
	glDisable(GL_STENCIL_TEST);

	drawRock1(Projection, View, *ObjectShader, *CoreRenderer);
}
/// Cleanup application.
/**
  Cleanup the application.
*/
void cleanup()
{
	delete CoreRenderer;

	delete SkyboxShader;
	delete ObjectShader;
	delete PyramidShader;
	delete InfiniteShader;
	delete BillboardShader;
}
/// Callback for display func.
/**
  Callback for display func.
*/
void displayCB()
{
	CoreRenderer->Clear();

	Projection = glm::perspective(glm::radians(60.0f), float(AppState.Width) / float(AppState.Height), 0.1f, 100.0f);
	View = CameraManager.Current->GetViewMatrix();

	draw();

	glutSwapBuffers();
}
/// Callback for reshape func.
/**
  Callback for reshape func.

  \param[in] w	New window width.
  \param[in] h	New window height.
*/
void reshapeCB(int w, int h)
{
	AppState.Width = w;
	AppState.Height = h;

	Renderer::SetViewport(0, 0, AppState.Width, AppState.Height);
}
/// Callback for keyboard func.
/**
  Callback for keyboard func.

  \param[in] key	Pressed key.
*/
void keyboardCB(unsigned char key, int, int)
{
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'd':
		AppState.KeyMap[AppState::KEY_RIGHT_ARROW] = true;
		break;
	case 'a':
		AppState.KeyMap[AppState::KEY_LEFT_ARROW] = true;
		break;
	case 'w':
		AppState.KeyMap[AppState::KEY_UP_ARROW] = true;
		break;
	case 's':
		AppState.KeyMap[AppState::KEY_DOWN_ARROW] = true;
		break;
	case 'q':
		CameraManager.CycleTo(-1);
		CameraManager.IsStatic = true;
		break;
	case 'e':
		CameraManager.CycleTo(1);
		CameraManager.IsStatic = true;
		break;
	case 'p':
		switchToPolice();
		break;
	case 'o':
		switchToPlayer();
		break;
	case 'i':
		switchToSpectate();
		break;
	case KEY_1:
		CameraManager.SwitchTo(&CameraManager.Cameras[0]);
		CameraManager.IsStatic = true;
		break;
	case KEY_2:
		CameraManager.SwitchTo(&CameraManager.Cameras[1]);
		CameraManager.IsStatic = true;
		break;
	case KEY_3:
		CameraManager.SwitchTo(&CameraManager.Cameras[2]);
		CameraManager.IsStatic = true;
		break;
	default:
		break;
	}
}
/// Callback for keyboard up func.
/**
  Callback for keyboard up func.

  \param[in] key	Released key.
*/
void keyboardUpCB(unsigned char key, int, int) 
{
	switch (key)
	{
	case 'd':
		AppState.KeyMap[AppState::KEY_RIGHT_ARROW] = false;
		break;
	case 'a':
		AppState.KeyMap[AppState::KEY_LEFT_ARROW] = false;
		break;
	case 'w':
		AppState.KeyMap[AppState::KEY_UP_ARROW] = false;
		break;
	case 's':
		AppState.KeyMap[AppState::KEY_DOWN_ARROW] = false;
		break;
	default:
		break;
	}
}
/// Callback for special keyboard func.
/**
  Callback for special keyboard func.

  \param[in] key	Pressed key.
*/
void specialKeyboardCB(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		AppState.KeyMap[AppState::KEY_RIGHT_ARROW] = true;
		break;
	case GLUT_KEY_LEFT:
		AppState.KeyMap[AppState::KEY_LEFT_ARROW] = true;
		break;
	case GLUT_KEY_UP:
		AppState.KeyMap[AppState::KEY_UP_ARROW] = true;
		break;
	case GLUT_KEY_DOWN:
		AppState.KeyMap[AppState::KEY_DOWN_ARROW] = true;
		break;
	case GLUT_KEY_F1:
		CameraManager.CanMove = CameraManager.IsStatic && !CameraManager.CanMove;
		break;
	case GLUT_KEY_F2:
		CameraManager.CanLook = CameraManager.IsStatic && !CameraManager.CanLook;
		break;
	default:
		break;
	}
}
/// Callback for special keyboard up func.
/**
  Callback for special keyboard up func.

  \param[in] key	Released key.
*/
void specialKeyboardUpCB(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		AppState.KeyMap[AppState::KEY_RIGHT_ARROW] = false;
		break;
	case GLUT_KEY_LEFT:
		AppState.KeyMap[AppState::KEY_LEFT_ARROW] = false;
		break;
	case GLUT_KEY_UP:
		AppState.KeyMap[AppState::KEY_UP_ARROW] = false;
		break;
	case GLUT_KEY_DOWN:
		AppState.KeyMap[AppState::KEY_DOWN_ARROW] = false;
		break;
	default:
		break;
	}
}
/// Callback for mouse func.
/**
  Callback for mouse func.

  \param[in] button		Active mouse button.
  \param[in] state		State of the active mouse button.
  \param[in] x			Position of mouse - x.
  \param[in] y			Position of mouse - y.
*/
void mouseCB(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		AppState.MouseMap[AppState::MOUSE_LEFT] = state == GLUT_DOWN;
		if (state == GLUT_DOWN) setClickedObject(x, y);
		break;
	case GLUT_RIGHT_BUTTON:
		AppState.MouseMap[AppState::MOUSE_RIGHT] = state == GLUT_DOWN;
		break;
	case GLUT_MIDDLE_BUTTON:
		CameraManager.Current->Print();
		break;
	default:
		break;
	}
}
/// Callback for mouse wheel func.
/**
  Callback for mouse wheel func.

  \param[in] offset		Offset of scrolled wheel.
*/
void mouseWheelCB(int, int offset, int, int)
{
	setElapsedTime();
	float timeDelta = setCurrentTime(CameraManager.CurrentTime);

	if (CameraManager.CanMove)
		CameraManager.Current->ProcessPosition(offset > 0 ? Camera::Movement::UPWARD : Camera::Movement::DOWNWARD, timeDelta * 3);
}
/// Callback for mouse motion func.
/**
  Callback for mouse motion func.

  \param[in] x	Position of mouse - x.
  \param[in] y	Position of mouse - y.
*/
void activeMouseMotionCB(int x, int y)
{
	if (CameraManager.CanLook && (x != AppState.Width / 2 || y != AppState.Height / 2))
	{
		setElapsedTime();
		float timeDelta = setCurrentTime(CameraManager.CurrentTime);

		if (AppState.MouseMap[AppState::MOUSE_LEFT])
			CameraManager.Current->ProcessPosition((x - AppState.Width / 2) < 0 ? Camera::Movement::LEFT : Camera::Movement::RIGHT, timeDelta);
		if (AppState.MouseMap[AppState::MOUSE_RIGHT])
			CameraManager.Current->ProcessPosition((y - AppState.Height / 2) < 0 ? Camera::Movement::FORWARD : Camera::Movement::BACKWARD, timeDelta);

		glutWarpPointer(AppState.Width / 2, AppState.Height / 2);
		glutPostRedisplay();
	}
}
/// Callback for passive mouse motion func.
/**
  Callback for passive mouse motion func.

  \param[in] x	Position of mouse - x.
  \param[in] y	Position of mouse - y.
*/
void passiveMouseMotionCB(int x, int y)
{
	if (CameraManager.CanLook && (x != AppState.Width / 2 || y != AppState.Height / 2))
		CameraManager.Current->ProcessMovement(x - AppState.Width / 2, (AppState.Height / 2) - y);

	glutWarpPointer(AppState.Width / 2, AppState.Height / 2);
	glutPostRedisplay();
}
/// Callback for timer func.
/**
  Callback for timer func.
*/
void refreshCB(int)
{
	update();

	glutTimerFunc(REFRESH_INTERVAL, &refreshCB, 0);
	glutPostRedisplay();
}
/// Application entry point.
/**
  Application entry point.
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow(WIN_TITLE);

	glutDisplayFunc(&displayCB);
	glutReshapeFunc(&reshapeCB);
	glutKeyboardFunc(&keyboardCB);
	glutKeyboardUpFunc(&keyboardUpCB);
	glutSpecialFunc(&specialKeyboardCB);
	glutSpecialUpFunc(&specialKeyboardUpCB);

	glutMouseFunc(&mouseCB);
	glutMouseWheelFunc(&mouseWheelCB);
	glutWarpPointer(AppState.Width / 2, AppState.Height / 2);
	glutMotionFunc(&activeMouseMotionCB);
	glutPassiveMotionFunc(&passiveMouseMotionCB);

	glutTimerFunc(REFRESH_INTERVAL, &refreshCB, 0);

	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("PGR init failed, required OpenGL not supported?");

	initialize();
	glutMainLoop();
	cleanup();

	return 0;
}
