#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

#include <student/linearAlgebra.h>
#include <student/globals.h>
#include <student/phongMethod.h>
#include <tests/performanceTest.h>
#include <student/cpu.h>

void runPerformanceTest() {
  GPU gpu;
  cpu_initGPU(&gpu,500,500);
  phong_onInit(&gpu);

  initGlobals();

  size_t const framesPerMeasurement = 10;
  auto         start                = std::chrono::high_resolution_clock::now();
  for (size_t i   = 0; i < framesPerMeasurement; ++i){
    phong_onDraw(&gpu);
    cpu_finish(&gpu);
  }
  auto        end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = end - start;
  float const time = elapsed.count() / float(framesPerMeasurement);

  std::cout << "Seconds per frame: " << std::scientific << std::setprecision(10)
            << time << std::endl;

  phong_onExit(&gpu);
}
