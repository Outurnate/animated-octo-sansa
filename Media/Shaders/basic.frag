#version 130

varying vec3 vertex_view;
varying vec3 vertex_normal;

void main()
{
	vec3 light = normalize(gl_LightSource[0].position.xyz - vertex_view);

	gl_FragColor =
		gl_FrontLightModelProduct.sceneColor
	  + gl_FrontLightProduct[0].ambient
	  + clamp(gl_FrontLightProduct[0].diffuse * max(dot(vertex_normal, light), 0.0), 0.0, 1.0)
	  + clamp(gl_FrontLightProduct[0].specular * pow(max(dot(normalize(-reflect(light, vertex_normal)), normalize(-vertex_view)), 0.0),0.3 * gl_FrontMaterial.shininess), 0.0, 1.0);
}
