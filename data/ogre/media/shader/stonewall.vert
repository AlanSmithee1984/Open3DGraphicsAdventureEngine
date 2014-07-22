attribute vec3 tangent;

varying vec4 texCoords;
varying vec3 fragPos;
varying mat3 TBNMatrix;
varying vec3 vertNormalCamSpace;

void main(void)
{
    texCoords= gl_MultiTexCoord0;
    //    normalTexCoords= gl_MultiTexCoord1;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    fragPos = gl_Position.xyz;



    vec3 vertexNormal_cameraspace = gl_NormalMatrix * normalize(gl_Normal);
    vec3 vertexTangent_cameraspace = gl_NormalMatrix * normalize(tangent);
    vec3 vertexBitangent_cameraspace = cross(vertNormalCamSpace, vertexTangent_cameraspace);


    // Form a rotation matrix out of the vectors
    TBNMatrix = mat3(vertexTangent_cameraspace,
                     vertexBitangent_cameraspace,
                     vertexNormal_cameraspace);



    vertNormalCamSpace = normalize(vertexNormal_cameraspace);
}
