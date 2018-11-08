#include "Shaders.h"


//---------------------------------------------------------------------------------------
// Constructor de la clase (llama a las funciones privadas createShader y createProgram)
//---------------------------------------------------------------------------------------
Shaders::Shaders(const GLchar* vShaderPath, const GLchar* fShaderPath) { 

    GLuint vShader = createShader(GL_VERTEX_SHADER  , vShaderPath);
    GLuint fShader = createShader(GL_FRAGMENT_SHADER, fShaderPath);
    program = createProgram(vShader, fShader);
    
}


//--------------------------------------
// Crea un shader (vértices/fragmentos)
//--------------------------------------
GLuint Shaders::createShader(GLenum shader, const char* shaderFile) {
   
 // Se crea un objeto shader
    GLuint shaderID = glCreateShader(shader); 
    
 // Se asigna su código fuente
    const char *shaderSrc = readShader(shaderFile);
    glShaderSource(shaderID, 1, &shaderSrc, NULL);
    
 // Se compila con control de errores
    GLint compiled;    
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
    if(compiled == GL_FALSE) {
        GLint logSize;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetShaderInfoLog(shaderID, logSize, NULL, logMsg);
        std::cout << "Error al compilar el Shader " << shaderFile << ": " << logMsg;
        delete[] logMsg;
        return 0;
    }
    
    return shaderID;
}


//-----------------------------------
// Lee el código fuente de un shader
//-----------------------------------
const char* Shaders::readShader(const GLchar* shaderPath) {
   
    std::string   shaderCode;
    std::ifstream shaderFile(shaderPath, std::ios::in);
    if(shaderFile.is_open()) {
        std::string line = "";
        while(getline(shaderFile, line)) shaderCode += "\n" + line;
        shaderFile.close();
    }
    else {
        std::cout << "El fichero " << shaderPath << " no se puede abrir." << std::endl;
        return 0;
    }
    
    return shaderCode.c_str();
}


//-------------------------------------------------------------------
// Crea un programa(identificador) que usa los shaders especificados
//-------------------------------------------------------------------
GLuint Shaders::createProgram(GLuint vShader, GLuint fShader) {
    
 // Se crea un objeto programa
    GLuint program = glCreateProgram();
    
 // Se adjuntan los shaders y se eliminan los objetos correspondientes
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    
 // Se enlaza el programa con control de errores
    GLint linked;    
    glLinkProgram(program);
    glGetShaderiv(program, GL_COMPILE_STATUS, &linked);
    if(linked == GL_FALSE) {
        GLint logSize;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetShaderInfoLog(program, logSize, NULL, logMsg);
        std::cout << "Error al enlazar el Shader: " << logMsg;
        delete[] logMsg;
        return 0;
    }
    
    return program;    
}


//-----------------------------------------------------
// Fija el valor de una variable uniforme de tipo mat4
//-----------------------------------------------------
void Shaders::setMat4(const std::string &name, glm::mat4 value) {
    
   glUniformMatrix4fv(glGetUniformLocation(program,name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); 
    
}


//-----------------------------------------
// Usa el shader para renderizar la escena
//-----------------------------------------
void Shaders::use() {
    
    glUseProgram(program);
    
}


//------------------------
// Destructor de la clase
//------------------------
Shaders::~Shaders() { 
    
    glDeleteProgram(program);

}
