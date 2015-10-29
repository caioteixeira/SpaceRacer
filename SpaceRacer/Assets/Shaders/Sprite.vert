// Sprite.vert
// Vertex shader for sprite rendering

#version 150
#extension GL_ARB_explicit_attrib_location : require

layout (std140) uniform MatrixBlock
{ 
	mat4 uViewProj;
	mat4 uWorldTransform;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec3 outNormal;
out vec2 outTexCoord;

void main()
{
	// Just propagate the texture coordinates
	outTexCoord = inTexCoord;
	
	// Transform into world space
	vec4 newPos = uWorldTransform * vec4(inPosition, 1.0);
	// Now into projection space
	newPos = uViewProj * newPos;
    gl_Position = newPos;

	// Add this line b/c we need normals
	// due to a single vertex format
	outNormal = inNormal;
}
