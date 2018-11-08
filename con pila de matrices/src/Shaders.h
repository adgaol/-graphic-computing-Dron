#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream> 

class Shaders {
    
    public:
        
        Shaders(const GLchar* vShaderPath, const GLchar* fShaderPath);

        void setMat4(const std::string &name, glm::mat4 value);
        void use();
         
        virtual ~Shaders();
               
    private:
                   
        GLuint program;
                
        GLuint createShader (GLenum shader , const GLchar* shaderFile);
        GLuint createProgram(GLuint vShader, GLuint fShader);
        
        const char* readShader(const GLchar* filename);

};

#endif /* SHADERS_H */
