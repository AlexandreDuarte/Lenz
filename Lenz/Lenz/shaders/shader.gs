#version 460 core

layout (triangles) in;


layout (triangle_strip, max_vertices = 3) out;


in VS_OUT {
	float normal;
} vs_in[];

out float norm;

void main() {

	
	gl_Position = gl_in[0].gl_Position;
	norm = vs_in[0].normal;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	norm = vs_in[1].normal;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	norm = vs_in[2].normal;
	EmitVertex();
    EndPrimitive();
}