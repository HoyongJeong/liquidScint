liquidScint
=============

# How to build
    $ mkdir build
    $ cd build
    $ cmake [Where the source locate]
    $ make


# How to run
usage: liquidScint [-b] [-g] [-m macrofile]

Examples:
  liquidScint -b -m myRun.mac  # Run in batch mode with macro.
  liquidScint -g               # Run in graphical mode.

Options:
  -b  Execute in batch mode
  -g  Execute in graphical mode
      Note: Default is command mode
  -h  Show help message
  -m  Run with macro


# Development Guid
1. If you want to change material property? Go to the line 139 of src/LSDetCon.cc
2. If you want to change surface property? Go to the line 91 of src/LSDetCon.cc
3. If you want to edit ntuple structure? Go to the line 31 of src/LSRunAct.cc
4. If you want to revise data acquisition? Go to the line 80 of the src/LSSteAct.cc



#   History

= [1.0.0] 8. Jun. 2023. by Hoyong Jeong =
  - First version of liquidScint
