#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>

const float PI = 3.14159265358979323;

const float g = 9.80665;

const int GRID_WIDTH = 20;

const int GRID_HEIGHT = 20;

const float gridCellSize = 5;


class box {

	public:

		float mass = 10000000000;

		GLfloat verticesbox[16] = {

			-1.0, -1.0,
			 1.0, -1.0,

			 1.0, -1.0,
			 1.0,  1.0,

			 1.0,  1.0,
			-1.0,  1.0,

			-1.0,  1.0,
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


		void updateObjectPosition(std::vector<object>& objects, box box1, std::vector<std::vector<object*>>& grid) {

		GLfloat totalTime = glfwGetTime();
		deltaTime = totalTime - frameTime;
		frameTime = totalTime;

		borderCollision(box1);

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


void init(std::vector<object>& objects, int numberOfParticles);

void display(std::vector<object>& objects, box box1, GLFWwindow* window, GLuint& shaderProgram, std::vector<std::vector<object*>>& grid);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void rendersphere(object& obt);

void renderbox(box box1);

void updateObjectPosition(object& obt);

void updateGrid(std::vector<object>& objects, std::vector<std::vector<object*>>& grid);

void collisionDetection(std::vector<object>& objects, std::vector<std::vector<object*>>& grid);


int main() {

	std::vector<object> objects;

	std::vector<std::vector<object*>> grid(GRID_WIDTH, std::vector<object*>(GRID_HEIGHT, nullptr));

	box box1;

	GLuint shaderProgram;

	int numberOfParticles = 5;


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

    init(objects, numberOfParticles);


    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
		updateGrid(objects, grid);
		collisionDetection(objects, grid);
		display(objects, box1, window, shaderProgram, grid);
    
	}

    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}

void init(std::vector<object>& objects, int numberOfParticles) {

	float xposition = -0.0;
	float yposition =  0.0;
	float yprime = yposition;
	float height = 0.5;
	float increase = height / numberOfParticles;
	

	for (int i = 0; i < numberOfParticles; i++) {

		objects.push_back(object());

		objects[i].radius = 0.01;						//0.001538
		objects[i].mass = 4.809 * pow(10, -23);
		objects[i].color1 = 1.0;
		objects[i].color2 = 0.0;
		objects[i].color3 = 0.0;
		objects[i].e = 0.85;

		objects[i].coordinatesX = xposition;
		objects[i].coordinatesY = yposition;
	
		objects[i].velocityX = 0.5;
		objects[i].velocityY = 0.4;

		objects[i].accelerationX = 0.0;
		objects[i].accelerationY = -0.0;

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
	
}

void display(std::vector<object>& objects, box box1, GLFWwindow* window, GLuint& shaderProgram, std::vector<std::vector<object*>>& grid) {


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glUseProgram(shaderProgram);


	for (int i = 0; i < objects.size(); i++) {
		
		objects[i].updateObjectPosition(objects, box1, grid);
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

void updateGrid(std::vector<object>& objects, std::vector<std::vector<object*>>& grid) {

	float sum = 1 / gridCellSize;

	for (int x = 0; x < GRID_WIDTH; x++) {
        		
		for (int y = 0; y < GRID_HEIGHT; y++) {
            
			grid[x][y] = nullptr;
        		
		}
    		
	}

	for (int i = 0; i < objects.size(); i++) {

		int gridX = static_cast<int>((objects[i].coordinatesX / gridCellSize) + (sum));
		int gridY = static_cast<int>((objects[i].coordinatesY / gridCellSize) + (sum));

		if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {

			grid[gridX][gridY] = &objects[i];

		}

	}
	
}


void collisionDetection(std::vector<object>& objects, std::vector<std::vector<object*>>& grid) {
	
	float sum = 1 / gridCellSize;

	for (int i = 0; i < objects.size(); i++) {

		int actualCellX = static_cast<int>((objects[i].coordinatesX / gridCellSize) + sum);
		int actualCellY = static_cast<int>((objects[i].coordinatesY / gridCellSize) + sum);

		for (int dx = -1; dx <= 1; dx++) {

			for (int dy = -1; dy <= 1; dy++) {

				int comparasionCellX = actualCellX + dx;
				int comparasionCellY = actualCellY + dy;

				if (comparasionCellX >= 0 && comparasionCellX < GRID_WIDTH && comparasionCellY >= 0 && comparasionCellY < GRID_HEIGHT) {

					float dx = (grid[actualCellX][actualCellY]->coordinatesX - grid[comparasionCellX][comparasionCellY]->coordinatesX) - (grid[actualCellX][actualCellY]->radius - grid[comparasionCellX][comparasionCellY]->radius);
					float dy = (grid[actualCellX][actualCellY]->coordinatesY - grid[comparasionCellX][comparasionCellY]->coordinatesY) - (grid[actualCellX][actualCellY]->radius - grid[comparasionCellX][comparasionCellY]->radius);
					float distance = sqrt((dx * dx) + (dy * dy));
	
					float limit = (grid[actualCellX][actualCellY]->radius) + (grid[comparasionCellX][comparasionCellY]->radius);
		
					if (distance <= limit) {

						float normalX = dx / distance;
						float normalY = dy / distance;

						float relativeVelocityX = grid[actualCellX][actualCellY]->velocityX - grid[comparasionCellX][comparasionCellY]->velocityX;
						float relativeVelocityY = grid[actualCellX][actualCellY]->velocityY - grid[comparasionCellX][comparasionCellY]->velocityY;

						float dotProduct = (relativeVelocityX * normalX) + (relativeVelocityY * normalY);

						grid[actualCellX][actualCellY]->velocityX += normalX * dotProduct;
						grid[actualCellX][actualCellY]->velocityY += normalY * dotProduct;		
		
						grid[comparasionCellX][comparasionCellY]->velocityX -= normalX * dotProduct;
						grid[comparasionCellX][comparasionCellY]->velocityY -= normalY * dotProduct;

					}

				}

			}

		}

	}

}
