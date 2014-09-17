uniform sampler2D colorMap;
uniform sampler2D reflectionMap;

varying vec2 colorMapCoords;
varying vec2 reflectionMapCoords;

varying vec3 diffuseLight;
varying vec3 specularLight;


void main()
{
    vec4 texCol = texture2D(colorMap, colorMapCoords);
    vec4 reflectionCol = texture2D(reflectionMap, reflectionMapCoords);



    vec4 ambientColor = gl_LightModel.ambient;

    vec3 finalCol =
            texCol.rgb * diffuseLight
            + specularLight * reflectionCol.a
            + ambientColor.rgb;


//    finalCol = diffuseLight;
//    finalCol = specularLight * reflectionCol.a;


    gl_FragColor = vec4(finalCol, texCol.a);

//    gl_FragColor = reflectionCol;

}

