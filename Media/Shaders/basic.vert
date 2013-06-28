#version 130

varying vec3 vertex_view;
varying vec3 vertex_normal;
varying vec3 pos;

void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	pos	       = gl_Vertex.xyz;
	vertex_normal  = normalize(gl_NormalMatrix * gl_Normal);
	vertex_view    = vec3(gl_ModelViewMatrix * gl_Vertex);
}
