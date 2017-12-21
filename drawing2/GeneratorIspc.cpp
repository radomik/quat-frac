#include "stdafx.h"
#include "GeneratorIspc.hpp"

const unsigned int  GeneratorIspc::MIN_BUF_SIZE = 128;
const unsigned int  GeneratorIspc::MAX_BUF_SIZE = 10 * 1024 * 1024;
const unsigned int  GeneratorIspc::DEFAULT_BUF_SIZE = 250000;

GeneratorIspc::GeneratorIspc(FILE *f_out, const OptGenerator &_opt, size_t points_item_size)
  : m_f_out(f_out)
{
  _initPointBufCount(_opt.buf_size, points_item_size);
}

void GeneratorIspc::_initPointBufCount(size_t buf_size, size_t points_item_size) {
  if (buf_size < MIN_BUF_SIZE || buf_size > MAX_BUF_SIZE) buf_size = DEFAULT_BUF_SIZE;

  size_t point_buf_count = buf_size / points_item_size;
  if (point_buf_count < 4) point_buf_count = 4;

  // m_point_buf_count should not exceed INT_MAX
  m_point_buf_count = point_buf_count > INT_MAX
    ? INT_MAX
    : (unsigned int)point_buf_count;

  //fprintf(stderr, "[DBG] GeneratorIspc::_initPointBufCount: m_point_buf_count = %u\n", m_point_buf_count);
}
