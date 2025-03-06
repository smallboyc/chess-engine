#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

layout(location = 3) in mat4 instanceModel; 

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 worldPosition = instanceModel * vec4(aPos, 1.0);
    FragPos = vec3(worldPosition);
    Normal = mat3(transpose(inverse(instanceModel))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * worldPosition;
}
