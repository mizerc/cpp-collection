#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <stdint.h>
#include <math.h>

// r,g,b => 0xRRGGBBAA
static inline uint32_t packRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
{
	return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

static inline uint32_t lerpColor(uint32_t a, uint32_t b, float t)
{
	// linear blend in RGB (cheap)
	uint8_t ar = (a >> 16) & 255, ag = (a >> 8) & 255, ab = a & 255;
	uint8_t br = (b >> 16) & 255, bg = (b >> 8) & 255, bb = b & 255;
	uint8_t rr = (uint8_t)(ar + t * (br - ar));
	uint8_t rg = (uint8_t)(ag + t * (bg - ag));
	uint8_t rb = (uint8_t)(ab + t * (bb - ab));
	return packRGB(rr, rg, rb);
}

static inline float clamp01(float v)
{
	return v < 0.f ? 0.f : (v > 1.f ? 1.f : v);
}

static inline float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

static inline float smooth(float t)
{
	// Cheap smoothstep: 3t^2 - 2t^3
	return t * t * (3.f - 2.f * t);
}

// Rotate (x,z) around Y by angle a (radians)
static inline void rotateY(float *x, float *y, float *z, float a)
{
	(void)y; // y unchanged for Y-rotation
	float c = cosf(a), s = sinf(a);
	float x0 = *x, z0 = *z;
	*x = c * x0 + s * z0;
	*z = -s * x0 + c * z0;
}

// Returns 1 if pixel hits the sphere, and outputs unit normal (nx,ny,nz)
static inline int pixelToSphereNormal(int px, int py, int cx, int cy, int R, float *nx, float *ny, float *nz)
{
	// px, py => current pixel coordinates
	// cx,cy => current sphere center point in pixel coord
	// R => sphere radius in pixel coord

	// Distance from pixel to sphere center
	float dx = (float)(px - cx);
	float dy = (float)(py - cy);
	float rr = (float)(R * R);
	float d2 = dx * dx + dy * dy;
	if(d2 > rr) return 0; // outside sphere

	float dz = sqrtf(rr - d2); // front hemisphere

	// Normalize to unit sphere
	float invR = 1.0f / (float)R;
	*nx = dx * invR;
	*ny = dy * invR;
	*nz = dz * invR;
	return 1;
}

#endif /* MATH_UTILS_H */
