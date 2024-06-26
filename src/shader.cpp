#include "shader.h"

ShaderManager::ShaderManager() {}

GLuint ShaderManager::load_shader(GLenum type, const char * path) {
    // Check if we already cached this shader and therefore have its "name" (id)
    auto cached = cache.find(path);
    if (cached != cache.end()) {
        return cached->second;
    }

    GLuint id = glCreateShader(type);
    std::string code;
    std::stringstream tmp;

    GLint result = GL_FALSE;
    int info_log_len;

    // Read the code into a string
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Couldn't open shader " << path << std::endl;
        return 0;
    }
    tmp << file.rdbuf();
    code = tmp.str();

    // Compile it!
    const char * code_ptr = code.c_str();
    glShaderSource(id, 1, &code_ptr, NULL);
    glCompileShader(id);

    // Check for warnings
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
        std::cout << "Error compiling " << path << std::endl;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_len);
        char log[info_log_len + 1];
        glGetShaderInfoLog(id, info_log_len, NULL, log);
        std::cout << log;

        return 0;
	}

    cache[path] = id;

    return id;
}

GLuint ShaderManager::link_program(GLuint vert, GLuint frag) {
    GLuint id = glCreateProgram();

    glAttachShader(id, vert);
    glAttachShader(id, frag);
    glLinkProgram(id);

    GLint result = GL_FALSE;
    int info_log_len;

    glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (result != GL_TRUE){
        std::cout << "Error linking program" << std::endl;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_len);
        char log[info_log_len + 1];
        glGetProgramInfoLog(id, info_log_len, NULL, log);
        std::cout << log;

        return 0;
	}

    glDetachShader(id, vert);
    glDetachShader(id, frag);

    return id;
}

ShaderProgram::ShaderProgram(GLuint id) {
    this->id = id;
    
    vertex_attributes.position = glGetAttribLocation(id, "e33_vertexposition");
    vertex_attributes.uv       = glGetAttribLocation(id, "e33_vertexuv");
    vertex_attributes.normal   = glGetAttribLocation(id, "e33_vertexnormal");
    vertex_attributes.color    = glGetAttribLocation(id, "e33_vertexcolor");

    uniforms.mvp              = glGetUniformLocation(id, "e33_mvp");
    uniforms.modelmatrix      = glGetUniformLocation(id, "e33_modelmatrix");
    uniforms.cameramatrix     = glGetUniformLocation(id, "e33_cameramatrix");
    uniforms.projectionmatrix = glGetUniformLocation(id, "e33_projectionmatrix");

    for (uint8_t i = 0; i < 16; i++) {
        uniforms.textures[i] = glGetUniformLocation(id, ("e33_texture" + std::to_string(i)).c_str());
    }
}
