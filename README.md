# liquidScint


## How to build
```bash
$ mkdir liquidScint
$ cd liquidScint
$ git clone git@github.com:HoyongJeong/liquidScint.git source
$ mkdir build
$ cd build
$ cmake ../source
$ make -j
```


## How to run
`./liquigScint -h` shows help message:
```bash
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
```

Executing `./liquidScint -g` will open w GUI window like,
![GUI1](./assets/g1.png)



## Drawing histogram
Please use scripts/DrawHist.C or write by yourself.


## Development Guid
1. If you want to change material property? Go to the line 139 of src/LSDetCon.cc
2. If you want to change surface property? Go to the line 91 of src/LSDetCon.cc
3. If you want to edit ntuple structure? Go to the line 31 of src/LSRunAct.cc
4. If you want to revise data acquisition? Go to the line 80 of the src/LSSteAct.cc


## History

= [1.0.1] 12th Jul 2024 by Hoyong Jeong =
  - README update

= [1.0.0] 8th Jun 2024 by Hoyong Jeong =
  - First version of liquidScint
