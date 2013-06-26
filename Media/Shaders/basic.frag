#version 130

varying vec3 vertex_view;
varying vec3 vertex_normal;
varying vec4 ambient;
varying vec4 diffuse;
varying vec4 specular;

void main()
{
  vec3 light = normalize(gl_LightSource[0].position.xyz - vertex_view);

  gl_FragColor =
  	       gl_FrontLightModelProduct.sceneColor
	     + ambient
	     + clamp(diffuse * max(dot(vertex_normal, light), 0.0), 0.0, 1.0)
	     + clamp(specular * pow(max(dot(normalize(-reflect(light, vertex_normal)), normalize(-vertex_view)), 0.0),0.3 * gl_FrontMaterial.shininess), 0.0, 1.0);
}
