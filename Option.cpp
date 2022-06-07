#include <cctype>
#include "Option.hpp"

const char *is_prefix(const char *prefix, const char *string)
{
  if (prefix == nullptr || string == nullptr)
    return nullptr;

  while (*prefix != '\0' && *prefix == *string)
  {
    prefix++;
    string++;
  }

  return *prefix == '\0' ? string : nullptr;
}

CmdParser::Iterator CmdParser::Iterator::create(
  uint32_t start,
  const char **argv,
  uint32_t argc,
  const OptionSpec *option_list,
  uint32_t option_count
  )
{
  return {
    option_list,
    argv,
    option_count,
    argc,
    argc,
    start,
    1,
    start >= argc,
    true,
  };
}

CmdParser::Option CmdParser::Iterator::advance()
{
  if (is_done)
    return { INVALID_OPTION, uint32_t(-1), nullptr, line, 0 };

  const char *const arg = argv[line];
  Option option = { GOT_OPTION, uint32_t(-1), nullptr, line, 0 };

  if (column > 1 || (arg[0] == '-' && std::isalpha(arg[1])))
  {
    for (option.index = 0; option.index < option_count; option.index++)
    {
      if (option_list[option.index].short_opt == arg[column])
        break;
    }

    option.column = column++;

    if (option.index >= option_count)
    {
      option.status = INVALID_OPTION;

      if (arg[column] == '\0')
      {
        ++line;
        column = 1;
      }
    }
    else if (option_list[option.index].has_arg)
    {
      ++line;

      if (arg[column] != '\0')
        option.argument = arg + column;
      else if (line < end)
        option.argument = argv[line++];
      else
        option.status = MISSING_ARGUMENT;

      column = 1;
    }
    else if (arg[column] == '\0')
    {
      ++line;
      column = 1;
    }
  }
  else if (arg[0] == '-' && arg[1] == '-' && std::isalpha(arg[2]))
  {
    const char *after_prefix = nullptr;

    for (option.index = 0; option.index < option_count; option.index++)
    {
      after_prefix = is_prefix(
        option_list[option.index].long_opt, arg + 2
        );

      if (after_prefix)
        break;
    }

    ++line;

    if (option.index >= option_count)
    {
      option.status = INVALID_OPTION;
    }
    else if (option_list[option.index].has_arg)
    {
      if (*after_prefix == '=')
        option.argument = after_prefix + 1;
      else if (*after_prefix != '\0')
        option.status = INVALID_OPTION;
      else if (line < end)
        option.argument = argv[line++];
      else
        option.status = MISSING_ARGUMENT;
    }
  }
  else
  {
    option.status = GOT_NON_OPTION;

    ++line;

    if (should_rotate)
    {
      for (uint32_t i = line; i < argc; i++)
        argv[i - 1] = argv[i];
      argv[argc - 1] = arg;

      --line;
      --end;
      option.line = argc - 1;
    }
  }

  is_done = line >= end;

  return option;
}
