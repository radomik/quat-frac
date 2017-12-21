/*
  Copyright (c) 2010-2012, Intel Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.


   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/// Average speed up is now: 3.508 for all files in data folder
/// even with some performance warnings

typedef float<3> float3;
typedef float<4> float4;

typedef int<2> int2;

// NOTE:
// float3 has members in order: .x or .r, .y or .g, .z or .b
// float4 has members in order: .x or .r, .y or .g, .z or .b, .w or .a
//

/// Quaternion square
/// @return q^2
/// @remarks May still optimize this method
inline static float4 quat_sqr(varying float4 q) {
  // from QJuliaFragment.cg by Keenan Crane (kcrane@uiuc.edu)
  //r.w   = q.w*q.w - dot(q.xyz, q.xyz);
  //r.xyz = 2*q.x*q.xyz;

  float4 t = q * q;
  float3 u = 2 * q.w * q.xyz;

  float4 r = {
    u.x, u.y, u.z,
    t.w - t.x - t.y - t.z};
  return r;
}

/// Quaternion multiplication
/// @return p * q
/// @remarks May still optimize this method
inline static float4 quat_mul(varying float4 p, varying float4 q) {
  // definition
  //~ float4 r;
  //~ r.x = (p.w * q.x) +  (p.x * q.w) +  (p.y * q.z) - (p.z * q.y);
  //~ r.y = (p.w * q.y) +  (p.y * q.w) +  (p.z * q.x) - (p.x * q.z);
  //~ r.z = (p.w * q.z) +  (p.z * q.w) +  (p.x * q.y) - (p.y * q.x);
  //~ r.w = (p.w * q.w) -  (p.x * q.x) -  (p.y * q.y) - (p.z * q.z);
  //~
  //~ return r;

  // r = -q1 * q2 + q1 x q2
  //

  // from QJuliaFragment.cg by Keenan Crane (kcrane@uiuc.edu)
  //r.w   = p.w*q.w - dot(p.xyz, q.xyz) // = p.w*q.w - p.x*q.x - p.y*q.y - p.z*q.z;
  //r.xyz = p.w*q.xyz + q.w*p.xyz + cross(p.xyz, q.xyz) = {
  //  p.w*q.x + p.x*q.w + (p.y * q.z) - (p.z * q.y) );
  //  p.w*q.y + p.y*q.w + (p.z * q.x) - (p.x * q.z) );
  //  p.w*q.z + p.z*q.w + (p.x * q.y) - (p.y * q.x) );
  //}

  // t = { p.x*q.x, p.y*q.y, p.z*q.z };
  float4 t = p * q;

  // u = {
  //  p.w*q.x + p.x*q.w + p.y*q.z - p.z*q.y;
  //  p.w*q.y + p.y*q.w + p.z*q.x - p.x*q.z;
  //  p.w*q.z + p.z*q.w + p.x*q.y - p.y*q.x;
  //}
  float3 u = p.w*q.xyz + q.w*p.xyz + p.yzx * q.zxy - p.zxy * q.yzx;

  float4 r = {
    u.x, u.y, u.z,
    t.w - t.x - t.y - t.z
  };

  return r;
}

/// Quaternion norm squared
/// return ||q||^2
/// @remarks May still optimize this method
inline static float quat_len_sqr(varying float4 q) {
  float4 cv = q * q; // dot product
  return cv[0] + cv[1] + cv[2] + cv[3];
}

/// Convert 3D point p to quaternion using quaternion polar representation
/// @return quaternion q :
/// q.w = mod_q * cos(p.x)
/// q.x = mod_q * sin(p.x) * cos(p.y)
/// q.y = mod_q * sin(p.x) * sin(p.y) * cos(p.z)
/// q.z = mod_q * sin(p.x) * sin(p.y) * sin(p.z)
/// @remarks May still optimize this method
inline static float4 vec3d_to_quat_polar(varying float3 p, uniform float mod_q) {
  float sin_fi  = sin(p[0]);
  float sin_psi = sin(p[1]);

  float4 q = {mod_q, mod_q, mod_q, mod_q};

  q[3] *= cos(p[0]);            // q[3] *= cp[0]
  q[0] *= sin_fi * cos(p[1]);       // q[0] *= sp[0] * cp[1]
  q[1] *= sin_fi * sin_psi * cos(p[2]); // q[1] *= sp[0] * sp[1] * cp[2]
  q[2] *= sin_fi * sin_psi * sin(p[2]); // q[2] *= sp[0] * sp[1] * sp[2]

  return q;
}

export void quat_ispc_v1_pow_1(
  uniform float3      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       mod_q,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = vec3d_to_quat_polar(points[i], mod_q);

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    //~ unsigned int8 iter_count = 0;
    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q = q + c
      q += c[0];

      // 2. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}


export void quat_ispc_v2_pow_1(
  uniform float4      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = points[i];

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    //~ unsigned int8 iter_count = 0;
    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q = q + c
      q += c[0];

      // 2. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}

export void quat_ispc_v1_pow_2(
  uniform float3      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       mod_q,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = vec3d_to_quat_polar(points[i], mod_q);

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    //~ unsigned int8 iter_count = 0;
    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q = q2 + c
      q = q2 + c[0];

      // 3. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}


export void quat_ispc_v2_pow_2(
  uniform float4      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = points[i];

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    //~ unsigned int8 iter_count = 0;
    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q = q2 + c
      q = q2 + c[0];

      // 3. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = (it[1] >= iter_min);
    indices[i] = it;
  }
}

export void quat_ispc_v1_pow_3(
  uniform float3      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       mod_q,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = vec3d_to_quat_polar(points[i], mod_q);

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q3 = q2 * q
      float4 q3 = quat_mul(q2, q);

      // 3. compute q = q3 + c
      q = q3 + c[0];

      // 4. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}


export void quat_ispc_v2_pow_3(
  uniform float4      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = points[i];

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q3 = q2 * q
      float4 q3 = quat_mul(q2, q);

      // 3. compute q = q3 + c
      q = q3 + c[0];

      // 4. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}

export void quat_ispc_v1_pow_4(
  uniform float3      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       mod_q,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = vec3d_to_quat_polar(points[i], mod_q);

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q3 = q2^2
      float4 q3 = quat_sqr(q2);

      // 3. compute q = q3 + c
      q = q3 + c[0];

      // 4. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}


export void quat_ispc_v2_pow_4(
  uniform float4      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = points[i];

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q3 = q2^2
      float4 q3 = quat_sqr(q2);

      // 3. compute q = q3 + c
      q = q3 + c[0];

      // 4. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}

export void quat_ispc_v1_pow_5(
  uniform float3      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       mod_q,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = vec3d_to_quat_polar(points[i], mod_q);

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q3 = q2^2
      float4 q3 = quat_sqr(q2);

      // 3. compute q4 = q3 * q
      float4 q4 = quat_mul(q3, q);

      // 4. compute q = q4 + c
      q = q4 + c[0];

      // 5. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}

export void quat_ispc_v2_pow_5(
  uniform float4      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = points[i];

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q3 = q2^2
      float4 q3 = quat_sqr(q2);

      // 3. compute q4 = q3 * q
      float4 q4 = quat_mul(q3, q);

      // 4. compute q = q4 + c
      q = q4 + c[0];

      // 5. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}

export void quat_ispc_v1_pow_6(
  uniform float3      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       mod_q,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = vec3d_to_quat_polar(points[i], mod_q);

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q3 = q2^2
      float4 q3 = quat_sqr(q2);

      // 3. compute q4 = q3 * q2
      float4 q4 = quat_mul(q3, q2);

      // 4. compute q = q4 + c
      q = q4 + c[0];

      // 5. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}

export void quat_ispc_v2_pow_6(
  uniform float4      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = points[i];

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = q^2
      float4 q2 = quat_sqr(q);

      // 2. compute q3 = q2^2
      float4 q3 = quat_sqr(q2);

      // 3. compute q4 = q3 * q2
      float4 q4 = quat_mul(q3, q2);

      // 4. compute q = q4 + c
      q = q4 + c[0];

      // 5. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}

export void quat_ispc_v1_sin(
  uniform float3      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       mod_q,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = vec3d_to_quat_polar(points[i], mod_q);

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = sin(q)
      float4 q2 = { sin(q.x), sin(q.y), sin(q.z), sin(q.w) };

      // 2. compute q = q2 + c
      q = q2 + c[0];

      // 3. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}

export void quat_ispc_v2_sin(
  uniform float4      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = points[i];

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = sin(q)
      float4 q2 = { sin(q.x), sin(q.y), sin(q.z), sin(q.w) };

      // 2. compute q = q2 + c
      q = q2 + c[0];

      // 3. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}

export void quat_ispc_v1_cos(
  uniform float3      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       mod_q,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = vec3d_to_quat_polar(points[i], mod_q);

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = cos(q)
      float4 q2 = { cos(q.x), cos(q.y), cos(q.z), cos(q.w) };

      // 2. compute q = q2 + c
      q = q2 + c[0];

      // 3. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}


export void quat_ispc_v2_cos(
  uniform float4      points[],
  uniform int2      indices[],
  uniform float4      c[],
  uniform int       point_buf_count,
  uniform unsigned int8   iter_min,
  uniform unsigned int8   iter_max,
  uniform float       max_len_sqr
)
{
    foreach (i = 0 ... point_buf_count) {
    float4 q = points[i];

    // it[1] - iter_count
    // it[0] - add point flag
    int2 it = {0, 0};

    float len = 0.0f;
    do {
      it[1]++;

      // 1. compute q2 = cos(q)
      float4 q2 = { cos(q.x), cos(q.y), cos(q.z), cos(q.w) };

      // 2. compute q = q2 + c
      q = q2 + c[0];

      // 3. compute len = |q|^2
      len = quat_len_sqr(q);
    }
    while ((it[1] < iter_max) && (len < max_len_sqr));

    it[0] = it[1] >= iter_min;
    indices[i] = it;
  }
}
