void __assert_func(const char* file, int line, const char* func, const char* failedexpr)
{
  __asm("bkpt 1");
}
