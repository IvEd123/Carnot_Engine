#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 1) out;

void main(){
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}