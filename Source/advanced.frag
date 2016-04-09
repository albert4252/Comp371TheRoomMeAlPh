#version 330 core
in vec3 textureDir; // Direction vector representing a 3D texture coordinate
uniform samplerCube cubemap;  // Cubemap texture sampler
out vec4 color;

void main()
{             
    color = texture(cubemap, textureDir);
} 
