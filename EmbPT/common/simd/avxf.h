// ======================================================================== //
// Copyright 2009-2011 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#ifndef __EMBREE_AVXF_H__
#define __EMBREE_AVXF_H__

namespace embree
{
  /*! 8-wide AVX float type. */
  struct avxf
  {
    union { __m256 m256; float v[8]; };

    ////////////////////////////////////////////////////////////////////////////////
    /// Constructors, Assignment & Cast Operators
    ////////////////////////////////////////////////////////////////////////////////

    typedef avxb Mask;

    __forceinline avxf           ( ) {}
    __forceinline avxf           ( const avxf& other ) { m256 = other.m256; }
    __forceinline avxf& operator=( const avxf& other ) { m256 = other.m256; return *this; }

    __forceinline          avxf( const __m256  a ) : m256(a) {}
    __forceinline explicit avxf( const __m256i a ) : m256(_mm256_cvtepi32_ps(a)) {}

    __forceinline explicit avxf( const float* const a ) : m256(_mm256_loadu_ps(a)) {}
    __forceinline          avxf( const float&       a ) : m256(_mm256_broadcast_ss(&a)) {}

    __forceinline explicit avxf( const ssef& a ) : m256(_mm256_insertf128_ps(_mm256_castps128_ps256(a),a,1)) {}
    __forceinline avxf( const ssef& a, const ssef& b ) : m256(_mm256_insertf128_ps(_mm256_castps128_ps256(a),b,1)) {}
    __forceinline avxf( float a, float b, float c, float d, float e, float f, float g, float h ) : m256(_mm256_set_ps(h, g, f, e, d, c, b, a)) {}

    __forceinline operator const __m256&( void ) const { return m256; }
    __forceinline operator       __m256&( void )       { return m256; }
    __forceinline operator         ssef ( void ) const { return ssef(_mm256_castps256_ps128(m256)); }

    ////////////////////////////////////////////////////////////////////////////////
    /// Constants
    ////////////////////////////////////////////////////////////////////////////////

    __forceinline avxf( ZeroTy   ) : m256(_mm256_setzero_ps()) {}
    __forceinline avxf( OneTy    ) : m256(_mm256_set1_ps(1.0f)) {}
    __forceinline avxf( PosInfTy ) : m256(_mm256_set1_ps(pos_inf)) {}
    __forceinline avxf( NegInfTy ) : m256(_mm256_set1_ps(neg_inf)) {}
    __forceinline avxf( StepTy   ) : m256(_mm256_set_ps(7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f)) {}

    ////////////////////////////////////////////////////////////////////////////////
    /// Properties
    ////////////////////////////////////////////////////////////////////////////////

    __forceinline const float& operator []( const size_t index ) const { assert(index < 8); return v[index]; }
    __forceinline       float& operator []( const size_t index )       { assert(index < 8); return v[index]; }
  };


  ////////////////////////////////////////////////////////////////////////////////
  /// Unary Operators
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const avxf operator +( const avxf& a ) { return a; }
  __forceinline const avxf operator -( const avxf& a ) { const __m256 mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000)); return _mm256_xor_ps(a.m256, mask); }
  __forceinline const avxf abs  ( const avxf& a ) { const __m256 mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff)); return _mm256_and_ps(a.m256, mask); }
  __forceinline const avxf sign ( const avxf& a ) { return _mm256_blendv_ps(avxf(one), -avxf(one), _mm256_cmp_ps(a, avxf(zero), _CMP_NGE_UQ )); }
  __forceinline const avxf rcp  ( const avxf& a ) { const avxf r = _mm256_rcp_ps(a.m256); return _mm256_sub_ps(_mm256_add_ps(r, r), _mm256_mul_ps(_mm256_mul_ps(r, r), a)); }
  __forceinline const avxf sqrt ( const avxf& a ) { return _mm256_sqrt_ps(a.m256); }
  __forceinline const avxf sqr  ( const avxf& a ) { return _mm256_mul_ps(a,a); }
  __forceinline const avxf rsqrt( const avxf& a ) { const avxf r = _mm256_rsqrt_ps(a.m256); return _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(1.5f), r), _mm256_mul_ps(_mm256_mul_ps(_mm256_mul_ps(a, _mm256_set1_ps(-0.5f)), r), _mm256_mul_ps(r, r))); }


  ////////////////////////////////////////////////////////////////////////////////
  /// Binary Operators
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const avxf operator +( const avxf& a, const avxf& b ) { return _mm256_add_ps(a.m256, b.m256); }
  __forceinline const avxf operator +( const avxf& a, const float b ) { return a + avxf(b); }
  __forceinline const avxf operator +( const float a, const avxf& b ) { return avxf(a) + b; }

  __forceinline const avxf operator -( const avxf& a, const avxf& b ) { return _mm256_sub_ps(a.m256, b.m256); }
  __forceinline const avxf operator -( const avxf& a, const float b ) { return a - avxf(b); }
  __forceinline const avxf operator -( const float a, const avxf& b ) { return avxf(a) - b; }

  __forceinline const avxf operator *( const avxf& a, const avxf& b ) { return _mm256_mul_ps(a.m256, b.m256); }
  __forceinline const avxf operator *( const avxf& a, const float b ) { return a * avxf(b); }
  __forceinline const avxf operator *( const float a, const avxf& b ) { return avxf(a) * b; }

  __forceinline const avxf operator /( const avxf& a, const avxf& b ) { return a * rcp(b); }
  __forceinline const avxf operator /( const avxf& a, const float b ) { return a * rcp(b); }
  __forceinline const avxf operator /( const float a, const avxf& b ) { return a * rcp(b); }

  __forceinline const avxf min( const avxf& a, const avxf& b ) { return _mm256_min_ps(a.m256, b.m256); }
  __forceinline const avxf min( const avxf& a, const float b ) { return _mm256_min_ps(a.m256, avxf(b)); }
  __forceinline const avxf min( const float a, const avxf& b ) { return _mm256_min_ps(avxf(a), b.m256); }

  __forceinline const avxf max( const avxf& a, const avxf& b ) { return _mm256_max_ps(a.m256, b.m256); }
  __forceinline const avxf max( const avxf& a, const float b ) { return _mm256_max_ps(a.m256, avxf(b)); }
  __forceinline const avxf max( const float a, const avxf& b ) { return _mm256_max_ps(avxf(a), b.m256); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Assignment Operators
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline avxf& operator +=( avxf& a, const avxf& b ) { return a = a + b; }
  __forceinline avxf& operator +=( avxf& a, const float b ) { return a = a + b; }

  __forceinline avxf& operator -=( avxf& a, const avxf& b ) { return a = a - b; }
  __forceinline avxf& operator -=( avxf& a, const float b ) { return a = a - b; }

  __forceinline avxf& operator *=( avxf& a, const avxf& b ) { return a = a * b; }
  __forceinline avxf& operator *=( avxf& a, const float b ) { return a = a * b; }

  __forceinline avxf& operator /=( avxf& a, const avxf& b ) { return a = a / b; }
  __forceinline avxf& operator /=( avxf& a, const float b ) { return a = a / b; }


  ////////////////////////////////////////////////////////////////////////////////
  /// Comparison Operators + Select
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const avxb operator ==( const avxf& a, const avxf& b ) { return _mm256_cmp_ps(a.m256, b.m256, _CMP_EQ_UQ ); }
  __forceinline const avxb operator < ( const avxf& a, const avxf& b ) { return _mm256_cmp_ps(a.m256, b.m256, _CMP_NGE_UQ ); }
  __forceinline const avxb operator <=( const avxf& a, const avxf& b ) { return _mm256_cmp_ps(a.m256, b.m256, _CMP_NGT_UQ ); }
  __forceinline const avxb operator !=( const avxf& a, const avxf& b ) { return _mm256_cmp_ps(a.m256, b.m256, _CMP_NEQ_UQ); }
  __forceinline const avxb operator >=( const avxf& a, const avxf& b ) { return _mm256_cmp_ps(a.m256, b.m256, _CMP_NLT_UQ); }
  __forceinline const avxb operator > ( const avxf& a, const avxf& b ) { return _mm256_cmp_ps(a.m256, b.m256, _CMP_NLE_UQ); }

  __forceinline const avxb operator ==( const avxf& a, const float b ) { return _mm256_cmp_ps(a.m256, avxf(b), _CMP_EQ_UQ ); }
  __forceinline const avxb operator < ( const avxf& a, const float b ) { return _mm256_cmp_ps(a.m256, avxf(b), _CMP_NGE_UQ ); }
  __forceinline const avxb operator <=( const avxf& a, const float b ) { return _mm256_cmp_ps(a.m256, avxf(b), _CMP_NGT_UQ ); }
  __forceinline const avxb operator !=( const avxf& a, const float b ) { return _mm256_cmp_ps(a.m256, avxf(b), _CMP_NEQ_UQ); }
  __forceinline const avxb operator >=( const avxf& a, const float b ) { return _mm256_cmp_ps(a.m256, avxf(b), _CMP_NLT_UQ); }
  __forceinline const avxb operator > ( const avxf& a, const float b ) { return _mm256_cmp_ps(a.m256, avxf(b), _CMP_NLE_UQ); }

  __forceinline const avxb operator ==( const float a, const avxf& b ) { return _mm256_cmp_ps(avxf(a), b.m256, _CMP_EQ_UQ ); }
  __forceinline const avxb operator < ( const float a, const avxf& b ) { return _mm256_cmp_ps(avxf(a), b.m256, _CMP_NGE_UQ ); }
  __forceinline const avxb operator <=( const float a, const avxf& b ) { return _mm256_cmp_ps(avxf(a), b.m256, _CMP_NGT_UQ ); }
  __forceinline const avxb operator !=( const float a, const avxf& b ) { return _mm256_cmp_ps(avxf(a), b.m256, _CMP_NEQ_UQ); }
  __forceinline const avxb operator >=( const float a, const avxf& b ) { return _mm256_cmp_ps(avxf(a), b.m256, _CMP_NLT_UQ); }
  __forceinline const avxb operator > ( const float a, const avxf& b ) { return _mm256_cmp_ps(avxf(a), b.m256, _CMP_NLE_UQ); }

  __forceinline const avxf select( const avxb& mask, const avxf& t, const avxf& f ) { return _mm256_blendv_ps(f, t, mask); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Rounding Functions
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const avxf round_even( const avxf& a ) { return _mm256_round_ps(a, _MM_FROUND_TO_NEAREST_INT); }
  __forceinline const avxf round_down( const avxf& a ) { return _mm256_round_ps(a, _MM_FROUND_TO_NEG_INF    ); }
  __forceinline const avxf round_up  ( const avxf& a ) { return _mm256_round_ps(a, _MM_FROUND_TO_POS_INF    ); }
  __forceinline const avxf round_zero( const avxf& a ) { return _mm256_round_ps(a, _MM_FROUND_TO_ZERO       ); }

  __forceinline const avxf floor( const avxf& a ) { return _mm256_round_ps(a, _MM_FROUND_TO_NEG_INF    ); }
  __forceinline const avxf ceil ( const avxf& a ) { return _mm256_round_ps(a, _MM_FROUND_TO_POS_INF    ); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Movement/Shifting/Shuffling Functions
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline const avxf broadcast(const float* ptr) { return _mm256_broadcast_ss(ptr); }
  template<ssize_t index> __forceinline const avxf insert (const avxf& a, const ssef& b) { return _mm256_insertf128_ps (a,b,index); }
  template<ssize_t index> __forceinline const ssef extract(const avxf& a               ) { return _mm256_extractf128_ps(a  ,index); }

  __forceinline avxf unpacklo( const avxf& a, const avxf& b ) { return _mm256_unpacklo_ps(a.m256, b.m256); }
  __forceinline avxf unpackhi( const avxf& a, const avxf& b ) { return _mm256_unpackhi_ps(a.m256, b.m256); }

  template<ssize_t index> __forceinline const avxf shuffle( const avxf& a ) {
    return _mm256_permute_ps(a, _MM_SHUFFLE(index, index, index, index));
  }

  template<ssize_t index_0, ssize_t index_1> __forceinline const avxf shuffle( const avxf& a ) {
    return _mm256_permute2f128_ps(a, a, (index_1 << 4) | (index_0 << 0));
  }

  template<ssize_t index_0, ssize_t index_1> __forceinline const avxf shuffle( const avxf& a,  const avxf& b) {
    return _mm256_permute2f128_ps(a, b, (index_1 << 4) | (index_0 << 0));
  }

  template<ssize_t index_0, ssize_t index_1, ssize_t index_2, ssize_t index_3> __forceinline const avxf shuffle( const avxf& a ) {
    return _mm256_permute_ps(a, _MM_SHUFFLE(index_3, index_2, index_1, index_0));
  }

  template<ssize_t index_0, ssize_t index_1, ssize_t index_2, ssize_t index_3> __forceinline const avxf shuffle( const avxf& a, const avxf& b ) {
    return _mm256_shuffle_ps(a, b, _MM_SHUFFLE(index_3, index_2, index_1, index_0));
  }

  template<> __forceinline const avxf shuffle<0, 0, 2, 2>( const avxf& b ) { return _mm256_moveldup_ps(b); }
  template<> __forceinline const avxf shuffle<1, 1, 3, 3>( const avxf& b ) { return _mm256_movehdup_ps(b); }
  template<> __forceinline const avxf shuffle<0, 1, 0, 1>( const avxf& b ) { return _mm256_castpd_ps(_mm256_movedup_pd(_mm256_castps_pd(b))); }

  __forceinline void transpose4(const avxf& r0, const avxf& r1, const avxf& r2, const avxf& r3, avxf& c0, avxf& c1, avxf& c2, avxf& c3)
  {
    avxf l02 = unpacklo(r0,r2);
    avxf h02 = unpackhi(r0,r2);
    avxf l13 = unpacklo(r1,r3);
    avxf h13 = unpackhi(r1,r3);
    c0 = unpacklo(l02,l13);
    c1 = unpackhi(l02,l13);
    c2 = unpacklo(h02,h13);
    c3 = unpackhi(h02,h13);
  }

  __forceinline void transpose(const avxf& r0, const avxf& r1, const avxf& r2, const avxf& r3, const avxf& r4, const avxf& r5, const avxf& r6, const avxf& r7,
                               avxf& c0, avxf& c1, avxf& c2, avxf& c3, avxf& c4, avxf& c5, avxf& c6, avxf& c7)
  {
    avxf h0,h1,h2,h3; transpose4(r0,r1,r2,r3,h0,h1,h2,h3);
    avxf h4,h5,h6,h7; transpose4(r4,r5,r6,r7,h4,h5,h6,h7);
    c0 = shuffle<0,2>(h0,h4);
    c1 = shuffle<0,2>(h1,h5);
    c2 = shuffle<0,2>(h2,h6);
    c3 = shuffle<0,2>(h3,h7);
    c4 = shuffle<1,3>(h0,h4);
    c5 = shuffle<1,3>(h1,h5);
    c6 = shuffle<1,3>(h2,h6);
    c7 = shuffle<1,3>(h3,h7);
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Reductions
  ////////////////////////////////////////////////////////////////////////////////

  __forceinline avxf reduce_min2(const avxf& v) { return min(v,shuffle<1,0,3,2>(v)); }
  __forceinline avxf reduce_min4(const avxf& v) { avxf v1 = reduce_min2(v); return min(v1,shuffle<2,3,0,1>(v1)); }
  __forceinline avxf reduce_min (const avxf& v) { avxf v1 = reduce_min4(v); return min(v1,shuffle<1,0>(v1)); }

  __forceinline avxf reduce_max2(const avxf& v) { return max(v,shuffle<1,0,3,2>(v)); }
  __forceinline avxf reduce_max4(const avxf& v) { avxf v1 = reduce_max2(v); return max(v1,shuffle<2,3,0,1>(v1)); }
  __forceinline avxf reduce_max (const avxf& v) { avxf v1 = reduce_max4(v); return max(v1,shuffle<1,0>(v1)); }

  ////////////////////////////////////////////////////////////////////////////////
  /// Output Operators
  ////////////////////////////////////////////////////////////////////////////////

  inline std::ostream& operator<<(std::ostream& cout, const avxf& a) {
    return cout << "<" << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << a[4] << ", " << a[5] << ", " << a[6] << ", " << a[7] << ">";
  }
}

#endif