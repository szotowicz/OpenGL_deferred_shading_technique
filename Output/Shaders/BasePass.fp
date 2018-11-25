#version 140
precision highp float;

// 7a. Create input channel through which texture coordinates will be received from vertex shader
// ...
in vec2 TexCoord;
// 11a. Create input channel through which normal vector will be received from vertex shader
// ...
in vec3 Normal;
// 26. Create input channel for vertex position passed from vertex shader
// ...
in vec3 Position;

out vec4 oColor;

// 17. Add another output data for normal vector
// ...
out vec3 oNormal;
// 27. Add another output data for position
// ...
out vec3 oPosition;
uniform sampler2D uTexture;

void main()
{
	oColor = vec4( 0.5 );

	// 8. Sample uTexture with received texture coordinates
	// Save color read from texture to fragment shader output
	// ...
	oColor = texture(uTexture, TexCoord);
	// 13. Save normalized normal vector to color output
	// ...
	
	//oColor = normalize(vec4(Normal, 0.0));
	// 18. Save normalized normal vector to newly created output channel
	// ...
	oNormal = normalize(Normal);
	// 26b. Temporarily save position in color or normal output
	// ...
	
	//oNormal = Position;

	// 28. Store position in newly created output channel
	// ...
	oPosition = Position;
}
