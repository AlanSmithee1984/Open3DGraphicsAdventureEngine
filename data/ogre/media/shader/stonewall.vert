varying vec4 texCoords;


varying vec3 fragPos;

void main(void)
{
    texCoords= gl_MultiTexCoord0;
//    normalTexCoords= gl_MultiTexCoord1;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    fragPos = gl_Position.xyz;

}
