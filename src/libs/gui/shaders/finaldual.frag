#version 330 core

uniform sampler2DRect depthBlenderTex;	//depth blending output
uniform sampler2DRect frontBlenderTex;	//front blending output
uniform sampler2DRect backBlenderTex;	//back blending output

out vec4 frag_color; //fragment shader output

void main()
{
	//get the front and back blender colors
	vec4 frontColor = texture(frontBlenderTex, gl_FragCoord.xy);
	vec3 backColor = texture(backBlenderTex, gl_FragCoord.xy).rgb; 

	// front + back
	//composite the front and back blending results
	frag_color.rgb = frontColor.rgb + backColor * frontColor.a;
	
	// front blender
	//frag_color.rgb = frontColor + vec3(alphaMultiplier);
	
	// back blender
	//frag_color.rgb = backColor;
}