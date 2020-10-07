#version 460 core

layout (std140) uniform MatrixBlock
{
  dmat4 projection;
  dmat4 view;
  double zoom;
};


layout (location = 0) in dvec3 vertex_position;


layout (location = 1) in double vertex_normal;

out VS_OUT {
	float normal;
} vs_out;



void main() {
	
  vs_out.normal = float(vertex_normal);

	gl_Position = vec4(projection * view * dvec4(vertex_position.x, vertex_position.y, vertex_position.z, 1));
}