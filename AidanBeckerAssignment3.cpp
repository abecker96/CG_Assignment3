//General includes
#include <stdio.h>
#include <iostream>

//Opengl includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Project-specific includes
#include "FractalMesh.h"
#include "glCamera.h"


const int meshRows = 2;
const int meshCols = 2;
const int meshDepth = 2;

FractalMesh fractalMeshA;
FractalMesh fractalMeshes[meshRows][meshCols][meshDepth];
Camera camera = Camera();

bool translateMeshes = false;
bool rotateMeshes = false;

// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats/5289624
// Generates a random float between two values.
// Does not care if the largest value is first or last
float randomBetween(float a, float b) {
    if(a > b) {
        float temp = a;
        a = b;
        b = temp;
    }
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b-a;
    float r = random * diff;
    return a + r;
}


// mousebutton callback function
// A left click generates more triangles, while a right click resets to a new triangle
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        for(int i = 0; i < meshRows; i++)
        {
            for(int j = 0; j < meshCols; j++)
            {
                for(int k = 0; k < meshDepth; k++)
                {
                    fractalMeshes[i][j][k].fractalize();
                }
            }
        }
        //fractalMeshA.fractalize();
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        for(int i = 0; i < meshRows; i++)
        {
            for(int j = 0; j < meshCols; j++)
            {
                for(int k = 0; k < meshDepth; k++)
                {
                    fractalMeshes[i][j][k].reset(i+j*10+k*100);
                }
            }
        }
        //fractalMeshA.reset(i+j+k);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    // Check that this event is for an initial key press
    if(action == GLFW_PRESS)
    {
        switch(key){
            case GLFW_KEY_1:        // 1-4 change colors
                for(int i = 0; i < meshRows; i++)
                {
                    for(int j = 0; j < meshCols; j++)
                    {
                        for(int k = 0; k < meshDepth; k++)
                        {
                            fractalMeshes[i][j][k].setColor(0);
                        }
                    }
                }
                //fractalMeshA.setColor(0);
                // colorSelector = 0;
                break;
            case GLFW_KEY_2:
                for(int i = 0; i < meshRows; i++)
                {
                    for(int j = 0; j < meshCols; j++)
                    {
                        for(int k = 0; k < meshDepth; k++)
                        {
                            fractalMeshes[i][j][k].setColor(1);
                        }
                    }
                }
                //fractalMeshA.setColor(1);
                // colorSelector = 1;
                break;
            case GLFW_KEY_3:
                for(int i = 0; i < meshRows; i++)
                {
                    for(int j = 0; j < meshCols; j++)
                    {
                        for(int k = 0; k < meshDepth; k++)
                        {
                            fractalMeshes[i][j][k].setColor(2);
                        }
                    }
                }
                //fractalMeshA.setColor(2);
                // colorSelector = 2;
                break;
            case GLFW_KEY_4:
                for(int i = 0; i < meshRows; i++)
                {
                    for(int j = 0; j < meshCols; j++)
                    {
                        for(int k = 0; k < meshDepth; k++)
                        {
                            fractalMeshes[i][j][k].setColor(3);
                        }
                    }
                }
                //fractalMeshA.setColor(3);
                // colorSelector = 3;
                break;
            case GLFW_KEY_Q:        // Q toggles wireframe rendering
                for(int i = 0; i < meshRows; i++)
                {
                    for(int j = 0; j < meshCols; j++)
                    {
                        for(int k = 0; k < meshDepth; k++)
                        {
                            fractalMeshes[i][j][k].toggleWireframe();
                        }
                    }
                }
                //fractalMeshA.toggleWireframe();
                // renderWireframe = !renderWireframe;
                break;
            case GLFW_KEY_E:        // E toggles faces rendering
                for(int i = 0; i < meshRows; i++)
                {
                    for(int j = 0; j < meshCols; j++)
                    {
                        for(int k = 0; k < meshDepth; k++)
                        {
                            fractalMeshes[i][j][k].toggleFaces();
                        }
                    }
                }
                //fractalMeshA.toggleFaces();
                // renderFaces = !renderFaces;
                break;
            case GLFW_KEY_T:
                translateMeshes = !translateMeshes;
                break;
            case GLFW_KEY_R:
                rotateMeshes = !rotateMeshes;
                break;
            default:
                break;
        }
    }
}

void glfwErrorCB(int error, const char* description) {
    fputs(description, stderr);
}

int main() {
    int windowWidth, windowHeight, windowSizeX, windowSizeY;    //Screen space values

    const float cameraSpeed = 0.3f;                 //Camera speed
    const float mouseSensitivity = 0.05f;           //Mouse sensitivity
    float horizontalAngle = 0.0f;                   //initial camera angle
    float verticalAngle = 0.0f;                     //initial camera angle
    float initialFoV = 62.0f;                       //initial camera field of view
    glm::vec3 cameraPosition(0, 0, -1);             //initial camera position

    // Start a timer to limit framerate
    double start = glfwGetTime();
    double current = start;
    double deltaTime;

    //Initialize variables for translation and rotation speed
    glm::vec3 translation = glm::vec3(0.01, 0, 0);
    glm::vec3 rotationAxis = glm::vec3(0, 1, 0);
    float rotationSpeed = 0.1;
    float currentRotation = 0;

    // Necessary due to glew bug
    glewExperimental = true;
    glfwSetErrorCallback(glfwErrorCB);
    // Initialize glfw
    if(!glfwInit())
    {
        fprintf( stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Create the window
    glfwWindowHint(GLFW_SAMPLES, 8); // 8x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    // Check size of screen's available work area
    // This is the area not taken up by taskbars and other OS objects
    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &windowWidth, &windowHeight, &windowSizeX, &windowSizeY);
    //std::cout << "windowSizeX: " << windowSizeX << " windowSizeY: " << windowSizeY << " windowWidth: " << windowWidth << " windowHeight: " << windowHeight << std::endl;
    window = glfwCreateWindow( windowSizeX, windowSizeY, "Aidan Becker Assignment 1", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key and mouse clicks being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    // Initialize fractalMesh objects
    int spacing = 1;
    for(int i = 0; i < meshRows; i++)
    {
        for(int j = 0; j < meshCols; j++)
        {
            for(int k = 0; k < meshDepth; k++)
            {
                fractalMeshes[i][j][k].init(window, 
                    glm::vec3(spacing*i, spacing*j, spacing*k), 
                    glm::scale(glm::vec3(0.5, 0.5, 0.5)), 
                    glm::rotate(0.0f, glm::vec3(0, 1, 0)),
                    i%2+1, j%2);
            }
        }
    }


    // Initialize camera object
    camera.init(
        window, cameraPosition, 
        glm::perspective(
            glm::radians<float>(55),
            (float)windowSizeX/(float)windowSizeY,
            0.0001f, 
            1000.0f
        ),
        horizontalAngle, verticalAngle,
        cameraSpeed*2, mouseSensitivity
    );

    // Set callback functions for user input
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    // Enable depth test so things render based on distance from camera
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set default background color to something a little less void-colored
    glClearColor( 0.1, 0.1, 0.1 , 1.0 );
    do{
        // Clear the screen before drawing new things
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        // Check if 33 ms have elapsed since the last frame
        current = glfwGetTime();
        deltaTime = current-start;
        //if(deltaTime > 0.006944)
        {   
            // std::cout << "New Frame in: " << deltaTime << std::endl;
            // Reset timer
            start = glfwGetTime();
            // Draw!

            camera.update();
            for(int i = 0; i < meshRows; i++)
            {
                for(int j = 0; j < meshCols; j++)
                {
                    for(int k = 0; k < meshDepth; k++)
                    {
                        if(translateMeshes)
                        {
                            fractalMeshes[i][j][k].translate(translation);
                        }
                        else
                        {
                            fractalMeshes[i][j][k].resetPosition();
                        }
                        if(rotateMeshes)
                        {
                            currentRotation += rotationSpeed*deltaTime;
                            fractalMeshes[i][j][k].rotateAroundOrigin(currentRotation, rotationAxis);
                        }
                        else
                        {
                            currentRotation = 0;
                            fractalMeshes[i][j][k].rotateAroundOrigin(0, glm::vec3(0, 1, 0));
                        }
                        fractalMeshes[i][j][k].draw(camera.getViewMatrix(), camera.getProjectionMatrix());
                    }
                }
            }
            glfwSwapBuffers(window);    // actually draw
        }

        // Update input events
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 );

    return 0;
}