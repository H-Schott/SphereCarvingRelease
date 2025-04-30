#version 430 core

uniform ivec2 uResolution;

// orbiter
uniform vec3 center = vec3(0);
uniform vec3 eye = vec3(10, 0, 0);
uniform float focal = 2.;
uniform vec3 up = vec3(0, 0, 1);

out vec4 FragColor;


float d_sphere(vec3 p, float r) {
    return length(p) - r;
}

float d_box(vec3 p, vec3 size) {
    vec3 q = abs(p) - size;
	return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.);
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
