#version 430 core

uniform ivec2 uResolution;

// orbiter
uniform vec3 center = vec3(0);
uniform vec3 eye = vec3(10, 0, 0);
uniform float focal = 2.;
uniform vec3 up = vec3(0, 0, 1);

out vec4 FragColor;


// sdf data
float d_sphere(vec3 p, float r) {
    return length(p) - r;
}
float d_box(vec3 p, vec3 size) {
    vec3 q = abs(p) - size;
	return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.);
}
float d_cylinder(vec3 p, float h, float r) {
  vec2 d = abs(vec2(length(p.xz), p.y)) - vec2(r, h);
  return min(max(d.x, d.y), 0.) + length(max(d, 0.));
}
float d_cylinder(vec3 p, vec3 a, vec3 axis, float r, float l) {
	vec3 b = a + axis * l;

	vec3  ba = b - a;
	vec3  pa = p - a;
	float baba = dot(ba, ba);
	float paba = dot(pa, ba);
	float x = length(pa * baba - ba * paba) - r * baba;
	float y = abs(paba - baba * 0.5f) - baba * 0.5f;
	float x2 = x * x;
	float y2 = y * y * baba;

	float d = (max(x, y) < 0.0f) ? -min(x2, y2) : (((x > 0.0f) ? x2 : 0.0f) + ((y > 0.0f) ? y2 : 0.0f));

	return sign(d) * sqrt(abs(d)) / baba;
}
float d_cylinder(vec3 p, vec3 a, vec3 b, float r) {
	return d_cylinder(p, a, normalize(b - a), r, length(b - a));
}
float d_smoothUnion(float d1, float d2, float r) {
	float h = max(r - abs(d1 - d2), 0.0f) / r;
	float cube = h * h * h * r * (1.0f / 6.0f);
	return (min(d1, d2) - cube) / max(1.f, r);
}
float d_torus(vec3 p, vec3 c, vec3 axis, float r, float s) {
	vec3 n = p - c;
	float z = dot(n, axis);
	z *= z;
	float d = dot(n, n);
	float y = r - sqrt(d - z);
	y = sqrt(y * y + z);
	return y - s;
}
float c2_sdfBox2(vec2 p2, vec2 c, vec2 d) {
	vec2 q = abs(p2 - c) - d;
	return length(max(q, vec2(0))) + min(max(q.x, q.y), 0.f);
}
float c2_cylinders(vec3 p, vec3 a, vec3 axis, float l, float ri, float re, float e) {
	vec3 n = p - a;
	float y = abs(dot(axis, n));
	float x = sqrt(dot(n, n) - y * y);
	vec2 p2 = vec2(x, y);
	float b1 = c2_sdfBox2(p2, vec2(0, 0), vec2(ri, l));
	float b2 = c2_sdfBox2(p2, vec2(0, l - e / 2.f), vec2(re, e / 2.f));
	return min(b1, b2);
}
float c2_disk2(vec2 p2, vec2 c, float r) {
	return length(p2 - c) - r;
}
float c2_tubular(vec3 p, vec3 a, vec3 axis, float mr, float ht, float l) {
	vec3 q = p - a;
	float s = dot(q, axis);
	float y = length(q - s * axis);
	s = s > l / 2. ? s - l : -s;
	y = abs(y - mr) - ht;
	if (s > 0.)return sqrt(s * s + max(y, 0.f) * max(y, 0.f));
	return max(y, s);
}
float c2_triangle2(vec2 p2, vec2 a, vec2 b, vec2 c) {
	vec2 ab = b - a;
	vec2 ap = p2 - a;
	vec2 bc = c - b;
	vec2 bp = p2 - b;
	vec2 ca = a - c;
	vec2 cp = p2 - c;
	float nab = dot(vec2(ab.y, -ab.x), ap);
	float nbc = dot(vec2(bc.y, -bc.x), bp);
	float nca = dot(vec2(ca.y, -ca.x), cp);
	if ((nab > 0.0f) || (nbc > 0.0f) || (nca > 0.0f)) return min(min(distance(ab * clamp(dot(ab, ap) / dot(ab, ab), 0.0f, 1.0f), ap), distance(bc * clamp(dot(bc, bp) / dot(bc, bc), 0.0f, 1.0f), bp)), distance(ca * clamp(dot(ca, cp) / dot(ca, ca), 0.0f, 1.0f), cp));
	return max(max(nab / length(ab), nbc / length(bc)), nca / length(ca));
}
float c2_subtree_0(vec3 p) {
	float a = atan(p.y, p.x);
	float r = length(vec2(p));
	a += a < 0.0f ? 2.0f * 3.14159f : 0.0f;
	a = mod(a, 1.0472f);
	a = a > 0.5f * 1.0472f ? 1.0472f - a : a;
	p = vec3(r * cos(a), r * sin(a), p.z);
	return c2_cylinders(p, vec3(7.79423f, 4.5f, -0.4f), vec3(0, 0, 1), 0.5f, 0.3f, 0.4f, 0.2f);
}
float c2_subtree_1(vec3 p) {
	float a = atan(p.y, p.x);
	float r = length(vec2(p));
	a += a < 0.0f ? 2.0f * 3.14159f : 0.0f;
	a = mod(a, 1.0472f);
	a = a > 0.5f * 1.0472f ? 1.0472f - a : a;
	p = vec3(r * cos(a), r * sin(a), p.z);
	return c2_cylinders(p, vec3(4.8f, 0, 5.6f), vec3(0, 0, 1), 0.3f, 0.2f, 0.3f, 0.2f);
}
float c2_subtree_3(vec2 p2) {
	float a = atan(p2.y, p2.x);
	float r = length(p2);
	a += a < 0.0f ? 2.0f * 3.14159f : 0.0f;
	a = mod(a, 1.0472f);
	a = a > 0.5f * 1.0472f ? 1.0472f - a : a;
	p2 = vec2(r * cos(a), r * sin(a));
	return c2_disk2(p2, vec2(7.79423f, 4.5f), 1.4f);
}
float c2_subtree_2(vec3 p) {
	vec3 n = p - vec3(0, 0, -0.8f);
	float s = dot(vec3(0, 0, 1), n);
	vec3 q = vec3(0, 0, -0.8f) + s * vec3(0, 0, 1);
	vec2 p2 = vec2(dot(vec3(1, 0, 0), (p - q)), dot(vec3(0, 1, 0), (p - q)));
	float d = d_smoothUnion(c2_disk2(p2, vec2(0, 0), 9), c2_subtree_3(p2), 1.4f);
	if (s <= 0) return length(vec2(s, max(d, 0.0f)));
	if (s >= 0.8f) return length(vec2(s - 0.8, max(d, 0.0f)));
	return max(d, max(-s, s - 0.8f));
}
float c2_subtree_5(vec3 p) {
	vec3 n = p - vec3(0, 0, 0.7);
	float s = dot(vec3(0, 0, 1), n);
	vec3 q = vec3(0, 0, 0.7) + s * vec3(0, 0, 1);
	vec2 p2 = vec2(dot(vec3(1, 0, 0), (p - q)), dot(vec3(0, 1, 0), (p - q)));
	float d = c2_triangle2(p2, vec2(3.4641f, 2), vec2(12.4641f, 2), vec2(7.9641f, 9.79423f));
	if (s <= 0.0f) return length(vec2(s, max(d, 0.0f)));
	if (s >= 3.1f) return length(vec2(s - 3.1f, max(d, 0.0f)));
	return max(d, max(-s, s - 3.1f));
}
float c2_subtree_4(vec3 p) {
	float a = atan(p.y, p.x);
	float r = length(vec2(p));
	a += a < 0.0f ? 2.0f * 3.14159f : 0.0f;
	a = mod(a, 1.0472f);
	a = a > 0.5f * 1.0472f ? 1.0472f - a : a;
	p = vec3(r * cos(a), r * sin(a), p.z);
	return (c2_subtree_5(p) - (0.7f));
}
float c2_tree(vec3 p) {
	float A = max(d_cylinder(p, vec3(0, 0, -0.8f), vec3(0, 0, 1), 9, 6.1f), -d_torus(p, vec3(0, 0, 5), vec3(0, 0, 1), 9, 5));
	float B = min(max(A, -c2_subtree_4(p)), d_cylinder(p, vec3(0, 0, -1.4), vec3(0, 0, 1), 8, 1.4f));
	return max(max(min(max(min(B, c2_subtree_2(p)), -(length(clamp(dot(p - vec3(0, 0, -2), vec3(0, 0, 0.111111f)), 0.0f, 1.0f) * vec3(0, 0, 9) + vec3(0, 0, -2) - p) - 2)), max(min(c2_tubular(p, vec3(0, 0, 4.8f), vec3(0, 0, 1), 3.65f, 0.65f, 0.5f), c2_tubular(p, vec3(0, 0, 5.3f), vec3(0, 0, 1), 4.7f, 0.8f, 0.5f)), -d_torus(p, vec3(0, 0, 5.4), vec3(0, 0, 1), 3.7f, 0.1f))), -c2_subtree_1(p)), -c2_subtree_0(p));
}
float tp_smax( float a, float b, float k) {
    return -d_smoothUnion(-a,-b,k);
}
float tp_smax( float a, float b) {
    return tp_smax(a,b,0.1);
}
float tp_sq(float x){
	return x*x;
}
float tp_torus(float x, float y, float z, float R, float r) {
    vec2 xz = vec2(x, z); 
    vec2 q = vec2(length(xz)-R,y); 
    return length(q)-r;
}
float tp_lid(float x, float y, float z) {
    float min_y = 0.06;
    float ampli = 0.45;
    y -= min_y;
    float v = sqrt(tp_sq(x)+tp_sq(y-0.55)+tp_sq(z))-1.4;
    v = d_smoothUnion(v,tp_torus(y-2.,x,z,.2,.08),.1);
    v = tp_smax(v,-sqrt(tp_sq(x)+tp_sq(y-0.55)+tp_sq(z))+1.3);
    v = tp_smax(v,sqrt(tp_sq(x)+tp_sq(y-2.5)+tp_sq(z))-1.3);

    v = tp_smax(v,-sqrt(tp_sq(x-.25)+tp_sq(z-.35))+0.05,.05);
    v = d_smoothUnion(v,tp_torus(x,(y-1.45)*.75,z,.72,.065),.2);
    return v;
}
float tp_nose(float x, float y, float z) {
    z -= sin((y+0.8)*3.6)*.15;
    
    float v = sqrt(tp_sq(x)+tp_sq(z));
    
    v = abs(v-.3+sin(y*1.6+.5)*0.18)-.05;
    v = tp_smax(v,-y-1.);
    v = tp_smax(v,y-0.85,.075);
    
    return v;
}
float tp_final(vec3 p) {
    float x = p.x;
    float y = p.y;
    float z = p.z;

    float v = 0.0;
    v = sqrt(x*x+z*z)-1.2-sin(y*1.5+2.0)*.4;
    v = tp_smax(v,abs(y)-1.,0.3);

    float v1 = sqrt(x*x*4.+tp_sq(y+z*.1)*1.6+tp_sq(z+1.2))-1.0;
    v1 = tp_smax(v1,-sqrt(tp_sq(z+1.2)+tp_sq(y+z*.12+.015)*1.8)+.8,.3);
    
    v = d_smoothUnion(v,tp_torus(y*1.2+.2+z*.3,x*.75,z+1.25+y*.2,.8,.1),.25);
    v = d_smoothUnion(v,sqrt(tp_sq(x)+tp_sq(y-1.1)+tp_sq(z+1.8))-.05,.32);

    float v3 = tp_nose(x,(y+z)*sqrt(.5)-1.6,(z-y)*sqrt(.5)-1.1);

    v = d_smoothUnion(v,v3,0.2);
    
    v = tp_smax(v,d_smoothUnion(sin(y*1.4+2.0)*0.5+.95-sqrt(x*x+z*z),y+.8, .2));
    v = tp_smax(v,-sqrt(tp_sq(x)+tp_sq(y+.15)+tp_sq(z-1.5))+.12);

    v = d_smoothUnion(v,tp_torus(x,y-0.95,z,0.9,.075), 0.1);
    v = d_smoothUnion(v,tp_torus(x,y+1.05,z,1.15,.05),0.15);
    
    float anim = 1.;
    float v2 = tp_lid(x, y + .5 - 0.5 * anim,z);
    v = min(v,v2);

    return v;
}
float tp_tree(vec3 p) {
	p.xyz = p.xzy;
    const float scale = 0.5;
    p *= 1. / scale;
    return tp_final(p) * scale * 0.8;
}


#place_sdf_code_here

vec3 sdfNormal(vec3 p) {
    vec2 eps = vec2(0.0001, 0);
    vec3 n;
    n.x = sdf(p + eps.xyy) - sdf(p - eps.xyy);
    n.y = sdf(p + eps.yxy) - sdf(p - eps.yxy);
    n.z = sdf(p + eps.yyx) - sdf(p - eps.yyx);
    return normalize(n);
}

bool sphereTracing(vec3 ro, vec3 rd, out float t) {

    t = 0;

    for (int k = 0; k < 256; k++) {
        vec3 p = ro + t * rd;
        float d = sdf(p);

        if (d < 0.0001) {
            return true;
        }

        t += max(0.0001, d);

        if (t > 20.) return false;
    }

    return false;
}

void main() {

    float screen_ratio = float(uResolution.x) / float(uResolution.y);

    // p in [0, 1]^2
    vec2 u = vec2(gl_FragCoord.x / float(uResolution.x), gl_FragCoord.y / float(uResolution.y));
    u -= 0.5;
    u *= 0.4;

    vec3 view_axis = normalize(center - eye);
    vec3 local_x = normalize(cross(view_axis, up));
    vec3 local_y = normalize(cross(local_x, view_axis));

    vec3 ro = eye + u.x * local_x * screen_ratio + u.y * local_y;
    vec3 focal_point = eye - focal * view_axis;
    vec3 rd = normalize(ro - focal_point);

    float t = 0;
    bool hit = sphereTracing(ro , rd, t);

    vec3 color = vec3(0.3);

    if (hit) {
        color = 0.7 + 0.3 * sdfNormal(ro + t * rd);
    }

	FragColor = vec4(color, 1.);
}
