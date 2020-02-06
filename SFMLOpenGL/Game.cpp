#include <Game.h>

static bool flip;

Game::Game() : window(sf::VideoMode(800, 600), "OpenGL Cube Vertex and Fragment Shaders")
{

}

Game::~Game() {}

void Game::run()
{
	initialize();

	sf::Event event;

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}
}

std::string Game::shaderFile(std::string t_filePath)
{
	std::string fileStuff;
	std::string line;
	std::ifstream reading(t_filePath);

	if (reading.is_open())
	{
		while (std::getline(reading, line))
		{
			int length = line.length();
			int size = line.size();

			for (int character = 0; character < length; character++)
			{
				if (line[character] != '\\' && line[character] != 'n' && line[character] != 'r')
				{
					line += line[character];

					character++;
				}
				else if (line[character + 1] == 'n')
				{
					line += '\n';
					character++;
				}					
				else if (line[character + 1] == 'r')
				{
					line += '\r';
					character++;
				}
			}

			fileStuff += line;
		}
	}

	reading.close();
	
	return fileStuff;
}

typedef struct
{
	float coordinate[3];
	float color[4];
} Vertex;

Vertex vertex[8];
GLubyte triangles[36];

/* Variable to hold the VBO identifier and shader data */
GLuint	index, //Index to draw
		vsid, //Vertex Shader ID
		fsid, //Fragment Shader ID
		progID, //Program ID
		vbo = 1, // Vertex Buffer ID
		positionID, //Position ID
		colorID; // Color ID

void Game::initialize()
{
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	glewInit();

	glEnable(GL_CULL_FACE);

	/* Vertices counter-clockwise winding */
	vertex[0].coordinate[0] = -1.0f;
	vertex[0].coordinate[1] = -1.0f;
	vertex[0].coordinate[2] = 1.0f;

	vertex[1].coordinate[0] = 1.0f;
	vertex[1].coordinate[1] = -1.0f;
	vertex[1].coordinate[2] = 1.0f;

	vertex[2].coordinate[0] = 1.0f;
	vertex[2].coordinate[1] = 1.0f;
	vertex[2].coordinate[2] = 1.0f;

	vertex[3].coordinate[0] = -1.0f;
	vertex[3].coordinate[1] = 1.0f;
	vertex[3].coordinate[2] = 1.0f;

	vertex[4].coordinate[0] = -1.0f;
	vertex[4].coordinate[1] = -1.0f;
	vertex[4].coordinate[2] = -1.0f;

	vertex[5].coordinate[0] = 1.0f;
	vertex[5].coordinate[1] = -1.0f;
	vertex[5].coordinate[2] = -1.0f;

	vertex[6].coordinate[0] = 1.0f;
	vertex[6].coordinate[1] = 1.0f;
	vertex[6].coordinate[2] = -1.0f;

	vertex[7].coordinate[0] = -1.0f;
	vertex[7].coordinate[1] = 1.0f;
	vertex[7].coordinate[2] = -1.0f;

	vertex[0].color[0] = 0.5f;
	vertex[0].color[1] = 1.0f;
	vertex[0].color[2] = 0.5f;
	vertex[0].color[3] = 0.0f;

	vertex[1].color[0] = 0.0f;
	vertex[1].color[1] = 1.0f;
	vertex[1].color[2] = 0.5f;
	vertex[1].color[3] = 0.5f;

	vertex[2].color[0] = 0.5f;
	vertex[2].color[1] = 0.0f;
	vertex[2].color[2] = 0.5f;
	vertex[2].color[3] = 1.0f;

	/*Index of Poly / Triangle to Draw */
	triangles[0] = 0;   triangles[1] = 1;   triangles[2] = 2;
	triangles[3] = 2;   triangles[4] = 3;   triangles[5] = 0;
	triangles[6] = 2;   triangles[7] = 6;   triangles[8] = 3;

	triangles[9] = 6;   triangles[10] = 7;   triangles[11] = 3;
	triangles[12] = 7;   triangles[13] = 6;   triangles[14] = 5;
	triangles[15] = 5;   triangles[16] = 4;   triangles[17] = 7;

	triangles[18] = 5;   triangles[19] = 0;   triangles[20] = 4;
	triangles[21] = 0;   triangles[22] = 5;   triangles[23] = 1;
	triangles[24] = 6;   triangles[25] = 1;   triangles[26] = 5;

	triangles[27] = 6;   triangles[28] = 2;   triangles[29] = 1;
	triangles[30] = 4;   triangles[31] = 0;   triangles[32] = 3;
	triangles[33] = 7;   triangles[34] = 4;   triangles[35] = 3;

	/* Create a new VBO using VBO id */
	glGenBuffers(1, &vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 36, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Vertex Shader which would normally be loaded from an external file */
	std::string vtxshader = shaderFile(std::string("vtxshader.txt"));

	//Vertex Shader Src
	const char* vs_src = &vtxshader[0]; 

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER); //Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL); // Set the shaders source
	glCompileShader(vsid); //Check that the shader compiles

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	// Fragment Shader which would normally be loaded from an external file 
	std::string frgshader = shaderFile(std::string("frgshader.txt"));

	//Fragment Shader Src
	const char* fs_src = &frgshader[0];

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);
	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	//Create program in GPU
	glAttachShader(progID, vsid); //Attach Vertex Shader to Program
	glAttachShader(progID, fsid); //Attach Fragment Shader to Program
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	// Find variables in the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
}

void Game::update()
{
	elapsed = clock.getElapsedTime();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		Matrix3 rotation;

		for (int index = 0; index < 8; index++)
		{
			Vector3 vector;
			vector.setX(vertex[index].coordinate[0]);
			vector.setY(vertex[index].coordinate[1]);
			vector.setZ(vertex[index].coordinate[2]);

			vector = rotation.RotationX(rotationAngle) * vector;
			vertex[index].coordinate[0] = vector.getX();
			vertex[index].coordinate[1] = vector.getY();
			vertex[index].coordinate[2] = vector.getZ();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		Matrix3 rotation;

		for (int index = 0; index < 8; index++)
		{
			Vector3 vector;
			vector.setX(vertex[index].coordinate[0]);
			vector.setY(vertex[index].coordinate[1]);
			vector.setZ(vertex[index].coordinate[2]);

			vector = rotation.RotationY(rotationAngle) * vector;
			vertex[index].coordinate[0] = vector.getX();
			vertex[index].coordinate[1] = vector.getY();
			vertex[index].coordinate[2] = vector.getZ();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		Matrix3 rotation;

		for (int index = 0; index < 8; index++)
		{
			Vector3 vector;
			vector.setX(vertex[index].coordinate[0]);
			vector.setY(vertex[index].coordinate[1]);
			vector.setZ(vertex[index].coordinate[2]);

			vector = rotation.RotationZ(rotationAngle) * vector;
			vertex[index].coordinate[0] = vector.getX();
			vertex[index].coordinate[1] = vector.getY();
			vertex[index].coordinate[2] = vector.getZ();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
	{
		Matrix3 scale;

		for (int index = 0; index < 8; index++)
		{
			Vector3 vector;
			vector.setX(vertex[index].coordinate[0]);
			vector.setY(vertex[index].coordinate[1]);
			vector.setZ(vertex[index].coordinate[2]);

			vector = scale.Scale3D(99.9) * vector;
			vertex[index].coordinate[0] = vector.getX();
			vertex[index].coordinate[1] = vector.getY();
			vertex[index].coordinate[2] = vector.getZ();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
	{
		Matrix3 scale;

		for (int index = 0; index < 8; index++)
		{
			Vector3 vector;
			vector.setX(vertex[index].coordinate[0]);
			vector.setY(vertex[index].coordinate[1]);
			vector.setZ(vertex[index].coordinate[2]);

			vector = scale.Scale3D(100.1) * vector;
			vertex[index].coordinate[0] = vector.getX();
			vertex[index].coordinate[1] = vector.getY();
			vertex[index].coordinate[2] = vector.getZ();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		Matrix3 translate;

		for (int index = 0; index < 8; index++)
		{
			Vector3 vector;
			vector.setX(vertex[index].coordinate[0]);
			vector.setY(vertex[index].coordinate[1]);
			vector.setZ(vertex[index].coordinate[2]);

			vector.setZ(1);
			vector = translate.Translate(0, 0.005) * vector;

			vertex[index].coordinate[0] = vector.getX();
			vertex[index].coordinate[1] = vector.getY();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		Matrix3 translate;

		for (int index = 0; index < 8; index++)
		{
			Vector3 vector;
			vector.setX(vertex[index].coordinate[0]);
			vector.setY(vertex[index].coordinate[1]);
			vector.setZ(vertex[index].coordinate[2]);

			vector.setZ(1);
			vector = translate.Translate(0, -0.005) * vector;

			vertex[index].coordinate[0] = vector.getX();
			vertex[index].coordinate[1] = vector.getY();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		Matrix3 translate;

		for (int index = 0; index < 8; index++)
		{
			Vector3 vector;
			vector.setX(vertex[index].coordinate[0]);
			vector.setY(vertex[index].coordinate[1]);
			vector.setZ(vertex[index].coordinate[2]);

			vector.setZ(1);
			vector = translate.Translate(-0.005, 0) * vector;

			vertex[index].coordinate[0] = vector.getX();
			vertex[index].coordinate[1] = vector.getY();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		Matrix3 translate;

		for (int index = 0; index < 8; index++)
		{
			Vector3 vector;
			vector.setX(vertex[index].coordinate[0]);
			vector.setY(vertex[index].coordinate[1]);
			vector.setZ(vertex[index].coordinate[2]);

			vector.setZ(1);
			vector = translate.Translate(0.005, 0) * vector;

			vertex[index].coordinate[0] = vector.getX();
			vertex[index].coordinate[1] = vector.getY();
		}
	}

#if (DEBUG >= 2)
	DEBUG_MSG("Update up...");
#endif

}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Drawing...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 36, vertex, GL_STATIC_DRAW);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain
		model components that 'are' and 'are not' to be drawn )	*/

	// Set pointers for each parameter
	// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (float*)NULL+3);

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	window.display();
}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);
	glDeleteBuffers(1, &vbo);
}