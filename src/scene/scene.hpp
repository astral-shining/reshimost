#pragma once
#include <memory>
#include <util/smartvector.hpp>
#include <util/constexpr_for.hpp>
#include <util/tuple_utils.hpp>
#include <util/pretty.hpp>
#include <object/camera.hpp>
#include <util/fixedstring.hpp>
#include <gl/texture.hpp>
#include <gl/shader.hpp>
//#include <object/entity.hpp>

struct SceneBase;
extern double delta_time;
extern double current_time;
extern uint32_t fps;
extern SceneBase* current_scene;

struct SceneBase {
    Camera camera;

    SceneBase();

    template<typename T>
    T& cast() {
        return static_cast<T&>(*this);
    }

    //void destroyObject(GameObject& object);
    virtual void updateScene() {}
    virtual void update() {}
    
    void run();
    virtual ~SceneBase() = default;
};

template<typename... Ts>
struct Scene : SceneBase {
    std::tuple<typename Ts::Pool...> pools;

    using Textures = norepeated_tuple_t<std::tuple<typename Ts::Pool::texture_name...>>;
    Textures textures;

    void forEachPool(auto&& fn) {
        constexpr_for(int i=0, i<sizeof...(Ts), i+1, 
            fn(std::get<i>(pool));
        );
    }

    void forEachTexture(auto&& fn) {
        constexpr_for(int i=0, i<std::tuple_size_v<Textures>, i+1,
            fn(std::get<i>(textures));
        );
    }

    Scene() {
        forEachPool([&] <typename T> (T& pool) {
            pool.texture = &std::get<typename T::texture_name>(textures).texture;
        });
    }

    ~Scene() {
    }

    template<FixedString str>
    Texture& getTexture() {
        return std::get<TextureEntry<str>>(textures).texture;
    }

    void updateScene() {
        camera.update();
        update();
        forEachManager([] (auto& manager) {
            manager.update();
        });
    }

    template<typename T>
    void destroyObject(T& obj) {
    }

    template<typename T>
    auto& getManager() {
        return std::get<typename T::Manager>(managers);
    }

    template<typename T, typename... Args>
    auto create(Args&&... args) {
        return std::get<typename T::Manager>(managers).create(std::forward<Args>(args)...);
    }

};

template<typename T>
void loadScene() {
    static std::unique_ptr<SceneBase> scene;
    scene = std::make_unique<T>();
    scene->run();
}

