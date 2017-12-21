/*
Copyright (c) 2010-2011, Intel Corporation
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

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)
#endif

#include "stdafx.h"
#include "GeneratorFirstIspc.hpp"
#include "Generator.hpp"
#include "BinSerialization.hpp"
#include "FractalFunc.hpp"
#include "GeomMath.hpp"

GeneratorFirstIspc::GeneratorFirstIspc(const OptGeneratorFirst &opt, FILE *f_out)
  : GeneratorIspc(f_out, opt, sizeof(ispc::float3)), m_opt(opt)
{
  m_points = new ispc::float3[m_point_buf_count];
  m_indices = new ispc::int32_t2[m_point_buf_count];
}

GeneratorFirstIspc::~GeneratorFirstIspc() {
  if (m_points) {
    delete[] m_points;
    m_points = NULL;
  }
  if (m_indices) {
    delete[] m_indices;
    m_indices = NULL;
  }
}

inline int GeneratorFirstIspc::_writeData(unsigned int point_index, size_t &written_count)
{
  written_count = 0;
  for (unsigned int i = 0; i < point_index; i++) {
    if (m_indices[i].v[0] != 0) {
      RangeIter::DATA_TYPE iter_count = (RangeIter::DATA_TYPE)m_indices[i].v[1];

      if ((BinSerialization::writeArray<float>(m_f_out, m_points[i].v, sizeof(m_points[i].v) / sizeof(m_points[i].v[0])) != 0) ||
        (BinSerialization::writeValue<RangeIter::DATA_TYPE>(m_f_out, iter_count) != 0))
      {
        fprintf(stderr, "GeneratorFirstIspc: Failed to write data item to file\n");
        return -1;
      }

      written_count++;
    }
  }

  return 0;
}

#ifdef DBG_ISPC
void GeneratorFirstIspc::_printDebug(FILE *dbg_file, size_t &abs_index, size_t point_count) {
  for (size_t k = 0; k < point_count; k++) {
    fprintf(dbg_file, "%8zu | %8zu) %10.3f ; %10.3f ; %10.3f | iter = %3d | add = %d\n",
      abs_index++, k, m_points[k].v[0], m_points[k].v[1], m_points[k].v[2], m_indices[k].v[1], m_indices[k].v[0]);
  }
}
#endif

int GeneratorFirstIspc::run() {
  fractal_ispc_func_v1_t  ispc_fun_cb = FractalFunc::IMPL_ISPC_V1[m_opt.fun_index];
  Range3Df        range = m_opt.range;
  ispc::float3 p;
  ispc::float4 c;
  size_t count = 0;
  size_t created_points_count = 0;
  unsigned int point_index = 0;

  assert(ispc_fun_cb != NULL);

  if (m_opt.mul_range_by_pi)
    range.mulByPi();

  vec4::set(c.v, m_opt.initial.valueConst().v);

#ifdef DBG_ISPC
  FILE *dbg_file = fopen("dbg_ispc.txt", "w");
  size_t abs_index = 0;
  if (! dbg_file) {
    SysError ex;
    fprintf(stderr, "[ERROR] %s::%s: Error opening debug output file [%s]\n", __FILE__, __FUNCTION__, ex.msg);
    return -1;
  }
#endif

  for (p.v[0] = range.x.min; p.v[0] <= range.x.max; p.v[0] += range.x.step) {
    for (p.v[1] = range.y.min; p.v[1] <= range.y.max; p.v[1] += range.y.step) {
      for (p.v[2] = range.z.min; p.v[2] <= range.z.max; p.v[2] += range.z.step) {
        if (point_index >= m_point_buf_count) {
          // flush
          ispc_fun_cb(
            m_points,
            m_indices,
            &c,
            point_index,
            (uint8_t)m_opt.iter.min,
            (uint8_t)m_opt.iter.max,
            m_opt.mod_q,
            m_opt.max_len_sqr
            );

#ifdef DBG_ISPC
          _printDebug(dbg_file, abs_index, point_index);
#endif
          // write
          if (_writeData(point_index, count) != 0) {
#ifdef DBG_ISPC
            fclose(dbg_file);
#endif
            return -1;
          }

          created_points_count += count;

          if ((count != 0) && ((created_points_count == count) || (created_points_count + 1 % 10000 == 0))) {
            Generator::printCountCreated(created_points_count + 1);
          }

          // reset
          point_index = 0;
        }

        m_points[point_index++] = p;

        //fprintf(stderr, "Written: %f ; %f ; %f (%zu)\n",
        //  p.v[0], p.v[1], p.v[2], point_index);
      }
    }
  }

  if (point_index != 0) {
    // flush
    ispc_fun_cb(
      m_points,
      m_indices,
      &c,
      point_index,
      (uint8_t)m_opt.iter.min,
      (uint8_t)m_opt.iter.max,
      m_opt.mod_q,
      m_opt.max_len_sqr
      );

#ifdef DBG_ISPC
    _printDebug(dbg_file, abs_index, point_index);
#endif

    // write
    if (_writeData(point_index, count) != 0) {
#ifdef DBG_ISPC
      fclose(dbg_file);
#endif
      return -1;
    }

    created_points_count += count;

    Generator::printCountCreated(created_points_count + 1);
  }

#ifdef DBG_ISPC
  fclose(dbg_file);
#endif

  return 0;
}
