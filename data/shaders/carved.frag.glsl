#version 430 core

uniform ivec2 uResolution;

// orbiter
uniform vec3 center = vec3(0);
uniform vec3 eye = vec3(10, 0, 0);
uniform float focal = 2.;
uniform vec3 up = vec3(0, 0, 1);

out vec4 FragColor;

// spheres data
layout(binding = 0, std430) readonly  buffer InSPheres  { vec4  spheres[]; };
uniform vec4 big_sphere;
uniform int nb_spheres;
/*
const int nb_spheres = 30;
const vec4 big_sphere = vec4(0, 0, 0, 2);
const vec4[30] spheres = vec4[30](vec4(0, 0, 2, 1.2),
								vec4(0, 0, -2, 1.2),
								vec4(0, 2, 0, 1.2),
								vec4(0, -2, 0, 1.2),
								vec4(2, 0, 0, 1.2),
								vec4(-2, 0, 0, 1.2),
								vec4(1, 2, 0, 1.5),
								vec4(2, 1, 0, 1.5),
								vec4(0, 2, 1, 1.5),
								vec4(0, 1, 2, 1.5),
								vec4(2, 0, 1, 1.5),
								vec4(1, 0, 2, 1.5),
								vec4(-1, 2, 0, 1.5),
								vec4(-2, 1, 0, 1.5),
								vec4(0, -2, 1, 1.5),
								vec4(0, -1, 2, 1.5),
								vec4(-2, 0, 1, 1.5),
								vec4(-1, 0, 2, 1.5),
								vec4(-1, -2, 0, 1.5),
								vec4(-2, -1, 0, 1.5),
								vec4(0, -2, -1, 1.5),
								vec4(0, -1, -2, 1.5),
								vec4(-2, 0, -1, 1.5),
								vec4(-1, 0, -2, 1.5),
								vec4(1, -2, 0, 1.5),
								vec4(2, -1, 0, 1.5),
								vec4(0, 2, -1, 1.5),
								vec4(0, 1, -2, 1.5),
								vec4(2, 0, -1, 1.5),
								vec4(1, 0, -2, 1.5));
*/

bool inSphere(vec3 p, vec4 s) {
	return length(p - vec3(s)) < s.w;
}

float distSphere(vec3 p, vec4 s) {
	return length(p - vec3(s)) - s.w;
}

bool isInShape(vec3 p) {
	if (!inSphere(p, big_sphere)) return false;

	for (int i = 0; i < nb_spheres; i++) {
		if (inSphere(p, spheres[i])) return false;
	}

	return true;
}

vec4 closestSphere(vec3 p, out bool isBigSphere) {
	vec4 smin = big_sphere;
	float dmin = abs(distSphere(p, big_sphere));
	isBigSphere = true;

	for (int i = 0; i < nb_spheres; i++) {
		float d = abs(distSphere(p, spheres[i]));
		if (d < dmin) {
			smin = spheres[i];
			dmin = d;
			isBigSphere = false;
		}
	}

	return smin;
}

vec3 getNormal(vec3 p) {
	bool isBigSphere;
	vec4 s = closestSphere(p, isBigSphere);
	return normalize(vec3(s) - p) * (isBigSphere ? -1. : 1.);
}

bool root2(float a, float b, float c, out float r1, out float r2) {
	float delta = b * b - 4 * a * c;
	if (delta < 0.) return false;
	float sdelta = sqrt(delta);
	float inv2a = 1. / (2. * a);
	r1 = (-b + sdelta) * inv2a;
	r2 = (-b - sdelta) * inv2a;
	return true;
}

bool raySphereInter(vec3 ro, vec3 rd, vec4 s, out vec3 p) {
	float a = dot(rd, rd);
	float b = 2. * dot(ro - vec3(s), rd);
	float c = dot(ro - vec3(s), ro - vec3(s)) - s.w * s.w;

	float t1;
	float t2;
	bool hit = root2(a, b, c, t1, t2);
	if (!hit) return false;

	float t = min(t1, t2) < 0. ? max(t1, t2) : min(t1, t2);
	if (t < 0.) return false;

	p = ro + t * rd;
	return true;
}

bool SphereTraveling(vec3 ro, vec3 rd, out vec3 p, out vec3 n, out int s) {

	// already inside big sphere
	if (inSphere(ro, big_sphere)) {
		p = ro;
	}
	// intersect big sphere
	else {
		bool hit = raySphereInter(ro, rd, big_sphere, p);
		// no intersection with initial volume
		if (!hit) return false;
	}


	// SPHERE TRAVELING
	p += 0.001 * rd;
	bool inCarved = true;
	int last_sphere = -1;
	s = 0;
	for(int k = 0; k < 100; k++) {
		s++;
		inCarved = true;

		// search a sphere that contains p
		for(int i = 0; i < nb_spheres; i++) {
			if (inSphere(p, spheres[i])) {
				inCarved = false;

				vec3 q;
				bool hit_s = raySphereInter(p, rd, spheres[i], q);
				p = q + 0.001 * rd;
				last_sphere = i;
			}
		}

		if (inCarved) {
			n = (last_sphere == -1) ? normalize(p - vec3(big_sphere)) : normalize(vec3(spheres[last_sphere]) - p);
			return true;
		}

		if (!inSphere(p, big_sphere)) return false;
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
    bool hit = SphereTraveling(ro , rd, p, n, s);

    vec3 color = vec3(0.3);
	//color = vec3(float(s) / 10.);

    if (hit) {
        color = 0.7 + 0.3 * n;
    }

	FragColor = vec4(color, 1.);
}
