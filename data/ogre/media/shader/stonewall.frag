#extension GL_EXT_texture_array : enable


uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2DArray offsetMap;
uniform sampler2D glossMap;

varying vec4 texCoords;
varying vec3 fragPos;
varying mat3 TBNMatrix;
varying vec3 vertNormalCamSpace;


// FIXME: harcoded steps
const int numInclinationSteps = 10;
//const float incStepLength = 90.0 / float(numInclinationSteps);

const int numAzimuthalSteps = 10;

const float PI = 3.14159265358979323846264;


vec3 transFormToTangentSpace(vec3 vecInCamSpace)
{

    vec3 vecInTangentSpace = TBNMatrix * vecInCamSpace;

    vecInTangentSpace = normalize(vecInTangentSpace);

    vecInTangentSpace *= vec3(1.0, -1.0, 1.0);
    //    vecInTangentSpace *= vec3(-1.0, 1.0, 1.0);

    return vecInTangentSpace;
}

float getInclinationFraction(vec3 camDirection)
{
    float inclination = acos(camDirection.z);

    // normalize to 0 deg - 90 deg (0 - 1/2 PI)
    float xComponentZeroToOne = inclination / (0.5 * PI);

    return xComponentZeroToOne;
}

float getAzimuthalFraction(vec3 camDirection)
{
    float xDir = camDirection.x;
    float yDir = camDirection.y;

    float azimuth = atan(yDir, xDir);

    if(azimuth < 0.0)
    {
        azimuth = PI + PI + azimuth;
    }

    // normalize to 0 deg - 360 deg (0 - 2 PI)
    float yComponentZeroToOne = azimuth / (2.0 * PI);


    return yComponentZeroToOne;

}

float getZComponentFromViewDir(vec3 camDirection)
{

    float xComponentZeroToOne = getInclinationFraction(camDirection);
    float yComponentZeroToOne = getAzimuthalFraction(camDirection);


    float xComponentExpanded = xComponentZeroToOne * float(numInclinationSteps);
    xComponentExpanded *= float(numAzimuthalSteps);

    float yComponentExpanded = yComponentZeroToOne * float(numAzimuthalSteps);


    float layer = xComponentExpanded + yComponentExpanded;

//    layer = xComponentExpanded;
//    layer = yComponentExpanded;

    return layer;



}

vec4 getDisplacementTextureValue(vec2 originalTexCoords, vec3 viewDirCamSpace)
{
    vec3 camDirection = normalize(vertNormalCamSpace);
//    vec3 camDirection = viewDirCamSpace;

    float layer = getZComponentFromViewDir(camDirection);

    float tmpVal = layer / float(numInclinationSteps * numAzimuthalSteps);

//    tmpVal = 0.1;

//    tmpVal = layer;

//    return vec4(tmpVal, 0.0, 0.0, 1.0);

//    float xComponentZeroToOne = getInclinationFraction(camDirection);
//    float yComponentZeroToOne = getAzimuthalFraction(camDirection);

//    return vec4(xComponentZeroToOne, yComponentZeroToOne, 0.0, 1.0);
//    return vec4(xComponentZeroToOne, 0.0, 0.0, 1.0);
//    return vec4(0.0, yComponentZeroToOne, 0.0, 1.0);



    int layerDirect = int(layer);
    vec3 directCoordinate = vec3(originalTexCoords, layerDirect);
    vec4 valueDirect = texture2DArray(offsetMap, directCoordinate);
    float weightDirect = 1.0 / abs(layer - float(layerDirect));
//    return valueDirect;


    int layerAziAfter = layerDirect + 1;
    vec4 valueAziAfter = texture2DArray(offsetMap, vec3(originalTexCoords, layerAziAfter));
    float weightAziAfter = 1.0 / abs(layer - float(layerAziAfter));


    int layerIncAfter = layerDirect + numAzimuthalSteps;
    vec4 valueIncAfter = texture2DArray(offsetMap, vec3(originalTexCoords, layerIncAfter));
    float weightIncAfter = 1.0 / abs(layer - float(layerIncAfter));


    int layerIncAziAfter = layerIncAfter + 1;
    vec4 valueIncAziAfter = texture2DArray(offsetMap, vec3(originalTexCoords, layerIncAziAfter));
    float weightIncAziAfter = 1.0 / abs(layer - float(layerIncAziAfter));



//    float weightSum = weightDirect + weightAziAfter + weightIncAfter + weightIncAziAfter;


//    weightDirect /= weightSum;
//    weightAziAfter /= weightSum;
//    weightIncAfter /= weightSum;
//    weightIncAziAfter /= weightSum;

//    vec4 value = valueDirect * weightDirect
//            + valueAziAfter * weightAziAfter
//            + valueIncAfter* weightIncAfter
//            + valueIncAziAfter * weightIncAziAfter;


    vec2 texDimension = vec2(1, numAzimuthalSteps);

    vec2 f = fract( directCoordinate.xy * texDimension );
    vec4 tA = mix( valueIncAfter, valueIncAziAfter, f.x );
    vec4 tB = mix( valueDirect, valueAziAfter, f.x );
    vec4 value = mix( tA, tB, f.y );

    return value;
}


vec2 paralaxMap(vec2 originalTexCoords, vec3 viewDirCamSpace)
{

//    return vec2(0.5);

    vec4 value = getDisplacementTextureValue(originalTexCoords, viewDirCamSpace);

//    return value.rg;

//    float displacement = texCoordOffset.b * 255.0
//            + texCoordOffset.g * 255.0 * 256.0
//            + texCoordOffset.r * 255.0 * 256.0 * 256.0

    float displacement = value.r;

//    displacement *= 0.05;

    displacement *= 10.0;
//    return vec2(displacement, 0.0);


    vec3 viewDirTangentSpace = transFormToTangentSpace(viewDirCamSpace);
    viewDirTangentSpace = normalize(viewDirCamSpace);

    vec2 texCoordOffset = viewDirTangentSpace.xy * displacement;

//    texCoordOffset.y = 0.0;

//    return texCoordOffset;

    return originalTexCoords + texCoordOffset;

}


void main(void)
{
    vec3 viewDirCamSpace = normalize(fragPos);

    vec2 resultingTexCoords = paralaxMap(texCoords.st, viewDirCamSpace);

    vec3 normal = texture2D(normalMap, resultingTexCoords).rgb;

    // blow up
    normal = (normal - 0.5) * 2.0;
    //    normal -= 0.5;

    normal = gl_NormalMatrix * normal;

    normal = normalize(normal);

    //    normal = -normal;


    //    normal = vec3(0.0, 0.0, 1.0);



    vec4 lightPos = gl_LightSource[0].position;

    // assume directional light
    vec3 lightDir = normalize(lightPos.xyz);
    lightDir = -lightDir;

    //    lightDir = vec3(0.0, 0.0, 1.0);



    float lambertian = max( dot( lightDir, normal ), 0.0);

    vec4 diffuseLightColor = gl_LightSource[0].diffuse;
    vec3 diffuseLight = diffuseLightColor.rgb * lambertian;

    vec3 specularLight;

    if(lambertian > 0.0)
    {
        vec3 reflectDir = reflect(-lightDir, normal).xyz;


        float specAngle = max( dot( reflectDir, viewDirCamSpace ), 0.0 );
        float specular = pow(specAngle, 64.0);

        specular *= lambertian;

        vec4 specLightColor = gl_LightSource[0].specular;
        specularLight = specLightColor.rgb * specular;

    }
    else
    {
        specularLight *= 0.0;
    }

//    vec4 glossyVal = texture2D(glossMap, resultingTexCoords);
//    specularLight *= glossyVal.r;




    vec4 textureColor = texture2D(colorMap, resultingTexCoords);

    //    textureColor = vec4(1.0, 0.0, 0.0, 1.0);


    vec4 ambientColor = gl_LightModel.ambient;

    vec3 finalColorRGB = textureColor.rgb * diffuseLight + specularLight + ambientColor.rgb;

    //    finalColorRGB = normal;
    //    finalColorRGB = lightDir;

    //    finalColorRGB = diffuseLight;
    //    finalColorRGB = specularLight;

    //    finalColorRGB = vec4(lambertian);

    //    finalColorRGB = textureColor.rgb;

    //    finalColorRGB =  texture2D(heightMap, texCoords.st).rgb;
    //    finalColorRGB = rotation[1];

//    finalColorRGB = viewDirCamSpace;

    //    finalColorRGB = viewDirTangentSpace;

//    finalColorRGB = vec3(resultingTexCoords.xy, 0.0);

//    if(resultingTexCoords.g < 0.0)
//    {
//        finalColorRGB.b = -resultingTexCoords.g;
//    }



//    finalColorRGB = vertNormalCamSpace;

    //    finalColorRGB = textureColor;

    gl_FragColor = vec4(finalColorRGB, textureColor.a);
}
