#pragma once
#include <string_view>
#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>

#include <util/smartvector.hpp>
#include <gl/vbo.hpp>

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

    static void compileAll();
    static void forEach(auto&& f) {
        for (auto& s : list) {
            f(s);
        }
    }

    void compile();
    void use();

    template<bool shared = false, typename T>
    std::conditional_t<shared, std::shared_ptr<VBO>, VBO> setAttribute(std::initializer_list<const char*>, std::initializer_list<T> buffer, uint32_t draw_type = 0x88E4);
    template<typename T>
    void setAttribPointer(std::initializer_list<const char*> arr);

    void uniform(const char*, const int);
    void uniform(const char*, const glm::vec2&);
    void uniform(const char*, const glm::vec3&);
    void uniform(const char*, const glm::mat4&);
    void uniform(const char*, const glm::uvec2&);
    void uniform(const char*, const float);

    const AttribInfo& getAttribInfo(const char*) const;
    uint32_t getProgram() const;
    uint32_t getAttrib(const char*) const;
    void destroy();
    ~Shader();
};

extern Shader* current_shader;
