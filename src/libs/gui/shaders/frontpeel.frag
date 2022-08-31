#version 330 core

uniform sampler2DRect  depthTexture;		//depth texture (for frontpeel)

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

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
	//vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
	

	//read the depth value from the depth texture
	float frontDepth = texture(depthTexture, gl_FragCoord.xy).r;
	//printf("%d",frontDepth);
	//compare the current fragment depth with the depth in the depth texture
	//if it is less, discard the current fragment
	if(gl_FragCoord.z <= frontDepth)
		discard;
	if (use_textures == false){
		//if(use_transparency==true)
		FragColor = vec4(computeBasicShading() * objectColor, alpha);

		//FragColor = vec4( objectColor, alpha);
		//FragColor = vec4(1,0,0,0.5);
		//else
		//FragColor = vec4(computeBasicShading() * objectColor, 1);
		
	}else{
		FragColor = vec4(computeBasicShading(), alpha) * texture(texture_diffuse1, TexCoords);
		FragColor = vec4(0,0,0,0.2);
	}
	

}















