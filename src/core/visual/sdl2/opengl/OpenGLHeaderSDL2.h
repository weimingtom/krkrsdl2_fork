#ifndef __ANGLE_GL_H__
#define __ANGLE_GL_H__
// Function pointers are defined here so they can be dynamically set from SDL2.
#ifdef __cplusplus
extern "C" {
#endif

/*
** Copyright (c) 2013-2015 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/
/*
** This header is generated from the Khronos OpenGL / OpenGL ES XML
** API Registry. The current version of the Registry, generator scripts
** used to make the header, and the header can be found at
**   http://www.opengl.org/registry/
**
** Khronos $Revision: 31566 $ on $Date: 2015-06-23 08:48:48 -0700 (Tue, 23 Jun 2015) $
*/

#include <KHR/khrplatform.h>
#include <EGL/eglplatform.h>

#include <GLES3/gl3platform.h>

#ifndef GL_APIENTRYP
#define GL_APIENTRYP GL_APIENTRY*
#endif

/* Generated on date 20150809 */

/* Generated C header for:
 * API: gles2
 * Profile: common
 * Versions considered: 2\.[0-9]|3\.0
 * Versions emitted: .*
 * Default extensions included: None
 * Additional extensions included: _nomatch_^
 * Extensions removed: _nomatch_^
 */

#ifndef GL_ES_VERSION_2_0
#define GL_ES_VERSION_2_0 1
#include <KHR/khrplatform.h>
typedef khronos_int8_t GLbyte;
typedef khronos_float_t GLclampf;
typedef khronos_int32_t GLfixed;
typedef short GLshort;
typedef unsigned short GLushort;
typedef void GLvoid;
typedef struct __GLsync *GLsync;
typedef khronos_int64_t GLint64;
typedef khronos_uint64_t GLuint64;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef char GLchar;
typedef khronos_float_t GLfloat;
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_intptr_t GLintptr;
typedef unsigned int GLbitfield;
typedef int GLint;
typedef unsigned char GLboolean;
typedef int GLsizei;
typedef khronos_uint8_t GLubyte;
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_FUNC_ADD                       0x8006
#define GL_BLEND_EQUATION                 0x8009
#define GL_BLEND_EQUATION_RGB             0x8009
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_BLEND_COLOR                    0x8005
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_CURRENT_VERTEX_ATTRIB          0x8626
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_FRONT_AND_BACK                 0x0408
#define GL_TEXTURE_2D                     0x0DE1
#define GL_CULL_FACE                      0x0B44
#define GL_BLEND                          0x0BE2
#define GL_DITHER                         0x0BD0
#define GL_STENCIL_TEST                   0x0B90
#define GL_DEPTH_TEST                     0x0B71
#define GL_SCISSOR_TEST                   0x0C11
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505
#define GL_CW                             0x0900
#define GL_CCW                            0x0901
#define GL_LINE_WIDTH                     0x0B21
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5
#define GL_VIEWPORT                       0x0BA2
#define GL_SCISSOR_BOX                    0x0C10
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_RED_BITS                       0x0D52
#define GL_GREEN_BITS                     0x0D53
#define GL_BLUE_BITS                      0x0D54
#define GL_ALPHA_BITS                     0x0D55
#define GL_DEPTH_BITS                     0x0D56
#define GL_STENCIL_BITS                   0x0D57
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_TEXTURE_BINDING_2D             0x8069
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_DONT_CARE                      0x1100
#define GL_FASTEST                        0x1101
#define GL_NICEST                         0x1102
#define GL_GENERATE_MIPMAP_HINT           0x8192
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_FIXED                          0x140C
#define GL_DEPTH_COMPONENT                0x1902
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_MAX_VERTEX_UNIFORM_VECTORS     0x8DFB
#define GL_MAX_VARYING_VECTORS            0x8DFC
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS   0x8DFD
#define GL_SHADER_TYPE                    0x8B4F
#define GL_DELETE_STATUS                  0x8B80
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
#define GL_INVERT                         0x150A
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_TEXTURE                        0x1702
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_MIRRORED_REPEAT                0x8370
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_COMPILE_STATUS                 0x8B81
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_SHADER_COMPILER                0x8DFA
#define GL_SHADER_BINARY_FORMATS          0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS      0x8DF9
#define GL_LOW_FLOAT                      0x8DF0
#define GL_MEDIUM_FLOAT                   0x8DF1
#define GL_HIGH_FLOAT                     0x8DF2
#define GL_LOW_INT                        0x8DF3
#define GL_MEDIUM_INT                     0x8DF4
#define GL_HIGH_INT                       0x8DF5
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RGBA4                          0x8056
#define GL_RGB5_A1                        0x8057
#define GL_RGB565                         0x8D62
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_NONE                           0
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506

#define glActiveTexture funcptr_glActiveTexture
extern void (GL_APIENTRY* glActiveTexture)(GLenum texture);
#define glAttachShader funcptr_glAttachShader
extern void (GL_APIENTRY* glAttachShader)(GLuint program, GLuint shader);
#define glBindAttribLocation funcptr_glBindAttribLocation
extern void (GL_APIENTRY* glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name);
#define glBindBuffer funcptr_glBindBuffer
extern void (GL_APIENTRY* glBindBuffer)(GLenum target, GLuint buffer);
#define glBindFramebuffer funcptr_glBindFramebuffer
extern void (GL_APIENTRY* glBindFramebuffer)(GLenum target, GLuint framebuffer);
#define glBindRenderbuffer funcptr_glBindRenderbuffer
extern void (GL_APIENTRY* glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
#define glBindTexture funcptr_glBindTexture
extern void (GL_APIENTRY* glBindTexture)(GLenum target, GLuint texture);
#define glBlendColor funcptr_glBlendColor
extern void (GL_APIENTRY* glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
#define glBlendEquation funcptr_glBlendEquation
extern void (GL_APIENTRY* glBlendEquation)(GLenum mode);
#define glBlendEquationSeparate funcptr_glBlendEquationSeparate
extern void (GL_APIENTRY* glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
#define glBlendFunc funcptr_glBlendFunc
extern void (GL_APIENTRY* glBlendFunc)(GLenum sfactor, GLenum dfactor);
#define glBlendFuncSeparate funcptr_glBlendFuncSeparate
extern void (GL_APIENTRY* glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
#define glBufferData funcptr_glBufferData
extern void (GL_APIENTRY* glBufferData)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
#define glBufferSubData funcptr_glBufferSubData
extern void (GL_APIENTRY* glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
#define glCheckFramebufferStatus funcptr_glCheckFramebufferStatus
extern GLenum (GL_APIENTRY* glCheckFramebufferStatus)(GLenum target);
#define glClear funcptr_glClear
extern void (GL_APIENTRY* glClear)(GLbitfield mask);
#define glClearColor funcptr_glClearColor
extern void (GL_APIENTRY* glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
#define glClearDepthf funcptr_glClearDepthf
extern void (GL_APIENTRY* glClearDepthf)(GLfloat d);
#define glClearStencil funcptr_glClearStencil
extern void (GL_APIENTRY* glClearStencil)(GLint s);
#define glColorMask funcptr_glColorMask
extern void (GL_APIENTRY* glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
#define glCompileShader funcptr_glCompileShader
extern void (GL_APIENTRY* glCompileShader)(GLuint shader);
#define glCompressedTexImage2D funcptr_glCompressedTexImage2D
extern void (GL_APIENTRY* glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
#define glCompressedTexSubImage2D funcptr_glCompressedTexSubImage2D
extern void (GL_APIENTRY* glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
#define glCopyTexImage2D funcptr_glCopyTexImage2D
extern void (GL_APIENTRY* glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
#define glCopyTexSubImage2D funcptr_glCopyTexSubImage2D
extern void (GL_APIENTRY* glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
#define glCreateProgram funcptr_glCreateProgram
extern GLuint (GL_APIENTRY* glCreateProgram)(void);
#define glCreateShader funcptr_glCreateShader
extern GLuint (GL_APIENTRY* glCreateShader)(GLenum type);
#define glCullFace funcptr_glCullFace
extern void (GL_APIENTRY* glCullFace)(GLenum mode);
#define glDeleteBuffers funcptr_glDeleteBuffers
extern void (GL_APIENTRY* glDeleteBuffers)(GLsizei n, const GLuint *buffers);
#define glDeleteFramebuffers funcptr_glDeleteFramebuffers
extern void (GL_APIENTRY* glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
#define glDeleteProgram funcptr_glDeleteProgram
extern void (GL_APIENTRY* glDeleteProgram)(GLuint program);
#define glDeleteRenderbuffers funcptr_glDeleteRenderbuffers
extern void (GL_APIENTRY* glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
#define glDeleteShader funcptr_glDeleteShader
extern void (GL_APIENTRY* glDeleteShader)(GLuint shader);
#define glDeleteTextures funcptr_glDeleteTextures
extern void (GL_APIENTRY* glDeleteTextures)(GLsizei n, const GLuint *textures);
#define glDepthFunc funcptr_glDepthFunc
extern void (GL_APIENTRY* glDepthFunc)(GLenum func);
#define glDepthMask funcptr_glDepthMask
extern void (GL_APIENTRY* glDepthMask)(GLboolean flag);
#define glDepthRangef funcptr_glDepthRangef
extern void (GL_APIENTRY* glDepthRangef)(GLfloat n, GLfloat f);
#define glDetachShader funcptr_glDetachShader
extern void (GL_APIENTRY* glDetachShader)(GLuint program, GLuint shader);
#define glDisable funcptr_glDisable
extern void (GL_APIENTRY* glDisable)(GLenum cap);
#define glDisableVertexAttribArray funcptr_glDisableVertexAttribArray
extern void (GL_APIENTRY* glDisableVertexAttribArray)(GLuint index);
#define glDrawArrays funcptr_glDrawArrays
extern void (GL_APIENTRY* glDrawArrays)(GLenum mode, GLint first, GLsizei count);
#define glDrawElements funcptr_glDrawElements
extern void (GL_APIENTRY* glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);
#define glEnable funcptr_glEnable
extern void (GL_APIENTRY* glEnable)(GLenum cap);
#define glEnableVertexAttribArray funcptr_glEnableVertexAttribArray
extern void (GL_APIENTRY* glEnableVertexAttribArray)(GLuint index);
#define glFinish funcptr_glFinish
extern void (GL_APIENTRY* glFinish)(void);
#define glFlush funcptr_glFlush
extern void (GL_APIENTRY* glFlush)(void);
#define glFramebufferRenderbuffer funcptr_glFramebufferRenderbuffer
extern void (GL_APIENTRY* glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
#define glFramebufferTexture2D funcptr_glFramebufferTexture2D
extern void (GL_APIENTRY* glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
#define glFrontFace funcptr_glFrontFace
extern void (GL_APIENTRY* glFrontFace)(GLenum mode);
#define glGenBuffers funcptr_glGenBuffers
extern void (GL_APIENTRY* glGenBuffers)(GLsizei n, GLuint *buffers);
#define glGenerateMipmap funcptr_glGenerateMipmap
extern void (GL_APIENTRY* glGenerateMipmap)(GLenum target);
#define glGenFramebuffers funcptr_glGenFramebuffers
extern void (GL_APIENTRY* glGenFramebuffers)(GLsizei n, GLuint *framebuffers);
#define glGenRenderbuffers funcptr_glGenRenderbuffers
extern void (GL_APIENTRY* glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
#define glGenTextures funcptr_glGenTextures
extern void (GL_APIENTRY* glGenTextures)(GLsizei n, GLuint *textures);
#define glGetActiveAttrib funcptr_glGetActiveAttrib
extern void (GL_APIENTRY* glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
#define glGetActiveUniform funcptr_glGetActiveUniform
extern void (GL_APIENTRY* glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
#define glGetAttachedShaders funcptr_glGetAttachedShaders
extern void (GL_APIENTRY* glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
#define glGetAttribLocation funcptr_glGetAttribLocation
extern GLint (GL_APIENTRY* glGetAttribLocation)(GLuint program, const GLchar *name);
#define glGetBooleanv funcptr_glGetBooleanv
extern void (GL_APIENTRY* glGetBooleanv)(GLenum pname, GLboolean *data);
#define glGetBufferParameteriv funcptr_glGetBufferParameteriv
extern void (GL_APIENTRY* glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
#define glGetError funcptr_glGetError
extern GLenum (GL_APIENTRY* glGetError)(void);
#define glGetFloatv funcptr_glGetFloatv
extern void (GL_APIENTRY* glGetFloatv)(GLenum pname, GLfloat *data);
#define glGetFramebufferAttachmentParameteriv funcptr_glGetFramebufferAttachmentParameteriv
extern void (GL_APIENTRY* glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
#define glGetIntegerv funcptr_glGetIntegerv
extern void (GL_APIENTRY* glGetIntegerv)(GLenum pname, GLint *data);
#define glGetProgramiv funcptr_glGetProgramiv
extern void (GL_APIENTRY* glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
#define glGetProgramInfoLog funcptr_glGetProgramInfoLog
extern void (GL_APIENTRY* glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
#define glGetRenderbufferParameteriv funcptr_glGetRenderbufferParameteriv
extern void (GL_APIENTRY* glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
#define glGetShaderiv funcptr_glGetShaderiv
extern void (GL_APIENTRY* glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
#define glGetShaderInfoLog funcptr_glGetShaderInfoLog
extern void (GL_APIENTRY* glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
#define glGetShaderPrecisionFormat funcptr_glGetShaderPrecisionFormat
extern void (GL_APIENTRY* glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
#define glGetShaderSource funcptr_glGetShaderSource
extern void (GL_APIENTRY* glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
#define glGetString funcptr_glGetString
extern const GLubyte *(GL_APIENTRY* glGetString)(GLenum name);
#define glGetTexParameterfv funcptr_glGetTexParameterfv
extern void (GL_APIENTRY* glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
#define glGetTexParameteriv funcptr_glGetTexParameteriv
extern void (GL_APIENTRY* glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
#define glGetUniformfv funcptr_glGetUniformfv
extern void (GL_APIENTRY* glGetUniformfv)(GLuint program, GLint location, GLfloat *params);
#define glGetUniformiv funcptr_glGetUniformiv
extern void (GL_APIENTRY* glGetUniformiv)(GLuint program, GLint location, GLint *params);
#define glGetUniformLocation funcptr_glGetUniformLocation
extern GLint (GL_APIENTRY* glGetUniformLocation)(GLuint program, const GLchar *name);
#define glGetVertexAttribfv funcptr_glGetVertexAttribfv
extern void (GL_APIENTRY* glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);
#define glGetVertexAttribiv funcptr_glGetVertexAttribiv
extern void (GL_APIENTRY* glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);
#define glGetVertexAttribPointerv funcptr_glGetVertexAttribPointerv
extern void (GL_APIENTRY* glGetVertexAttribPointerv)(GLuint index, GLenum pname, void **pointer);
#define glHint funcptr_glHint
extern void (GL_APIENTRY* glHint)(GLenum target, GLenum mode);
#define glIsBuffer funcptr_glIsBuffer
extern GLboolean (GL_APIENTRY* glIsBuffer)(GLuint buffer);
#define glIsEnabled funcptr_glIsEnabled
extern GLboolean (GL_APIENTRY* glIsEnabled)(GLenum cap);
#define glIsFramebuffer funcptr_glIsFramebuffer
extern GLboolean (GL_APIENTRY* glIsFramebuffer)(GLuint framebuffer);
#define glIsProgram funcptr_glIsProgram
extern GLboolean (GL_APIENTRY* glIsProgram)(GLuint program);
#define glIsRenderbuffer funcptr_glIsRenderbuffer
extern GLboolean (GL_APIENTRY* glIsRenderbuffer)(GLuint renderbuffer);
#define glIsShader funcptr_glIsShader
extern GLboolean (GL_APIENTRY* glIsShader)(GLuint shader);
#define glIsTexture funcptr_glIsTexture
extern GLboolean (GL_APIENTRY* glIsTexture)(GLuint texture);
#define glLineWidth funcptr_glLineWidth
extern void (GL_APIENTRY* glLineWidth)(GLfloat width);
#define glLinkProgram funcptr_glLinkProgram
extern void (GL_APIENTRY* glLinkProgram)(GLuint program);
#define glPixelStorei funcptr_glPixelStorei
extern void (GL_APIENTRY* glPixelStorei)(GLenum pname, GLint param);
#define glPolygonOffset funcptr_glPolygonOffset
extern void (GL_APIENTRY* glPolygonOffset)(GLfloat factor, GLfloat units);
#define glReadPixels funcptr_glReadPixels
extern void (GL_APIENTRY* glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
#define glReleaseShaderCompiler funcptr_glReleaseShaderCompiler
extern void (GL_APIENTRY* glReleaseShaderCompiler)(void);
#define glRenderbufferStorage funcptr_glRenderbufferStorage
extern void (GL_APIENTRY* glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
#define glSampleCoverage funcptr_glSampleCoverage
extern void (GL_APIENTRY* glSampleCoverage)(GLfloat value, GLboolean invert);
#define glScissor funcptr_glScissor
extern void (GL_APIENTRY* glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
#define glShaderBinary funcptr_glShaderBinary
extern void (GL_APIENTRY* glShaderBinary)(GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
#define glShaderSource funcptr_glShaderSource
extern void (GL_APIENTRY* glShaderSource)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
#define glStencilFunc funcptr_glStencilFunc
extern void (GL_APIENTRY* glStencilFunc)(GLenum func, GLint ref, GLuint mask);
#define glStencilFuncSeparate funcptr_glStencilFuncSeparate
extern void (GL_APIENTRY* glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
#define glStencilMask funcptr_glStencilMask
extern void (GL_APIENTRY* glStencilMask)(GLuint mask);
#define glStencilMaskSeparate funcptr_glStencilMaskSeparate
extern void (GL_APIENTRY* glStencilMaskSeparate)(GLenum face, GLuint mask);
#define glStencilOp funcptr_glStencilOp
extern void (GL_APIENTRY* glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
#define glStencilOpSeparate funcptr_glStencilOpSeparate
extern void (GL_APIENTRY* glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
#define glTexImage2D funcptr_glTexImage2D
extern void (GL_APIENTRY* glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
#define glTexParameterf funcptr_glTexParameterf
extern void (GL_APIENTRY* glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
#define glTexParameterfv funcptr_glTexParameterfv
extern void (GL_APIENTRY* glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
#define glTexParameteri funcptr_glTexParameteri
extern void (GL_APIENTRY* glTexParameteri)(GLenum target, GLenum pname, GLint param);
#define glTexParameteriv funcptr_glTexParameteriv
extern void (GL_APIENTRY* glTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
#define glTexSubImage2D funcptr_glTexSubImage2D
extern void (GL_APIENTRY* glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
#define glUniform1f funcptr_glUniform1f
extern void (GL_APIENTRY* glUniform1f)(GLint location, GLfloat v0);
#define glUniform1fv funcptr_glUniform1fv
extern void (GL_APIENTRY* glUniform1fv)(GLint location, GLsizei count, const GLfloat *value);
#define glUniform1i funcptr_glUniform1i
extern void (GL_APIENTRY* glUniform1i)(GLint location, GLint v0);
#define glUniform1iv funcptr_glUniform1iv
extern void (GL_APIENTRY* glUniform1iv)(GLint location, GLsizei count, const GLint *value);
#define glUniform2f funcptr_glUniform2f
extern void (GL_APIENTRY* glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
#define glUniform2fv funcptr_glUniform2fv
extern void (GL_APIENTRY* glUniform2fv)(GLint location, GLsizei count, const GLfloat *value);
#define glUniform2i funcptr_glUniform2i
extern void (GL_APIENTRY* glUniform2i)(GLint location, GLint v0, GLint v1);
#define glUniform2iv funcptr_glUniform2iv
extern void (GL_APIENTRY* glUniform2iv)(GLint location, GLsizei count, const GLint *value);
#define glUniform3f funcptr_glUniform3f
extern void (GL_APIENTRY* glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
#define glUniform3fv funcptr_glUniform3fv
extern void (GL_APIENTRY* glUniform3fv)(GLint location, GLsizei count, const GLfloat *value);
#define glUniform3i funcptr_glUniform3i
extern void (GL_APIENTRY* glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
#define glUniform3iv funcptr_glUniform3iv
extern void (GL_APIENTRY* glUniform3iv)(GLint location, GLsizei count, const GLint *value);
#define glUniform4f funcptr_glUniform4f
extern void (GL_APIENTRY* glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
#define glUniform4fv funcptr_glUniform4fv
extern void (GL_APIENTRY* glUniform4fv)(GLint location, GLsizei count, const GLfloat *value);
#define glUniform4i funcptr_glUniform4i
extern void (GL_APIENTRY* glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
#define glUniform4iv funcptr_glUniform4iv
extern void (GL_APIENTRY* glUniform4iv)(GLint location, GLsizei count, const GLint *value);
#define glUniformMatrix2fv funcptr_glUniformMatrix2fv
extern void (GL_APIENTRY* glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#define glUniformMatrix3fv funcptr_glUniformMatrix3fv
extern void (GL_APIENTRY* glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#define glUniformMatrix4fv funcptr_glUniformMatrix4fv
extern void (GL_APIENTRY* glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#define glUseProgram funcptr_glUseProgram
extern void (GL_APIENTRY* glUseProgram)(GLuint program);
#define glValidateProgram funcptr_glValidateProgram
extern void (GL_APIENTRY* glValidateProgram)(GLuint program);
#define glVertexAttrib1f funcptr_glVertexAttrib1f
extern void (GL_APIENTRY* glVertexAttrib1f)(GLuint index, GLfloat x);
#define glVertexAttrib1fv funcptr_glVertexAttrib1fv
extern void (GL_APIENTRY* glVertexAttrib1fv)(GLuint index, const GLfloat *v);
#define glVertexAttrib2f funcptr_glVertexAttrib2f
extern void (GL_APIENTRY* glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
#define glVertexAttrib2fv funcptr_glVertexAttrib2fv
extern void (GL_APIENTRY* glVertexAttrib2fv)(GLuint index, const GLfloat *v);
#define glVertexAttrib3f funcptr_glVertexAttrib3f
extern void (GL_APIENTRY* glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
#define glVertexAttrib3fv funcptr_glVertexAttrib3fv
extern void (GL_APIENTRY* glVertexAttrib3fv)(GLuint index, const GLfloat *v);
#define glVertexAttrib4f funcptr_glVertexAttrib4f
extern void (GL_APIENTRY* glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
#define glVertexAttrib4fv funcptr_glVertexAttrib4fv
extern void (GL_APIENTRY* glVertexAttrib4fv)(GLuint index, const GLfloat *v);
#define glVertexAttribPointer funcptr_glVertexAttribPointer
extern void (GL_APIENTRY* glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
#define glViewport funcptr_glViewport
extern void (GL_APIENTRY* glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);

#endif /* GL_ES_VERSION_2_0 */

#ifndef GL_ES_VERSION_3_0
#define GL_ES_VERSION_3_0 1
typedef unsigned short GLhalf;
#define GL_READ_BUFFER                    0x0C02
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_PACK_ROW_LENGTH                0x0D02
#define GL_PACK_SKIP_ROWS                 0x0D03
#define GL_PACK_SKIP_PIXELS               0x0D04
#define GL_COLOR                          0x1800
#define GL_DEPTH                          0x1801
#define GL_STENCIL                        0x1802
#define GL_RED                            0x1903
#define GL_RGB8                           0x8051
#define GL_RGBA8                          0x8058
#define GL_RGB10_A2                       0x8059
#define GL_TEXTURE_BINDING_3D             0x806A
#define GL_UNPACK_SKIP_IMAGES             0x806D
#define GL_UNPACK_IMAGE_HEIGHT            0x806E
#define GL_TEXTURE_3D                     0x806F
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_MAX_TEXTURE_LOD_BIAS           0x84FD
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_CURRENT_QUERY                  0x8865
#define GL_QUERY_RESULT                   0x8866
#define GL_QUERY_RESULT_AVAILABLE         0x8867
#define GL_BUFFER_MAPPED                  0x88BC
#define GL_BUFFER_MAP_POINTER             0x88BD
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_DRAW_BUFFER0                   0x8825
#define GL_DRAW_BUFFER1                   0x8826
#define GL_DRAW_BUFFER2                   0x8827
#define GL_DRAW_BUFFER3                   0x8828
#define GL_DRAW_BUFFER4                   0x8829
#define GL_DRAW_BUFFER5                   0x882A
#define GL_DRAW_BUFFER6                   0x882B
#define GL_DRAW_BUFFER7                   0x882C
#define GL_DRAW_BUFFER8                   0x882D
#define GL_DRAW_BUFFER9                   0x882E
#define GL_DRAW_BUFFER10                  0x882F
#define GL_DRAW_BUFFER11                  0x8830
#define GL_DRAW_BUFFER12                  0x8831
#define GL_DRAW_BUFFER13                  0x8832
#define GL_DRAW_BUFFER14                  0x8833
#define GL_DRAW_BUFFER15                  0x8834
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_2D_SHADOW              0x8B62
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING      0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING    0x88EF
#define GL_FLOAT_MAT2x3                   0x8B65
#define GL_FLOAT_MAT2x4                   0x8B66
#define GL_FLOAT_MAT3x2                   0x8B67
#define GL_FLOAT_MAT3x4                   0x8B68
#define GL_FLOAT_MAT4x2                   0x8B69
#define GL_FLOAT_MAT4x3                   0x8B6A
#define GL_SRGB                           0x8C40
#define GL_SRGB8                          0x8C41
#define GL_SRGB8_ALPHA8                   0x8C43
#define GL_COMPARE_REF_TO_TEXTURE         0x884E
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGBA16F                        0x881A
#define GL_RGB16F                         0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER    0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS       0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET       0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET       0x8905
#define GL_MAX_VARYING_COMPONENTS         0x8B4B
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_TEXTURE_BINDING_2D_ARRAY       0x8C1D
#define GL_R11F_G11F_B10F                 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV   0x8C3B
#define GL_RGB9_E5                        0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV       0x8C3E
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS    0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD             0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS            0x8C8C
#define GL_SEPARATE_ATTRIBS               0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER      0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI                       0x8D70
#define GL_RGB32UI                        0x8D71
#define GL_RGBA16UI                       0x8D76
#define GL_RGB16UI                        0x8D77
#define GL_RGBA8UI                        0x8D7C
#define GL_RGB8UI                         0x8D7D
#define GL_RGBA32I                        0x8D82
#define GL_RGB32I                         0x8D83
#define GL_RGBA16I                        0x8D88
#define GL_RGB16I                         0x8D89
#define GL_RGBA8I                         0x8D8E
#define GL_RGB8I                          0x8D8F
#define GL_RED_INTEGER                    0x8D94
#define GL_RGB_INTEGER                    0x8D98
#define GL_RGBA_INTEGER                   0x8D99
#define GL_SAMPLER_2D_ARRAY               0x8DC1
#define GL_SAMPLER_2D_ARRAY_SHADOW        0x8DC4
#define GL_SAMPLER_CUBE_SHADOW            0x8DC5
#define GL_UNSIGNED_INT_VEC2              0x8DC6
#define GL_UNSIGNED_INT_VEC3              0x8DC7
#define GL_UNSIGNED_INT_VEC4              0x8DC8
#define GL_INT_SAMPLER_2D                 0x8DCA
#define GL_INT_SAMPLER_3D                 0x8DCB
#define GL_INT_SAMPLER_CUBE               0x8DCC
#define GL_INT_SAMPLER_2D_ARRAY           0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_2D        0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D        0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE      0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY  0x8DD7
#define GL_BUFFER_ACCESS_FLAGS            0x911F
#define GL_BUFFER_MAP_LENGTH              0x9120
#define GL_BUFFER_MAP_OFFSET              0x9121
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_DEPTH32F_STENCIL8              0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT            0x8218
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_UNSIGNED_NORMALIZED            0x8C17
#define GL_DRAW_FRAMEBUFFER_BINDING       0x8CA6
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       0x8CAA
#define GL_RENDERBUFFER_SAMPLES           0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_COLOR_ATTACHMENT16             0x8CF0
#define GL_COLOR_ATTACHMENT17             0x8CF1
#define GL_COLOR_ATTACHMENT18             0x8CF2
#define GL_COLOR_ATTACHMENT19             0x8CF3
#define GL_COLOR_ATTACHMENT20             0x8CF4
#define GL_COLOR_ATTACHMENT21             0x8CF5
#define GL_COLOR_ATTACHMENT22             0x8CF6
#define GL_COLOR_ATTACHMENT23             0x8CF7
#define GL_COLOR_ATTACHMENT24             0x8CF8
#define GL_COLOR_ATTACHMENT25             0x8CF9
#define GL_COLOR_ATTACHMENT26             0x8CFA
#define GL_COLOR_ATTACHMENT27             0x8CFB
#define GL_COLOR_ATTACHMENT28             0x8CFC
#define GL_COLOR_ATTACHMENT29             0x8CFD
#define GL_COLOR_ATTACHMENT30             0x8CFE
#define GL_COLOR_ATTACHMENT31             0x8CFF
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES                    0x8D57
#define GL_HALF_FLOAT                     0x140B
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT       0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT      0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT         0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT         0x0020
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_RG8                            0x822B
#define GL_R16F                           0x822D
#define GL_R32F                           0x822E
#define GL_RG16F                          0x822F
#define GL_RG32F                          0x8230
#define GL_R8I                            0x8231
#define GL_R8UI                           0x8232
#define GL_R16I                           0x8233
#define GL_R16UI                          0x8234
#define GL_R32I                           0x8235
#define GL_R32UI                          0x8236
#define GL_RG8I                           0x8237
#define GL_RG8UI                          0x8238
#define GL_RG16I                          0x8239
#define GL_RG16UI                         0x823A
#define GL_RG32I                          0x823B
#define GL_RG32UI                         0x823C
#define GL_VERTEX_ARRAY_BINDING           0x85B5
#define GL_R8_SNORM                       0x8F94
#define GL_RG8_SNORM                      0x8F95
#define GL_RGB8_SNORM                     0x8F96
#define GL_RGBA8_SNORM                    0x8F97
#define GL_SIGNED_NORMALIZED              0x8F9C
#define GL_PRIMITIVE_RESTART_FIXED_INDEX  0x8D69
#define GL_COPY_READ_BUFFER               0x8F36
#define GL_COPY_WRITE_BUFFER              0x8F37
#define GL_COPY_READ_BUFFER_BINDING       0x8F36
#define GL_COPY_WRITE_BUFFER_BINDING      0x8F37
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_UNIFORM_BUFFER_BINDING         0x8A28
#define GL_UNIFORM_BUFFER_START           0x8A29
#define GL_UNIFORM_BUFFER_SIZE            0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS      0x8A2B
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS    0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS    0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS    0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE         0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS          0x8A36
#define GL_UNIFORM_TYPE                   0x8A37
#define GL_UNIFORM_SIZE                   0x8A38
#define GL_UNIFORM_NAME_LENGTH            0x8A39
#define GL_UNIFORM_BLOCK_INDEX            0x8A3A
#define GL_UNIFORM_OFFSET                 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE           0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE          0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR           0x8A3E
#define GL_UNIFORM_BLOCK_BINDING          0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE        0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH      0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS  0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX                  0xFFFFFFFFu
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS   0x9122
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS  0x9125
#define GL_MAX_SERVER_WAIT_TIMEOUT        0x9111
#define GL_OBJECT_TYPE                    0x9112
#define GL_SYNC_CONDITION                 0x9113
#define GL_SYNC_STATUS                    0x9114
#define GL_SYNC_FLAGS                     0x9115
#define GL_SYNC_FENCE                     0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE     0x9117
#define GL_UNSIGNALED                     0x9118
#define GL_SIGNALED                       0x9119
#define GL_ALREADY_SIGNALED               0x911A
#define GL_TIMEOUT_EXPIRED                0x911B
#define GL_CONDITION_SATISFIED            0x911C
#define GL_WAIT_FAILED                    0x911D
#define GL_SYNC_FLUSH_COMMANDS_BIT        0x00000001
#define GL_TIMEOUT_IGNORED                0xFFFFFFFFFFFFFFFFull
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR    0x88FE
#define GL_ANY_SAMPLES_PASSED             0x8C2F
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_SAMPLER_BINDING                0x8919
#define GL_RGB10_A2UI                     0x906F
#define GL_TEXTURE_SWIZZLE_R              0x8E42
#define GL_TEXTURE_SWIZZLE_G              0x8E43
#define GL_TEXTURE_SWIZZLE_B              0x8E44
#define GL_TEXTURE_SWIZZLE_A              0x8E45
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_INT_2_10_10_10_REV             0x8D9F
#define GL_TRANSFORM_FEEDBACK             0x8E22
#define GL_TRANSFORM_FEEDBACK_PAUSED      0x8E23
#define GL_TRANSFORM_FEEDBACK_ACTIVE      0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING     0x8E25
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH          0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS     0x87FE
#define GL_PROGRAM_BINARY_FORMATS         0x87FF
#define GL_COMPRESSED_R11_EAC             0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC      0x9271
#define GL_COMPRESSED_RG11_EAC            0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC     0x9273
#define GL_COMPRESSED_RGB8_ETC2           0x9274
#define GL_COMPRESSED_SRGB8_ETC2          0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC      0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279
#define GL_TEXTURE_IMMUTABLE_FORMAT       0x912F
#define GL_MAX_ELEMENT_INDEX              0x8D6B
#define GL_NUM_SAMPLE_COUNTS              0x9380
#define GL_TEXTURE_IMMUTABLE_LEVELS       0x82DF

#define glReadBuffer funcptr_glReadBuffer
extern void (GL_APIENTRY* glReadBuffer)(GLenum src);
#define glDrawRangeElements funcptr_glDrawRangeElements
extern void (GL_APIENTRY* glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
#define glTexImage3D funcptr_glTexImage3D
extern void (GL_APIENTRY* glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
#define glTexSubImage3D funcptr_glTexSubImage3D
extern void (GL_APIENTRY* glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
#define glCopyTexSubImage3D funcptr_glCopyTexSubImage3D
extern void (GL_APIENTRY* glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
#define glCompressedTexImage3D funcptr_glCompressedTexImage3D
extern void (GL_APIENTRY* glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
#define glCompressedTexSubImage3D funcptr_glCompressedTexSubImage3D
extern void (GL_APIENTRY* glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
#define glGenQueries funcptr_glGenQueries
extern void (GL_APIENTRY* glGenQueries)(GLsizei n, GLuint *ids);
#define glDeleteQueries funcptr_glDeleteQueries
extern void (GL_APIENTRY* glDeleteQueries)(GLsizei n, const GLuint *ids);
#define glIsQuery funcptr_glIsQuery
extern GLboolean (GL_APIENTRY* glIsQuery)(GLuint id);
#define glBeginQuery funcptr_glBeginQuery
extern void (GL_APIENTRY* glBeginQuery)(GLenum target, GLuint id);
#define glEndQuery funcptr_glEndQuery
extern void (GL_APIENTRY* glEndQuery)(GLenum target);
#define glGetQueryiv funcptr_glGetQueryiv
extern void (GL_APIENTRY* glGetQueryiv)(GLenum target, GLenum pname, GLint *params);
#define glGetQueryObjectuiv funcptr_glGetQueryObjectuiv
extern void (GL_APIENTRY* glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
#define glUnmapBuffer funcptr_glUnmapBuffer
extern GLboolean (GL_APIENTRY* glUnmapBuffer)(GLenum target);
#define glGetBufferPointerv funcptr_glGetBufferPointerv
extern void (GL_APIENTRY* glGetBufferPointerv)(GLenum target, GLenum pname, void **params);
#define glDrawBuffers funcptr_glDrawBuffers
extern void (GL_APIENTRY* glDrawBuffers)(GLsizei n, const GLenum *bufs);
#define glUniformMatrix2x3fv funcptr_glUniformMatrix2x3fv
extern void (GL_APIENTRY* glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#define glUniformMatrix3x2fv funcptr_glUniformMatrix3x2fv
extern void (GL_APIENTRY* glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#define glUniformMatrix2x4fv funcptr_glUniformMatrix2x4fv
extern void (GL_APIENTRY* glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#define glUniformMatrix4x2fv funcptr_glUniformMatrix4x2fv
extern void (GL_APIENTRY* glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#define glUniformMatrix3x4fv funcptr_glUniformMatrix3x4fv
extern void (GL_APIENTRY* glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#define glUniformMatrix4x3fv funcptr_glUniformMatrix4x3fv
extern void (GL_APIENTRY* glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#define glBlitFramebuffer funcptr_glBlitFramebuffer
extern void (GL_APIENTRY* glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
#define glRenderbufferStorageMultisample funcptr_glRenderbufferStorageMultisample
extern void (GL_APIENTRY* glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
#define glFramebufferTextureLayer funcptr_glFramebufferTextureLayer
extern void (GL_APIENTRY* glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
#define glMapBufferRange funcptr_glMapBufferRange
extern void *(GL_APIENTRY* glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
#define glFlushMappedBufferRange funcptr_glFlushMappedBufferRange
extern void (GL_APIENTRY* glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
#define glBindVertexArray funcptr_glBindVertexArray
extern void (GL_APIENTRY* glBindVertexArray)(GLuint array);
#define glDeleteVertexArrays funcptr_glDeleteVertexArrays
extern void (GL_APIENTRY* glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
#define glGenVertexArrays funcptr_glGenVertexArrays
extern void (GL_APIENTRY* glGenVertexArrays)(GLsizei n, GLuint *arrays);
#define glIsVertexArray funcptr_glIsVertexArray
extern GLboolean (GL_APIENTRY* glIsVertexArray)(GLuint array);
#define glGetIntegeri_v funcptr_glGetIntegeri_v
extern void (GL_APIENTRY* glGetIntegeri_v)(GLenum target, GLuint index, GLint *data);
#define glBeginTransformFeedback funcptr_glBeginTransformFeedback
extern void (GL_APIENTRY* glBeginTransformFeedback)(GLenum primitiveMode);
#define glEndTransformFeedback funcptr_glEndTransformFeedback
extern void (GL_APIENTRY* glEndTransformFeedback)(void);
#define glBindBufferRange funcptr_glBindBufferRange
extern void (GL_APIENTRY* glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
#define glBindBufferBase funcptr_glBindBufferBase
extern void (GL_APIENTRY* glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
#define glTransformFeedbackVaryings funcptr_glTransformFeedbackVaryings
extern void (GL_APIENTRY* glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
#define glGetTransformFeedbackVarying funcptr_glGetTransformFeedbackVarying
extern void (GL_APIENTRY* glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
#define glVertexAttribIPointer funcptr_glVertexAttribIPointer
extern void (GL_APIENTRY* glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
#define glGetVertexAttribIiv funcptr_glGetVertexAttribIiv
extern void (GL_APIENTRY* glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
#define glGetVertexAttribIuiv funcptr_glGetVertexAttribIuiv
extern void (GL_APIENTRY* glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
#define glVertexAttribI4i funcptr_glVertexAttribI4i
extern void (GL_APIENTRY* glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
#define glVertexAttribI4ui funcptr_glVertexAttribI4ui
extern void (GL_APIENTRY* glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
#define glVertexAttribI4iv funcptr_glVertexAttribI4iv
extern void (GL_APIENTRY* glVertexAttribI4iv)(GLuint index, const GLint *v);
#define glVertexAttribI4uiv funcptr_glVertexAttribI4uiv
extern void (GL_APIENTRY* glVertexAttribI4uiv)(GLuint index, const GLuint *v);
#define glGetUniformuiv funcptr_glGetUniformuiv
extern void (GL_APIENTRY* glGetUniformuiv)(GLuint program, GLint location, GLuint *params);
#define glGetFragDataLocation funcptr_glGetFragDataLocation
extern GLint (GL_APIENTRY* glGetFragDataLocation)(GLuint program, const GLchar *name);
#define glUniform1ui funcptr_glUniform1ui
extern void (GL_APIENTRY* glUniform1ui)(GLint location, GLuint v0);
#define glUniform2ui funcptr_glUniform2ui
extern void (GL_APIENTRY* glUniform2ui)(GLint location, GLuint v0, GLuint v1);
#define glUniform3ui funcptr_glUniform3ui
extern void (GL_APIENTRY* glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
#define glUniform4ui funcptr_glUniform4ui
extern void (GL_APIENTRY* glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
#define glUniform1uiv funcptr_glUniform1uiv
extern void (GL_APIENTRY* glUniform1uiv)(GLint location, GLsizei count, const GLuint *value);
#define glUniform2uiv funcptr_glUniform2uiv
extern void (GL_APIENTRY* glUniform2uiv)(GLint location, GLsizei count, const GLuint *value);
#define glUniform3uiv funcptr_glUniform3uiv
extern void (GL_APIENTRY* glUniform3uiv)(GLint location, GLsizei count, const GLuint *value);
#define glUniform4uiv funcptr_glUniform4uiv
extern void (GL_APIENTRY* glUniform4uiv)(GLint location, GLsizei count, const GLuint *value);
#define glClearBufferiv funcptr_glClearBufferiv
extern void (GL_APIENTRY* glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value);
#define glClearBufferuiv funcptr_glClearBufferuiv
extern void (GL_APIENTRY* glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value);
#define glClearBufferfv funcptr_glClearBufferfv
extern void (GL_APIENTRY* glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
#define glClearBufferfi funcptr_glClearBufferfi
extern void (GL_APIENTRY* glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
#define glGetStringi funcptr_glGetStringi
extern const GLubyte *(GL_APIENTRY* glGetStringi)(GLenum name, GLuint index);
#define glCopyBufferSubData funcptr_glCopyBufferSubData
extern void (GL_APIENTRY* glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
#define glGetUniformIndices funcptr_glGetUniformIndices
extern void (GL_APIENTRY* glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
#define glGetActiveUniformsiv funcptr_glGetActiveUniformsiv
extern void (GL_APIENTRY* glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
#define glGetUniformBlockIndex funcptr_glGetUniformBlockIndex
extern GLuint (GL_APIENTRY* glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);
#define glGetActiveUniformBlockiv funcptr_glGetActiveUniformBlockiv
extern void (GL_APIENTRY* glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
#define glGetActiveUniformBlockName funcptr_glGetActiveUniformBlockName
extern void (GL_APIENTRY* glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
#define glUniformBlockBinding funcptr_glUniformBlockBinding
extern void (GL_APIENTRY* glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
#define glDrawArraysInstanced funcptr_glDrawArraysInstanced
extern void (GL_APIENTRY* glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
#define glDrawElementsInstanced funcptr_glDrawElementsInstanced
extern void (GL_APIENTRY* glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
#define glFenceSync funcptr_glFenceSync
extern GLsync (GL_APIENTRY* glFenceSync)(GLenum condition, GLbitfield flags);
#define glIsSync funcptr_glIsSync
extern GLboolean (GL_APIENTRY* glIsSync)(GLsync sync);
#define glDeleteSync funcptr_glDeleteSync
extern void (GL_APIENTRY* glDeleteSync)(GLsync sync);
#define glClientWaitSync funcptr_glClientWaitSync
extern GLenum (GL_APIENTRY* glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
#define glWaitSync funcptr_glWaitSync
extern void (GL_APIENTRY* glWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
#define glGetInteger64v funcptr_glGetInteger64v
extern void (GL_APIENTRY* glGetInteger64v)(GLenum pname, GLint64 *data);
#define glGetSynciv funcptr_glGetSynciv
extern void (GL_APIENTRY* glGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
#define glGetInteger64i_v funcptr_glGetInteger64i_v
extern void (GL_APIENTRY* glGetInteger64i_v)(GLenum target, GLuint index, GLint64 *data);
#define glGetBufferParameteri64v funcptr_glGetBufferParameteri64v
extern void (GL_APIENTRY* glGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64 *params);
#define glGenSamplers funcptr_glGenSamplers
extern void (GL_APIENTRY* glGenSamplers)(GLsizei count, GLuint *samplers);
#define glDeleteSamplers funcptr_glDeleteSamplers
extern void (GL_APIENTRY* glDeleteSamplers)(GLsizei count, const GLuint *samplers);
#define glIsSampler funcptr_glIsSampler
extern GLboolean (GL_APIENTRY* glIsSampler)(GLuint sampler);
#define glBindSampler funcptr_glBindSampler
extern void (GL_APIENTRY* glBindSampler)(GLuint unit, GLuint sampler);
#define glSamplerParameteri funcptr_glSamplerParameteri
extern void (GL_APIENTRY* glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
#define glSamplerParameteriv funcptr_glSamplerParameteriv
extern void (GL_APIENTRY* glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param);
#define glSamplerParameterf funcptr_glSamplerParameterf
extern void (GL_APIENTRY* glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
#define glSamplerParameterfv funcptr_glSamplerParameterfv
extern void (GL_APIENTRY* glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param);
#define glGetSamplerParameteriv funcptr_glGetSamplerParameteriv
extern void (GL_APIENTRY* glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params);
#define glGetSamplerParameterfv funcptr_glGetSamplerParameterfv
extern void (GL_APIENTRY* glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params);
#define glVertexAttribDivisor funcptr_glVertexAttribDivisor
extern void (GL_APIENTRY* glVertexAttribDivisor)(GLuint index, GLuint divisor);
#define glBindTransformFeedback funcptr_glBindTransformFeedback
extern void (GL_APIENTRY* glBindTransformFeedback)(GLenum target, GLuint id);
#define glDeleteTransformFeedbacks funcptr_glDeleteTransformFeedbacks
extern void (GL_APIENTRY* glDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);
#define glGenTransformFeedbacks funcptr_glGenTransformFeedbacks
extern void (GL_APIENTRY* glGenTransformFeedbacks)(GLsizei n, GLuint *ids);
#define glIsTransformFeedback funcptr_glIsTransformFeedback
extern GLboolean (GL_APIENTRY* glIsTransformFeedback)(GLuint id);
#define glPauseTransformFeedback funcptr_glPauseTransformFeedback
extern void (GL_APIENTRY* glPauseTransformFeedback)(void);
#define glResumeTransformFeedback funcptr_glResumeTransformFeedback
extern void (GL_APIENTRY* glResumeTransformFeedback)(void);
#define glGetProgramBinary funcptr_glGetProgramBinary
extern void (GL_APIENTRY* glGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
#define glProgramBinary funcptr_glProgramBinary
extern void (GL_APIENTRY* glProgramBinary)(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
#define glProgramParameteri funcptr_glProgramParameteri
extern void (GL_APIENTRY* glProgramParameteri)(GLuint program, GLenum pname, GLint value);
#define glInvalidateFramebuffer funcptr_glInvalidateFramebuffer
extern void (GL_APIENTRY* glInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
#define glInvalidateSubFramebuffer funcptr_glInvalidateSubFramebuffer
extern void (GL_APIENTRY* glInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
#define glTexStorage2D funcptr_glTexStorage2D
extern void (GL_APIENTRY* glTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
#define glTexStorage3D funcptr_glTexStorage3D
extern void (GL_APIENTRY* glTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
#define glGetInternalformativ funcptr_glGetInternalformativ
extern void (GL_APIENTRY* glGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);

#endif /* GL_ES_VERSION_3_0 */

#ifdef __cplusplus
}
#endif

extern bool LoadLibEGL( const tjs_string& dllpath );
extern bool LoadLibGLESv2( const tjs_string& dllpath );
#endif
