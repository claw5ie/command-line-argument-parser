#include <iostream>
#include "Option.hpp"

int main(int argc, const char **argv)
{
  CmdParser::OptionSpec constexpr opt_list[] = {
    { "append", 'a', true },
    { nullptr, 'b', false },
    { "car", '\0', false },
    { nullptr, 'd', false }
  };

  CmdParser::Iterator opt = CmdParser::Iterator::create(
    1,
    argv,
    argc,
    opt_list,
    sizeof (opt_list) / sizeof (*opt_list)
    );

  while (!opt.is_done)
  {
    uint32_t const error_line = opt.line + opt.argc - opt.end;
    CmdParser::Option const option = opt.advance();

    switch (option.status)
    {
    case CmdParser::GOT_OPTION:
      std::cout << "option:"
                << error_line
                << ": "
                << option.index
                << "; arg = "
                << (option.argument ? option.argument : "(null)")
                << '\n';
      break;
    case CmdParser::GOT_NON_OPTION:
      std::cout << "non option:"
                << error_line
                << ": "
                << argv[option.line]
                << '\n';
      break;
    case CmdParser::MISSING_ARGUMENT:
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
    case CmdParser::INVALID_OPTION:
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
