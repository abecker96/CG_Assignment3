#version 400
//VERTEX SHADER

// layout location needs to match attribute in glVertexAttribPointer()
// "in" notes that this is input data
// takes a vec3, vPosition_Modelspace is just a name that makes sense
layout(location = 0) in vec3 vPosition_Modelspace;
layout(location = 1) in vec3 vertexColor;
uniform mat4 MVP;

out vec3 fragmentColor;
out vec3 vPosition_out;

void main() {
    //link vertexPos with gl_Position
    gl_Position = vec4(vPosition_Modelspace, 1.0);

    //forward color data on to fragment shader
    fragmentColor = vertexColor;
    vPosition_out = vPosition_Modelspace;
}