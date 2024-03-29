#include <glad/glad.hpp>
#include <gl/texture.hpp>
#include <iostream>
#include <util/terminate.hpp>
#include <png.h>

std::string imagepath = "img/";
Texture* current_texture;

Texture::Texture() {
    create();
}

Texture::Texture(std::string_view str) {
    create();
    bindImage(str);
}

Texture::Texture(uint32_t texture) : id(texture) {}

Texture& Texture::operator=(Texture& other) {
    size = other.size;
    id = other.id;
    other.id = 0;
    return *this;
}
Texture& Texture::operator=(std::string_view name) {
    destroy();
    create();
    bindImage(name);
    return *this;
}

void Texture::create() {
    glGenTextures(1, &id);
    std::cout << "create texture " << id << "\n";
}

void Texture::use() {
//    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    current_texture = this;
}

void Texture::bindImage(std::string_view imagestr) {
    FILE *fp = fopen((imagepath + imagestr.data() + ".png").data(), "rb");
    if (!fp) {
        terminate("ERROR: Image not found", imagestr);
    }
    static auto onErr = [] (png_structp pngPtr, png_const_charp msg) {
        terminate("ERROR:", msg);
    };
    static auto onWarn = [] (png_structp pngPtr, png_const_charp msg) {
        terminate("WARN:", msg);
    };

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, onErr, onWarn);
    if (!png) {
        terminate("ERROR: creating png struct", __FILE__);
    }
    
    png_infop info = png_create_info_struct(png);
    png_init_io(png, fp);
    png_read_info(png, info);

    size.x = png_get_image_width(png, info);
    size.y = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);
    
    uint32_t row_size = png_get_rowbytes(png, info);
    png_bytep data = (png_bytep) malloc(size.y * row_size * sizeof(png_byte));
    png_bytepp row_pointers = (png_bytepp) malloc(size.y * sizeof(png_bytep));

    for (uint32_t i = 0; i < size.y; i++) {
        row_pointers[i] = data + (i * row_size);
    }

    png_read_image(png, row_pointers);

    int alpha {};
    switch (png_get_color_type(png, info)) {
        case PNG_COLOR_TYPE_RGBA:
            alpha = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_RGB:
            alpha = GL_RGB;
            break;
        default:
            terminate("Color type", std::to_string(png_get_color_type(png, info)), "not supported!\n");
    }

    fclose(fp);
    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);

    use();
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, alpha, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    free(data);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);

}

void Texture::destroy() {
    if (id) {
        std::cout << "destroy texture " << id << "\n";
        glDeleteTextures(1, &id);
        id = 0;
    }
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    destroy();
}
