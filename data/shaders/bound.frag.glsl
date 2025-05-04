#version 430 core

uniform ivec2 uResolution;

// orbiter
uniform vec3 center = vec3(0);
uniform vec3 eye = vec3(10, 0, 0);
uniform float focal = 2.;
uniform vec3 up = vec3(0, 0, 1);

out vec4 FragColor;

// spheres data
layout(binding = 0, std430) readonly  buffer InHalfPlanes  { vec4  hplanes[]; };

uniform int nb_hplanes = 0;

bool in_hplane(vec3 p, vec4 hp) {
    return dot(p, vec3(hp)) < hp.w + 0.0001;
}

float rayPlaneIntersection(vec3 ro, vec3 rd, vec4 plane) {
    return (plane.w - dot(ro, vec3(plane))) / dot(rd, vec3(plane));
}

bool HalfPlaneTraveling(vec3 ro, vec3 rd, out vec3 p, out vec3 n, out int nb_steps) {
    nb_steps = 0;
    p = ro;
    int last_plane = -1;
    bool in_bound;

    while (nb_steps < 50) {
        nb_steps++;
        in_bound = true;

        // find a half-plane that does not contain p
        for (int i = 0; i < nb_hplanes; i++) {
            if (i == last_plane) continue;
            if (!in_hplane(p, hplanes[i])) {
                in_bound = false;
                last_plane = i;

                // intersect ray and the halfplane
                float t = rayPlaneIntersection(p, rd, hplanes[i]);
                if (t < 0) return false;
                p += t * rd;

                continue;
            }
        }

        if (in_bound) {
            n = (last_plane == -1) ? vec3(0) : vec3(hplanes[last_plane]);
            return true;
        }

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

    vec3 p, n;
    int s;
    bool hit = HalfPlaneTraveling(ro, rd, p, n, s);

    vec3 color = vec3(0.3);
    //color = vec3(float(s) / 3.);

    if (hit) {
        color = 0.7 + 0.3 * n;
    }

	FragColor = vec4(color, 1.);
}
