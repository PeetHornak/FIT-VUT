/*!
 * @file
 * @brief This file contains structures and function declarations of GPU HW.
 * Main structure is GPU that holds all components of the GPU.
 * GPU is composed of list of buffers, list of vertex pullers and list of programs.
 * GPU also contains framebuffer, active program and vertex puller and states.
 * GPU executes commands that are stored in CommandQueue.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#pragma once

#include <stdint.h>
#include <student/linearAlgebra.h>
#include <vector/vector.h>
#include <stack/stack.h>
#include <queue/queue.h>

/**
 * @brief This macro represents maximum number of uniform variables
 */
#define MAX_UNIFORMS            16
/**
 * @brief This macro represents maximum size of uniform variable
 */
#define MAX_UNIFORM_SIZE        sizeof(Mat4)
/**
 * @brief This macro represents maximum attribute size
 */
#define MAX_ATTRIBUTE_SIZE      sizeof(Vec4)
/**
 * @brief This macro represents empty object id
 */
#define EMPTY_ID                0xffffffff

/**
 * @brief This enum represents vertex/fragment attribute type.
 */
enum AttributeType{
  ATTRIBUTE_EMPTY =   0            , ///< disabled attribute
  ATTRIBUTE_FLOAT =   sizeof(float), ///< 1 32-bit float
  ATTRIBUTE_VEC2  =   sizeof(Vec2 ), ///< 2 32-bit floats
  ATTRIBUTE_VEC3  =   sizeof(Vec3 ), ///< 3 32-bit floats
  ATTRIBUTE_VEC4  =   sizeof(Vec4 ), ///< 4 32-bit floats
};

typedef enum AttributeType AttributeType; ///< shortcut

/**
 * @brief This enum represents uniform type.
 */
enum GPUUniformType {
  UNIFORM_FLOAT = sizeof(float), ///< single 32-bit float
  UNIFORM_VEC2  = sizeof(Vec2 ), ///< two    32-bit floats
  UNIFORM_VEC3  = sizeof(Vec3 ), ///< three  32-bit floats
  UNIFORM_VEC4  = sizeof(Vec4 ), ///< four   32-bit floats
  UNIFORM_MAT4  = sizeof(Mat4 ), ///< matrix 4x4, base type is 32-bit float
};


/**
 * @brief This struct represents one attribute (vertex or fragment)
 */
struct GPUAttribute{
  uint8_t data[MAX_ATTRIBUTE_SIZE]; ///< attribute data
};

typedef struct GPUAttribute GPUAttribute;///< shortcut

/**
 * @brief This struct represents input vertex of vertex shader.
 */
struct GPUInVertex{
  GPUAttribute attributes[MAX_ATTRIBUTES]; ///< vertex attributes
  uint32_t     gl_VertexID               ; ///< vertex id
};

typedef struct GPUInVertex    GPUInVertex   ; ///< shortcut

/**
 * @brief This struct represents output vertex of vertex shader.
 */
struct GPUOutVertex{
  GPUAttribute attributes[MAX_ATTRIBUTES]; ///< vertex attributes
  Vec4         gl_Position               ; ///< clip space position
};

typedef struct GPUOutVertex   GPUOutVertex  ; ///< shortcut

/**
 * @brief This struct represents input fragment.
 */
struct GPUInFragment{
  GPUAttribute attributes[MAX_ATTRIBUTES]; ///< fragment attributes
  Vec4         gl_FragCoord              ; ///< fragment coordinates
};

typedef struct GPUInFragment  GPUInFragment ; ///< shortcut

/**
 * @brief This struct represents output fragment.
 */
struct GPUOutFragment{
  Vec4 gl_FragColor; ///< fragment color
};

typedef struct GPUOutFragment GPUOutFragment; ///< shortcut


typedef enum GPUUniformType GPUUniformType; ///< shortcut

/**
 * @brief This struct represents one uniform.
 */
struct GPUUniform{
  uint8_t        data[MAX_UNIFORM_SIZE]; ///< uniform data
  GPUUniformType type                  ; ///< uniform type
  int            active                ; ///< is this uniform active?
};

typedef struct GPUUniform  GPUUniform ; ///< shortcut

/**
 * @brief All uniforms
 */
struct GPUUniforms{
  GPUUniform uniform[MAX_UNIFORMS]; ///< uniform variables
};

typedef struct GPUUniforms GPUUniforms; ///< shortcut

/**
 * @brief Vertex shader data
 */
struct GPUVertexShaderData{
  GPUUniforms const*uniforms ; ///< pointer to uniform variables
  GPUInVertex       inVertex ; ///< input vertex
  GPUOutVertex      outVertex; ///< output vertex
};

/**
 * @brief Fragment shader data
 */
struct GPUFragmentShaderData{
  GPUUniforms const*uniforms   ; ///< pointer to uniform variables
  GPUInFragment     inFragment ; ///< input fragment
  GPUOutFragment    outFragment; ///< output fragment
};

typedef struct GPUVertexShaderData GPUVertexShaderData; ///< shortcut
typedef struct GPUFragmentShaderData GPUFragmentShaderData; ///< shortcut

typedef void(*VertexShader  )(GPUVertexShaderData  *const); ///< vertex shader type
typedef void(*FragmentShader)(GPUFragmentShaderData*const); ///< fragment shader type

/**
 * @brief This struct represents one vertex puller head
 */
struct GPUVertexPullerHead{
  uint64_t      stride  ; ///< reading stride in bytes
  uint64_t      offset  ; ///< reading offset in bytes
  uint32_t      bufferId; ///< id of buffer in the GPU
  int           enabled ; ///< is this head enabled?
  AttributeType type    ; ///< type of the attribute (can be translated to size in bytes)
};

typedef struct GPUVertexPullerHead GPUVertexPullerHead; ///< shortcut

/**
 * @brief This enum represents index type
 */
enum IndexType{
  UINT8  = 1, ///< uin8_t type
  UINT16 = 2, ///< uin16_t type
  UINT32 = 4, ///< uint32_t type
};

typedef enum IndexType IndexType; ///< shortcut

/**
 * @brief This struct represents indices stored in vertex puller
 */
struct GPUIndices{
  uint32_t   bufferId; ///< index of the buffer that contains indices
  IndexType  type    ; ///< the type of indices
};

typedef struct GPUIndices GPUIndices; ///< shortcut

/**
 * @brief This struct represents vertex puller;
 */
struct GPUVertexPuller{
  GPUVertexPullerHead heads[MAX_ATTRIBUTES]; ///< vertex puller heads
  GPUIndices          indices              ; ///< vertex puller indices
};

typedef struct GPUVertexPuller GPUVertexPuller; ///< shortcut

/**
 * @brief This struct represents one GPU buffer.
 */
struct GPUBuffer{
  void*    data; ///< data
  uint64_t size; ///< size of data
};

typedef struct GPUBuffer GPUBuffer; ///< shortcut

/**
 * @brief This struct represents all GPU buffers.
 */
struct GPUBuffers{
  Vector buffers; ///< all buffers on the GPU
  Stack  freeIds; ///< empty ids
};

typedef struct GPUBuffers GPUBuffers; ///< shortcut

/**
 * @brief This struct represents all vertex pullers
 */
struct GPUVertexPullers{
  Vector pullers; ///< vertex pullers
  Stack  freeIds; ///< empty ids
};

typedef struct GPUVertexPullers GPUVertexPullers; ///< shortcut

/**
 * @brief This structure represents shader program.
 */
struct GPUProgram{
  VertexShader   vertexShader          ; ///< vertex shader
  FragmentShader fragmentShader        ; ///< fragment shader
  AttributeType  vs2fsType[MAX_ATTRIBUTES];///< types of attributes that are sent from vertex shader to fragment shader
  GPUUniforms    uniforms              ; ///< uniform variables
};

typedef struct GPUProgram GPUProgram; ///< shortcut

/**
 * @brief This struct represents all shader programs on GPU.
 */
struct GPUPrograms{
  Vector programs; ///< shader programs
  Stack  freeIds ; ///< empty program ids (can be used in future)
};

typedef struct GPUPrograms GPUPrograms; ///< shortcut

/**
 * @brief This enum represents type of GPU commands.
 */
enum GPUCommandType{
  COMMAND_DRAW , ///< draw command - forces GPU to draw
  COMMAND_CLEAR, ///< clear command - forces GPU to clear framebuffer
  COMMAND_USE_PROGRAM,///< sets active program
  COMMAND_BIND_PULLER,///< sets active vertex puller
  COMMAND_UNBIND_PULLER,///< unsets active vertex puller
};

typedef enum GPUCommandType GPUCommandType; ///< shortcut

/**
 * @brief This enum represents type of draw call.
 */
enum GPUDrawType{
  DRAW_POINTS   ,///< draw mesh using points
  DRAW_TRIANGLES,///< draw mesh using triangles
};

typedef enum GPUDrawType GPUDrawType; ///< shortcut

/**
 * @brief This struct represents command for GPU
 */
struct GPUCommand{
  GPUCommandType type         ; ///< the type of command
  uint64_t       parameters[8]; ///< parameters of the command
};

typedef struct GPUCommand GPUCommand;///< shortcut

/**
 * @brief This struct represents framebuffer
 */
struct GPUFramebuffer{
  Vec4*    color ; ///< color buffer
  float*   depth ; ///< depth buffer 
  uint32_t width ; ///< width of buffers
  uint32_t height; ///< height of buffers
};

typedef struct GPUFramebuffer GPUFramebuffer; ///< shortcut


/**
 * @brief This struct represents GPU data
 */
struct GPU{
  GPUBuffers       buffers           ; ///< all buffers
  GPUVertexPullers pullers           ; ///< all vertex pullers
  GPUPrograms      programs          ; ///< all programs
  VertexPullerID   activeVertexPuller; ///< active puller, if too large -> no active puller
  ProgramID        activeProgram     ; ///< active program, if too large -> no active program
  GPUFramebuffer   framebuffer       ; ///< framebuffer
  Queue            commandQueue      ; ///< command queue
  Vec4             clearColor        ; ///< clear color of framebuffer
  float            clearDepth        ; ///< clear depth of framebuffer
  int              debug             ; ///< enable debugging
};

typedef struct GPU GPU; ///< shortcut

#ifdef __cplusplus
extern "C" {
#endif

void gpu_initBuffer(GPUBuffer*const b);
void gpu_freeBuffer(GPUBuffer*const b);
void gpu_bufferData(GPUBuffer*const b,uint64_t size,void*data);
int  gpu_isBuffer(GPU const*const gpu,BufferID id);

void gpu_initBuffers(GPUBuffers *const b);
void gpu_freeBuffers(GPUBuffers *const b);

void gpu_initVertexPullerHead(GPUVertexPullerHead *const head);
void gpu_initIndices(GPUIndices *const i);
void gpu_initVertexPuller(GPUVertexPuller *const puller);
void gpu_activatePuller(GPU *const gpu,VertexPullerID id);
void gpu_deactivatePuller(GPU *const gpu);
int  gpu_isPuller(GPU const*const gpu,VertexPullerID id);

void gpu_initVertexPullers(GPUVertexPullers *const v);
void gpu_freeVertexPullers(GPUVertexPullers *const v);

void gpu_initUniform (GPUUniform* const u);
void gpu_initUniforms(GPUUniforms*const u);
void gpu_initProgram (GPUProgram* const p);
void gpu_activateProgram(GPU* const gpu,ProgramID id);
int  gpu_isProgram(GPU const*const gpu,ProgramID id);

void gpu_initPrograms(GPUPrograms*const p);
void gpu_freePrograms(GPUPrograms*const p);

void gpu_pushCommand(GPU* const gpu,GPUCommandType type);
void gpu_pushCommand1(GPU* const gpu,GPUCommandType type,uint64_t p0);
void gpu_pushCommand2(GPU* const gpu,GPUCommandType type,uint64_t p0,uint64_t p1);

void gpu_initFramebuffer(GPUFramebuffer *const f);
void gpu_allocateFramebuffer(GPUFramebuffer *const f,uint32_t width,uint32_t height);
void gpu_freeFramebuffer(GPUFramebuffer *const f);
void gpu_clearFramebuffer(GPU* const gpu);
void gpu_finish(GPU* const gpu);

GPUUniform* gpu_getUniform(GPU* const gpu,ProgramID programId,uint32_t uniformId);
uint8_t gpu_floatColorToUint32(float const value);

GPUBuffer const*gpu_getBuffer(GPU const*const gpu,BufferID id);
GPUVertexPuller const*gpu_getActivePuller(GPU const*const gpu);
GPUProgram const*gpu_getActiveProgram(GPU const*const gpu);

void gpu_drawTriangles(GPU *const gpu,uint32_t nofVertices);
void gpu_drawPoints(GPU *const gpu,uint32_t nofVertices);
void gpu_checkDraw(GPU const*const gpu,uint32_t nofVertices);

#ifdef __cplusplus
}
#endif
