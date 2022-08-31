#version 330 core


uniform sampler2DRect tempTexture; //intermediate blending result (for blend)


// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	
	//return the intermediate blending result
	frag_color = texture(tempTexture, gl_FragCoord.xy); 
	//if the alpha is 0, we discard that fragment
	if(frag_color.a == 0) 
		discard;



}