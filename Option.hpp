#ifndef OPTION_HPP
#define OPTION_HPP

#include <cstdint>

namespace CmdParser
{
  struct OptionSpec
  {
    const char *long_opt;
    char short_opt;
    bool has_arg;
  };

  enum Status: uint32_t
  {
    GOT_OPTION,
    GOT_NON_OPTION,
    MISSING_ARGUMENT,
    INVALID_OPTION
  };

  struct Option
  {
    Status status;
    uint32_t index;
    const char *argument;

    uint32_t line,
      column;
  };

  struct Iterator
  {
    const OptionSpec *option_list;
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
      const OptionSpec *option_list,
      uint32_t option_count
      );

    Option advance();
  };
}

#endif // OPTION_HPP
