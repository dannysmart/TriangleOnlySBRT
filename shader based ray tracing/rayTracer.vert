#version 330 core

uniform mat4 viewMatrix, projMatrix;
in vec4 position;
out vec3 worldPos;

void main(void)
{
	//worldPos = vec3(transform * vec4(pos,1));
	//gl_Position = eye * vec4(worldPos, 1);
	worldPos = vec3(projMatrix * viewMatrix * position);
	gl_Position = projMatrix * viewMatrix * position ;

}