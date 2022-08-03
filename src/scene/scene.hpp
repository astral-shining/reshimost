#pragma once
#include <memory>
#include <util/smartvector.hpp>
#include <util/constexpr_for.hpp>
#include <util/tuple_utils.hpp>
#include <util/pretty.hpp>
#include <object/camera.hpp>
#include <object/entity.hpp>

struct SceneBase;
extern double delta_time;
extern double current_time;
extern uint32_t fps;
extern SceneBase* current_scene;

struct SceneBase {
    Camera camera;

    SceneBase(void);

    template<typename T>
    T& cast() {
        return static_cast<T&>(*this);
    }

    //void destroyObject(GameObject& object);
    virtual void updateScene(void) {}
    virtual void update(void) {}
    
    void run(void);
    virtual ~SceneBase() = default;
};

template<typename... Ts>
struct Scene : SceneBase {
    std::tuple<typename Ts::Manager...> managers {
        ((typename Ts::Manager*) 0, *this)...
    };

    template<FixedString s, typename T>
    struct FixedString_T {
        static constexpr FixedString str { s };
        T value;
    };

    using Textures = norepeated_tuple_t<std::tuple<FixedString_T<Ts::Manager::texture_name, Texture>...>>;
    Textures textures;

    Scene() {
        constexpr_for(int i=0, i<std::tuple_size_v<Textures>, i+1, 
            auto& x = std::get<i>(textures); 
            x.value.bindImage(std::remove_reference_t<decltype(x)>::str);
        );
    }

    ~Scene() {
    }


    template<FixedString str>
    Texture& getTexture() {
        return std::get<FixedString_T<str, Texture>>(textures).value;
    }

    void updateScene() {
        camera.update();
        update();
        constexpr_for(int i=0, i<sizeof...(Ts), i+1, 
            using iT = std::tuple_element_t<i AND std::tuple<Ts...>>;
            auto& manager = std::get<i>(managers);
            manager.update();
        );
    }

    template<typename T>
    void destroyObject(T& obj) {
    }

    template<typename T>
    auto& getManager() {
        return std::get<T::Manager>(managers);
        //return .emplace_back();
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

