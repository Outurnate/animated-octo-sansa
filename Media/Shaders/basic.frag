#version 130

varying vec3 vertex_view;
varying vec3 vertex_normal;
varying float height;

#define LOWER_BAND 42
#define UPPER_BAND 85
#define BAND_BREADTH 8
#define MAX_HEIGHT 128
#define BLOCK_ZONE(LOWER, UPPER, VAL) ((VAL > LOWER) && (VAL <= UPPER))

void main()
{
  vec3 terr_type = vec3(
  BLOCK_ZONE(0,                           (LOWER_BAND - BAND_BREADTH),      height),
  BLOCK_ZONE((LOWER_BAND + BAND_BREADTH), (UPPER_BAND - BAND_BREADTH),      height),
  BLOCK_ZONE((UPPER_BAND + BAND_BREADTH), MAX_HEIGHT,                       height)) + 
  (mix(vec3(1, 0, 0), vec3(0, 1, 0), (clamp(height, (LOWER_BAND - BAND_BREADTH), (LOWER_BAND + BAND_BREADTH)) - (LOWER_BAND - BAND_BREADTH)) / ((LOWER_BAND + BAND_BREADTH) - (LOWER_BAND - BAND_BREADTH)))
  * float(BLOCK_ZONE((LOWER_BAND - BAND_BREADTH), (LOWER_BAND + BAND_BREADTH), height))) + 
  (mix(vec3(0, 1, 0), vec3(0, 0, 1), (clamp(height, (UPPER_BAND - BAND_BREADTH), (UPPER_BAND + BAND_BREADTH)) - (UPPER_BAND - BAND_BREADTH)) / ((UPPER_BAND + BAND_BREADTH) - (UPPER_BAND - BAND_BREADTH)))
  * float(BLOCK_ZONE((UPPER_BAND - BAND_BREADTH), (UPPER_BAND + BAND_BREADTH), height)));
  vec4 ambient        = vec4(terr_type.x * .1, terr_type.y * .1, terr_type.z * .1, 1);
  vec4 diffuse        = vec4(terr_type.x, terr_type.y, terr_type.z, 1);
  vec4 specular       = vec4(0, 0, 0, 1);

  vec3 light = normalize(gl_LightSource[0].position.xyz - vertex_view);

  gl_FragColor =
  	       gl_FrontLightModelProduct.sceneColor
	     + ambient
	     + clamp(diffuse * max(dot(vertex_normal, light), 0.0), 0.0, 1.0)
	     + clamp(specular * pow(max(dot(normalize(-reflect(light, vertex_normal)), normalize(-vertex_view)), 0.0),0.3 * gl_FrontMaterial.shininess), 0.0, 1.0);
}
