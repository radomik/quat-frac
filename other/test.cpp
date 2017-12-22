#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <regex>
#include "../drawing2/Log.hpp"

class U32 {
public:
  unsigned int  value;

  static unsigned int parse2(const std::string &str);

private:
  static unsigned int _st_strToUint(const std::string &str);
  static unsigned int _st_strToUint2(const std::string &str);
};

unsigned int U32::parse2(const std::string &str) {
  try {
    return U32::_st_strToUint(str);
  } catch (const std::exception &e) {
    log_debug("> %s", e.what());
    Q_STTHROW("'%s' is not a valid unsigned integer", str.c_str());
  }
}

unsigned int U32::_st_strToUint2(const std::string &str) {
  assert(sizeof(long long int) == 8);
  assert(sizeof(unsigned int) == 4);

//~   std::string maxUintStr = std::to_string(UINT_MAX);
//~   if (str.size() > maxUintStr.size()) { // must contain no more chars than in UINT_MAX converted to string
//~     Q_STTHROW("'%s' is too long (than: %s)", str.c_str(), maxUintStr.c_str());
//~   }

  long long int lli = strtoll(str.c_str(), NULL, 0);

  if (lli == LLONG_MAX && errno == ERANGE) {
    Q_STTHROW("'%s' is out of long long int range [%lld ... %lld]", str.c_str(), LLONG_MIN, LLONG_MAX);
  }

  long long int max = UINT_MAX;

  if (lli > max) { // must not be above UINT
    Q_STTHROW("'%s' <-> %lld is out of unsigned int range [0 ... %lld]", str.c_str(), lli, max);
  }
  return (unsigned int)lli;
}

unsigned int U32::_st_strToUint(const std::string &str) {
  static const char *regex_str = "\\d+";
  std::regex e(regex_str);
  if (! std::regex_match(str, e)) { // contains non-digits
    Q_STTHROW("'%s' does not match regex '%s'", str.c_str(), regex_str);
  }

  size_t firstNonZero = str.find_first_not_of('0');
  if (firstNonZero == std::string::npos) { // all zeros
    return 0;
  } else if (firstNonZero == 0) { // no leading zeros
    return U32::_st_strToUint2(str);
  } else { // trim leading zeros
    std::string str2 = str.substr(firstNonZero);
    return U32::_st_strToUint2(str2);
  }
}

static bool test(unsigned int *expected, const char *str) {
  log_info("Parsing '%s'", str);
  try {
    unsigned int actual = U32::parse2(str);
    if (expected) {
      if (*expected == actual) {
        log_info(">>> Proper conversion: %u", actual);
        return true;
      }
      log_error(">>> Expected result: %u. Actual result: %u", *expected, actual);
      return false;
    }
    log_error(">>> Unexpected result: %u", actual);
    return false;
  } catch (const std::exception &e) {
    if (!expected) {
      log_info(">>> Got expected exception: %s", e.what());
      return true;
    }
    log_error(">>> Expected result: %u. Got unexpected exception %s", *expected, e.what());
    return false;
  }
}

int main(int argc, char** argv) {
  unsigned int err_count = 0;
  unsigned int e;
  e = 0; if (! test(&e, "0")) err_count++;
  e = 0; if (! test(&e, "0000000000")) err_count++;
  e = 0; if (! test(NULL, "         0")) err_count++;
  e = 0; if (! test(NULL, "0         ")) err_count++;
  e = 1; if (! test(&e, "1")) err_count++;
  e = 1; if (! test(&e, "00000000001")) err_count++;
  e = 1; if (! test(NULL, "")) err_count++;
  e = 1; if (! test(NULL, "zxxx")) err_count++;
  e = 1; if (! test(NULL, "   ")) err_count++;
  e = 123; if (! test(&e, "123")) err_count++;
  e = 4294967295; if (! test(&e, "4294967295")) err_count++;
  e = 4294967295; if (! test(NULL, "0xFFFFFFFF")) err_count++;
  e = 4294967295; if (! test(NULL, "4294967296")) err_count++;
  e = 4294967295; if (! test(NULL, "4294967299")) err_count++;
  e = 4294967295; if (! test(NULL, "42949672990")) err_count++;
  e = 4294967295; if (! test(NULL, "9223372036854775807")) err_count++;
  e = 4294967295; if (! test(NULL, "9223372036854775808")) err_count++;
  e = 4294967295; if (! test(NULL, "92233720368547758080")) err_count++;
  e = 4294967295; if (! test(NULL, "9223372036854775808000000000")) err_count++;
  e = 4294967295; if (! test(NULL, "-1")) err_count++;

  if (err_count != 0) {
    log_error("Test FAILED with %u errors", err_count);
  } else {
    log_info("All tests has been PASSED");
  }
  return err_count;
}
