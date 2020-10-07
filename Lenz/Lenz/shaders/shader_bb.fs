#version 460 core
out vec4 FragColor;

in vec3 norm;

void main() {

    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0) - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    FragColor = diff * vec4(1.0, 1.0, 1.0, 0.5);
}