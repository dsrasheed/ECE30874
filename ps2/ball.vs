#version 330 core
layout (location = 0) in vec2 pos;

uniform mat4 scale;
uniform mat4 translate;

void main() {
    gl_Position = translate * scale * vec4(pos, 0.0, 1.0);
}