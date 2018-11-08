#include "Mesh.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Constructor de la clase (lee los atributos del modelo de un fichero de texto y los almacena en los vectores correspondientes)
//-------------------------------------------------------------------------------------------------------------------------------
Mesh::Mesh(const char* modelPath) {

 // Abrimos el fichero con los atributos del modelo
    std::ifstream modelFile(modelPath, std::ios::in);
    if(!modelFile.is_open()) {
        std::cout << "El fichero " << modelPath << " no se puede abrir." << std::endl;
        std::cin.get();
        exit(1);
    }
 
 // Lectura de atributos
    std::string line;
    std::vector<glm::vec3> nCoords;
    std::vector<glm::vec2> tCoords;
    while(getline(modelFile,line)) { 
        if(line[0]=='#' || line[0]=='o') continue;
        std::vector<std::string> tokens = split(line,(std::string)" ");
        if     (tokens[0] == "v" ) aPosition.push_back(glm::vec3(stod(tokens[1]),stod(tokens[2]),stod(tokens[3])));
        else if(tokens[0] == "vc") aColor.push_back   (glm::vec3(stod(tokens[1]),stod(tokens[2]),stod(tokens[3])));
        else if(tokens[0] == "vn") nCoords.push_back  (glm::vec3(stod(tokens[1]),stod(tokens[2]),stod(tokens[3])));
        else if(tokens[0] == "vt") {
            tCoords.push_back(glm::vec2(stod(tokens[1]),stod(tokens[2])));
            aTextureCoord.push_back(glm::vec2(0.0f));
        }
        else if(tokens[0] == "s")  break;
        else {
            std::cout << "Error en el modelo." << std::endl;
            std::cin.get();
            exit(1);
        }
    }
    
 // Lectura de Caras
    int v[3], t[3], n[3];
    std::vector<GLushort> nLists[aPosition.size()];
    while(getline(modelFile,line)) {
        std::vector<std::string> tokens = split(line,(std::string)" ");
        for(int i=0; i<3; i++) {
            std::vector<std::string> subtokens = split(tokens[i+1],(std::string)"/");
            v[i] = stoi(subtokens[0])-1;
            t[i] = stoi(subtokens[1])-1;
            n[i] = stoi(subtokens[2])-1;
            vindex.push_back((GLushort)v[i]);
            nLists[v[i]].push_back(n[i]);
            aTextureCoord[v[i]] = tCoords[t[i]];
        }
    }
    modelFile.close();

 // Calculo de normales
    if(nCoords.size()>0) {
        for(int i=0; i<aPosition.size(); i++) {
            glm::vec3 normal = glm::vec3(0.0f,0.0f,0.0f);
            for(int j=0; j<nLists[i].size(); j++) normal += nCoords[nLists[i][j]];
            aNormal.push_back(glm::normalize(normal));
        }
    }
    else {
        std::vector<glm::vec3> normales[aPosition.size()];
        for(int i=0; i<vindex.size(); i+=3) {
            normales[vindex[i+0]].push_back(glm::normalize(glm::cross(aPosition[vindex[i+1]]-aPosition[vindex[i+0]],aPosition[vindex[i+2]]-aPosition[vindex[i+0]])));
            normales[vindex[i+1]].push_back(glm::normalize(glm::cross(aPosition[vindex[i+2]]-aPosition[vindex[i+1]],aPosition[vindex[i+0]]-aPosition[vindex[i+1]])));
            normales[vindex[i+2]].push_back(glm::normalize(glm::cross(aPosition[vindex[i+0]]-aPosition[vindex[i+2]],aPosition[vindex[i+1]]-aPosition[vindex[i+2]])));
        }
        for(int i=0; i<aPosition.size(); i++) {
            glm::vec3 normal = glm::vec3(0.0f,0.0f,0.0f);
            for(int j=0; j<normales[i].size(); j++) normal += normales[i][j];
            aNormal.push_back(glm::normalize(normal));
        }
    }
    
}


//--------------------------------------------------------
// Tokeniza las líneas del fichero de texto con el modelo
//--------------------------------------------------------
std::vector<std::string> Mesh::split(std::string line, std::string separator) {
    
    std::vector<std::string> list;
    size_t pos = 0;
    std::string token;
    while((pos=line.find(separator.c_str())) != std::string::npos) {
        token = line.substr(0,pos);       
        if(token!="") list.push_back(token);
        line.erase(0,pos+1);
    }
    token = line.substr(0,pos);
    if(token!="") list.push_back(token);
    
    return list;
}


//----------------------------------------------------------------------------
// Establece unos valores para el atributo color (si el modelo no lo contine)
//----------------------------------------------------------------------------
void Mesh::setColor(glm::vec3 color) {
    
    for(int i=0; i<aPosition.size(); i++) aColor.push_back(color);
    
}


//------------------------------------------
// Crea un VAO con los atributos del modelo
//------------------------------------------
void Mesh::createVao() {
    
    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vboPosition);
    glGenBuffers(1,&vboColor);
    glGenBuffers(1,&vboNormal);
    glGenBuffers(1,&vboTextureCoord);
    glGenBuffers(1,&ebo);
    
    glBindVertexArray(vao);
     // Posiciones
        glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*aPosition.size(), &(aPosition.front()), GL_STATIC_DRAW);  
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(0);
     // Colores
        glBindBuffer(GL_ARRAY_BUFFER, vboColor);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*aColor.size(), &(aColor.front()), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(1);
     // Normales   
        glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*aNormal.size(), &(aNormal.front()), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(2);
     // Texturas
        glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoord);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*aTextureCoord.size(), &(aTextureCoord.front()), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(3);
     // Índices (secuencia de vértices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*vindex.size(), &(vindex.front()), GL_STATIC_DRAW);
    glBindVertexArray(0);
    
}

//--------------------------------
// Renderiza el VAO con el modelo
//--------------------------------
void Mesh::render(GLenum mode) {
    
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    
    glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, vindex.size(), GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);
    
}


//------------------------
// Destructor de la clase
//------------------------
Mesh::~Mesh() {
  
    glDeleteBuffers(1,&vao);
    glDeleteBuffers(1,&vboPosition);
    glDeleteBuffers(1,&vboColor);
    glDeleteBuffers(1,&vboNormal);
    glDeleteBuffers(1,&vboTextureCoord);
    glDeleteBuffers(1,&ebo);
    
}
