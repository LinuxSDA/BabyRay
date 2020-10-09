#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 glCoord;
out vec2 glTexCoord;

void main()
{
    gl_Position = position;
    glCoord = vec2(position);
    glTexCoord = texCoord;

}

#shader fragment
#version 330 core

in vec2 glCoord;
in vec2 glTexCoord;

uniform vec2        u_LightPos;
uniform sampler2D   u_DiffuseTexture;

layout(location = 0) out vec3 glFragColor;

struct Line
{
    vec2 startPoint;
    vec2   endPoint;
};

const float eps = 1e-6;
const int numRays = 5;
const int numWalls = 4;
const int numCircles = 3;
const int numLights = 3;

// Since this is GPU code I'm not checking for nulls
bool RayLineIntersection(Line line1, Line line2, inout vec2 point)
{
    float p0_x = line1.startPoint.x;
    float p0_y = line1.startPoint.y;

    float p1_x = line1.endPoint.x;
    float p1_y = line1.endPoint.y;

    float p2_x = line2.startPoint.x;
    float p2_y = line2.startPoint.y;

    float p3_x = line2.endPoint.x;
    float p3_y = line2.endPoint.y;


    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float det = s1_x * s2_y - s1_y * s2_x;

    // If det is zero, there is no intersection
    if (abs(det) < eps)
        return false;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / det;
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / det;

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        point.x = p0_x + (t * s1_x);
        point.y = p0_y + (t * s1_y);
        return true;
    }

    return false; // No collision
}


bool RayCircleIntersection(Line line, vec2 center, float r, inout vec2 intersectionPoint)
{
   float bb4ac;
   vec2 p1 = line.startPoint;
   vec2 p2 = line.endPoint;
   
   vec2 dp = p2 - p1;

    float a = dot(dp, dp);
    float b = 2.0 * dot(dp, p1 - center);
    float c = dot(center, center) + dot(p1, p1) - 2.0 * dot(center, p1) - r*r;

   bb4ac = b * b - 4 * a * c;

   if (abs(a) < eps || bb4ac < 0) {
      return false;
   }

   float mu1 = (-b + sqrt(bb4ac)) / (2 * a);
   float mu2 = (-b - sqrt(bb4ac)) / (2 * a);

    if((mu1 > 0 && mu1 < 1) || (mu2 > 0 && mu2 < 1))
    {
        // get point closer to circle
        intersectionPoint = p1 + min(mu1, mu2) * (p2 - p1);
        return true;
    }
    

    return false;
}

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void GetRayLines(inout Line rays[numRays], int numRays)
{
    float seperationAngle = radians(360/numRays);
    float offsetAngle = rand(glCoord) * radians(360);
    float radius = 10.0;

    for(int index = 0; index < numRays; ++index)
    {
        rays[index].startPoint = glCoord;

        rays[index].endPoint.x = glCoord.x + radius * cos(index * seperationAngle + offsetAngle);
        rays[index].endPoint.y = glCoord.y + radius * sin(index * seperationAngle + offsetAngle);
    }
}

bool RayWallsIntersection(in Line ray, in Line walls[numWalls], inout vec2 intersection)
{
    for(int wall = 0; wall < numWalls; ++wall)
    {
        if(RayLineIntersection(ray, walls[wall], intersection))
            return true;
    }
    
    return false;
}

bool RayCirclesIntersection(in Line ray, in vec2 circleCenter[numCircles], in float circleRadius[numCircles], inout vec2 intersection)
{
    for(int index = 0; index < numCircles; ++index)
    {
        if(RayCircleIntersection(ray, circleCenter[index], circleRadius[index], intersection))
            return true;
    }
    
    return false;
}



void main()
{
    const float kc = 1.0f;
    const float kl = 1.2f;
    const float kq = 2.2f;

    vec3 lightColor[numLights];
    lightColor[0] = vec3(1.0, 1.0, 0.0);
    lightColor[1] = vec3(0.2, 0.6, 0.2);
    lightColor[2] = vec3(1.0, 0.0, 0.0);

    vec2 lightPosition[numLights];
    lightPosition[0] = u_LightPos;
    lightPosition[1] = vec2(-0.8, 0.8);
    lightPosition[2] = vec2(0.8, -0.8);

    Line walls[numWalls];

    walls[0].startPoint = vec2(-1.0f, 1.0f);
    walls[0].endPoint = vec2(1.0f, 1.0f);

    walls[1].startPoint = vec2(1.0f, 1.0f);
    walls[1].endPoint = vec2(1.0f, -1.0f);

    walls[2].startPoint = vec2(1.0f, -1.0f);
    walls[2].endPoint = vec2(-1.0f, -1.0f);

    walls[3].startPoint = vec2(-1.0f, -1.0f);
    walls[3].endPoint = vec2(-1.0f, 1.0f);

    
    float circleRadius[numCircles];
    circleRadius[0] = 0.10f;
    circleRadius[1] = 0.25f;
    circleRadius[2] = 0.20f;
//    circleRadius[3] = 0.15f;
//    circleRadius[4] = 0.30f;

    vec2 circleCenter[numCircles];
    circleCenter[0] = vec2(0.5f, 0.0f);
    circleCenter[1] = vec2(-0.5f, -0.4f);
    circleCenter[2] = vec2(0.0f, 0.4f);
//    circleCenter[3] = vec2(0.3f, -0.6f);
//    circleCenter[4] = vec2(-0.6f, 0.3f);

    glFragColor = vec3(0.0f, 0.0f, 0.0f);

    for(int index = 0; index < numLights; ++index)
    {
        Line directRay;
        directRay.startPoint = lightPosition[index];
        directRay.endPoint = glCoord;
        
        vec2 intersection;
        if(!RayCirclesIntersection(directRay, circleCenter, circleRadius, intersection))
        {
            float distance = length(directRay.startPoint - directRay.endPoint);
            float attenuation = 1.0 / (kc + kl * distance + kq * (distance * distance));
            glFragColor += lightColor[index] * vec3(texture(u_DiffuseTexture, glTexCoord)) * attenuation;
        }
    }
    
    /* Direct lighting has been computed. Let's compute indirect lighting now */

    Line rays[numRays];
    GetRayLines(rays, numRays);
    vec2 intersection;

    for(int index = 0; index < numRays; ++index)
    {
        if (!RayCirclesIntersection(rays[index], circleCenter, circleRadius, intersection))
            RayWallsIntersection(rays[index], walls, intersection);

        for(int index = 0; index < numLights; ++index)
        {
            Line bounceRay;
            bounceRay.startPoint = lightPosition[index];
            bounceRay.endPoint = intersection;
                
            if(!RayCirclesIntersection(bounceRay, circleCenter, circleRadius, intersection))
            {
                vec3 color;
                
                {
                    float distance = length(bounceRay.endPoint - bounceRay.startPoint);
                    float attenuation = 1.0 / (kc + kl * distance + kq * (distance * distance));
                    color = lightColor[index] * attenuation;
                }
                
                {
                    float distance = length(bounceRay.endPoint - glCoord);
                    float attenuation = 1.0 / (kc + kl * distance + kq * (distance * distance));
                    glFragColor += color * vec3(texture(u_DiffuseTexture, glTexCoord)) * attenuation/numRays;
                }
            }
        }
    }

    for(int index = 0; index < numLights; ++index)
    {
        vec2 p = (glCoord - lightPosition[index]) / 0.03;
        float r = sqrt(dot(p, p));

        if(r < 1)
            glFragColor += lightColor[index] * (1 - r + vec3(texture(u_DiffuseTexture, glTexCoord)));

    }
}
