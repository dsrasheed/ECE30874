#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tx;

uniform mat4 transform;
uniform vec3 translate;
uniform mat4 rotateScene;
uniform mat4 translateScene;
uniform mat4 projection;

out vec3 aPos;
out vec3 aNorm;
out vec2 aTxPos;

void main() {
    mat4 model = mat4(
        transform[0],
        transform[1],
        transform[2],
        vec4(translate, 1.0)
    );
    vec4 pw = model * vec4(pos, 1.0);
    aPos = pw.xyz;
    aNorm = normalize(vec4(norm, 0.0)).xyz;
    aTxPos = tx;
    gl_Position = projection * rotateScene * translateScene * pw;
}
