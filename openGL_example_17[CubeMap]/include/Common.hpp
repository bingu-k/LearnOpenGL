#ifndef COMMON_HPP
#define COMMON_HPP

#include <memory>
#include <string>
#include <optional>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <vector>

#define CLASS_PTR(klassName)\
class klassName;\
using klassName ## UPtr = std::unique_ptr<klassName>;\
using klassName ## SPtr = std::shared_ptr<klassName>;\
using klassName ## WPtr = std::weak_ptr<klassName>;

const GLuint	WINDOW_WIDTH=800;
const GLuint	WINDOW_HEIGHT=600;

int	putError(std::string str)
{
	std::cerr << str << std::endl;
	return (-1);
};

std::optional<std::string>  LoadTextFile(const std::string& filename)
{
    std::ifstream   fin(filename);
    if (!fin.is_open())
    {
        putError("Failed to open file: " + filename);
        return {};
    }
    std::stringstream   text;
    text << fin.rdbuf();
    return (text.str());
};

glm::vec3   GetAttenuationCoeff(float distance) {
    const auto linear_coeff = glm::vec4(
        8.4523112e-05,
        4.4712582e+00,
        -1.8516388e+00,
        3.3955811e+01
    );
    const auto quad_coeff = glm::vec4(
        -7.6103583e-04,
        9.0120201e+00,
        -1.1618500e+01,
        1.0000464e+02
    );

    float kc = 1.0f;
    float d = 1.0f / distance;
    auto dvec = glm::vec4(1.0f, d, d*d, d*d*d);
    float kl = glm::dot(linear_coeff, dvec);
    float kq = glm::dot(quad_coeff, dvec);
    
    return glm::vec3(kc, glm::max(kl, 0.0f), glm::max(kq*kq, 0.0f));
}

#endif