#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
public:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;

	vec3 u, v, w;
	double lens_radius;

public:
	camera(
		vec3 lookfrom, vec3 lookat, vec3 vup,
		double vfov,
		double aspect,
		double aperture,double focus_dist
	) {
		origin = lookfrom;
		lens_radius = aperture / 2;

		auto theta = degrees_to_radians(vfov);
		auto half_height = tan(theta / 2);
		auto half_width = aspect * half_height;
		w = unit_vector(lookfrom - lookat);  //正交与相机平面的基向量,z轴
		u = unit_vector(cross(vup, w));   //相机平面的水平向量。x轴
		v = cross(w, u);   

		lower_left_corner = origin 
							- half_width * focus_dist * u
							- half_height * focus_dist * v
							- focus_dist*w;
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;

	}

	ray get_ray(double s, double t) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, 
			lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}
};

#endif