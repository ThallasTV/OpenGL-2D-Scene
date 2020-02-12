
//
// 16005481 Coursework for Computer Graphics
//

#include "stdafx.h"
#include <glew\glew.h>
#include <freeglut\freeglut.h>
#include <CoreStructures\CoreStructures.h>
#include "texture_loader.h"
#include "shader_setup.h"
#include <random>


using namespace std;
using namespace CoreStructures;


// Globals

GLuint locT; // location of "T" uniform variable in myShaderProgram
GLuint locT2;


// Textures
GLuint myGroundTexture = 0;
GLuint snowball = 0;
GLuint myMountainTexture = 0;
GLuint myTexture = 0;
GLuint myRoof = 0;
GLuint myDoor = 0;
GLuint myWindow = 0;
GLuint myStar = 0;

// Mouse input (rotation) example
// Variable we'll use to animate (rotate) our star object
float theta = 0.0f;

// Variables needed to track where the mouse pointer is so we can determine which direction it's moving in
int mouse_x, mouse_y;
bool mDown = false;

// Shader program object for applying textures to our shapes
GLuint myShaderProgram;

// Second shader progam object for non textured shapes such as our rainbow star
GLuint myShaderProgramNoTexture;


// Vertex Buffer Object IDs for the star 
GLuint starPosVBO, starColourVBO, starTexCoordVBO, starIndicesVBO;

// Vertex Buffer Object IDs for the snow texture object
GLuint quadPosVBO, quadColourVBO, quadTexCoordVBO, quadIndicesVBO;

// Vertex Buffer Object IDs for the sky and sky texture object
GLuint quadMPosVBO, quadMColourVBO, quadMTexCoordVBO, quadMIndicesVBO;

// Vertex Buffer Object IDs for the house texture object
GLuint housePosVBO, houseColourVBO, houseTexCoordVBO, houseIndicesVBO;

// Vertex Buffer Object IDs for the roof texture object
GLuint roofPosVBO, roofColourVBO, roofTexCoordVBO, roofIndicesVBO;

// Vertex Buffer Object IDs for the door texture object
GLuint doorPosVBO, doorColourVBO, doorTexCoordVBO, doorIndicesVBO;

// Vertex Buffer Object IDs for the door texture object
GLuint windowPosVBO, windowColourVBO, windowTexCoordVBO, windowIndicesVBO;

// Vertex Buffer Object IDs for the door texture object
GLuint window2PosVBO, window2ColourVBO, window2TexCoordVBO, window2IndicesVBO;

// Packed vertex arrays for the star object

//1) Position Array - Store vertices as(x, y) pairs
static GLfloat starVertices[] = {

	0.0, 0.25f,
	0.1f, 0.1f,
	0.25f, 0.08f,
	0.15f, -0.05f,
	0.25f, -0.25f,
	0.0, -0.125f,
	-0.25f, -0.25f,
	-0.15f, -0.05f,
	-0.25f, 0.08f,
	-0.1f, 0.1f
};

//2) Colour Array - Store RGB values as unsigned bytes
static GLubyte starColors[] = {

	255, 255, 0, 0,
	255, 255, 0, 0,
	255, 255, 0, 0,
	255, 255, 0, 0,
	255, 255, 0, 0,
	255, 255, 0, 0,
	255, 255, 0, 0,
	255, 255, 0, 0,
	255, 255, 0, 0,
	255, 255, 0, 0,
};

//3) Texture coordinate array(store uv coordinates as floating point values)
static float starTextureCoords[] = {

	0.5f, 1.0f,
	0.6f, 0.7f,
	1.0f, 0.7f,
	0.8f, 0.4f,
	0.95f, 0.0f,
	0.0f, 0.3f,
	-0.95f, 0.0f,
	-0.8f, 0.4f,
	-1.0f, 0.7f,
	-0.6f, 0.7f
};

//4) Index Array - Store indices to star vertices - this determines the order the vertices are to be processed
static GLubyte starVertexIndices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

// Packed vertex arrays for the ground object

// 1) Position Array - Store vertices as (x,y) pairs
static GLfloat quadVertices[] = {

	-1.0, -0.7f,
	-1.0f, 0.7f,
	1.0f, -0.7f,
	1.0f, 0.7f
};

// 2) Colour Array - Store RGB values as unsigned bytes
static GLubyte quadColors[] = {

	255, 0, 0, 255,
	255, 255, 0, 255,
	0, 255, 0, 255,
	0, 255, 255, 255
	
};

// 3) Texture coordinate array (store uv coordinates as floating point values)
static GLfloat quadTextureCoords[] = {

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f

};

// 4) Index Array - Store indices to quad vertices - this determines the order the vertices are to be processed
static GLubyte quadVertexIndices[] = { 0, 1, 2, 3 };

// -----------------------
// Packed vertex arrays for the snow and sky object

// 1) Position Array - Store vertices as (x,y) pairs
static GLfloat quadMVertices[] = {

	-1.0f, -0.5f,
	-1.0f, 1.0f,
	1.0f, -0.5f,
	1.0f, 1.0f
};

// 2) Colour Array - Store RGB values as unsigned bytes
static GLubyte quadMColors[] = {

	255, 0, 0, 255,
	255, 255, 0, 255,
	0, 255, 0, 255,
	0, 255, 255, 255

};

// 3) Texture coordinate array (store uv coordinates as floating point values)
static float quadMTextureCoords[] = {

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f

};

// 4) Index Array - Store indices to quad vertices - this determines the order the vertices are to be processed
static GLubyte quadMVertexIndices[] = { 0, 1, 2, 3 };

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// 1) Position Array - Store vertices as (x,y) pairs
static GLfloat houseVertices[] = {

	-0.3f, -0.2f,
	-0.3f, 0.2f,
	0.3f, -0.2f,
	0.3f, 0.2f
};

// 2) Colour Array - Store RGB values as unsigned bytes
static GLubyte houseColors[] = {

	255, 0, 0, 255,
	255, 255, 0, 255,
	0, 255, 0, 255,
	0, 255, 255, 255

};

// 3) Texture coordinate array (store uv coordinates as floating point values)
static float houseTextureCoords[] = {

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f

};

// 4) Index Array - Store indices to quad vertices - this determines the order the vertices are to be processed
static GLubyte houseVertexIndices[] = { 0, 1, 2, 3 };

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// 1) Position Array - Store vertices as (x,y) pairs
static GLfloat roofVertices[] = {

	-0.3f, -0.1f,
	0.3f, -0.1f,
	0.0f, 0.3f
};

// 2) Colour Array - Store RGB values as unsigned bytes
static GLubyte roofColors[] = {

	255, 0, 0, 255,
	255, 255, 0, 255,
	0, 255, 0, 255,

};

// 3) Texture coordinate array (store uv coordinates as floating point values)
static float roofTextureCoords[] = {

	-0.3f, -0.1f,
	0.3f, -0.1f,
	0.0f, 0.3f,
	
};

// 4) Index Array - Store indices to quad vertices - this determines the order the vertices are to be processed
static GLubyte roofVertexIndices[] = { 0, 1, 2 };
 //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// 1) Position Array - Store vertices as (x,y) pairs
static GLfloat doorVertices[] = {

	-0.1f, -0.2f,
	-0.1f, 0.1f,
	0.1f, -0.2f,
	0.1f, 0.1f
};

// 2) Colour Array - Store RGB values as unsigned bytes
static GLubyte doorColors[] = {

	255, 0, 0, 255,
	255, 255, 0, 255,
	0, 255, 0, 255,
	0, 255, 255, 255

};

// 3) Texture coordinate array (store uv coordinates as floating point values)
static float doorTextureCoords[] = {

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f

};

// 4) Index Array - Store indices to quad vertices - this determines the order the vertices are to be processed
static GLubyte doorVertexIndices[] = { 0, 1, 2, 3 };

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// 1) Position Array - Store vertices as (x,y) pairs
static GLfloat windowVertices[] = {

	-0.1f, -0.1f,
	-0.1f, 0.1f,
	0.1f, -0.1f,
	0.1f, 0.1f
};

// 2) Colour Array - Store RGB values as unsigned bytes
static GLubyte windowColors[] = {

	255, 0, 0, 255,
	255, 255, 0, 255,
	0, 255, 0, 255,
	0, 255, 255, 255

};

// 3) Texture coordinate array (store uv coordinates as floating point values)
static float windowTextureCoords[] = {

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f

};

// 4) Index Array - Store indices to quad vertices - this determines the order the vertices are to be processed
static GLubyte windowVertexIndices[] = { 0, 1, 2, 3 };

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// 1) Position Array - Store vertices as (x,y) pairs
static GLfloat window2Vertices[] = {

	-0.1f, -0.1f,
	-0.1f, 0.1f,
	0.1f, -0.1f,
	0.1f, 0.1f
};

// 2) Colour Array - Store RGB values as unsigned bytes
static GLubyte window2Colors[] = {

	255, 0, 0, 255,
	255, 255, 0, 255,
	0, 255, 0, 255,
	0, 255, 255, 255

};

// 3) Texture coordinate array (store uv coordinates as floating point values)
static float window2TextureCoords[] = {

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f

};

// 4) Index Array - Store indices to quad vertices - this determines the order the vertices are to be processed
static GLubyte window2VertexIndices[] = { 0, 1, 2, 3 };

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Function Prototypes

void init(int argc, char* argv[]);
void setupStarVBO(void);
void setupQuadTextureGroundVBO(void);
void setupHouseVBO(void);
void setupRoofVBO(void);
void setupDoorVBO(void);
void setupWindowsVBO(void);
void setupWindows2VBO(void);
void setupQuadTextureMountainVBO(void);
void report_version(void);
void display(void);
void drawTexturedQuad(void);
void drawTexturedQuadVBOGround(void);
void drawTexturedHouseVBO(void);
void drawTexturedRoofVBO(void);
void drawTexturedDoorVBO(void);
void drawTexturedWindowVBO(void);
void drawTexturedWindow2VBO(void);
void drawTexturedQuadVBOMountain(void);
void drawStar(void);
void drawStar_VBO(void);
void drawStar2_VBO(void);
//void starRotaion(void);
void update(void);
void mouseButtonDown(int button_id, int state, int x, int y);
//void mouseMove(int x, int y);
//void keyDown(unsigned char key, int x, int y);


int _tmain(int argc, char* argv[]) {

	init(argc, argv);

	glutMainLoop();

	// Shut down COM
	shutdownCOM();

	return 0;
}


void init(int argc, char* argv[]) {

	// Initialise COM so we can use Windows Imaging Component
	initCOM();

	// Initialise FreeGLUT
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(64, 64);
	glutCreateWindow("16005481 Coursework");

	// Register callback functions
	glutDisplayFunc(display);
	glutIdleFunc(update);
	//glutKeyboardFunc(keyDown);
	glutMouseFunc(mouseButtonDown);
	//glutMotionFunc(mouseMove);


	// Initialise GLEW library
	GLenum err = glewInit();

	// Ensure GLEW was initialised successfully before proceeding
	if (err == GLEW_OK) {

		cout << "GLEW initialised okay\n";

	} else {

		cout << "GLEW could not be initialised\n";
		throw;
	}
	
	// Example query OpenGL state (get version number)
	report_version();


	// Report maximum number of vertex attributes
	GLint numAttributeSlots;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributeSlots);
	cout << "GL_MAX_VERTEX_ATTRIBS = " << numAttributeSlots << endl;


	// Initialise OpenGL...

	// Setup colour to clear the window
	glClearColor(0.2f, 0.2f, 0.8f, 0.0f);

	glLineWidth(9.0f);

	// Load demo texture
	myGroundTexture = fiLoadTexture("snow.jpg");
	myMountainTexture = fiLoadTexture("sky.jpg");
	myTexture = fiLoadTexture("wall.jpg");
	myRoof = fiLoadTexture("roof.jpg");
	myDoor = fiLoadTexture("door.jpg");
	myWindow = fiLoadTexture("window.jpg");
	myStar = fiLoadTexture("star.jpg");

	// Shader setup 
	myShaderProgram = setupShaders(string("Shaders\\basic_vertex_shader.txt"), string("Shaders\\basic_fragment_shader.txt"));
	myShaderProgramNoTexture = setupShaders(string("Shaders\\notexture_vertex_shader.txt"), string("Shaders\\notexture_fragment_shader.txt"));

	// Get uniform location of "T" variable in shader program (we'll use this in the play function to give the uniform variable "T" a value)
	locT = glGetUniformLocation(myShaderProgram, "T");
	locT2 = glGetUniformLocation(myShaderProgramNoTexture, "T2");

	
	setupHouseVBO();
	setupDoorVBO();
	setupRoofVBO();
	setupWindowsVBO();
	setupWindows2VBO();
	setupStarVBO();
	setupStarVBO();
	setupQuadTextureGroundVBO();
	setupQuadTextureMountainVBO();
}


void report_version(void) {

	int majorVersion, minorVersion;

	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	cout << "OpenGL version " << majorVersion << "." << minorVersion << "\n\n";
}

void setupHouseVBO(void) {

	// setup VBO for the quad object position data
	glGenBuffers(1, &housePosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, housePosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);

	// setup VBO for the quad object colour data
	glGenBuffers(1, &houseColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, houseColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(houseColors), houseColors, GL_STATIC_DRAW);

	// setup VBO for the quad object texture coord data
	glGenBuffers(1, &houseTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, houseTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(houseTextureCoords), houseTextureCoords, GL_STATIC_DRAW);

	// setup quad vertex index array
	glGenBuffers(1, &houseIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(houseVertexIndices), houseVertexIndices, GL_STATIC_DRAW);
}

void setupRoofVBO(void) {

	//setup VBO for the quad object position data
	glGenBuffers(1, &roofPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, roofPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roofVertices), roofVertices, GL_STATIC_DRAW);

	// setup VBO for the quad object colour data
	glGenBuffers(1, &roofColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, roofColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roofColors), roofColors, GL_STATIC_DRAW);

	// setup VBO for the quad object texture coord data
	glGenBuffers(1, &roofTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, roofTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roofTextureCoords), roofTextureCoords, GL_STATIC_DRAW);

	// setup quad vertex index array
	glGenBuffers(1, &roofIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roofVertexIndices), roofVertexIndices, GL_STATIC_DRAW);
}

void setupDoorVBO(void) {

	// setup VBO for the quad object position data
	glGenBuffers(1, &doorPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, doorPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), doorVertices, GL_STATIC_DRAW);

	// setup VBO for the quad object colour data
	glGenBuffers(1, &doorColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, doorColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(doorColors), doorColors, GL_STATIC_DRAW);

	// setup VBO for the quad object texture coord data
	glGenBuffers(1, &doorTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, doorTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(doorTextureCoords), doorTextureCoords, GL_STATIC_DRAW);

	// setup quad vertex index array
	glGenBuffers(1, &doorIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, doorIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(doorVertexIndices), doorVertexIndices, GL_STATIC_DRAW);
}

void setupWindowsVBO(void) {

	// setup VBO for the quad object position data
	glGenBuffers(1, &windowPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), windowVertices, GL_STATIC_DRAW);

	// setup VBO for the quad object colour data
	glGenBuffers(1, &windowColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowColors), windowColors, GL_STATIC_DRAW);

	// setup VBO for the quad object texture coord data
	glGenBuffers(1, &windowTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowTextureCoords), windowTextureCoords, GL_STATIC_DRAW);

	// setup quad vertex index array
	glGenBuffers(1, &windowIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(windowVertexIndices), windowVertexIndices, GL_STATIC_DRAW);
}

void setupWindows2VBO(void) {

	// setup VBO for the quad object position data
	glGenBuffers(1, &windowPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), windowVertices, GL_STATIC_DRAW);

	// setup VBO for the quad object colour data
	glGenBuffers(1, &windowColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowColors), windowColors, GL_STATIC_DRAW);

	// setup VBO for the quad object texture coord data
	glGenBuffers(1, &windowTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowTextureCoords), windowTextureCoords, GL_STATIC_DRAW);

	// setup quad vertex index array
	glGenBuffers(1, &windowIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(windowVertexIndices), windowVertexIndices, GL_STATIC_DRAW);
}

void setupQuadTextureGroundVBO(void) {

	// setup VBO for the quad object position data
	glGenBuffers(1, &quadPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	// setup VBO for the quad object colour data
	glGenBuffers(1, &quadColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadColors), quadColors, GL_STATIC_DRAW);

	// setup VBO for the quad object texture coord data
	glGenBuffers(1, &quadTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadTextureCoords), quadTextureCoords, GL_STATIC_DRAW);

	// setup quad vertex index array
	glGenBuffers(1, &quadIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadVertexIndices), quadVertexIndices, GL_STATIC_DRAW);
}

void setupQuadTextureMountainVBO(void) {

	// setup VBO for the quad object position data
	glGenBuffers(1, &quadMPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadMPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadMVertices), quadMVertices, GL_STATIC_DRAW);

	// setup VBO for the quad object colour data
	glGenBuffers(1, &quadMColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadMColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadMColors), quadMColors, GL_STATIC_DRAW);

	// setup VBO for the quad object texture coord data
	glGenBuffers(1, &quadMTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadMTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadMTextureCoords), quadMTextureCoords, GL_STATIC_DRAW);

	// setup quad vertex index array
	glGenBuffers(1, &quadMIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadMVertexIndices), quadMVertexIndices, GL_STATIC_DRAW);
}

void setupStarVBO(void) {

	/*setup VBO for the star object position data*/
		glGenBuffers(1, &starPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, starPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starVertices), starVertices, GL_STATIC_DRAW);

	/*setup VBO for the star object colour data*/
		glGenBuffers(1, &starColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, starColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starColors), starColors, GL_STATIC_DRAW);

	/*setup VBO for the star object texture coord data*/
		glGenBuffers(1, &starTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, starTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starTextureCoords), starTextureCoords, GL_STATIC_DRAW);

	/*setup star vertex index array*/
		glGenBuffers(1, &starIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(starVertexIndices), starVertexIndices, GL_STATIC_DRAW);
}

void setupStar2VBO(void) {

	/*setup VBO for the star object position data*/
	glGenBuffers(1, &starPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, starPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starVertices), starVertices, GL_STATIC_DRAW);

	/*setup VBO for the star object colour data*/
	glGenBuffers(1, &starColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, starColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starColors), starColors, GL_STATIC_DRAW);

	/*setup VBO for the star object texture coord data*/
	glGenBuffers(1, &starTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, starTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starTextureCoords), starTextureCoords, GL_STATIC_DRAW);

	/*setup star vertex index array*/
	glGenBuffers(1, &starIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(starVertexIndices), starVertexIndices, GL_STATIC_DRAW);
}


void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawTexturedQuadVBOMountain();

	drawTexturedQuadVBOGround();

	drawTexturedHouseVBO();

	drawTexturedRoofVBO();

	drawTexturedDoorVBO();

	drawTexturedWindowVBO();

	drawTexturedWindow2VBO();

	 drawStar_VBO();
	 
	 drawStar2_VBO();

	glutSwapBuffers();
}

void drawStar_VBO(void) {
	
       
	glUseProgram(myShaderProgramNoTexture);

	GUMatrix4 T = GUMatrix4::translationMatrix(0.65f, 0.65f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);


	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myStar);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, starPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, starColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, starTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starIndicesVBO);

	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_LINE_LOOP, 10, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}

void drawStar2_VBO(void) {


	glUseProgram(myShaderProgramNoTexture);

	GUMatrix4 T = GUMatrix4::translationMatrix(-0.65f, 0.65f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);


	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myStar);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, starPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, starColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, starTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starIndicesVBO);

	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_LINE_LOOP, 10, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisable(GL_TEXTURE_2D);
}

//void starRotaion(void) {
//
//	random_device rd;
//
//	// Random number engine - seed with a PRN (pseudo-random number) from the random_device rd.  This mean we get a different sequence of numbers every time we run the program.  If you want the same sequence of random numbers every time you run the program replace rd() with a constant value.
//	mt19937 mt(rd());
//
//
//	// Define the ranges we want random numbers generated in
//
//	// Stars lie in the [-2.25, +2.25] range on (x, y) plane
//	auto positionRange = uniform_real_distribution<float>(-2.25f, 2.25f);
//
//	// Scale stars range - generate a random index in the range [0, 9] to access the scale_prob array
//	auto scaleProbRange = uniform_int_distribution<int>(0, 9);
//
//
//	for (int i = 0; i < NUM_STARS; ++i) {
//		// Get scale from scale_prob array (setup so bias to certain size range for the stars around 0.1)
//		float scale = scale_prob[scaleProbRange(mt)];
//
//		// Calculate orientation bassed on position
//		float theta = atan2f(y, x) - (gu_pi / 2.0f);
//
//		// Setup matrices based on variables created above
//		stars[i].pos = GUMatrix4::translationMatrix(x, y, 0.0f);
//		stars[i].scale = GUMatrix4::scaleMatrix(scale, scale, 1.0f);
//		stars[i].rot = GUMatrix4::rotationMatrix(0.0f, 0.0f, theta);
//
//		// Combine matices into a single (model) transform the the current star instance.
//		stars[i].T = stars[i].pos * stars[i].rot * stars[i].scale;
//	}
//}

void drawTexturedHouseVBO(void)
{
	glUseProgram(myShaderProgram);

	
	GUMatrix4 T = GUMatrix4::translationMatrix(0.0f, -0.2f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);


	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, housePosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, houseColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, houseTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseIndicesVBO);

	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDisable(GL_TEXTURE_2D);

	// use to force disable our shaderprogram
	//glUseProgram(0);
}

void drawTexturedRoofVBO(void)
{
	glUseProgram(myShaderProgram);

	
	GUMatrix4 T = GUMatrix4::translationMatrix(0.0f, 0.1f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);


	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myRoof);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, roofPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, roofColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, roofTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseIndicesVBO);

	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDisable(GL_TEXTURE_2D);

	// use to force disable our shaderprogram
	//glUseProgram(0);
}

void drawTexturedWindowVBO(void)
{
	glUseProgram(myShaderProgram);

	
	GUMatrix4 T = GUMatrix4::translationMatrix(-0.2f, -0.2f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);
	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myWindow);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, windowPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, windowColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, windowTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowIndicesVBO);

	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDisable(GL_TEXTURE_2D);

	// use to force disable our shaderprogram
	//glUseProgram(0);
}

void drawTexturedWindow2VBO(void)
{
	glUseProgram(myShaderProgram);

	
	GUMatrix4 S = GUMatrix4::translationMatrix(0.2f, -0.2f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&S);
	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myWindow);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, windowPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, windowColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, windowTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowIndicesVBO);

	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDisable(GL_TEXTURE_2D);

	// use to force disable our shaderprogram
	//glUseProgram(0);
}

void drawTexturedQuadVBOGround(void) {
	
	glUseProgram(myShaderProgram);

	// Move our ground shape to the bottom half of the screen
	GUMatrix4 T = GUMatrix4::translationMatrix(0.0f, -1.0f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);


	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)
	
	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myGroundTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);
	
	glBindBuffer(GL_ARRAY_BUFFER, quadPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, quadColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesVBO);

	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDisable(GL_TEXTURE_2D);

	// use to force disable our shaderprogram
 	//glUseProgram(0);

}

void drawTexturedQuadVBOMountain(void) {
	
	glUseProgram(myShaderProgram);

	//Move our shape into the top position
	GUMatrix4 T = GUMatrix4::translationMatrix(0.0f, 0.3f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);


	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myMountainTexture);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, quadPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, quadColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesVBO);


	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDisable(GL_TEXTURE_2D);
	

}

void drawTexturedDoorVBO(void)
{
	glUseProgram(myShaderProgram);

	
	GUMatrix4 T = GUMatrix4::translationMatrix(0.0f, -0.2f, 0.0f);
	glUniformMatrix4fv(locT, 1, GL_FALSE, (GLfloat*)&T);


	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myDoor);
	glUniform1i(glGetUniformLocation(myShaderProgram, "texture"), 0);
	glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, doorPosVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, doorColourVBO);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, doorTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);


	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, doorIndicesVBO);

	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDisable(GL_TEXTURE_2D);

	// use to force disable our shaderprogram
	//glUseProgram(0);
}

// square movement -------------------------------------------------------------
// update is called every frame
void update(void) {

	// Redraw the screen
	glutPostRedisplay();
}

#pragma region Event handling functions

void mouseButtonDown(int button_id, int state, int x, int y) {

	if (button_id == GLUT_LEFT_BUTTON) {

		if (state == GLUT_DOWN) {

			mouse_x = x;
			mouse_y = y;

			mDown = true;

		}
		else if (state == GLUT_UP) {

			mDown = false;
		}
	}
}

void mouseMove(int x, int y) {

	if (mDown) {

		int dx = x - mouse_x;
		int dy = y - mouse_y;

		theta += (float)dy * (3.142f * 0.01f);

		mouse_x = x;
		mouse_y = y;

		glutPostRedisplay();
	}
}

#pragma endregion
