uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;

varying vec4 texCoords;


varying vec3 fragPos;


varying mat3 TBNMatrix;


vec2 paralaxMap(vec2 originalTexCoords, vec3 viewDirTangetSpace)
{

    float depth = texture2D(heightMap, originalTexCoords).r;


    depth = 1.0 - depth;
    depth *= 0.05;

//    depth = 0.0;

    vec3 displaceOffset = viewDirTangetSpace * depth;

//    return displaceOffset.xy;

    return originalTexCoords + displaceOffset.xy;

}

vec3 transFormToTangentSpace(vec3 vecInCamSpace)
{

    vec3 vecInTangentSpace = TBNMatrix * vecInCamSpace;

    vecInTangentSpace = normalize(vecInTangentSpace);

    vecInTangentSpace *= vec3(1.0, -1.0, 1.0);
//    vecInTangentSpace *= vec3(-1.0, 1.0, 1.0);

    return vecInTangentSpace;
}


void main(void)
{
    vec3 viewDirCamSpace = normalize(fragPos);

    vec3 viewDirTangentSpace = transFormToTangentSpace(viewDirCamSpace);




    vec2 resultingTexCoords = paralaxMap(texCoords.st, viewDirTangentSpace);

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

    gl_FragColor = vec4(finalColorRGB, textureColor.a);
}
