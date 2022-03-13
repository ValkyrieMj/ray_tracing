#pragma once
#include "rtweekend.h"

class texture {
public:
	virtual vec3 value(double u, double v, const vec3& p) const = 0;
};

class constant_texture :public texture {
public:
	vec3 color;
public:
	constant_texture(){}
	constant_texture(vec3 c):color(c){}
	virtual vec3 value(double u, double v, const vec3& p) const {
		return color;
	}
};

class checker_texture :public texture {
public:
	shared_ptr<texture> odd;
	shared_ptr<texture> even;
public:
	checker_texture(){}
	checker_texture(shared_ptr<texture> t0,shared_ptr<texture> t1):even(t0),odd(t1){}

	virtual vec3 value(double u, double v, const vec3& p) const {
		auto sines = sin(10 * p.x()) * sin(10 * p.y())* sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
};

//class image_texture :public texture {
//public:
//	unsigned char* data;
//	int nx, ny;
//
//public:
//	image_texture() {}
//	image_texture(unsigned char *pixels, int A, int B):data(pixels),nx(A),ny(B){}
//	~image_texture() {
//		delete data;
//	}
//
//	virtual vec3 value(double u, double v, const vec3& p) const {
//		if (data == nullptr)
//			return vec3(0, 1, 1);
//
//		auto i = static_cast<int>((u)*nx);
//		auto j = static_cast<int>((1 - v) * ny - 0.001);
//
//		if (i < 0) i = 0;
//		if (j < 0) j = 0;
//		if (i > nx - 1) i = nx - 1;
//		if (j > ny - 1) j = ny - 1;
//
//		auto r = static_cast<int>(data[3 * i + 3 * nx * j + 0]) / 255.0;
//		auto g = static_cast<int>(data[3 * i + 3 * nx * j + 1]) / 255.0;
//		auto b = static_cast<int>(data[3 * i + 3 * nx * j + 2]) / 255.0;
//
//		return vec3(r, g, b);
//	}
//};