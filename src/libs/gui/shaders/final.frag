#version 330 core


uniform sampler2DRect colorTexture;	//colour texture from previous pass (for final)

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{

	//get the colour from the colour buffer
	vec4 color = texture(colorTexture, gl_FragCoord.xy);
	//combine the colour read from the colour texture with the background colour
	//by multiplying the colour alpha with the background colour and adding the 
	//product to the given colour uniform
	frag_color = color + (0.8f, 0.8f, 0.8f, 1.00f)*color.a;


}