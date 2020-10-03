#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 glCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    glCoord.x = aTexCoords.x;
    glCoord.y = -1 * aTexCoords.y;
}

#shader fragment
#version 330 core

in vec2 glCoord;
out vec3 glFragColor;
uniform sampler2D FrameBufferInputTexture;


struct Line
{
    vec2 startPoint;
    vec2   endPoint;
};

const float eps = 1e-6;

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


void GetRayLines(inout Line rays[6], int numRays)
{
    float seperationAngle = radians(360/numRays);
    float offsetAngle = rand(glCoord) * radians(360);
    float radius = 5.0;

    for(int index = 0; index < numRays; ++index)
    {
        rays[index].startPoint = glCoord;

        rays[index].endPoint.x = glCoord.x + radius * cos(index * seperationAngle + offsetAngle);
        rays[index].endPoint.y = glCoord.y + radius * sin(index * seperationAngle + offsetAngle);
    }
}

bool IsFragmentInsideCircle(vec2 point, vec2 center, float radius)
{
    if((point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y) < radius * radius)
        return true;
    
    return false;
}

void main()
{

    const float kc = 1.0f;
    const float kl = 2.2f;
    const float kq = 3.2f;

    vec3 lightColor = vec3(1.0, 0.6, 0.4);

    Line walls[4];
    int numWalls = 4;
    
    walls[0].startPoint = vec2(0.02f, -0.02f);
    walls[0].endPoint = vec2(0.98f, -0.02f);

    walls[1].startPoint = vec2(0.98f, -0.02f);
    walls[1].endPoint = vec2(0.98f, -0.98f);

    walls[2].startPoint = vec2(0.98f, -0.98f);
    walls[2].endPoint = vec2(0.02f, -0.98f);

    walls[3].startPoint = vec2(0.02f, -0.98f);
    walls[3].endPoint = vec2(0.02f, -0.02f);

    float circleRadius = 0.05f;
    vec2 circleCenter = vec2(0.65f, -0.35f);

    glFragColor = vec3(texture(FrameBufferInputTexture, glCoord));

    Line rays[6];
    int numRays = 6;

    GetRayLines(rays, numRays);

    vec2 intersection;
    
    for(int index = 0; index < numRays; ++index)
    {
        for(int wall = 0; wall < numWalls; ++wall)
        {
            if(IsFragmentInsideCircle(glCoord, circleCenter, circleRadius))
                continue;
                
            if(RayCircleIntersection(rays[index], circleCenter, circleRadius, intersection) || RayLineIntersection(rays[index], walls[wall], intersection))
            {
                float distance = length(glCoord - intersection);
                float attenuation = 1.0 / (kc + kl * distance + kq * (distance * distance));
                glFragColor += vec3(texture(FrameBufferInputTexture, intersection)) * attenuation;
                /* I need to divide this by original texture to get only light*/
            }
        }
    }
    
    if(glCoord.y > walls[0].startPoint.y)
        glFragColor = vec3(1.0f, 1.0f, 1.0f);

    if(glCoord.x > walls[1].startPoint.x)
        glFragColor = vec3(1.0f, 1.0f, 1.0f);

    if(glCoord.y < walls[2].startPoint.y)
        glFragColor = vec3(1.0f, 1.0f, 1.0f);

    if(glCoord.x < walls[3].startPoint.x)
        glFragColor = vec3(1.0f, 1.0f, 1.0f);
}
