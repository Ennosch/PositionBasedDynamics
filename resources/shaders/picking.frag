#version 330

uniform uint gDrawIndex;
uniform float gObjectIndex;

uniform vec3 color;

out vec3 FragColor;

void main()
{
    FragColor = vec3(float(gObjectIndex), float(gDrawIndex),float(gl_PrimitiveID + 1));
//    FragColor =  vec3(1,float(gObjectIndex),0);


    if(gObjectIndex == 1.0)
    {
        FragColor = vec3(0,1,0);
    }
    else if(gObjectIndex == 2.0)
    {
        FragColor = vec3(1,0,0);
    }
    else
    {
        FragColor = vec3(0,0,1);
    }
////    FragColor = color;
}












