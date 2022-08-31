#version 330 core

out vec4 FragColor;
uniform vec3 objectColor;
uniform float alpha;
uniform float transparency;
uniform sampler2D texture_diffuse1;
in vec2 TexCoords;
//in vec3 pixel
uniform bool use_transparency;
uniform bool use_textures;
uniform bool drawtransparency;


#include "compute_shading.frag"

void main()
{

	
	if (use_textures == false){
		//if(use_transparency==true)
		FragColor = vec4(computeBasicShading() * objectColor, alpha);

		//FragColor = vec4( objectColor, alpha);
		//FragColor = vec4(1,0,0,0.5);
		//else
		//FragColor = vec4(computeBasicShading() * objectColor, 1);
		
	}else{
		//FragColor = vec4(computeBasicShading(), alpha) * texture(texture_diffuse1, TexCoords);
		FragColor = vec4(0,0,0,0.2);
	}
	
} 
