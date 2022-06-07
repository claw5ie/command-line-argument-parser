# Command line argument parser
Simple command line argument parser in C++ capable of parsing short and long options one by one.

## Features

- Short options:
  `-a -b -c`
- Long options:
  `--long --long-option`
- Chaining:
  `-abc`
- Argument passing:
  `-aarg1 -a arg2 --long=arg3 --long arg4`

## Compilation and execution
```
g++ -o example.out example.cpp Option.cpp
./example.out non-option1 -bd --append=arg1 non-option2 -aarg2
```
