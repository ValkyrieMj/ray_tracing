#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

void get_sphere_uv(const vec3& p, double& u, double& v)
{
	auto phi = atan2(p.z(), p.x());
	auto theta = asin(p.y());
	u = 1 - (phi + pi) / (2 * pi);
	v = (theta + pi / 2) / pi;
}

class material;

struct hit_record {
	vec3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	double u; //存储纹理图案击中点信息
	double v;
	bool front_face;

	//判断射入内侧还是外侧
	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;  //纯虚函数
};

#endif // !HITTABLE_H
