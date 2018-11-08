#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>

class Mesh {
    
    public:

        Mesh(const char* modelPath);

        void setColor(glm::vec3 color);
        void createVao();
        void render(GLenum mode);
         
        virtual ~Mesh();
         
    private:
        
        std::vector<glm::vec3> aPosition;
        std::vector<glm::vec3> aColor;
        std::vector<glm::vec3> aNormal;
        std::vector<glm::vec2> aTextureCoord;
        std::vector<GLushort>  vindex;
    
        GLuint vao;
        GLuint vboPosition;
        GLuint vboColor;
        GLuint vboNormal;
        GLuint vboTextureCoord;
        GLuint ebo;
        
        std::vector<std::string> split(std::string line, std::string separator);

};

#endif /* MESH_H */
