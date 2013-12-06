#version 150

out vec4  fColor;
uniform vec4 inColor;

void
main()
{
    fColor = inColor;// vec4( 0.0, 0.0, 1.0, 1.0 );
	//gl_FragColor = inColor;//vec4(0.0, 1.0, 0.0, 1.0);
}
