#version 330 core
in vec3 aPos;
in vec3 aNorm;
out vec4 FragColor;

struct Material {
    vec3  d, s;
    float p;
};

struct Light {
    vec3 a, d, s, p;
    float kl, kq;
    bool dir;
};

uniform Light lights[8];
uniform int nLights;
uniform Material material;
uniform vec3 eye;
uniform bool phong;

void main() {
    vec3 bNorm = normalize(aNorm);

    vec3 color = vec3(0, 0, 0);
    vec3 L, R, V, H;
    float dist, attenuation, specular;

    for (int i = 0; i < nLights; i++) {
        if (lights[i].dir) {
            L = normalize(-lights[i].p);
            attenuation = 1.0;
        }
        else {
            L = normalize(lights[i].p - aPos);
            dist = length(lights[i].p - aPos);
            attenuation = 1.0 / (1.0 + lights[i].kl * dist + lights[i].kq * pow(dist, 2.0));
        }

        V = normalize(eye - aPos);
        if (phong) {
            R = normalize(2 * dot(L, bNorm) * bNorm - L);
            specular = pow(max(0, dot(R, V)), material.p);
        }
        else {
            H = normalize(L + V);
            specular = pow(max(0, dot(H, bNorm)), material.p);
        }

        color += material.d * lights[i].a * attenuation;
        color += material.d * lights[i].d * max(0, dot(bNorm, L)) * attenuation;
        color += material.s * lights[i].s * specular * attenuation;
    }

    FragColor = vec4(pow(color, vec3(1.0/2.2)), 0.0);
}
