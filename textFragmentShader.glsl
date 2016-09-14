#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D textTextureSampler;

void main(){
    
    //vec4 colorTmp = texture(textTextureSampler, UV);
	color = texture(textTextureSampler, UV);
	/*
	if (colorTmp.r == 1.0 && colorTmp.g == 0.0 && colorTmp.b == 0.0) {
	    color = vec4(0.0, 0.0, 0.0, 0.0);
    } else {
        colorTmp.a = 1.0;
        color = colorTmp;
    }
    */
	
}
