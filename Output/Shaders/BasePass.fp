#version 140
precision highp float;

// 7a. Create input channel through which texture coordinates will be received from vertex shader
// ...

// 11a. Create input channel through which normal vector will be received from vertex shader
// ...

// 26. Create input channel for vertex position passed from vertex shader
// ...

out vec4 oColor;

// 17. Add another output data for normal vector
// ...

// 27. Add another output data for position
// ...

uniform sampler2D uTexture;

void main()
{
	oColor = vec4( 0.5 );

	// 8. Sample uTexture with received texture coordinates
	// Save color read from texture to fragment shader output
	// ...

	// 13. Save normalized normal vector to color output
	// ...

	// 18. Save normalized normal vector to newly created output channel
	// ...

	// 26b. Temporarily save position in color or normal output
	// ...

	// 28. Store position in newly created output channel
	// ...
}
