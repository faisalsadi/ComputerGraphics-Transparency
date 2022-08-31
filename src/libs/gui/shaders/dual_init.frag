#version 330 core

out vec4 FragColor; //fragment shader output
uniform vec4 objectColor;		//solid colour of the cube
uniform float alpha;
  
void main()
{
	//set the fragment colour as -fragment depth and fragment depth
	//in the red and green channel. This when combined with min/max
	//blending will help in peeling front and back layers simultaneously
	FragColor.xy = vec2(-gl_FragCoord.z, gl_FragCoord.z);
}