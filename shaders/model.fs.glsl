#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightPos1;
uniform vec3 lightColor1;
uniform vec3 lightPos2; // Deuxième lumière (mobile)
uniform vec3 lightColor2;

uniform vec3 viewPos;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Ns;

uniform bool useTexture;
uniform sampler2D map_Kd;
uniform vec3 colorFactor;

in vec3 PieceColor;  // Kd

void main() {
    vec3 diffuseColor = useTexture ? texture(map_Kd, TexCoords).rgb : PieceColor;
    diffuseColor *= colorFactor; 
    // Ambient
    vec3 ambient = Ka * ((lightColor1 + lightColor2) * 0.05);

    vec3 V = normalize(viewPos - FragPos);
    vec3 N = normalize(Normal);

    // Diffuse et Specular pour la lumière 1
    vec3 L1 = normalize(lightPos1 - FragPos);
    float diff1 = max(dot(L1, N), 0.0);
    vec3 diffuse1 = diff1 * diffuseColor * lightColor1;

    vec3 H1 = normalize(L1 + V);
    float spec1 = pow(max(dot(N, H1), 0.0), Ns);
    vec3 specular1 = spec1 * Ks * lightColor1;

    // Diffuse et Specular pour la lumière 2 (mobile)
    vec3 L2 = normalize(lightPos2 - FragPos);
    float diff2 = max(dot(L2, N), 0.0);
    vec3 diffuse2 = diff2 * diffuseColor * lightColor2;

    vec3 H2 = normalize(L2 + V);
    float spec2 = pow(max(dot(N, H2), 0.0), Ns);
    vec3 specular2 = spec2 * Ks * lightColor2;

    // Somme des contributions
    vec3 result = ambient + diffuse1 + specular1 + diffuse2 + specular2;
    FragColor = vec4(result, 1.0);
}
