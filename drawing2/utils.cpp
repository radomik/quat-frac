#include "stdafx.h"
#include "utils.hpp"
#include <sstream>
#include <string>

const char *CO_TRUE_STR  = "true";
const char *CO_FALSE_STR = "false";
const char *CO_YES_STR  = "yes";
const char *CO_NO_STR = "no";

bool argline::hasOption(int argc, wchar_t* argv[], const wchar_t *op) {
  if ((argc == 1) || (!op) || (!argv) || (!(*op))) return false;
  else {
    for (int i = 1; i < argc; i++) {
      if (!wcscmp(argv[i], op)) return true;
    }
    return false;
  }
}

bool argline::hasOption(int argc, char* argv[], const char *op) {
  if ((argc == 1) || (!op) || (!argv) || (!(*op))) return false;
  else {
    for (int i = 1; i < argc; i++) {
      if (!strcmp(argv[i], op)) return true;
    }
    return false;
  }
}

const wchar_t* argline::getOption(
  int       argc,
  wchar_t*    argv[],
  const wchar_t*  op,
  const wchar_t*  defaultValue)
{
  if ((argc == 1) || (!op) || (!argv) || (!(*op))) return defaultValue;
  else {
    for (int i = 1; i < argc; i++) {
      if ((!wcscmp(argv[i], op)) && (i + 1 < argc)) return argv[i + 1];
    }
    return defaultValue;
  }
}

const char* argline::getOption(
  int       argc,
  char*     argv[],
  const char*   op,
  const char*   defaultValue)
{
  if ((argc == 1) || (!op) || (!argv) || (!(*op))) return defaultValue;
  else {
    for (int i = 1; i < argc; i++) {
      if ((!strcmp(argv[i], op)) && (i + 1 < argc)) return argv[i + 1];
    }
    return defaultValue;
  }
}

bool argline::hasHelpOption(int argc, wchar_t* argv[])
{
  const wchar_t *HELP_OPS[] = {
    L"--help", L"-h", L"-help", L"--h"
  };

  if (argc != 2) return false;
  else {
    for (unsigned int i = 0; i < sizeof(HELP_OPS) / sizeof(const wchar_t*); i++) {
      if (!wcscmp(HELP_OPS[i], argv[1]))
        return true;
    }
    return false;
  }
}

bool argline::hasHelpOption(int argc, char* argv[])
{
  const char *HELP_OPS[] = {
    "--help", "-h", "-help", "--h"
  };

  if (argc != 2) return false;
  else {
    for (unsigned int i = 0; i < sizeof(HELP_OPS) / sizeof(const char*); i++) {
      if (!strcmp(HELP_OPS[i], argv[1]))
        return true;
    }
    return false;
  }
}

void replaceOrAppendExt(std::wstring& s, const std::wstring& newExt) {
  std::wstring::size_type i = s.rfind('.', s.length()-1);

  if (i != std::wstring::npos) {
    s.replace(i, newExt.length(), newExt);
    std::wstring::size_type max_len = i + newExt.length();
    if (max_len < s.length())
        s.resize(max_len);
  }
  else s.append(newExt);
}

void replaceOrAppendExt(std::string& s, const std::string& newExt) {
  std::string::size_type i = s.rfind('.', s.length()-1);

  if (i != std::string::npos) {
    s.replace(i, newExt.length(), newExt);
    std::string::size_type max_len = i + newExt.length();
    if (max_len < s.length())
        s.resize(max_len);
  }
  else s.append(newExt);
}

/// (public,static)
/// Retrieve filename from filepath
/// @param filepath     Path to file or filename
/// @param filepath_len   Length of filepath (count of characters)
/// @return Index of first character of filename inside filepath
size_t getFileIndex(const char *filepath, size_t filepath_len) {
  size_t i;
  if (! filepath_len) return 0;
  for (i = filepath_len-1; ;) {
    if (filepath[i] == FILEPATH_SEPARATOR) return i + 1;
    if (i == 0) return 0;
    i--;
  }
}

/// (public,static)
/// Retrieve filename from filepath
/// @param filepath     Path to file or filename
/// @param filepath_len   Length of filepath (count of characters)
/// @return Index of first character of filename inside filepath
size_t getFileIndex(const wchar_t *filepath, size_t filepath_len) {
  size_t i;
  if (!filepath_len) return 0;
  for (i = filepath_len - 1;;) {
    if (filepath[i] == W_FILEPATH_SEPARATOR) return i + 1;
    if (i == 0) return 0;
    i--;
  }
}

/// (public,static)
/// Convert unicode string to multibyte string
/// Returned string must be deleted[] by caller
/// return NULL on error, empty string on conversion error
char* unicodeToMultibyte(const wchar_t *src, size_t src_len) {
  size_t src_size = src_len+1;
  char   *res;

  try {
    res = new char[src_size];
    //fprintf(stderr, "[DBG] %s @ %d [NEW] %p (char[] res)\n", __FILE__, __LINE__, res);
  }
  catch (const std::bad_alloc& e) {
    SysError ex;
#ifdef _WINDOWS
    fprintf(stderr, "%s: Failed to allocate char[%Iu] [what: %s, errno: %s]\n", __FUNCTION__, src_size, e.what(), ex.msg);
#else
    fprintf(stderr, "%s: Failed to allocate char[%zu] [what: %s, errno: %s]\n", __FUNCTION__, src_size, e.what(), ex.msg);
#endif
    return NULL;
  }


#ifdef _WINDOWS
  size_t cnt_conv;
  if (wcstombs_s(&cnt_conv, res, src_size, src, _TRUNCATE) != 0)
#else
  if (wcstombs(res, src, src_size) == (size_t)-1)
#endif
  {
    SysError ex;
    fwprintf(stderr, L"Cannot convert UNICODE string: \"%s\" to multibyte ", src);
    fprintf(stderr, "[%s]\n", ex.msg);
    res[0] = '\0';
  }

  return res;
}

/// Format byte_size
/// byte_size <= 1024 B --> out_str = "<byte_size> B"
/// byte_size <= 1 MB   --> out_str = "<%5.2f byte_size as KB> KB"
/// byte_size  > 1 MB   --> out_str = "<%5.2f byte_size as MB> MB"
const char* formatBytes(size_t byte_size, char *out_str, size_t out_str_size)
{
  if (byte_size <= 1024) { // up to 1 KB
#ifdef _WINDOWS
    sys_snprintf(out_str, out_str_size, "%Iu B", byte_size);
#elif _LINUX
    sys_snprintf(out_str, out_str_size, "%zu B", byte_size);
#endif
  }
  else {
    if (byte_size <= 1024 * 1024) { // up to 1 MB
      sys_snprintf(out_str, out_str_size, "%5.2f KB", (double)byte_size / 1024.0);
    }
    else { // above 1 MB
      sys_snprintf(out_str, out_str_size, "%5.2f MB", (double)byte_size / (1024.0*1024.0));
    }
  }
  return out_str;
}
