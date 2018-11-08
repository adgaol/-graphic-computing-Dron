#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <glm.hpp>

class MatrixStack {
    
    public:
        
        MatrixStack();
        virtual ~MatrixStack();
        
        glm::mat4 get ();
        void      set (const glm::mat4 &matrix);
        void      push();
        void      pop ();

    private:
        
        glm::mat4 matrices[50];
        int       index;

};

#endif /* MATRIXSTACK_H */

