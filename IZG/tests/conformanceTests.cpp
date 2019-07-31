#include <iostream>
#include <tests/conformanceTests.h>

#define CATCH_CONFIG_RUNNER
#include <3rdParty/catch.hpp>

void runConformanceTests() {
  int         argc   = 1;
  char const* argv[] = {"test"};

  Catch::Config cfg;
  auto nofTests = Catch::getAllTestCasesSorted(cfg).size();
  int  result = Catch::Session().run(argc, argv);
 
  size_t maxPoints = 18;
  std::cout << std::fixed << std::setprecision(1) << maxPoints * (float)(nofTests-result)/(float)nofTests << std::endl;
}
