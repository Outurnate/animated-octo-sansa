#version 130

uniform sampler2D lower_diffuse;
uniform sampler2D middle_diffuse;
uniform sampler2D upper_diffuse;

varying vec3 vertex_view;
varying vec3 vertex_normal;
varying vec3 pos;

#define LOWER_BAND 42
#define UPPER_BAND 85
#define BAND_BREADTH 4
#define MAX_HEIGHT 128
#define BLOCK_ZONE(LOWER, UPPER, VAL) ((VAL > LOWER) && (VAL <= UPPER))

void main()
{
  vec3 terr_type = vec3(
			BLOCK_ZONE(0,                           (LOWER_BAND - BAND_BREADTH),      pos.y),
			BLOCK_ZONE((LOWER_BAND + BAND_BREADTH), (UPPER_BAND - BAND_BREADTH),      pos.y),
			BLOCK_ZONE((UPPER_BAND + BAND_BREADTH), MAX_HEIGHT,                       pos.y)) + 
  			(mix(vec3(1, 0, 0), vec3(0, 1, 0), (clamp(pos.y, (LOWER_BAND - BAND_BREADTH), (LOWER_BAND + BAND_BREADTH)) - (LOWER_BAND - BAND_BREADTH)) / ((LOWER_BAND + BAND_BREADTH) - (LOWER_BAND - BAND_BREADTH)))
  			* float(BLOCK_ZONE((LOWER_BAND - BAND_BREADTH), (LOWER_BAND + BAND_BREADTH), pos.y))) + 
			(mix(vec3(0, 1, 0), vec3(0, 0, 1), (clamp(pos.y, (UPPER_BAND - BAND_BREADTH), (UPPER_BAND + BAND_BREADTH)) - (UPPER_BAND - BAND_BREADTH)) / ((UPPER_BAND + BAND_BREADTH) - (UPPER_BAND - BAND_BREADTH)))
			* float(BLOCK_ZONE((UPPER_BAND - BAND_BREADTH), (UPPER_BAND + BAND_BREADTH), pos.y)));
  vec4 diffuse        = vec4(
       		      	texture2D(lower_diffuse,  pos.xz / 10).rgb * terr_type.x
         	      + texture2D(middle_diffuse, pos.xz / 10).rgb * terr_type.y
		      + texture2D(upper_diffuse,  pos.xz / 10).rgb * terr_type.z, 1);
  vec4 ambient        = diffuse * .1;
  vec4 specular       = diffuse * .25;

  vec3 light = normalize(gl_LightSource[0].position.xyz - vertex_view);

  gl_FragColor =
  	       gl_FrontLightModelProduct.sceneColor
	     + ambient
	     + clamp(diffuse * max(dot(vertex_normal, light), 0.0), 0.0, 1.0)
	     + clamp(specular * pow(max(dot(normalize(-reflect(light, vertex_normal)), normalize(-vertex_view)), 0.0), 0.3 * gl_FrontMaterial.shininess), 0.0, 1.0);
}
