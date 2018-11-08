#version 330 core

in vec3 inPosition;
in vec3 inColor;

uniform mat4 uPVM;

out vec3 vColor;

void main() {

    gl_Position = uPVM * vec4(inPosition,1);

    vColor = inColor;
	
}
