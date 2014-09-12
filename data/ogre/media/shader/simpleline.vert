uniform vec3 start;
uniform vec3 end;

uniform vec4 startCol;
uniform vec4 endCol;

void main(void)
{
    vec2 texCoords = gl_MultiTexCoord0.st;

    vec4 startOnScreen = gl_ModelViewProjectionMatrix * vec4(start, 1.0);
    startOnScreen /= startOnScreen.w;

    vec4 endOnScreen = gl_ModelViewProjectionMatrix * vec4(end, 1.0);
    endOnScreen /= endOnScreen.w;

    vec2 lineOnScreen = (endOnScreen - startOnScreen).xy;
    vec2 normalOnScreen = vec2(-lineOnScreen.y, lineOnScreen.y);

//    float lengthOnScreen = length(lineOnScreen);
//    float widthOnScreen = 0.1 * lengthOnScreen;

    float widthOnScreen = 0.1;

    if(texCoords.y < 0.5)
    {
        // bottom
        startOnScreen -= widthOnScreen;
        endOnScreen -= widthOnScreen;
    }
    else
    {
        startOnScreen += widthOnScreen;
        endOnScreen += widthOnScreen;
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
