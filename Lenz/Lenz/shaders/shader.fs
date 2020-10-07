#version 460 core

out vec4 FragColor;

in float norm;

void main() {

    FragColor = vec4((norm + 1)/2, (norm + 1)/2, (norm + 1) /2, 0.9);
}