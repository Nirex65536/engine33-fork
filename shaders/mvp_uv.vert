#version 460 core

// Basic Model-View-Projection vertex shader

in vec3 e33_vertexposition;
in vec2 e33_vertexuv;

out vec2 uv;

uniform mat4 e33_mvp;

void main() {
    gl_Position = e33_mvp * vec4(e33_vertexposition, 1.0f);

    uv = e33_vertexuv;
}
