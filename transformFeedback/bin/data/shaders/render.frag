#version 330

uniform sampler2D spriteTex;

in vec4 colorVarying;

out vec4 fragColor;

void main (void) {
    vec4 particleSpriteTexCol = texture( spriteTex, gl_PointCoord );
    fragColor = particleSpriteTexCol * colorVarying;
}