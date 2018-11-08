#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shaders.h"
#include "Mesh.h"
#include "MatrixStack.h"

void funInit();
void funDestroy();
void funReshape(int w, int h);
void funDisplay();
void funSpecial(int key, int x, int y);

void drawTriangle();
void drawAspa();
void drawHelice();
void drawCilinder();
void drawPoste();
void funTimer(int value);
//speed
 GLint speed   = 10;
// Viewport
   int w = 500;
   int h = 500;
   
// Animaciones
   GLfloat desZ = 0.0f;
   GLfloat rotZ = 0.0f;
   GLfloat gY = 0.0f;
// Objetos
   Shaders *objShaders;
   Mesh    *triangle1, *triangle2,*cilinder,*cilinder2;
   
// Pila de matrices
   MatrixStack PVM;
    
int main(int argc, char** argv) {

 // Inicializamos GLUT y el contexto de OpenGL
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);   
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); 
    glutInitContextProfile(GLUT_CORE_PROFILE);
    
 // Inicializamos el FrameBuffer y la ventana   
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Sesion 05");
    
 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;
    
 // Inicializaciones específicas
    funInit();
    
 // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutSpecialFunc(funSpecial);
    glutTimerFunc(speed,funTimer,0);   
 // Bucle principal
    glutMainLoop();
    
 // Liberamos memoria
    funDestroy();
    
    return 0;
}

void funInit() {
   
 // Test de profundidad
    glEnable(GL_DEPTH_TEST); 
    glPolygonOffset(1.0f,1.0f);
    
 // Shaders
    objShaders = new Shaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Malla con el triángulo
    const char* trianglePath = "resources/models/triangle.obj";
    triangle1 = new Mesh(trianglePath);
    triangle1->setColor(glm::vec3(1.0f, 1.0f, 0.0f));
    triangle1->createVao();
    triangle2 = new Mesh(trianglePath);
    triangle2->setColor(glm::vec3(1.0f, 0.0f, 1.0f));
    triangle2->createVao();
 // Malla con el cilindro
    const char* cilinderPath = "resources/models/cilinder.obj";
    cilinder = new Mesh(cilinderPath);
    cilinder->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
    cilinder->createVao();
    cilinder2 = new Mesh(cilinderPath);
    cilinder2->setColor(glm::vec3(1.0f, 1.0f, 0.0f));
    cilinder2->createVao();
    
}

void funDestroy() {
      
    delete objShaders;
    delete triangle1, triangle2;
    delete cilinder;
}

void funReshape(int wnew, int hnew) {
    
 // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

 // Captura de w y h
    w = wnew;
    h = hnew;
    
}

void funDisplay() {
    
 // Borramos el buffer de color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
 // Matriz de Proyección P (Perspectiva)
    GLfloat fovy   = 60.0f;
    GLfloat nplane =  0.1f;
    GLfloat fplane = 25.0f;
    GLfloat aspectRatio = (GLfloat)w/(GLfloat)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspectRatio, nplane, fplane); 

 // Matriz de Vista V (Cámara)
    glm::vec3 pos   (0.0f, 3.0f, 4.0f);
    glm::vec3 lookat(0.0f, 0.0f, 0.0f);
    glm::vec3 up    (0.0f, 1.0f, 0.0f);
    glm::mat4 V = glm::lookAt(pos, lookat, up);   

 // Dibujamos la hélice (Modelo)
    PVM.set(P*V);
    PVM.set(glm::translate(PVM.get(), glm::vec3(0.0f, 0.0f, desZ)));
    PVM.set(glm::rotate(PVM.get(), glm::radians(gY), glm::vec3(0.0f, 1.0f, 0.0f))); 
    
    PVM.push();
        PVM.set(glm::rotate(PVM.get(), glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f))); 
        drawHelice();
    
    PVM.pop();
    drawPoste();
    
 // Intercambiamos los buffers
    glutSwapBuffers();
    
}

void drawTriangle() {
    
    objShaders->use();
    objShaders->setMat4("uPVM",PVM.get());
    glEnable(GL_POLYGON_OFFSET_FILL);
        triangle1->render(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);
    triangle2->render(GL_LINE);
    
}
void drawCilinder() {
    
    objShaders->use();
    objShaders->setMat4("uPVM",PVM.get());
    glEnable(GL_POLYGON_OFFSET_FILL);
        cilinder->render(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);
    cilinder2->render(GL_LINE);
    
}
void drawAspa() {

    PVM.push();
        PVM.set(glm::scale(PVM.get(), glm::vec3(0.4f, 0.8f, 1.0f)));
        PVM.set(glm::translate(PVM.get(), glm::vec3(0.0f, -1.0f, 0.0f)));
        drawTriangle();
    PVM.pop();
     
}

void drawHelice() {
    PVM.set(glm::rotate(PVM.get(), glm::radians( 120.0f), glm::vec3(0.0f, 0.0f, 1.0f))); drawAspa();
    PVM.set(glm::rotate(PVM.get(), glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f))); drawAspa();
    PVM.set(glm::rotate(PVM.get(), glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f))); drawAspa();
}
void drawPoste() {
    
    
    PVM.set(glm::scale(PVM.get(), glm::vec3(0.2f, 2.0f, 0.2f)));
    PVM.set(glm::translate(PVM.get(), glm::vec3(0.0f, -1.0f, 0.0f)));
    drawCilinder();
    
    /*PVM.set(glm::rotate(PVM.get(), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))); drawAspa();*/
     
}
void funSpecial(int key, int x, int y) {
   
    switch(key) {
        case GLUT_KEY_UP:    desZ -= 0.1f;   break;
        case GLUT_KEY_DOWN:  desZ += 0.1f;   break;
        case GLUT_KEY_LEFT:  gY += 5.0f;   break;
        case GLUT_KEY_RIGHT: gY -= 5.0f;   break;
        default:
           desZ  = 0.0f;   
           rotZ  = 0.0f;
           break;
    } 
    glutPostRedisplay();
        
}
void funTimer(int ignore) {
    
    rotZ=rotZ+2;
    glutPostRedisplay();
    glutTimerFunc(speed,funTimer,0);
    
}