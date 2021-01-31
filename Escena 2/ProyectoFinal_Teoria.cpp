// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void MovimientoVirus( );
void MovimientoLinfo1( );
void MovimientoLinfo2();

// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
//Para elegir que pez se verá y por default es 0
int figura = 0; 
//Mueven al virus
float Xvirus1 = 50.0;
float Xvirus2 = 50.0;
float Xvirus3 = 50.0;
//Mueven al linfo
float Ylinfo1 = 20;
float Zlinfo1 = -30;
float Zlinfo2 = 30;

bool es2 = false;
bool ActivaVirus = false;
bool ActivalinfoP1 = false; 
bool ActivalinfoP2 = false;

int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Proyecto - Escena 2 ", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shaders/modelLoading.vs", "Shaders/modelLoading.frag" );
    Model CajaDeRegalo((char*)"Models/Proyecto/CajaDeRegalo.obj");
 
    // Load models
    Model organismo((char *)"Models/Escena2/organismo.obj");
    Model linfocito((char*)"Models/Escena2/linfocito.obj");
    Model virus((char*)"Models/Escena2/virus.obj");

   
    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        shader.Use( );
        glm::mat4 model = glm::mat4(1);
        
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );


        if (ActivaVirus)
        {
            MovimientoVirus();
        }
        if (ActivalinfoP1)
        {
            MovimientoLinfo1();
        }
        if (ActivalinfoP2)
        {
            MovimientoLinfo2();
        }
        
        // Draw the loaded model

        //*********ESCENA 2*********

        //Organismo
        model = glm::mat4(1);
        model = glm::translate( model, glm::vec3( 0.0f, 0.0f, 0.0f ) ); // Translate it down a bit so it's at the center of the scene
        model = glm::scale( model, glm::vec3( 1.5f, 1.5f, 1.5f ) );	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        organismo.Draw(shader);
        
        

        //Linfocito
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, Zlinfo1, 0.0)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        linfocito.Draw(shader);

        //Linfocito 2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.5f, 2.0f, Ylinfo1)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        linfocito.Draw(shader);

        //Linfocito3
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-6.5f, Zlinfo2, -3.5)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        linfocito.Draw(shader);
        

        //Virus
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xvirus1, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        virus.Draw(shader);

        //Virus2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xvirus2, 2.0f, 3.5f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        virus.Draw(shader);

        //Virus3
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(Xvirus3, -2.0, -3.5f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        virus.Draw(shader);
        

        
      /*  switch(figura)
        {
            case 0:
                ourModel.Draw(shader);
                break;
            case 1: 
                ourModel2.Draw(shader);
                break;
            case 2:
                ourModel3.Draw(shader);
                break;
            case 3:
                ourModel4.Draw(shader);
                break;
        }*/

		   

     
        
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
    //Precionar tecla hacia adelante 
   
    if (keys[GLFW_KEY_1])
    {
        ActivaVirus = !ActivaVirus;
        
    }
    if (keys[GLFW_KEY_2])
    {
        ActivalinfoP1 = !ActivalinfoP1;
    }
    if (keys[GLFW_KEY_3])
    {
        ActivalinfoP2 = !ActivalinfoP2;
    }
}

void MovimientoVirus()
{
    if (ActivaVirus == true)
    {
        if (Xvirus1 > 0)
        {
            Xvirus1 -= 0.02;
            
        }
        if (Xvirus2 > -4.5)
        {
            Xvirus2 -= 0.015;
        }
        if (Xvirus3 > -6.5)
        {
            Xvirus3 -= 0.015;
        }
    }
}

void MovimientoLinfo1()
{
    if (ActivalinfoP1 == true)
    {
        if (Zlinfo1 < - 3.0)
        {
            Zlinfo1 += 0.02;
        }
        if (Ylinfo1 > 6.5)
        {
            Ylinfo1 -= 0.015;
        }
        if (Zlinfo2 > 2.0)
        {
            Zlinfo2 -= 0.015;
        }
    }
}

void MovimientoLinfo2()
{
    if (ActivalinfoP2 == true)
    {
        if (Zlinfo1 < 0.0)
        {
            Zlinfo1 += 0.02;
        }
        if (Ylinfo1 > 3.5)
        {
            Ylinfo1 -= 0.015;
        }
        if (Zlinfo2 > -2.0)
        {
            Zlinfo2 -= 0.015;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

