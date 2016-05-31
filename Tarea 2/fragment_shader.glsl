#version 330 core

in vec4 Color;

out vec4 color;

void main()
{
	//float gris =(Color.r*0.21 + Color.g*0.72 + Color.b*0.07);
	//color = vec4(gris, gris, gris, 1.0);
	color = vec4(1.0, 0.0, 0.0, 1.0);
	//color = Color;
}