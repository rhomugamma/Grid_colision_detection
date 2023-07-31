#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>

const float PI = 3.14159265358979323;

const float g = 9.80665;


class box {

	public:

		float mass = 10000000000;

		GLfloat verticesbox[16] = {

			-1.0, -1.0,
			 1.0, -1.0,

			 1.0, -1.0,
			 1.0,  1.0,

			 1.0, 1.0,
			-1.0, 1.0,

			-1.0, 1.0,
			-1.0, -1.0

		};


		GLfloat colorbox[24] {

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,

		};

		GLuint VAO;

		void renderBoxObject() {

	        glGenVertexArrays(1, &VAO);
    	    glBindVertexArray(VAO);

	        GLuint vertexVBO;
        	glGenBuffers(1, &vertexVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesbox), verticesbox, GL_STATIC_DRAW);
        	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	        glEnableVertexAttribArray(0);

        	GLuint colorVBO;
        	glGenBuffers(1, &colorVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(colorbox), colorbox, GL_STATIC_DRAW);
       		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
     	   	glEnableVertexAttribArray(1);

        	glBindVertexArray(0);
    	
		}

	    void cleanup() {

    	    glDeleteVertexArrays(1, &VAO);
    
		}

};

class object {

	public:

		//Physical properties
		float radius;
		float mass;
		float weight = (mass) * (g);
		float color1;
		float color2;
		float color3;
		float e;

		//Rendering properties
		int iterations = 15;
		float alpha = (360 / 15) * (2 * PI / 360);
		float beta = alpha;
		/* const static int verticesArraySize = 6 * iterations; */
		/* const static int colorsArraySize = 9 * iterations; */
		GLfloat vertices[90];													
		GLfloat colors[135];
		GLuint VAO;

		//Goberning characteristics
		float coordinatesX;
		float coordinatesY;

		float velocityX;
		float velocityY;

		float accelerationX;
		float accelerationY;

		//Time characteristics
		float deltaTime;
		float frameTime;

		void renderSphereObject() {

			GLfloat totalTime = glfwGetTime();
			deltaTime = totalTime - frameTime;
			frameTime = totalTime;


			for (int i = 0; i < 6 * iterations; i += 6) {

				vertices[i] = coordinatesX;
        		vertices[i + 1] = coordinatesY;

		        vertices[i + 2] = radius * cos(alpha) + coordinatesX;
 		       	vertices[i + 3] = radius * sin(alpha) + coordinatesY;

 		       	vertices[i + 4] = radius * cos(alpha + beta) + coordinatesX;
        		vertices[i + 5] = radius * sin(alpha + beta) + coordinatesY;

				alpha += beta;

			};

    		for (int i = 0; i < 9 * iterations; i += 3) {

		        colors[i] = color1;
       			colors[i + 1] = color2;
     		    colors[i + 2] = color3;
    
			};

		}


		void updateObjectPosition(std::vector<object>& objects) {

		GLfloat totalTime = glfwGetTime();
		deltaTime = totalTime - frameTime;
		frameTime = totalTime;

		coordinatesX = (coordinatesX) + (velocityX * deltaTime) + ((1/2) * (accelerationX) * (deltaTime * deltaTime));	
		coordinatesY = (coordinatesY) + (velocityY * deltaTime) + ((1/2) * (accelerationY) * (deltaTime * deltaTime));

		velocityX = (velocityX) + ((accelerationX) * (deltaTime));
		velocityY = (velocityY) + ((accelerationY) * (deltaTime));

		
		for (int i = 0; i < 6 * iterations; i += 6) {

				vertices[i] = coordinatesX;
	    	    vertices[i + 1] = coordinatesY;

				vertices[i + 2] = radius * cos(alpha) + coordinatesX;
    		    vertices[i + 3] = radius * sin(alpha) + coordinatesY;

 				vertices[i + 4] = radius * cos(alpha + beta) + coordinatesX;
	        	vertices[i + 5] = radius * sin(alpha + beta) + coordinatesY;

				alpha += beta;

			};
 
		}


		void renderObject() {

	        glGenVertexArrays(1, &VAO);
    	    glBindVertexArray(VAO);

	        GLuint vertexVBO;
        	glGenBuffers(1, &vertexVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	        glEnableVertexAttribArray(0);

        	GLuint colorVBO;
        	glGenBuffers(1, &colorVBO);
        	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
       		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
     	   	glEnableVertexAttribArray(1);

        	glBindVertexArray(0);
    	
		}


		void borderCollision(box box1) {


			if (coordinatesX + radius > box1.verticesbox[2] || coordinatesX - radius < box1.verticesbox[0]) {

				velocityX =	((velocityX) * ((mass) - (box1.mass * e))) / (mass + box1.mass);

			}
				
				
			else if (coordinatesY + radius > box1.verticesbox[7] || coordinatesY - radius < box1.verticesbox[1]) {

				velocityY =	((velocityY) * ((mass) - (box1.mass * e))) / (mass + box1.mass);

			}

		}	

	    void cleanup() {

    	    glDeleteVertexArrays(1, &VAO);
    
		}

};


class grid {

	public: 

		int width;
		int height;
		float gridCellSize;

};


const GLchar* vertexShaderSource = R"(

    #version 450 core

    layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 color;

	out vec3 vertexColor;

    void main() {

		vertexColor = color;
        gl_Position = vec4(position.x, position.y, 0.0, 1.0);

    }

)";

const GLchar* fragmentShaderSource = R"(

    #version 450 core

	in vec3 vertexColor;
	out vec4 fragColor;

    void main() {

        fragColor = vec4(vertexColor, 1.0f);

    }

)";


void init(std::vector<object>& objects, grid& grid1, std::vector<std::vector<std::vector<object*>>>& gr, GLFWwindow* window);

void display(std::vector<object>& objects, box box1, GLFWwindow* window, GLuint& shaderProgram);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void rendersphere(object& obt);

void renderbox(box box1);

void update(std::vector<object>& objects, std::vector<std::vector<std::vector<object*>>>& gr, grid& grid1, box box1);

void updateGrid(std::vector<object>& objects, std::vector<std::vector<std::vector<object*>>>& gr, grid& grid1);

void handleCollision(std::vector<object>& objects, std::vector<std::vector<std::vector<object*>>>& gr, grid& grid1);


int main() {

	std::vector<object> objects;

	std::vector<std::vector<std::vector<object*>>> gr;

	box box1;

	GLuint shaderProgram;

	grid grid1;


    if (!glfwInit()) {

        std::cout << "GLFW initialization failed!" << std::endl;
        return -1;
    
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(3840, 2160, "Dynamic Window", nullptr, nullptr);

    if (!window) {

        std::cout << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return -1;

    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
		 
        std::cout << "GLEW initialization failed!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    
	}

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    init(objects, grid1, gr, window);

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
		update(objects, gr, grid1, box1);
		display(objects, box1, window, shaderProgram);
    
	}

    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}


void init(std::vector<object>& objects, grid& grid1, std::vector<std::vector<std::vector<object*>>>& gr, GLFWwindow* window) {

	float xposition = -0.0;
	float yposition = -0.0;
	float yprime = yposition;
	float numberObjects = 3;
	float height = 0.5;
	float increase = height / numberObjects;
	

	for (int i = 0; i < numberObjects; i++) {

		objects.push_back(object());

		objects[i].radius = 0.01;						//0.001538
		objects[i].mass = 4.809 * pow(10, -23);
		objects[i].color1 = 1.0;
		objects[i].color2 = 0.0;
		objects[i].color3 = 0.0;
		objects[i].e = 1.0;

		objects[i].coordinatesX = xposition;
		objects[i].coordinatesY = yposition;
	
		objects[i].velocityX = 0.5;
		objects[i].velocityY = 0.4;

		objects[i].accelerationX = 0.0;
		objects[i].accelerationY = 0.00;

		objects[i].deltaTime = 0.0;
		objects[i].frameTime = 0.0;

		objects[i].renderSphereObject();
					  
		if ((yposition + increase) < 1.0) {
			
			yposition += increase;

		}

		else {

			yposition = yprime;

		}

	}

	grid1.width = 50;
	grid1.width = 50;
	grid1.gridCellSize = 0.01;

	int gridWidth;
	int gridHeight;
	glfwGetFramebufferSize(window, &gridWidth, &gridHeight);
		

	gr.resize(gridWidth, std::vector<std::vector<object*>>(gridHeight));
	
}


void display(std::vector<object>& objects, box box1, GLFWwindow* window, GLuint& shaderProgram) {


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glUseProgram(shaderProgram);

	for (int i = 0; i < objects.size(); i++) {

		rendersphere(objects[i]);

	}


	renderbox(box1);

	glfwSwapBuffers(window);

}


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);

}


void rendersphere(object& obt) {

	obt.renderObject();
	glBindVertexArray(obt.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * obt.iterations);
	glBindVertexArray(0);
 
}


void renderbox(box box1) {

	box1.renderBoxObject();
	glBindVertexArray(box1.VAO);
	glDrawArrays(GL_LINES, 0, 4);
	glDrawArrays(GL_LINES, 4, 4);
	glDrawArrays(GL_LINES, 8, 4);
	glDrawArrays(GL_LINES, 12, 4);
	glBindVertexArray(0);

}


void update(std::vector<object>& objects, std::vector<std::vector<std::vector<object*>>>& gr, grid& grid1, box box1) {

	for (int i = 0; i < objects.size(); i++) {

		objects[i].updateObjectPosition(objects);
		objects[i].borderCollision(box1);

	}

	updateGrid(objects, gr, grid1);
	handleCollision(objects, gr, grid1);

}


void updateGrid(std::vector<object>& objects, std::vector<std::vector<std::vector<object*>>>& gr, grid& grid1) {

	float sum = 1 / grid1.gridCellSize;

	for (int x = 0; x < gr.size(); x++) {

		for (int y = 0; y < gr[x].size(); y++) {

			gr[x][y].clear();

		}

	}

	for (int i = 0; i < objects.size(); i++) {

		int actualCellX = static_cast<int>((objects[i].coordinatesX / grid1.gridCellSize) + sum);
		int actualCellY = static_cast<int>((objects[i].coordinatesY / grid1.gridCellSize) + sum);

		gr[actualCellX][actualCellY].push_back(&objects[i]);

	}

}


void handleCollision(std::vector<object>& objects, std::vector<std::vector<std::vector<object*>>>& gr, grid& grid1) {

	for (int x = 0; x < grid1.width - 1; x++) {

		for (int y = 0; y < grid1.height - 1; y++) {

			int actualCellX = x;
			int actualCellY = y;

			for (int dx = -1; dx <= 1; dx++) {

				for (int dy = -1; dy <= 1; dy++) {

					int comparasionCellX = actualCellX + dx;
					int comparasionCellY = actualCellY + dy;

					if (actualCellX >= 0 && actualCellX < grid1.width && comparasionCellY >= 0 && comparasionCellY < grid1.height) {

						for (int i = 0; i < gr[actualCellX][actualCellY].size(); i++) {

							for (int j = 0; j < gr[comparasionCellX][comparasionCellY].size(); j++) {

								float distanceX = gr[i][actualCellX][actualCellY]->coordinatesX - gr[j][comparasionCellX][comparasionCellY]->coordinatesX;
								float distanceY = gr[i][actualCellX][actualCellY]->coordinatesY - gr[j][comparasionCellX][comparasionCellY]->coordinatesY;
								float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));

								float limit = gr[i][actualCellX][actualCellY]->radius + gr[j][comparasionCellX][comparasionCellY]->radius;

								if (distance <= limit) {

									float normalX = distanceX / distance;
									float normalY = distanceY / distance;

									float relativeVelocityX = gr[i][actualCellX][actualCellY]->radius - gr[j][comparasionCellX][comparasionCellY]->radius;
									float relativeVelocityY = gr[i][actualCellX][actualCellY]->radius - gr[j][comparasionCellX][comparasionCellY]->radius;
									float dotProduct = (normalX * relativeVelocityX) + (normalY * relativeVelocityY);

									gr[i][actualCellX][actualCellY]->velocityX += normalX * dotProduct;
									gr[i][actualCellX][actualCellY]->velocityY += normalY * dotProduct;
									gr[j][comparasionCellX][comparasionCellY]->velocityX -= normalX * dotProduct;
									gr[j][comparasionCellX][comparasionCellY]->velocityY -= normalY * dotProduct;

								}

							}

						}

					}

				}

			}

		}

	}	

}
