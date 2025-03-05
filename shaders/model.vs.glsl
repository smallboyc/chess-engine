#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

layout(location = 3) in mat4 instanceModel; // Nouvelle entrée pour la matrice de modèle par instance

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main() {
    // Applique la matrice de modèle spécifique à l'instance
    vec4 worldPosition = instanceModel * vec4(aPos, 1.0);
    FragPos = vec3(worldPosition); // Coordonnée du fragment dans l'espace monde
    Normal = mat3(transpose(inverse(instanceModel))) * aNormal; // Transformation des normales avec la matrice d'instance
    TexCoords = aTexCoords;

    // Appliquer les transformations de vue et de projection
    gl_Position = projection * view * worldPosition;
}
