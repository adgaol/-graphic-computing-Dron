#include "MatrixStack.h"

MatrixStack::MatrixStack() {
    
    index = 0;

}

MatrixStack::~MatrixStack() {
}


glm::mat4 MatrixStack::get() {
    
    return matrices[index];

}

void MatrixStack::set(const glm::mat4 &matrix) {
    
    matrices[index] = matrix;

}

void MatrixStack::push() {
    
    index++;    
    matrices[index] = matrices[index-1];
    
}

void MatrixStack::pop() {
    
    index--;

}
