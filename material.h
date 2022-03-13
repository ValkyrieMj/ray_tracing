#pragma once
#include "ray.h"
#include "hittable.h"
#include "texture.h"
class material {
public:
	virtual bool scatter(  //判断有没有发生散射
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	)const = 0;
};


class lambertian :public material {   //理想漫反射材质
public:
	//vec3 albedo;
    shared_ptr<texture> albedo;

public:
    lambertian(shared_ptr<texture> a):albedo(a){}

    virtual bool scatter(   
        const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
    ) const {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo->value(rec.u,rec.v,rec.p);     //衰减率
        return true;
    }
};

class metal : public material {   //金属材质
public:
    metal(const vec3& a,double f) : albedo(a),fuzz(f<1?f:1) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
    ) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    vec3 albedo;
    double fuzz;
};

class dielectric :public material {
public:
    double ref_idx;
    dielectric(double ri):ref_idx(ri){}
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
    )const {
        attenuation = vec3(1.0, 1.0, 1.0);
        double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);

        vec3 unit_dirction = unit_vector(r_in.direction());
        double cos_theat = ffmin(dot(-unit_dirction, rec.normal), 1.0);
        double sin_theat = sqrt(1.0 - cos_theat * cos_theat);
        if (etai_over_etat * sin_theat > 1.0) {   //没有发生折射现象
            vec3 refracted = reflect(unit_dirction, rec.normal);
            scattered = ray(rec.p, refracted);
            return true;
        }
        double reflect_prob = schlick(cos_theat, etai_over_etat);
        if (random_double() < reflect_prob)
        {
            vec3 refracted = reflect(unit_dirction, rec.normal);
            scattered = ray(rec.p, refracted);
            return true;
        }
        vec3 refracted = refract(unit_dirction, rec.normal, etai_over_etat);
        scattered = ray(rec.p, refracted);
        return true;
    }
};

