/*!
 * @file
 * @brief This file contains forward declarations and constants.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>

/// maximal number of vertex attribute components
#define MAX_NUMBER_OF_ATTRIBUTE_COMPONENTS 4

/// maximal number of attributes
#define MAX_ATTRIBUTES 8

/**
 * @brief number of channels per color
 */
#define CHANNELS_PER_COLOR 4

/**
 * @brief pi constant
 */
#define MY_PI 3.14159265359f

struct Vec2;                          // forward declaration
struct Vec3;                          // forward declaration
struct Vec4;                          // forward declaration
struct Mat4;                          // forward declaration

typedef struct Vec2 Vec2;///< shortcut
typedef struct Vec3 Vec3;///< shortcut
typedef struct Vec4 Vec4;///< shortcut
typedef struct Mat4 Mat4;///< shortcut

/**
 * @brief A instance of this type stores handle to GPU object.
 *
 * Every object on GPU is represented by unsigned integer value - id/name.
 * CPU side can manipulate GPU object by these ids.
 */
typedef uint32_t ObjectID;

/**
 * @brief Type for storing buffer handle.
 *
 */
typedef ObjectID BufferID;

/**
 * @brief This type represents vertex puller handle - index to vertex puller
 * configuration.
 *
 */
typedef ObjectID VertexPullerID;

/**
 * @brief This type represents shader program id.
 *
 * A program is collection of shaders (vertex shader, fragment shader, ...).
 * This id points to one program on GPU
 */
typedef ObjectID ProgramID;

