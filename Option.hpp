#ifndef OPTION_HPP
#define OPTION_HPP

#include <cstdint>

enum Status: uint32_t
{
  GOT_OPTION,
  GOT_NON_OPTION,
  MISSING_ARGUMENT,
  INVALID_OPTION
};

struct Option
{
  const char *long_opt;
  char short_opt;
  bool has_arg;

  struct Info
  {
    Status status;
    uint32_t index;
    const char *argument;

    uint32_t line,
      column;
  };

  struct Iterator
  {
    const Option *option_list;
    const char **argv;

    uint32_t option_count;
    uint32_t argc;

    uint32_t end;
    uint32_t line,
      column;
    bool is_done: 1;
    bool should_rotate: 1;

    static Iterator create(
      uint32_t start,
      const char **argv,
      uint32_t argc,
      const Option *option_list,
      uint32_t option_count
      );

    Info advance();
  };
};

#endif // OPTION_HPP
