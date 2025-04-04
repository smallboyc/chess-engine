#pragma once
#include <glad/glad.h>

class VBO {
public:
    VBO();
    ~VBO();
    void init();
    void bind() const;
    void unbind() const;
    void set_data(const void* data, GLsizeiptr size);
private:
    GLuint m_id;
};