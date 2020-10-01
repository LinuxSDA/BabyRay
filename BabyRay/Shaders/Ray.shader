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

bool LineLineIntersection(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
    float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
    float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;
 
    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    // If d is zero, there is no intersection
    if (d == 0)
        return false;
    
    // Get the x and y
    float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
    float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
 
    // Check if the x and y coordinates are within both lines
    if ( x < min(x1, x2) || x > max(x1, x2) || x < min(x3, x4) || x > max(x3, x4) )
        return false;
    if ( y < min(y1, y2) || y > max(y1, y2) || y < min(y3, y4) || y > max(y3, y4) )
        return false;
 
    // Return the point of intersection
    // Point* ret = new Point();
    // ret->x = x;
    // ret->y = y;
    
    return true;
}


const float eps = 1e-6;
   
float sq(float x) {
    return x * x;
}

float fabs(float value)
{
    return sign(value) * value;
}

bool within(float x1, float y1, float x2, float y2, float x, float y) {
    float d1 = sqrt(sq(x2 - x1) + sq(y2 - y1));    // distance between end-points
    float d2 = sqrt(sq(x - x1) + sq(y - y1));      // distance from point to one end
    float d3 = sqrt(sq(x2 - x) + sq(y2 - y));      // distance from point to other end
    float delta = d1 - d2 - d3;
    return fabs(delta) < eps;   // true if delta is less than a small tolerance
}
 
int rxy(float x1, float y1, float x2, float y2, float x, float y, bool segment) {
    if (!segment || within(x1, y1, x2, y2, x, y)) {
        return 1;
    } else {
        return 0;
    }
}
 
float fx(float A, float B, float C, float x) {
    return -(A * x + C) / B;
}
 
float fy(float A, float B, float C, float y) {
    return -(B * y + C) / A;
}
 
bool LineCircleIntersection(vec2 p1, vec2 p2, vec2 cp, float r) {
    bool segment = true;
    
    float x0 = cp.x, y0 = cp.y;
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;
    float A = y2 - y1;
    float B = x1 - x2;
    float C = x2 * y1 - x1 * y2;
    float a = sq(A) + sq(B);
    float b, c, d;
    bool bnz = true;
    int cnt = 0;
 
    if (fabs(B) >= eps) {
        // if B isn't zero or close to it
        b = 2 * (A * C + A * B * y0 - sq(B) * x0);
        c = sq(C) + 2 * B * C * y0 - sq(B) * (sq(r) - sq(x0) - sq(y0));
    } else {
        b = 2 * (B * C + A * B * x0 - sq(A) * y0);
        c = sq(C) + 2 * A * C * x0 - sq(A) * (sq(r) - sq(x0) - sq(y0));
        bnz = false;
    }
    d = sq(b) - 4 * a * c; // discriminant
    if (d < 0) {
        return false;
    }
 
    if (d == 0) {
        // line is tangent to circle, so just one intersect at most
        if (bnz) {
            float x = -b / (2 * a);
            float y = fx(A, B, C, x);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
        } else {
            float y = -b / (2 * a);
            float x = fy(A, B, C, y);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
        }
    } else {
        // two intersects at most
        d = sqrt(d);
        if (bnz) {
            float x = (-b + d) / (2 * a);
            float y = fx(A, B, C, x);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
 
            x = (-b - d) / (2 * a);
            y = fx(A, B, C, x);
            cnt += rxy(x1, y1, x2, y2, x, y, segment);
        } else {
            float y = (-b + d) / (2 * a);
            float x = fy(A, B, C, y);
            cnt = rxy(x1, y1, x2, y2, x, y, segment);
 
            y = (-b - d) / (2 * a);
            x = fy(A, B, C, y);
            cnt += rxy(x1, y1, x2, y2, x, y, segment);
        }
    }
 
    if (cnt <= 0) {
      return false;
    }

    return true;
}

void main()
{
    const float kc = 1.0f;
    const float kl = 1.2f;
    const float kq = 2.2f;
    
    glFragColor = vec3(0.0, 0.0, 0.0);

    vec3 lightColor = vec3(1.0, 0.6, 0.4);
    float circleRadius = 0.1f;
    vec2 circleCenter = vec2(0.0f, 0.3f);

    if(!LineCircleIntersection(u_LightPos, glCoord, circleCenter, circleRadius))
    {
        float distance = length(glCoord - u_LightPos);
        float attenuation = 1.0 / (kc + kl * distance + kq * (distance * distance));
        glFragColor += lightColor * vec3(texture(u_DiffuseTexture, glTexCoord)) * attenuation;

    }
    
    vec2 p = (glCoord - u_LightPos) / 0.04;
    float r = sqrt(dot(p, p));
    
    if(r < 1)
        glFragColor = lightColor * (1 - r + vec3(texture(u_DiffuseTexture, glTexCoord)));
        
}
