varying vec2 texcoord;

void main() {

    gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;
    texcoord=gl_Vertex.xy; // or whatever you need to pass texture coordinates
}