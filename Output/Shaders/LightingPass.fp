#version 140
precision highp float;

in vec2 Texcoord2;

out vec4 oColor;

uniform sampler2DRect uColor;
uniform sampler2DRect uNormal;
uniform sampler2DRect uPosition;

void main()
{
	oColor = vec4( 0.4, 0.3, 0.2, 0.1 );

	// 16g. Read value from uColor texture using 2nd texture coordinates and save it to output
	// ...

	// 24. Read value from normal texture
	// ...

	// 24a. Store sampled normal vector in output channel
	// ...

	// 34. Read value from uPosition texture
	// ...

	// 34a. Save position to output
	// ...

	// 35. Calculate vector from current position to light source
	// Take (0, 0, -5) as a light position and normalize the result vector
	// ...

	// 36. Calculate cosinus of an angle between normal and light vectors (see dot function documentation)
	// ...
}
