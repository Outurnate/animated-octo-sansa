#version 130

varying vec3 vertex_view;
varying vec3 vertex_normal;
varying float height;

void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	height	       = gl_Vertex.y;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vertex_normal  = normalize(gl_NormalMatrix * gl_Normal);
	vertex_view    = vec3(gl_ModelViewMatrix * gl_Vertex);
}
