#vertex
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPos; // Vertex position input

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 parentTransform;

out vec3 FragPos; // Output position to the fragment shader

void main() {
    FragPos = (model * vec4(aPos, 1.0)).xyz; // Pass transformed position to fragment shader
    gl_Position = projection * view * parentTransform * model * vec4(aPos, 1.0);
}

#fragment
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

in vec3 FragPos; // Input position from the vertex shader

out vec4 FragColor; // Output color

void main() {
    // Simple color output for demonstration
    FragColor = vec4(FragPos, 1.0);
}