#include "stdafx.h"
#include "Generator.hpp"
#include "BinSerialization.hpp"
#include "timing.h"
#include "OptGenerator.hpp"

int Generator::compute(FILE *out_file, FILE *log_file) {
  const size_t DATA_POINT_COUNT_FIELD_SIZE = sizeof(unsigned int);
  const char *self_real_name = getName();
  const char *self_name = "Generator::compute";
  long int pos1, pos2;
  unsigned int data_item_size = getDataItemSize();
  unsigned int count = 0;

  if (log_file) {
    fprintf(log_file, "info=%s\n", self_real_name);
    fprintf(log_file, "ver=%u\n", getVersion());
    fprintf(log_file, "func=%s\n", FractalFunc::NAMES[getFuncIndex()]);
  }

  reset_and_start_timer();

  // 1. Get position before 'data point count' field (pos1)
  fflush(out_file);
  if ((pos1 = ftell(out_file)) < 0) {
    SysError ex;
    fprintf(stderr, "%s: Failed to get file position pos1 [%s]\n", self_name, ex.msg);
    return -1;
  }

  // 2. Skip 'data point count' field (now empty)
  if (BinSerialization::writeValue<unsigned int>(out_file, count) != 0) {
    SysError ex;
    fprintf(stderr, "%s: Failed to skip 'data point count' field [%s]\n", self_name, ex.msg);
    return -1;
  }

  // 3. Generate data
  fprintf(stderr, "Starting generator:    %s\n", self_real_name);
  if (computeCustom(out_file) != 0) {
    fprintf(stderr, "\n%s: Failed to generate points: computeCustom()\n", self_name);
    return -1;
  }
  fprintf(stderr, "\n");

  // 4. Get current position in file (pos2)
  fflush(out_file);
  if ((pos2 = ftell(out_file)) < 0) {
    SysError ex;
    fprintf(stderr, "%s: Failed to get file position pos2 [%s]\n", self_name, ex.msg);
    return -1;
  }

  // 5. Calculate count of created points
  size_t count_s = ((size_t)pos2 - (size_t)pos1 - DATA_POINT_COUNT_FIELD_SIZE) / data_item_size;

  // silent truncation - it almost impossible to create more than 4 mld points
  count = (unsigned int)count_s;

  // 6. Seek back to 'data point count' field
  if (fseek(out_file, pos1, SEEK_SET) < 0) {
    SysError ex;
    fprintf(stderr, "%s: Failed to seek back to 'data point count' field [%s]\n", self_name, ex.msg);
    return -1;
  }

  // 7. Write count of created points
  if (BinSerialization::writeValue<unsigned int>(out_file, count) != 0) {
    SysError ex;
    fprintf(stderr, "%s: Failed to write count of created points [%s]\n", self_name, ex.msg);
    return -1;
  }

  // 8. Print summary
  _printSummary(pos2, count, log_file);

  return 0;
}

void Generator::_printSummary(long int pos2, unsigned int count, FILE *log_file) {
  // Format file size
  char file_size_str[64];
  //if (pos2 > 1024)
  formatBytes(pos2, file_size_str, sizeof(file_size_str));

  fprintf(stderr, "\nSummary:\n");
  fprintf(stderr, "  * data point count: %u\n", count);

  if (pos2 <= 1024) {
    fprintf(stderr, "  * file size:        %s\n", file_size_str);
  }
  else {
    fprintf(stderr, "  * file size:        %s ( %ld B )\n", file_size_str, pos2);
  }

  double dt = get_elapsed_mcycles();
  fprintf(stderr, "  * execution time:   %.3f mln cycles\n", dt);

  if (log_file) {
    size_t count_in_points = getCountInPoints();
    size_t total_recurs_iter_count = getTotalRecursIterCount();

#ifdef _WINDOWS
    fprintf(log_file, "count_in_points=%Iu\n", count_in_points);
#else
    fprintf(log_file, "count_in_points=%zu\n", count_in_points);
#endif

#ifdef _WINDOWS
    fprintf(log_file, "total_recurs_iter_count=%Iu\n", total_recurs_iter_count);
#else
    fprintf(log_file, "total_recurs_iter_count=%zu\n", total_recurs_iter_count);
#endif

    if ((count_in_points != 0) && (total_recurs_iter_count != 0))
      fprintf(log_file, "avg_recurs_iter_count=%lf\n", (double)total_recurs_iter_count / count_in_points);
    else
      fprintf(log_file, "avg_recurs_iter_count=0\n");


    fprintf(log_file, "data_point_count=%u\n", count);
    fprintf(log_file, "file_size_bytes=%ld\n", pos2);
    fprintf(log_file, "file_size_str=%s\n", file_size_str);
    fprintf(log_file, "execution_time_mcycles=%lf\n", dt);
    fprintf(log_file, "-----------------\n");
  }
}

fractal_func_ind_t Generator::getFuncIndex() const {
  return getOptions().fun_index;
}
