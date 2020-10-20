#include <math.h>

#include "internal/basecommon.h"

#ifdef ENABLE_SIMD
#ifdef ENABLE_AVX
#include "immintrin.h" // AVX
#else
#include "emmintrin.h" // SSE
#endif // ENABLE_AVX
#endif // ENABLE_SIMD

#include "math_internal.h"

void bezier2(double pct, double **pts, int is3D, double *dst)
{
    double pct_inv = 1 - pct;
#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE

    double pctArray[] = {pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv};

    __m128d pctArray_reg = _mm_loadu_pd(pctArray);
    __m128d pct_inv_array_reg = _mm_loadu_pd(pct_inv_array);
    __m128d ctrl0_reg = _mm_loadu_pd(pts[0]);
    __m128d ctrl1_reg = _mm_loadu_pd(pts[1]);

    __m128d tmp_reg = _mm_mul_pd(pct_inv_array_reg, ctrl0_reg);
    __m128d res_reg = _mm_mul_pd(pctArray_reg, ctrl1_reg);
    res_reg = _mm_add_pd(tmp_reg, res_reg);
    _mm_storeu_pd(dst, res_reg);

    if (is3D)
    {
        dst[2] = pct_inv * pts[0][2] + pct * pts[1][2];
    }
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    double pctArray[] = {pct, pct, pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv, pct_inv, pct_inv};

    __m256d pctArray_reg = _mm256_loadu_pd(pctArray);
    __m256d pct_inv_array_reg = _mm256_loadu_pd(pct_inv_array);
    __m256d ctrl0_reg = _mm256_loadu_pd(pts[0]);
    __m256d ctrl1_reg = _mm256_loadu_pd(pts[1]);

    __m256d tmp_reg = _mm256_mul_pd(pct_inv_array_reg, ctrl0_reg);
    __m256d res_reg = _mm256_mul_pd(pctArray_reg, ctrl1_reg);
    res_reg = _mm256_add_pd(tmp_reg, res_reg);
    _mm256_storeu_pd(dst, res_reg);

    if (!is3D)
    {
        dst[2] = 0.;
    }
#else // pure c
    dst[0] = pct_inv * pts[0][0] + pct * pts[1][0];
    dst[1] = pct_inv * pts[0][1] + pct * pts[1][1];
    if (is3D)
    {
        dst[2] = pct_inv * pts[0][2] + pct * pts[1][2];
    }
#endif
}

void bezier3(double pct, double **pts, int is3D, double *dst)
{
    double pct_inv = 1 - pct;
#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
    double pctArray[] = {pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv};
    double twoArray[] = {2., 2.};

    __m128d pctArray_reg = _mm_loadu_pd(pctArray);
    __m128d pct_inv_array_reg = _mm_loadu_pd(pct_inv_array);
    __m128d twoArray_reg = _mm_loadu_pd(twoArray);
    __m128d ctrl0_reg = _mm_loadu_pd(pts[0]);
    __m128d ctrl1_reg = _mm_loadu_pd(pts[1]);
    __m128d ctrl2_reg = _mm_loadu_pd(pts[2]);

    __m128d res_reg = _mm_mul_pd(pct_inv_array_reg, pct_inv_array_reg);
    res_reg = _mm_mul_pd(pct_inv_array_reg, ctrl0_reg);

    __m128d tmp_reg = _mm_mul_pd(twoArray_reg, pct_inv_array_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm_mul_pd(pctArray_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);
    _mm_storeu_pd(dst, res_reg);

    if (is3D)
    {
        dst[2] = pct_inv * pct_inv * pts[0][2];
        dst[2] += (2. * pct_inv * pct * pts[1][2]);
        dst[2] += (pct * pct * pts[2][2]);
    }
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    double pctArray[] = {pct, pct, pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv, pct_inv, pct_inv};
    double twoArray[] = {2., 2., 2., 2.};

    __m256d pctArray_reg = _mm256_loadu_pd(pctArray);
    __m256d pct_inv_array_reg = _mm256_loadu_pd(pct_inv_array);
    __m256d twoArray_reg = _mm256_loadu_pd(twoArray);
    __m256d ctrl0_reg = _mm256_loadu_pd(pts[0]);
    __m256d ctrl1_reg = _mm256_loadu_pd(pts[1]);
    __m256d ctrl2_reg = _mm256_loadu_pd(pts[2]);

    __m256d res_reg = _mm256_mul_pd(pct_inv_array_reg, pct_inv_array_reg);
    res_reg = _mm256_mul_pd(pct_inv_array_reg, ctrl0_reg);

    __m256d tmp_reg = _mm256_mul_pd(twoArray_reg, pct_inv_array_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm256_mul_pd(pctArray_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);
    _mm256_storeu_pd(dst, res_reg);

    if (!is3D)
    {
        dst[2] = 0.;
    }
#else
    dst[0] = pct_inv * pct_inv * pts[0][0];
    dst[0] += (2. * pct_inv * pct * pts[1][0]);
    dst[0] += (pct * pct * pts[2][0]);

    dst[1] = pct_inv * pct_inv * pts[0][1];
    dst[1] += (2. * pct_inv * pct * pts[1][1]);
    dst[1] += (pct * pct * pts[2][1]);

    if (is3D)
    {
        dst[2] = pct_inv * pct_inv * pts[0][2];
        dst[2] += (2. * pct_inv * pct * pts[1][2]);
        dst[2] += (pct * pct * pts[2][2]);
    }
#endif
}

void bezier4(double pct, double **pts, int is3D, double *dst)
{
    double pct_inv = 1 - pct;
#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
    double pctArray[] = {pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv};
    double threeArray[] = {3., 3.};

    __m128d pctArray_reg = _mm_loadu_pd(pctArray);
    __m128d pct_inv_array_reg = _mm_loadu_pd(pct_inv_array);
    __m128d threeArray_reg = _mm_loadu_pd(threeArray);
    __m128d ctrl0_reg = _mm_loadu_pd(pts[0]);
    __m128d ctrl1_reg = _mm_loadu_pd(pts[1]);
    __m128d ctrl2_reg = _mm_loadu_pd(pts[2]);
    __m128d ctrl3_reg = _mm_loadu_pd(pts[3]);

    __m128d pct2Array_reg = _mm_mul_pd(pctArray_reg, pctArray_reg);
    __m128d pct_inv2_array_reg =
            _mm_mul_pd(pct_inv_array_reg, pct_inv_array_reg);

    __m128d res_reg = _mm_mul_pd(pct_inv2_array_reg, pct_inv_array_reg);
    res_reg = _mm_mul_pd(res_reg, ctrl0_reg);

    __m128d tmp_reg = _mm_mul_pd(threeArray_reg, pct_inv2_array_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm_mul_pd(threeArray_reg, pct_inv_array_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, pct2Array_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm_mul_pd(pct2Array_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl3_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);
    _mm_storeu_pd(dst, res_reg);

    if (is3D)
    {
        dst[2] = pct_inv * pct_inv * pct_inv * pts[0][2];
        dst[2] += (3. * pct_inv * pct_inv * pct * pts[1][2]);
        dst[2] += (3. * pct_inv * pct * pct * pts[2][2]);
        dst[2] += (pct * pct * pct * pts[3][2]);
    }
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    double pctArray[] = {pct, pct, pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv, pct_inv, pct_inv};
    double threeArray[] = {3., 3., 3., 3.};

    __m256d pctArray_reg = _mm256_loadu_pd(pctArray);
    __m256d pct_inv_array_reg = _mm256_loadu_pd(pct_inv_array);
    __m256d threeArray_reg = _mm256_loadu_pd(threeArray);
    __m256d ctrl0_reg = _mm256_loadu_pd(pts[0]);
    __m256d ctrl1_reg = _mm256_loadu_pd(pts[1]);
    __m256d ctrl2_reg = _mm256_loadu_pd(pts[2]);
    __m256d ctrl3_reg = _mm256_loadu_pd(pts[3]);

    __m256d pct2Array_reg = _mm256_mul_pd(pctArray_reg, pctArray_reg);
    __m256d pct_inv2_array_reg =
            _mm256_mul_pd(pct_inv_array_reg, pct_inv_array_reg);

    __m256d res_reg = _mm256_mul_pd(pct_inv2_array_reg, pct_inv_array_reg);
    res_reg = _mm256_mul_pd(res_reg, ctrl0_reg);

    __m256d tmp_reg = _mm256_mul_pd(threeArray_reg, pct_inv2_array_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm256_mul_pd(threeArray_reg, pct_inv_array_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, pct2Array_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm256_mul_pd(pct2Array_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl3_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);
    _mm256_storeu_pd(dst, res_reg);

    if (!is3D)
    {
        dst[2] = 0.;
    }
#else
    dst[0] = pct_inv * pct_inv * pct_inv * pts[0][0];
    dst[0] += (3.f * pct_inv * pct_inv * pct * pts[1][0]);
    dst[0] += (3.f * pct_inv * pct * pct * pts[2][0]);
    dst[0] += (pct * pct * pct * pts[3][0]);

    dst[1] = pct_inv * pct_inv * pct_inv * pts[0][1];
    dst[1] += (3.f * pct_inv * pct_inv * pct * pts[1][1]);
    dst[1] += (3.f * pct_inv * pct * pct * pts[2][1]);
    dst[1] += (pct * pct * pct * pts[3][1]);

    if (is3D)
    {
        dst[2] = pct_inv * pct_inv * pct_inv * pts[0][2];
        dst[2] += (3. * pct_inv * pct_inv * pct * pts[1][2]);
        dst[2] += (3. * pct_inv * pct * pct * pts[2][2]);
        dst[2] += (pct * pct * pct * pts[3][2]);
    }
#endif
}

void bezierN(double pct, double **pts, size_t ptsCount,
             int is3D, double *dst)
{
    double pct_inv = 1 - pct;
    size_t ni = ptsCount - 1;
    double nd = (double)ni;
    double bern = 0;
    double *pt;

#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
    double retArray[] = {0., 0.};
    __m128d ret_reg = _mm_loadu_pd(retArray);
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    double retArray[] = {0., 0., 0., 0.};
    __m256d ret_reg = _mm256_loadu_pd(retArray);
#endif
    for (size_t i = 0; i <= ni; ++i)
    {
        pt = pts[i];

        // Bernstein polynom
        // Binomial coefficient
        bern = fac(nd) / (fac((double)i) * fac((double)ni - i));
        bern *= pow(pct, (double)i);
        bern += pow(pct_inv, (double)(ni - i));

#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
        double bernArray[] = {bern, bern};
        __m128d bern_reg = _mm_loadu_pd(bernArray);
        __m128d pt_reg = _mm_loadu_pd(pt);

        __m128d tmp_reg = _mm_mul_pd(pt_reg, bern_reg);
        ret_reg = _mm_add_pd(ret_reg, tmp_reg);
        if (is3D)
        {
            dst[2] += (pt[2] * bern);
        }
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
        double bernArray[] = {bern, bern, bern, bern};
        __m256d bern_reg = _mm256_loadu_pd(bernArray);
        __m256d pt_reg = _mm256_loadu_pd(pt);

        __m256d tmp_reg = _mm256_mul_pd(pt_reg, bern_reg);
        ret_reg = _mm256_add_pd(ret_reg, tmp_reg);
#else
        dst[0] += (pt[0] * bern);
        dst[1] += (pt[1] * bern);
        if (is3D)
        {
            dst[2] += (pt[2] * bern);
        }
#endif
    }
#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
    _mm_storeu_pd(dst, ret_reg);
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    _mm256_storeu_pd(dst, ret_reg);
    if (!is3D)
    {
        dst[2] = 0.;
    }
#endif
}

double fac(double n)
{
    double k = 1.;
    for (double i = 2.; i <= n; ++i)
    {
        k *= i;
    }

    return k;
}
