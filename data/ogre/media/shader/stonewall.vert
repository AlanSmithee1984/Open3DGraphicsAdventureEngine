attribute vec3 tangent;

varying vec4 texCoords;


varying vec3 fragPos;
varying mat3 rotation;

void main(void)
{
    texCoords= gl_MultiTexCoord0;
//    normalTexCoords= gl_MultiTexCoord1;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    fragPos = gl_Position.xyz;



    // Calculate the binormal (NB we assume both normal and tangent are
    // already normalised)
    vec3 binormal = -cross(gl_Normal, tangent);


    // Form a rotation matrix out of the vectors
    rotation = mat3(tangent, binormal, gl_Normal);

}
