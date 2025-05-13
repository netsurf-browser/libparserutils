#ifndef TEST_TESTUTILS_H_
#define TEST_TESTUTILS_H_

#ifndef UNUSED
#define UNUSED(x) ((x) = (x))
#endif

#define assert(expr) \
  ((void) ((expr) || (__assert2 (#expr, __FILE__, __LINE__, __func__), 0)))

typedef bool (*line_func)(const char *data, size_t datalen, void *pw);

/* Redefine assert, so we can simply use the standard assert mechanism
 * within testcases and exit with the right output for the testrunner
 * to do the right thing. */
void __assert2(const char *expr, const char *file, int line, const char *function);

bool parse_testfile(const char *filename, line_func callback, void *pw);
size_t parse_filesize(const char *filename);

#endif