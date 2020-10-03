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
        intersectionPoint = p1 + max(mu1, mu2) * (p2 - p1);
        return true;
    }
    

    return false;
}


void main()
{
    const float kc = 1.0f;
    const float kl = 2.2f;
    const float kq = 3.2f;
    
    glFragColor = vec3(0.0, 0.0, 0.0);

    vec3 lightColor = vec3(1.0, 0.6, 0.4);
    float circleRadius = 0.1f;
    vec2 circleCenter = vec2(0.3f, 0.3f);


    Line ray;
    ray.startPoint = u_LightPos;
    ray.endPoint = glCoord;

    vec2 intersection;
    if(!RayCircleIntersection(ray, circleCenter, circleRadius, intersection))
    {
        float distance = length(glCoord - u_LightPos);
        float attenuation = 1.0 / (kc + kl * distance + kq * (distance * distance));
        glFragColor = lightColor * vec3(texture(u_DiffuseTexture, glTexCoord)) * attenuation;
    }

/*
    vec2 p = (glCoord - u_LightPos) / 0.05;
    float r = sqrt(dot(p, p));
    
    if(r < 1)
        glFragColor = lightColor * (1 - r + vec3(texture(u_DiffuseTexture, glTexCoord)));
*/
}
