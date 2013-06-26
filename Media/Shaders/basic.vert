#version 130

varying vec3 vertex_view;
varying vec3 vertex_normal;
varying vec4 ambient;
varying vec4 diffuse;
varying vec4 specular;

void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 terr_type;
	if (gl_Vertex.y < 24)
	   terr_type = vec3(1, 0, 0);
	else if ((gl_Vertex.y > 24) && (gl_Vertex.y < 40))
	   terr_type = mix(vec3(1, 0, 0), vec3(0, 1, 0), ((gl_Vertex.y - 24) / 128));
	else if ((gl_Vertex.y > 40) && (gl_Vertex.y < 88))
	   terr_type = vec3(0, 1, 0);
	else if ((gl_Vertex.y > 88) && (gl_Vertex.y < 104))
	   terr_type = mix(vec3(0, 1, 0), vec3(0, 0, 1), ((gl_Vertex.y - 88) / 128));
	else if (gl_Vertex.y < 104)
	   terr_type = vec3(0, 0, 1);
	ambient        = vec4(terr_type.x * .1, terr_type.y * .1, terr_type.z * .1, 1);
	diffuse        = vec4(terr_type.x, terr_type.y, terr_type.z, 1);
	specular       = vec4(0, 0, 0, 1);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vertex_normal  = normalize(gl_NormalMatrix * gl_Normal);
	vertex_view    = vec3(gl_ModelViewMatrix * gl_Vertex);
}
