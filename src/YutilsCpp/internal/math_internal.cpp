/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
*
*    SubFX is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation, either version 2.1
*    of the License, or (at your option) any later version.
*
*    SubFX is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General
*    Public License along with SubFX. If not, see
*    <http://www.gnu.org/licenses/>.
*/

#include "math_internal.hpp"

#include <cmath>

#ifdef ENABLE_SIMD
#ifdef ENABLE_AVX
#include "immintrin.h" // AVX
#else
#include "emmintrin.h" // SSE
#endif // ENABLE_AVX
#endif // ENABLE_SIMD

using namespace PROJ_NAMESPACE::Yutils;

std::tuple<double, double, double>
Math_Internal::bezier2(double pct,
                       std::vector<std::tuple<double, double, double>> &pts,
                       bool is3D) NOTHROW
{
    double pct_inv(1 - pct);
#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE

    double pctArray[] = {pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv};

    double ctrl0[] = {std::get<0>(pts.at(0)), std::get<1>(pts.at(0))};
    double ctrl1[] = {std::get<0>(pts.at(1)), std::get<1>(pts.at(1))};

    __m128d pctArray_reg(_mm_loadu_pd(pctArray));
    __m128d pct_inv_array_reg(_mm_loadu_pd(pct_inv_array));
    __m128d ctrl0_reg(_mm_loadu_pd(ctrl0));
    __m128d ctrl1_reg(_mm_loadu_pd(ctrl1));

    __m128d tmp_reg(_mm_mul_pd(pct_inv_array_reg, ctrl0_reg));
    __m128d res_reg(_mm_mul_pd(pctArray_reg, ctrl1_reg));
    res_reg = _mm_add_pd(tmp_reg, res_reg);
    _mm_storeu_pd(ctrl0, res_reg);

    if (is3D)
    {
        return std::make_tuple(
                ctrl0[0],
                ctrl0[1],
                pct_inv * std::get<2>(pts.at(0)) + pct * std::get<2>(pts.at(1)));
    }

    return std::make_tuple(ctrl0[0], ctrl0[1], 0.);
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    double pctArray[] = {pct, pct, pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv, pct_inv, pct_inv};
    double ctrl0[] = {std::get<0>(pts.at(0)), std::get<1>(pts.at(0)),
                      std::get<2>(pts.at(0)), 0};
    double ctrl1[] = {std::get<0>(pts.at(1)), std::get<1>(pts.at(1)),
                      std::get<2>(pts.at(1)), 0};

    __m256d pctArray_reg(_mm256_loadu_pd(pctArray));
    __m256d pct_inv_array_reg(_mm256_loadu_pd(pct_inv_array));
    __m256d ctrl0_reg(_mm256_loadu_pd(ctrl0));
    __m256d ctrl1_reg(_mm256_loadu_pd(ctrl1));

    __m256d tmp_reg(_mm256_mul_pd(pct_inv_array_reg, ctrl0_reg));
    __m256d res_reg(_mm256_mul_pd(pctArray_reg, ctrl1_reg));
    res_reg = _mm256_add_pd(tmp_reg, res_reg);
    _mm256_storeu_pd(ctrl0, res_reg);

    if (is3D)
    {
        return std::make_tuple(ctrl0[0], ctrl0[1], ctrl0[2]);
    }

    return std::make_tuple(ctrl0[0], ctrl0[1], 0.);
#else // pure c++
    double pts1(0.f), pts2(0.f), pts3(0.f);

    pts1 = pct_inv * std::get<0>(pts.at(0)) + pct * std::get<0>(pts.at(1));
    pts2 = pct_inv * std::get<1>(pts.at(0)) + pct * std::get<1>(pts.at(1));
    if (is3D)
    {
        pts3 = pct_inv * std::get<2>(pts.at(0)) + pct * std::get<2>(pts.at(1));
    }

    return std::make_tuple(pts1, pts2, pts3);
#endif
}

std::tuple<double, double, double>
Math_Internal::bezier3(double pct,
                       std::vector<std::tuple<double, double, double>> &pts,
                       bool is3D) NOTHROW
{
    double pct_inv(1 - pct);
#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
    double pctArray[] = {pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv};
    double twoArray[] = {2., 2.};

    double ctrl0[] = {std::get<0>(pts.at(0)), std::get<1>(pts.at(0))};
    double ctrl1[] = {std::get<0>(pts.at(1)), std::get<1>(pts.at(1))};
    double ctrl2[] = {std::get<0>(pts.at(2)), std::get<1>(pts.at(2))};

    __m128d pctArray_reg(_mm_loadu_pd(pctArray));
    __m128d pct_inv_array_reg(_mm_loadu_pd(pct_inv_array));
    __m128d twoArray_reg(_mm_loadu_pd(twoArray));
    __m128d ctrl0_reg(_mm_loadu_pd(ctrl0));
    __m128d ctrl1_reg(_mm_loadu_pd(ctrl1));
    __m128d ctrl2_reg(_mm_loadu_pd(ctrl2));

    __m128d res_reg(_mm_mul_pd(pct_inv_array_reg, pct_inv_array_reg));
    res_reg = _mm_mul_pd(pct_inv_array_reg, ctrl0_reg);

    __m128d tmp_reg(_mm_mul_pd(twoArray_reg, pct_inv_array_reg));
    tmp_reg = _mm_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm_mul_pd(pctArray_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);
    _mm_storeu_pd(ctrl0, res_reg);

    if (is3D)
    {
        double pts3 = pct_inv * pct_inv * std::get<2>(pts.at(0));
        pts3 += (2. * pct_inv * pct * std::get<2>(pts.at(1)));
        pts3 += (pct * pct * std::get<2>(pts.at(2)));
        return std::make_tuple(ctrl0[0], ctrl0[1], pts3);
    }

    return std::make_tuple(ctrl0[0], ctrl0[1], 0.);
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    double pctArray[] = {pct, pct, pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv, pct_inv, pct_inv};
    double twoArray[] = {2., 2., 2., 2.};

    double ctrl0[] = {std::get<0>(pts.at(0)), std::get<1>(pts.at(0)),
                      std::get<2>(pts.at(0)), 0};
    double ctrl1[] = {std::get<0>(pts.at(1)), std::get<1>(pts.at(1)),
                      std::get<2>(pts.at(1)), 0};
    double ctrl2[] = {std::get<0>(pts.at(2)), std::get<1>(pts.at(2)),
                      std::get<2>(pts.at(2)), 0};

    __m256d pctArray_reg(_mm256_loadu_pd(pctArray));
    __m256d pct_inv_array_reg(_mm256_loadu_pd(pct_inv_array));
    __m256d twoArray_reg(_mm256_loadu_pd(twoArray));
    __m256d ctrl0_reg(_mm256_loadu_pd(ctrl0));
    __m256d ctrl1_reg(_mm256_loadu_pd(ctrl1));
    __m256d ctrl2_reg(_mm256_loadu_pd(ctrl2));

    __m256d res_reg(_mm256_mul_pd(pct_inv_array_reg, pct_inv_array_reg));
    res_reg = _mm256_mul_pd(pct_inv_array_reg, ctrl0_reg);

    __m256d tmp_reg(_mm256_mul_pd(twoArray_reg, pct_inv_array_reg));
    tmp_reg = _mm256_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm256_mul_pd(pctArray_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);
    _mm256_storeu_pd(ctrl0, res_reg);

    if (is3D)
    {
        return std::make_tuple(ctrl0[0], ctrl0[1], ctrl0[2]);
    }

    return std::make_tuple(ctrl0[0], ctrl0[1], 0.);
#else
    double pts1(0.f), pts2(0.f), pts3(0.f);

    pts1 = pct_inv * pct_inv * std::get<0>(pts.at(0));
    pts1 += (2. * pct_inv * pct * std::get<0>(pts.at(1)));
    pts1 += (pct * pct * std::get<0>(pts.at(2)));

    pts2 = pct_inv * pct_inv * std::get<1>(pts.at(0));
    pts2 += (2. * pct_inv * pct * std::get<1>(pts.at(1)));
    pts2 += (pct * pct * std::get<1>(pts.at(2)));

    if (is3D)
    {
        pts3 = pct_inv * pct_inv * std::get<2>(pts.at(0));
        pts3 += (2. * pct_inv * pct * std::get<2>(pts.at(1)));
        pts3 += (pct * pct * std::get<2>(pts.at(2)));
    }

    return std::make_tuple(pts1, pts2, pts3);
#endif
}

std::tuple<double, double, double>
Math_Internal::bezier4(double pct,
                       std::vector<std::tuple<double, double, double>> &pts,
                       bool is3D) NOTHROW
{
    double pct_inv(1 - pct);
#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
    double pctArray[] = {pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv};
    double threeArray[] = {3., 3.};

    double ctrl0[] = {std::get<0>(pts.at(0)), std::get<1>(pts.at(0))};
    double ctrl1[] = {std::get<0>(pts.at(1)), std::get<1>(pts.at(1))};
    double ctrl2[] = {std::get<0>(pts.at(2)), std::get<1>(pts.at(2))};
    double ctrl3[] = {std::get<0>(pts.at(3)), std::get<1>(pts.at(3))};

    __m128d pctArray_reg(_mm_loadu_pd(pctArray));
    __m128d pct_inv_array_reg(_mm_loadu_pd(pct_inv_array));
    __m128d threeArray_reg(_mm_loadu_pd(threeArray));
    __m128d ctrl0_reg(_mm_loadu_pd(ctrl0));
    __m128d ctrl1_reg(_mm_loadu_pd(ctrl1));
    __m128d ctrl2_reg(_mm_loadu_pd(ctrl2));
    __m128d ctrl3_reg(_mm_loadu_pd(ctrl3));

    __m128d pct2Array_reg(_mm_mul_pd(pctArray_reg, pctArray_reg));
    __m128d pct_inv2_array_reg(_mm_mul_pd(pct_inv_array_reg, pct_inv_array_reg));

    __m128d res_reg(_mm_mul_pd(pct_inv2_array_reg, pct_inv_array_reg));
    res_reg = _mm_mul_pd(res_reg, ctrl0_reg);

    __m128d tmp_reg(_mm_mul_pd(threeArray_reg, pct_inv2_array_reg));
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
    _mm_storeu_pd(ctrl0, res_reg);

    if (is3D)
    {
        double pts3 = pct_inv * pct_inv * pct_inv * std::get<2>(pts.at(0));
        pts3 += (3. * pct_inv * pct_inv * pct * std::get<2>(pts.at(1)));
        pts3 += (3. * pct_inv * pct * pct * std::get<2>(pts.at(2)));
        pts3 += (pct * pct * pct * std::get<2>(pts.at(3)));
        return std::make_tuple(ctrl0[0], ctrl0[1], pts3);
    }

    return std::make_tuple(ctrl0[0], ctrl0[1], 0.);
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    double pctArray[] = {pct, pct, pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv, pct_inv, pct_inv};
    double threeArray[] = {3., 3., 3., 3.};

    double ctrl0[] = {std::get<0>(pts.at(0)), std::get<1>(pts.at(0)),
                      std::get<2>(pts.at(0)), 0};
    double ctrl1[] = {std::get<0>(pts.at(1)), std::get<1>(pts.at(1)),
                      std::get<2>(pts.at(1)), 0};
    double ctrl2[] = {std::get<0>(pts.at(2)), std::get<1>(pts.at(2)),
                      std::get<2>(pts.at(2)), 0};
    double ctrl3[] = {std::get<0>(pts.at(3)), std::get<1>(pts.at(3)),
                      std::get<2>(pts.at(3)), 0};

    __m256d pctArray_reg(_mm256_loadu_pd(pctArray));
    __m256d pct_inv_array_reg(_mm256_loadu_pd(pct_inv_array));
    __m256d threeArray_reg(_mm256_loadu_pd(threeArray));
    __m256d ctrl0_reg(_mm256_loadu_pd(ctrl0));
    __m256d ctrl1_reg(_mm256_loadu_pd(ctrl1));
    __m256d ctrl2_reg(_mm256_loadu_pd(ctrl2));
    __m256d ctrl3_reg(_mm256_loadu_pd(ctrl3));

    __m256d pct2Array_reg(_mm256_mul_pd(pctArray_reg, pctArray_reg));
    __m256d pct_inv2_array_reg(_mm256_mul_pd(pct_inv_array_reg, pct_inv_array_reg));

    __m256d res_reg(_mm256_mul_pd(pct_inv2_array_reg, pct_inv_array_reg));
    res_reg = _mm256_mul_pd(res_reg, ctrl0_reg);

    __m256d tmp_reg(_mm256_mul_pd(threeArray_reg, pct_inv2_array_reg));
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
    _mm256_storeu_pd(ctrl0, res_reg);

    if (is3D)
    {
        return std::make_tuple(ctrl0[0], ctrl0[1], ctrl0[2]);
    }

    return std::make_tuple(ctrl0[0], ctrl0[1], 0.);
#else
    double pts1(0.f), pts2(0.f), pts3(0.f);

    pts1 = pct_inv * pct_inv * pct_inv * std::get<0>(pts.at(0));
    pts1 += (3.f * pct_inv * pct_inv * pct * std::get<0>(pts.at(1)));
    pts1 += (3.f * pct_inv * pct * pct * std::get<0>(pts.at(2)));
    pts1 += (pct * pct * pct * std::get<0>(pts.at(3)));

    pts2 = pct_inv * pct_inv * pct_inv * std::get<1>(pts.at(0));
    pts2 += (3.f * pct_inv * pct_inv * pct * std::get<1>(pts.at(1)));
    pts2 += (3.f * pct_inv * pct * pct * std::get<1>(pts.at(2)));
    pts2 += (pct * pct * pct * std::get<1>(pts.at(3)));

    if (is3D)
    {
        pts3 = pct_inv * pct_inv * pct_inv * std::get<2>(pts.at(0));
        pts3 += (3. * pct_inv * pct_inv * pct * std::get<2>(pts.at(1)));
        pts3 += (3. * pct_inv * pct * pct * std::get<2>(pts.at(2)));
        pts3 += (pct * pct * pct * std::get<2>(pts.at(3)));
    }

    return std::make_tuple(pts1, pts2, pts3);
#endif
}

std::tuple<double, double, double>
Math_Internal::bezierN(double pct,
                       std::vector<std::tuple<double, double, double>> &pts,
                       bool is3D) NOTHROW
{
    double pct_inv(1 - pct);
    size_t ni(pts.size() - 1);
    double nd = static_cast<double>(ni);
    double bern(0);
    std::tuple<double, double, double> pt;

#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
    double ret_z(0.);
    double retArray[] = {0., 0.};
    __m128d ret_reg(_mm_loadu_pd(retArray));
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    double retArray[] = {0., 0., 0., 0.};
    __m256d ret_reg(_mm256_loadu_pd(retArray));
#else
    double ret_x(0.), ret_y(0.), ret_z(0.);
#endif
    for (size_t i = 0; i <= ni; ++i)
    {
        pt = pts.at(i);

        // Bernstein polynom
        // Binomial coefficient
        bern = fac(nd) / (fac(static_cast<double>(i)) * fac(static_cast<double>(ni - i)));
        bern *= pow(pct, static_cast<double>(i));
        bern += pow(pct_inv, static_cast<double>(ni - i));

#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
        double bernArray[] = {bern, bern};
        __m128d bern_reg(_mm_loadu_pd(bernArray));

        double ptArray[] = {std::get<0>(pt), std::get<1>(pt)};
        __m128d pt_reg(_mm_loadu_pd(ptArray));

        __m128d tmp_reg(_mm_mul_pd(pt_reg, bern_reg));
        ret_reg = _mm_add_pd(ret_reg, tmp_reg);
        if (is3D)
        {
            ret_z += (std::get<2>(pt) * bern);
        }
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
        double bernArray[] = {bern, bern, bern, bern};
        __m256d bern_reg(_mm256_loadu_pd(bernArray));

        double ptArray[] = {std::get<0>(pt), std::get<1>(pt),
                            std::get<2>(pt), 0.};
        __m256d pt_reg(_mm256_loadu_pd(ptArray));

        __m256d tmp_reg(_mm256_mul_pd(pt_reg, bern_reg));
        ret_reg = _mm256_add_pd(ret_reg, tmp_reg);
#else
        ret_x += (std::get<0>(pt) * bern);
        ret_y += (std::get<1>(pt) * bern);
        if (is3D)
        {
            ret_z += (std::get<2>(pt) * bern);
        }
#endif
    }
#if defined(ENABLE_SIMD) && !defined(ENABLE_AVX) // SSE
    _mm_storeu_pd(retArray, ret_reg);
    return std::make_tuple(retArray[0], retArray[1], ret_z);
#elif defined(ENABLE_SIMD) && defined(ENABLE_AVX) // AVX
    _mm256_storeu_pd(retArray, ret_reg);
    if (is3D)
    {
        return std::make_tuple(retArray[0], retArray[1], retArray[2]);
    }

    return std::make_tuple(retArray[0], retArray[1], 0.);
#else
    return std::make_tuple(ret_x, ret_y, ret_z);
#endif
}

double Math_Internal::fac(double n) NOTHROW
{
    double k = 1.;
    for (double i = 2.; i <= n; ++i)
    {
        k *= i;
    }

    return k;
}
