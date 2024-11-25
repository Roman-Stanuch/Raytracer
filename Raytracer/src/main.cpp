#include "vec3.h"
#include "color.h"
#include "ray.h"

#include <iostream>
#include <fstream>

double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = center - r.origin();
	auto a = r.direction().length_squared();
	auto h = dot(r.direction(), oc);
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = h * h - a * c;

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (h - std::sqrt(discriminant)) / a;
	}
}

color ray_color(const ray& r) {
	double t = hit_sphere(point3(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
	}

	vec3 unit_direction = unit_vector(r.direction());
	double a = 0.5 * (unit_direction.y() + 1.0); // Turn from -1 to 1 into 0 to 1
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
	// Image
	double aspect_ratio = 16.0 / 9.0;
	int image_width = 400;

	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Camera
	double focal_length = 1.0;
	double viewport_height = 2.0;
	double viewport_width = viewport_height * (double(image_width) / image_height);
	point3 camera_center = point3(0, 0, 0);

	// Viewport vectors
	vec3 viewport_u = vec3(viewport_width, 0, 0);
	vec3 viewport_v = vec3(0, -viewport_height, 0);

	// Pixel delta vectors
	vec3 pixel_delta_u = viewport_u / image_width;
	vec3 pixel_delta_v = viewport_v / image_height;

	// Upper left pixel location
	vec3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
	vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	// Render

	std::ofstream Output("image.ppm");

	Output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = 0; j < image_height; j++) {
		std::clog << "\rScanlines remaining: " << (image_height - 1 - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			vec3 ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r);
			write_color(Output, pixel_color);
		}
	}

	Output.close();
	std::clog << "Done \n";
}