#pragma once

/**
 * @brief This struct contains parsed command line arguments
 */
struct Arguments{
  int runConformanceTests;///< execute conformance tests
  int runPerformanceTests;///< execute performance tests
  int takeScreenShot;///< take screenshot
};

typedef struct Arguments Arguments;///< shortcut

void arguments_parse(Arguments*const args,int argc,char*argv[]);
