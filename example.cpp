#include <iostream>
#include "Option.hpp"

int main(int argc, const char **argv)
{
  Option constexpr opt_list[] = {
    { "append", 'a', true },
    { nullptr, 'b', false },
    { "car", '\0', false },
    { nullptr, 'd', false }
  };

  Option::Iterator opt = Option::Iterator::create(
    1,
    argv,
    argc,
    opt_list,
    sizeof (opt_list) / sizeof (*opt_list)
    );

  while (!opt.is_done)
  {
    uint32_t const error_line = opt.line + opt.argc - opt.end;
    Option::Info const option = opt.advance();

    switch (option.status)
    {
    case GOT_OPTION:
      std::cout << "option:"
                << error_line
                << ": "
                << option.index
                << "; arg = "
                << (option.argument ? option.argument : "(null)")
                << '\n';
      break;
    case GOT_NON_OPTION:
      std::cout << "non option:"
                << error_line
                << ": "
                << argv[option.line]
                << '\n';
      break;
    case MISSING_ARGUMENT:
      std::cerr << "ERROR:"
                << error_line
                << ':'
                << option.column + 1
                << ": missing argument for option `"
                << argv[option.line]
                << "` with index "
                << option.index
                << ".\n";
      break;
    case INVALID_OPTION:
      std::cerr << "ERROR:"
                << error_line
                << ':'
                << option.column + 1
                << ": invalid option `"
                << argv[option.line]
                << "`.\n";
      break;
    }
  }

  for (int32_t i = opt.line; i < argc; i++)
    std::cout << i << ": " << argv[i] << '\n';
}
