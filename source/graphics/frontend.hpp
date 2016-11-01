// @date 2016/10/26
// @author Mao Jingkai(oammix@gmail.com)

#pragma once

#include <forwards.hpp>
#include <core/core.hpp>

#include <graphics/graphics.hpp>
#include <graphics/drawcall.hpp>
#include <graphics/state.hpp>

#include <codebase/handle_object_set.hpp>

#include <math/color.hpp>
#include <math/string_hash.hpp>

#include <string>
#include <atomic>

NS_LEMON_GRAPHICS_BEGIN

//
// Here are some general thoughts about the designs of renderer.
// 
// First of all, we follows a data-oriented approach, and uses a task
// scheduler to perform multithread entities resolving. its much more
// convenient to provide mechanism to make video resource preparations
// and drawcall submits in different threads.
// 
// * Some tips here: in most cases dividing OpenGL rendering across
// multiple threads will not result in any draw performance improvement,
// due to the pipeline nature of OpenGL.
// 
// The most common solution is by using a double-buffer of commands.
// This consists of running the renderer backend in a speparate thread,
// where all draw calls and communication with the OpenGL API are
// performed. The frontend thread that runs the game logic communicates
// with the backend renderer via a command double-buffer. 
// 
// And its important to sort video commands (generated by different
// threads) before submiting them to OpenGL, for the sack of both
// correctness and performance. For example, to draw transparent objects
// via blending, we need draw opaque object first, usually from
// front-to-back, and draw translucents from back-to-front. We can
// achieve this point by sorting commands before swaping command-buffer.
// 
// There are some assumptions here to keep codes as simple as possible, this
// sometimes creates undesired results.
// 1. All the resource creations and manipulations will be happened before
// real draw call;
// 2. All the resource destructions will be finished after real draw call.
// 
// As a optimation, we use a key associated with every-drawcall to sort.
// Typically, a key is just a single 32-bit or 64-bit integer encoded
// certain data like distance, material, shader, etc. of a draw call in
// individual bits. Depending on where those bits are stored in the key,
// we can apply different sorting criteria for the same array of draw calls.
// 
// Finally, stateful draw call and render state manipulation is error-prone,
// and a bad abstraction. Ideally, submitting a draw call with whatever
// state we want should not affect any of the other draw calls, even in
// multi-thread environments.
// 

struct RenderBackend;
struct RenderFrame;
struct RenderFrontend : public core::Subsystem
{
    bool initialize() override;
    void dispose() override;

    /**
     * @brief      Creates static vertex buffer.
     *
     * @param[in]  data     Vertex buffer data.
     * @param[in]  size     The Size of data.
     * @param[in]  layout   Vertex layout declaration.
     * @param[in]  usage    Buffer usage hint.
     *
     * @return     Returns unique handle of vertex buffer.
     */
    Handle create_vertex_buffer(
        const void* data, size_t size, const VertexLayout& layout, BufferUsage usage);

    /**
     * @brief      Update dynamic(BufferUsage::DYNAMIC) vertex buffer.
     *
     * @param[in]  handle  Vertex buffer handle.
     * @param[in]  start   Start vertex.
     * @param[in]  data    Vertex buffer data.
     * @param[in]  size    The size of data.
     */
    void update_vertex_buffer(
        Handle handle, uint16_t start, const void* data, size_t size);

    /**
     * @brief      Destroy vertex buffer.
     * 
     * @param[in]  handle  Vertex buffer handle.
     */
    void free_vertex_buffer(Handle handle);

    /**
     * @brief      Creates an index buffer.
     *
     * @param[in]  data    Index buffer data.
     * @param[in]  size    The size of data.
     * @param[in]  format  Index element format.
     * @param[in]  usage    Buffer usage hint.
     *
     * @return     Returns unique handle of index buffer.
     */
    Handle create_index_buffer(
        const void* data, size_t size, IndexElementFormat format, BufferUsage usage);

    /**
     * @brief      Update dynamic(BufferUsage::DYNAMIC) index buffer.
     *
     * @param[in]  handle  Index buffer handle.
     * @param[in]  start   Start index.
     * @param[in]  data    Index buffer data.
     * @param[in]  size    The size of data.
     */
    void update_index_buffer(
        Handle handle, uint16_t start, const void* data, size_t size);

    /**
     * @brief      Destroy index buffer.
     * 
     * @param[in]  handle  Index buffer handle.
     */
    void free_index_buffer(Handle handle);

    /**
     * @brief      Creates a texture.
     *
     * @param[in]  data          The data of texture.
     * @param[in]  format        The format.
     * @param[in]  pixel_format  The pixel format.
     * @param[in]  width         The width.
     * @param[in]  height        The height.
     * @param[in]  usage         The buffer usage.
     *
     * @return     Returns unique handle of texture.
     */
    Handle create_texture(
        const void* data,
        TextureFormat format, TexturePixelFormat pixel_format,
        uint16_t width, uint16_t height,
        BufferUsage usage);

    /**
     * @brief      Generate/destroy mipmap of texture.
     *
     * @param[in]  handle  The handle of texture.
     * @param[in]  mipmap  The mipmap.
     */
    void update_texture_mipmap(Handle handle, bool mipmap);

    /**
     * @brief      Update texture address mode.
     *
     * @param[in]  handle  The handle of texture.
     * @param[in]  coord   The texture coordinate.
     * @param[in]  mode    The wrap mode.
     */
    void update_texture_address_mode(
        Handle handle, TextureCoordinate coord, TextureAddressMode mode);

    /**
     * @brief      Update texture filter mode.
     *
     * @param[in]  handle  The handle of texture.
     * @param[in]  mode    The filter mode.
     */
    void update_texture_filter_mode(
        Handle handle, TextureFilterMode mode);

    /**
     * @brief      Destroy texture.
     *
     * @param[in]  handle  The handle of texture.
     */
    void free_texture(Handle handle);

    /**
     * @brief      Compile and link shaders.
     *
     * @param[in]  VS    Vertex shader.
     * @param[in]  PS    Fragment shader.
     *
     * @return     Returns unique handle of program.
     */
    Handle create_program(const char* vs, const char* ps);

    /**
     * @brief      Creates an uniform belongs to program.
     *
     * @param[in]  handle  The handle of program.
     * @param[in]  name    The name of unifom variable.
     */
    void create_program_uniform(Handle handle, const char* name);

    /**
     * @brief      Bind and attribute belongs to program.
     *
     * @param[in]  handle  The handle of program.
     * @param[in]  va      The vertex attribute.
     * @param[in]  name    The name of attribute.
     */
    void create_program_attribute(
        Handle handle, VertexAttribute::Enum va, const char* name);

    /**
     * @brief      Destroy program.
     *
     * @param[in]  handle  The handle of program.
     */
    void free_program(Handle handle);

    /**
     * @brief      Creates a render state.
     *
     * @param[in]  state  Stateless state declaration.
     *
     * @return     Render unique handle of state.
     */
    Handle create_render_state(const RenderState& state);

    /**
     * @brief      Update a render state.
     *
     * @param[in]  handle  Render state handle.
     * @param[in]  state   Stateless state declaration.
     */
    void update_render_state(Handle handle, const RenderState& state);

    /**
     * @brief      Destroy render state.
     *
     * @param[in]  handle  Render state handle.
     */
    void free_render_state(Handle handle);

    /**
     * @brief      Allocate an managed uniform buffer, it will be recycled at the end of frame.
     *
     * @param[in]  num   The size of uniform buffer.
     *
     * @return     Returns unique handle of uniform buffer.
     */
    Handle allocate_uniform_buffer(size_t num);

    /**
     * @brief      Determines if uniform buffer alive.
     *
     * @param[in]  handle  The unique handle of uniform buffer.
     *
     * @return     True if uniform buffer alive, False otherwise.
     */
    bool is_uniform_buffer_alive(Handle handle) const;

    /**
     * @brief      Update uniform variable.
     *
     * @param[in]  handle  The handle of program.
     * @param[in]  name    The name of uniform constants.
     * @param[in]  value   The value of uniform constants.
     */
    void update_uniform_buffer(
        Handle handle, math::StringHash name, const UniformVariable& value);

    /**
     * @brief      Begins a frame.
     *
     * @return     Returns true if frontend is ready to draw, False otherwise
     */
    bool begin_frame();

    /**
     * @brief      Clear view clear flags and arguments
     *
     * @param[in]  option   The clear options.
     * @param[in]  color    The clear color.
     * @param[in]  depth    The clear depth value.
     * @param[in]  stencil  The clear stencil value.
     */
    void clear(ClearOption option,
        const math::Color& color = {0.f, 0.f, 0.f, 0.f}, float depth = 0.f, uint32_t stencil = 0);

    /**
     * @brief      Sumbmit drawcall state for rendering.
     *
     * @param[in]  drawcall  Drawcall which contains all the draw informations.    
     */
    void submit(const RenderDrawCall& drawcall);

    /**
     * @brief      Force to sync render thread and wait for performing all drawcalls.
     */
    void flush();

    /**
     * @brief      Ends a frame.
     */
    void end_frame();

protected:
    friend struct WindowDevice;
    bool restore_video_context(SDL_Window*);
    void dispose_video_context();
    void draw();

protected:
    struct UniformBuffer
    {
        std::atomic<uint32_t> _position;
        math::StringHash _names[kMaxUniforms];
        UniformVariable _values[kMaxUniforms];
    };

    struct UniformBufferView
    {
        uint32_t first;
        uint32_t num;
        uint32_t used;
    };

protected:
    Handle _paint;
    RenderFrame* _frames[2];
    RenderFrame* _submit = nullptr;
    RenderFrame* _draw = nullptr;

    std::unique_ptr<RenderBackend> _backend;
    HandleSet<kMaxProgram> _material_handles;
    HandleSet<kMaxIndexBuffer> _ib_handles;
    HandleSet<kMaxVertexBuffer> _vb_handles;
    HandleSet<kMaxTexture> _texture_handles;

    UniformBuffer _uniform_buffer;
    HandleObjectSet<UniformBufferView, kMaxUniforms> _ub_views;
    HandleObjectSet<RenderState, kMaxRenderState> _states;
};

NS_LEMON_GRAPHICS_END