#include "SceneRenderer.h"


void SceneRenderer::draw(int WIDTH, int HEIGHT, float dt)
{


	dLightPos = glm::vec3(0.0f, 0.0f, 0.0f) - lScene.dirLight.direction * 20.0f;
	glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, shadowNPlane, shadowFPlane);
	glm::mat4 lightView = glm::lookAt(dLightPos, dLightPos + lScene.dirLight.direction, glm::vec3(0, 1, 0));
	/*glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));*/

	glm::mat4 lightMat = lightProj * lightView;

	lScene.lMatrix = lightMat;

	// First draw scene from light point of view

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, shadowWidth, shadowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < (int)drawables.size(); i++)
	{
		drawables[i]->drawShadow(lightMat, &shadowShader);
	}



	// Draw now to our framebuffer
	glViewport(0, 0, WIDTH, HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


	//glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = glm::lookAt(camera.position, camera.target, camera.up);
	proj = glm::perspective(glm::radians(camera.fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, camera.close, camera.far);



	// TODO: WE COULD REALLY LOAD MATRICES HERE INSTEAD OF IN EVERY MESH
	for (int i = 0; i < (int)drawables.size(); i++)
	{
		drawables[i]->draw(view, proj, lScene, depthMap, skybox, camera.position);
	}


	// Draw debug shapes


	glm::mat4 def;
	def = glm::scale(def, glm::vec3(1.0f, 1.0f, 1.0f));

	
	debugShader.use();

	glUniformMatrix4fv(glGetUniformLocation(debugShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(def));
	glUniformMatrix4fv(glGetUniformLocation(debugShader.programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(debugShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	
	
	for (unsigned int i = 0; i < debug.size(); i++)
	{
		
		if (debug[i].duration >= 0)
		{
			glBindVertexArray(debug[i].VAO);
			glDrawArrays(debug[i].type, 0, debug[i].vertexCount);
			glBindVertexArray(0);
			debug[i].duration -= dt;
		}
		else
		{
			free(debug[i].data);
			glDeleteBuffers(1, &debug[i].VBO);
			glDeleteVertexArrays(1, &debug[i].VAO);
			debug.erase(debug.begin() + i);
		}
	}


	glm::mat4 skyView = glm::mat4(glm::mat3(view));

	//Draw skybox
	if (skybox != 0)
	{
		glDepthFunc(GL_LEQUAL);
		skyShader.use();

		//glUniformMatrix4fv(glGetUniformLocation(skyShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(skyOffset));
		glUniformMatrix4fv(glGetUniformLocation(skyShader.programID, "view"), 1, GL_FALSE, glm::value_ptr(skyView));
		glUniformMatrix4fv(glGetUniformLocation(skyShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(skyVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

	// Draw framebuffer quad
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glCullFace(GL_BACK);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	//glEnable(GL_FRAMEBUFFER_SRGB);

	frameShader.use();

	glUniform1i(glGetUniformLocation(frameShader.programID, "doEffect"), doEffect);
	glUniform1i(glGetUniformLocation(frameShader.programID, "screenTexture"), 0);
	glUniform1f(glGetUniformLocation(frameShader.programID, "exposure"), camera.exposure);

	glBindVertexArray(quadVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glActiveTexture(0);

	//glDisable(GL_FRAMEBUFFER_SRGB);
}

void SceneRenderer::drawDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float duration)
{
	DebugObject n = DebugObject();

	n.data = (GLfloat*)malloc(12 * sizeof(GLfloat));
	n.data_size = 12;
	n.data[0] = start.x; n.data[1] = start.y; n.data[2] = start.z;
	n.data[3] = color.x; n.data[4] = color.y; n.data[5] = color.z;
	n.data[6] = end.x; n.data[7] = end.y; n.data[8] = end.z;
	n.data[9] = color.x; n.data[10] = color.y; n.data[11] = color.z;

	glGenVertexArrays(1, &n.VAO);
	glGenBuffers(1, &n.VBO);
	glBindVertexArray(n.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, n.VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), n.data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 


	n.duration = duration;
	n.vertexCount = 2;
	n.type = GL_LINES;
	debug.push_back(n);
}

void SceneRenderer::drawDebugPoint(glm::vec3 pos, glm::vec3 color, float duration)
{
	DebugObject point = DebugObject();
	point.data = (GLfloat*)malloc(6 * sizeof(GLfloat));
	point.data_size = 6;
	point.data[0] = pos.x; point.data[1] = pos.y; point.data[2] = pos.z;
	point.data[3] = color.x; point.data[4] = color.y; point.data[5] = color.z;

	glGenVertexArrays(1, &point.VAO);
	glGenBuffers(1, &point.VBO);
	glBindVertexArray(point.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, point.VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), point.data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	point.duration = duration;
	point.vertexCount = 1;
	point.type = GL_POINTS;
	debug.push_back(point);
}

void SceneRenderer::drawDebugArrow(glm::vec3 start, glm::vec3 end, glm::vec3 color, float duration)
{
	drawDebugArrow(start, end, color, color, duration);
}

void SceneRenderer::drawDebugArrow(glm::vec3 start, glm::vec3 end, glm::vec3 color, glm::vec3 headColor, float duration)
{
	// Line
	DebugObject line = DebugObject();

	line.data = (GLfloat*)malloc(12 * sizeof(GLfloat));
	line.data_size = 12;
	line.data[0] = start.x; line.data[1] = start.y; line.data[2] = start.z;
	line.data[3] = color.x; line.data[4] = color.y; line.data[5] = color.z;
	line.data[6] = end.x; line.data[7] = end.y; line.data[8] = end.z;
	line.data[9] = color.x; line.data[10] = color.y; line.data[11] = color.z;

	glGenVertexArrays(1, &line.VAO);
	glGenBuffers(1, &line.VBO);
	glBindVertexArray(line.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, line.VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), line.data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	line.duration = duration;
	line.vertexCount = 2;
	line.type = GL_LINES;
	debug.push_back(line);

	// Head
	DebugObject head = DebugObject();
	head.data = (GLfloat*)malloc(3 * 12 * sizeof(GLfloat)); // 4 lines
	head.data_size = 3 * 12; // 4 lines

	// Generate Data

	glm::vec3 forwardVector = end - start;

	for (int i = 0; i < 3; i++)
	{
		glm::vec3 st;

		if (i == 0)
		{
			// Left line
			st = end - glm::normalize(glm::rotateY(forwardVector, glm::radians(45.0f)));
		}
		else if (i == 1)
		{
			// Right line
			st = end - glm::normalize(glm::rotateY(forwardVector, glm::radians(-45.0f)));
		}
		else if (i == 2)
		{
			// Front line
			st = end - glm::normalize(glm::rotateX(forwardVector, glm::radians(45.0f)));
		}
		
		head.data[i * 12] = st.x; head.data[i * 12 + 1] = st.y; head.data[i * 12 + 2] = st.z;
		head.data[i * 12 + 3] = headColor.x; head.data[i * 12 + 4] = headColor.y; head.data[i * 12 + 5] = headColor.z;
		head.data[i * 12 + 6] = end.x; head.data[i * 12 + 7] = end.y; head.data[i * 12 + 8] = end.z;
		head.data[i * 12 + 9] = headColor.x; head.data[i * 12 + 10] = headColor.y; head.data[i * 12 + 11] = headColor.z;

	}


	glGenVertexArrays(1, &head.VAO);
	glGenBuffers(1, &head.VBO);
	glBindVertexArray(head.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, head.VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * 3 * sizeof(GLfloat), head.data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	head.duration = duration;
	head.vertexCount = 6;
	head.type = GL_LINES;
	debug.push_back(head);
}

void SceneRenderer::createFramebuffer(int WIDTH, int HEIGHT)
{

	// Vertex attributes for our fullscreen quad


	GLfloat quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);



	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Generate texture
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG_F(ERROR, "Could not create framebuffer! Program is probably unstable");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Shadow framebuffer
	// -------------------------------------------------------------------------
	glGenFramebuffers(1, &depthMapFBO);


	//TODO Please let user change this


	// Generate the texture for the framebuffer
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Attach our texture to the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	// We need no color data! (thx learnopengl.com)
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void SceneRenderer::start(int WIDTH, int HEIGHT)
{
	createFramebuffer(WIDTH, HEIGHT);
	frameShader = Shader("Resource/Shader/framebuffer.vert", "Resource/Shader/framebuffer.frag");
	shadowShader = Shader("Resource/Shader/shadow_shader.vert", "Resource/Shader/shadow_shader.frag");
	debugShader = Shader("Resource/Shader/basic_unlit.vert", "Resource/Shader/basic_unlit.frag");
	skyShader = Shader("Resource/Shader/basic_skybox.vert", "Resource/Shader/basic_skybox.frag");

	// Create skybox data
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);
	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

GLuint SceneRenderer::generateAttachmentTexture(GLboolean depth, GLboolean stencil, int width, int height)
{
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, width, height, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

SceneRenderer::SceneRenderer()
{
}


SceneRenderer::~SceneRenderer()
{
}
