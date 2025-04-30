#version 430 core

layout(binding = 0, std430) readonly buffer Binding_00 { float in_terrain[]; };

uniform ivec2 terrain_size;

uniform ivec2 uResolution;

// orbiter
uniform vec3 center = vec3(0);
uniform vec3 eye = vec3(10, 0, 0);
uniform float focal = 2.;
uniform vec3 up = vec3(0, 0, 1);

out vec4 FragColor;

float sdfBox(vec2 p, vec2 b) {
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

float getTerrain(ivec2 ip) {
    return in_terrain[clamp(ip.y, 0, terrain_size.y - 1) * terrain_size.x + clamp(ip.x, 0, terrain_size.x - 1)];
}

vec2 uv2pix(vec2 uv) {
    vec2 p = uv * float(max(terrain_size.x, terrain_size.y)) / 2.;
    p += terrain_size / 2.;
    return p;
}

float height(vec2 p) {  // uv in [-1, 1]^2
    ivec2 ip = ivec2(int(p.x), int(p.y));

    // bilinear
    p -= ip;
    float h00 = getTerrain(ip);
    float h10 = getTerrain(ip + ivec2(1, 0));
    float h01 = getTerrain(ip + ivec2(0, 1));
    float h11 = getTerrain(ip + ivec2(1, 1));
    float a = h10 - h00;
    float b = h01 - h00;
    float c = h11 - h10 - h01 + h00;
    float d = h00;

    return a * p.x + b * p.y + c * p.x * p.y + d;
}

float sdf(vec3 p) {
    vec2 pix = uv2pix(p.xy);
    vec2 clamp_pix = clamp(pix, vec2(0), vec2(terrain_size.x - 1, terrain_size.y -1));
    float box_d = sdfBox(pix - terrain_size / 2., terrain_size / 2.);
    box_d *= 2. / float(max(terrain_size.x, terrain_size.y));
    float h = 0.0001 * height(clamp_pix.xy);
    float terr_d = 0.5 * (p.z - h);
    float terr_d_low = 0.5 * (p.z - h + 0.05);
    return max(max(terr_d, box_d), -terr_d_low);
}

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
