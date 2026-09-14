#version 330

// Input vertex attributes (from raylib's default vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Custom uniform, set from Lua every frame via SetShaderValueFloat
uniform float time;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);

    // Cycle a tint color through red/green/blue over time, 120 degrees
    // apart, then multiply it into the texture like a colored light.
    vec3 tint = vec3(
        0.5 + 0.5 * sin(time),
        0.5 + 0.5 * sin(time + 2.094),
        0.5 + 0.5 * sin(time + 4.188)
    );

    finalColor = texelColor * vec4(tint, 1.0) * colDiffuse * fragColor;
}
