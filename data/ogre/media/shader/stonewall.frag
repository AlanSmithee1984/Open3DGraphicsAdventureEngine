uniform sampler2D colorMap;
uniform sampler2D normalMap;

varying vec4 texCoords;


varying vec3 fragPos;

void main(void)
{
    vec3 normal = texture2D(normalMap, texCoords.st).rgb;

    normal = gl_NormalMatrix * normal;

//    normal = normalize(normal);

//    normal = vec3(0.0, 0.0, 1.0);

    vec4 lightPos = gl_LightSource[0].position;
    // assume directional light
    vec3 lightDir = normalize(lightPos.xyz);

//    lightDir = vec3(0.0, 0.0, 1.0);

    float lambertian = max( dot( lightDir, normal ), 0.0);

    vec4 diffuseLightColor = gl_LightSource[0].diffuse;
    vec3 diffuseLight = diffuseLightColor.rgb * lambertian;

    vec3 specularLight;

    if(lambertian > 0.0)
    {
        vec3 reflectDir = reflect(-lightDir, normal).xyz;
        vec3 viewDir = normalize(-fragPos);

        float specAngle = max( dot( reflectDir, viewDir ), 0.0 );
        float specular = pow(specAngle, 16.0);

        specular *= lambertian;

        vec4 specLightColor = gl_LightSource[0].specular;
        specularLight = specLightColor.rgb * specular;

    }
    else
    {
        specularLight *= 0.0;
    }



    vec4 textureColor = texture2D(colorMap, texCoords.st);

    textureColor = vec4(1.0, 0.0, 0.0, 1.0);


    vec4 ambientColor = gl_LightModel.ambient;

    vec3 finalColorRGB = textureColor.rgb * diffuseLight + specularLight + ambientColor.rgb;

//    finalColorRGB = normal;
//    finalColorRGB = diffuseLight;
//    finalColorRGB = specularLight;

//    finalColorRGB = vec4(lambertian);

//    finalColorRGB = textureColor.rgb;

    finalColorRGB = lightDir;

    gl_FragColor = vec4(finalColorRGB, textureColor.a);
}