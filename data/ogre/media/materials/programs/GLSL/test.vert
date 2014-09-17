varying vec2 colorMapCoords;
varying vec2 reflectionMapCoords;

varying vec3 diffuseLight;
varying vec3 specularLight;

void main()
{
    colorMapCoords = gl_MultiTexCoord0.st;
    reflectionMapCoords = gl_MultiTexCoord1.st;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;


    vec3 normal = gl_Normal;

    vec3 transformedNormal = gl_NormalMatrix * normal;
    transformedNormal = normalize(transformedNormal);


    vec4 lightPos = gl_LightSource[0].position;

    // assume directional light
    vec3 lightDir = vec3(lightPos);

    lightDir = vec3(0.0, 0.0, 1.0);

    lightDir = gl_NormalMatrix * lightDir;

    lightDir = normalize(lightDir);

    float lambertian = max( dot( lightDir, transformedNormal ), 0.0);
//    lambertian = 0.5;

    vec4 diffuseLightColor = gl_LightSource[0].diffuse;
    diffuseLight = diffuseLightColor.rgb * lambertian;


    if(lambertian > 0.0)
    {
        vec3 reflectDir = reflect(-lightDir, transformedNormal).xyz;



        // all following gemetric computations are performed in the
        // camera coordinate system (aka eye coordinates)
        vec3 vertScreenPos = gl_Position.xyz / gl_Position.w;

        vec3 viewDir = normalize(-vertScreenPos);

        float specAngle = max( dot( reflectDir, viewDir ), 0.0 );
        float specular = pow(specAngle, 16.0);

        specular *= lambertian;

        vec4 specLightColor = gl_LightSource[0].specular;
        specLightColor = vec4(1.0, 1.0, 1.0, 1.0);

        specularLight = specLightColor.rgb * specular;


    }
    else
    {
        specularLight *= 0.0;
    }


//    diffuseLight = gl_Normal;

//    diffuseLight = transformedNormal;

//    diffuseLight = lightDir;


//    diffuseLight = vec3(lambertian);



}
