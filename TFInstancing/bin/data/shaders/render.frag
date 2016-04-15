#version 330

in vec4 colorVarying;

in vec3 v_normalVarying;
in vec4 v_positionVarying;
in vec3 v_lightPos;

out vec4 fragColor;

void main (void) {
    float lambert = max(dot(normalize(v_lightPos - v_positionVarying.xyz), v_normalVarying), 0.0);
    fragColor = vec4(colorVarying.rgb * lambert, 1.0);
}