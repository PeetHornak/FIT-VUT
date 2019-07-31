/*!
 * @file
 * @brief This file contains global variables
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#include <student/globals.h>

/// This variable contains view matrix of orbit camera.
Mat4 viewMatrix;
/// This variable contains projection matrix of orbit camera.
Mat4 projectionMatrix;
/// This variable contains camera position in world-space.
Vec3 cameraPosition;
/// This variable contains light position in world-space.
Vec3 lightPosition;

void initGlobals(){
  viewMatrix.column[0].data[0] = +1.0000000000e+00f;
  viewMatrix.column[0].data[1] = +0.0000000000e+00f;
  viewMatrix.column[0].data[2] = +0.0000000000e+00f;
  viewMatrix.column[0].data[3] = +0.0000000000e+00f;
  viewMatrix.column[1].data[0] = +0.0000000000e+00f;
  viewMatrix.column[1].data[1] = +1.0000000000e+00f;
  viewMatrix.column[1].data[2] = +0.0000000000e+00f;
  viewMatrix.column[1].data[3] = +0.0000000000e+00f;
  viewMatrix.column[2].data[0] = +0.0000000000e+00f;
  viewMatrix.column[2].data[1] = +0.0000000000e+00f;
  viewMatrix.column[2].data[2] = +1.0000000000e+00f;
  viewMatrix.column[2].data[3] = +0.0000000000e+00f;
  viewMatrix.column[3].data[0] = +0.0000000000e+00f;
  viewMatrix.column[3].data[1] = +0.0000000000e+00f;
  viewMatrix.column[3].data[2] = -1.8800077438e+00f;
  viewMatrix.column[3].data[3] = +1.0000000000e+00f;

  projectionMatrix.column[0].data[0] = +1.0000000000e+00f;
  projectionMatrix.column[0].data[1] = +0.0000000000e+00f;
  projectionMatrix.column[0].data[2] = +0.0000000000e+00f;
  projectionMatrix.column[0].data[3] = +0.0000000000e+00f;
  projectionMatrix.column[1].data[0] = +0.0000000000e+00f;
  projectionMatrix.column[1].data[1] = +1.0000000000e+00f;
  projectionMatrix.column[1].data[2] = +0.0000000000e+00f;
  projectionMatrix.column[1].data[3] = +0.0000000000e+00f;
  projectionMatrix.column[2].data[0] = +0.0000000000e+00f;
  projectionMatrix.column[2].data[1] = +0.0000000000e+00f;
  projectionMatrix.column[2].data[2] = -1.0000199080e+00f;
  projectionMatrix.column[2].data[3] = -1.0000000000e+00f;
  projectionMatrix.column[3].data[0] = +0.0000000000e+00f;
  projectionMatrix.column[3].data[1] = +0.0000000000e+00f;
  projectionMatrix.column[3].data[2] = -2.0000198483e-01f;
  projectionMatrix.column[3].data[3] = +0.0000000000e+00f;
  init_Vec3(&lightPosition, 1000.f, 1000.f, 1000.f);
  init_Vec3(&cameraPosition,0,0,1.8800077438e+00f);
}
