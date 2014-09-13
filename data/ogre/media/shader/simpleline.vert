uniform vec3 start;
uniform vec3 end;

uniform vec4 startCol;
uniform vec4 endCol;

void main(void)
{
    vec2 texCoords = gl_MultiTexCoord0.st;

    vec4 startOnScreen = gl_ModelViewProjectionMatrix * vec4(start, 1.0);
    vec4 endOnScreen = gl_ModelViewProjectionMatrix * vec4(end, 1.0);

    vec2 normalOnScreen2D = (endOnScreen - startOnScreen).xy;

    vec2 shiftVecOnScreen = vec2(-normalOnScreen2D.y,
                                 normalOnScreen2D.x);

    float widthOnScreen = 0.01;

    // TODO: must be corrected by render window aspect ratio...
    shiftVecOnScreen *= widthOnScreen;

    vec4 shiftVecOnScreen4D = vec4(shiftVecOnScreen, 0.0, 0.0);


    if(texCoords.y < 0.5)
    {
        // bottom
        startOnScreen -= shiftVecOnScreen4D;
        endOnScreen -= shiftVecOnScreen4D;
    }
    else
    {
        startOnScreen += shiftVecOnScreen4D;
        endOnScreen += shiftVecOnScreen4D;
    }

    if(texCoords.x < 0.5)
    {
        // start
        gl_Position = startOnScreen;
        gl_FrontColor = startCol;
    }
    else
    {
        // end
        gl_Position = endOnScreen;
        gl_FrontColor = endCol;

    }




}
