#ifndef CHEAP_NOISE3D_H
#define CHEAP_NOISE3D_H

#include <stdint.h>
#include <math.h>

static inline uint32_t hash3i(int x, int y, int z, uint32_t seed)
{
	// Fast integer hash (good enough for noise)
	uint32_t h = seed;
	h ^= (uint32_t)x * 0x9E3779B1u;
	h ^= (uint32_t)y * 0x85EBCA77u;
	h ^= (uint32_t)z * 0xC2B2AE3Du;
	h ^= h >> 16;
	h *= 0x7FEB352Du;
	h ^= h >> 15;
	h *= 0x846CA68Bu;
	h ^= h >> 16;
	return h;
}

static inline float rand01_from_hash(uint32_t h)
{
	// 24-bit mantissa-ish -> [0,1)
	return (float)(h & 0x00FFFFFFu) * (1.0f / 16777216.0f);
}

/* -------------------------------------------------------
 * 3D Value Noise
 * ------------------------------------------------------- */

// Returns approximately [-1, 1]
static inline float valueNoise3D(float x, float y, float z, uint32_t seed)
{
	int x0 = (int)floorf(x);
	int y0 = (int)floorf(y);
	int z0 = (int)floorf(z);

	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;

	float fx = x - (float)x0;
	float fy = y - (float)y0;
	float fz = z - (float)z0;

	float u = smooth(fx);
	float v = smooth(fy);
	float w = smooth(fz);

	float c000 = rand01_from_hash(hash3i(x0, y0, z0, seed)) * 2.f - 1.f;
	float c100 = rand01_from_hash(hash3i(x1, y0, z0, seed)) * 2.f - 1.f;
	float c010 = rand01_from_hash(hash3i(x0, y1, z0, seed)) * 2.f - 1.f;
	float c110 = rand01_from_hash(hash3i(x1, y1, z0, seed)) * 2.f - 1.f;

	float c001 = rand01_from_hash(hash3i(x0, y0, z1, seed)) * 2.f - 1.f;
	float c101 = rand01_from_hash(hash3i(x1, y0, z1, seed)) * 2.f - 1.f;
	float c011 = rand01_from_hash(hash3i(x0, y1, z1, seed)) * 2.f - 1.f;
	float c111 = rand01_from_hash(hash3i(x1, y1, z1, seed)) * 2.f - 1.f;

	float x00 = lerp(c000, c100, u);
	float x10 = lerp(c010, c110, u);
	float x01 = lerp(c001, c101, u);
	float x11 = lerp(c011, c111, u);

	float y0v = lerp(x00, x10, v);
	float y1v = lerp(x01, x11, v);

	return lerp(y0v, y1v, w);
}

/* -------------------------------------------------------
 * Fractal Brownian Motion (FBM)
 * ------------------------------------------------------- */

// Returns approximately [-1, 1]
static inline float fbm3D(float x, float y, float z, uint32_t seed)
{
	float sum = 0.f;
	float amp = 0.5f;
	float freq = 1.f;

	// 4 octaves = good quality / cheap cost
	for(int i = 0; i < 4; ++i) {
		sum += amp * valueNoise3D(x * freq, y * freq, z * freq, seed + (uint32_t)i * 1013u);
		freq *= 2.0f;
		amp *= 0.5f;
	}
	return sum;
}

#endif /* CHEAP_NOISE3D_H */
