#version 130

varying vec3 vertex_view;
varying vec3 vertex_normal;

void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vertex_normal  = normalize(gl_NormalMatrix * gl_Normal);
	vertex_view    = vec3(gl_ModelViewMatrix * gl_Vertex);
}
