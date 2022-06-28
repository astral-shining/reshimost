#pragma once
#include <string_view>
#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>

#include "../smartvector.hpp"
#include "../utility.hpp"
#include "vbo.hpp"

template<auto... s>
using VData = std::initializer_list<CustomTuple<std::array<float, s>...>>;

class Shader {
    struct AttribInfo {
        uint32_t index;
        uint32_t location;
        uint32_t type;
        uint32_t size;
    };
    struct UniformInfo : Shader::AttribInfo {};

    std::unordered_map<std::string, AttribInfo> attribs;
    std::unordered_map<std::string, UniformInfo> uniforms;

    inline static SmartVector<Shader*, true> list {};
    uint32_t compiled_shader_index;
    uint32_t vs;
    uint32_t fs;
    uint32_t program {};
    bool compiled {};
    const char* vSource;
    const char* fSource;
public:
    inline static std::unordered_map<void*, std::weak_ptr<VBO>> shared_vbo_table; 
    Shader(const char* vSource_, const char* fSource_);

    static void compileAll(void);
    static void forEach(auto&& f) {
        for (auto& s : list) {
            f(s);
        }
    }

    void compile(void);
    void use(void);

    template<bool shared = false, typename T>
    std::conditional_t<shared, std::shared_ptr<VBO>, VBO> setAttribute(std::initializer_list<const char*>, std::initializer_list<T> buffer, uint32_t draw_type = 0x88E4);
    /*VBO setAttribute(const char* name, std::initializer_list<float> buffer, bool dynamic = false, uint32_t type = 0x1406);
    VBO setAttribute(std::initializer_list<const char*> arr, std::initializer_list<float> buffer, bool dynamic = false, uint32_t type = 0x1406);
    std::shared_ptr<VBO> setSharedAttribute(std::initializer_list<const char*> arr, std::initializer_list<float> buffer, bool dynamic = false, uint32_t type = 0x1406);
    std::shared_ptr<VBO> setSharedAttribute(const char* name, std::initializer_list<float> buffer, bool dynamic = false, uint32_t type = 0x1406);*/

    void uniformMat4f(const char* name, glm::mat4& m);
    void uniform1f(const char* name, float v);

    const AttribInfo& getAttribInfo(const char*) const;
    uint32_t getProgram(void) const;
    uint32_t getAttrib(const char*) const;
    void destroy(void);
    ~Shader();
};
