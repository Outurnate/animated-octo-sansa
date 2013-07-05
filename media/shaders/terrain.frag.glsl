#version 130

float snoise(vec2 v);

uniform sampler2D lower_A_diffuse;
uniform sampler2D lower_B_diffuse;
uniform sampler2D middle_A_diffuse;
uniform sampler2D middle_B_diffuse;
uniform sampler2D upper_A_diffuse;
uniform sampler2D upper_B_diffuse;

varying vec4 pos;
varying vec3 cam;
varying vec3 normal;

#define LOWER_BAND 42
#define UPPER_BAND 85
#define MAX_HEIGHT 128
#define BLOCK_ZONE(LOWER, UPPER, VAL) ((VAL > LOWER) && (VAL <= UPPER))

#define TEX_SCALE 20

void main()
{
  float noise = snoise(pos.xz / TEX_SCALE);
  vec3 terr_type = vec3(
			BLOCK_ZONE(0,                           (LOWER_BAND - (4 + abs(noise * 8))),      pos.y),
			BLOCK_ZONE((LOWER_BAND + (4 + abs(noise * 8))), (UPPER_BAND - (4 + abs(noise * 8))),      pos.y),
			BLOCK_ZONE((UPPER_BAND + (4 + abs(noise * 8))), MAX_HEIGHT,                       pos.y)) + 
  			(mix(vec3(1, 0, 0), vec3(0, 1, 0), (clamp(pos.y, (LOWER_BAND - (4 + abs(noise * 8))), (LOWER_BAND + (4 + abs(noise * 8)))) - (LOWER_BAND - (4 + abs(noise * 8)))) / ((LOWER_BAND + (4 + abs(noise * 8))) - (LOWER_BAND - (4 + abs(noise * 8)))))
  			* float(BLOCK_ZONE((LOWER_BAND - (4 + abs(noise * 8))), (LOWER_BAND + (4 + abs(noise * 8))), pos.y))) + 
			(mix(vec3(0, 1, 0), vec3(0, 0, 1), (clamp(pos.y, (UPPER_BAND - (4 + abs(noise * 8))), (UPPER_BAND + (4 + abs(noise * 8)))) - (UPPER_BAND - (4 + abs(noise * 8)))) / ((UPPER_BAND + (4 + abs(noise * 8))) - (UPPER_BAND - (4 + abs(noise * 8)))))
			* float(BLOCK_ZONE((UPPER_BAND - (4 + abs(noise * 8))), (UPPER_BAND + (4 + abs(noise * 8))), pos.y)));
  vec4 diffuse        = vec4(
       		      	mix(texture2D(lower_A_diffuse,  pos.xz / TEX_SCALE).rgb, texture2D(lower_B_diffuse,  pos.xz / TEX_SCALE).rgb, noise) * terr_type.x
         	      + mix(texture2D(middle_A_diffuse, pos.xz / TEX_SCALE).rgb, texture2D(middle_B_diffuse, pos.xz / TEX_SCALE).rgb, noise) * terr_type.y
		      + mix(texture2D(upper_A_diffuse,  pos.xz / TEX_SCALE).rgb, texture2D(upper_B_diffuse,  pos.xz / TEX_SCALE).rgb, noise) * terr_type.z, 1);
  vec4 ambient        = (diffuse * .1);
  vec4 specular       = (diffuse * 0);
  float shininess     = 0;

  vec4 color = ambient * gl_LightSource[0].ambient;
  vec3 n = normalize(normal);
  vec3 ldir = normalize(gl_LightSource[0].position.xyz);
  vec3 halfv = normalize(normalize(cam) + ldir);

  float nl = max(dot(n, ldir), 0.0);

  if (nl > 0.0)
    color += (diffuse * gl_LightSource[0].diffuse * nl)
          +  (specular * gl_LightSource[0].specular * pow(max(dot(n, normalize(halfv)), 0.0), shininess));

  gl_FragColor = color;
}
