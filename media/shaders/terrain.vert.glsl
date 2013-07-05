#version 130

uniform vec4 camera_pos;

varying vec4 pos;
varying vec3 cam;
varying vec3 normal;

void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	pos	       = gl_Vertex;
	normal 	       = normalize(gl_NormalMatrix * gl_Normal);
	cam	       = camera_pos.xyz - gl_Vertex.xyz;
}
