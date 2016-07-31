// @date 2016/06/01
// @author Mao Jingkai(oammix@gmail.com)

#include <graphics/color.hpp>

NS_FLOW2D_GFX_BEGIN

const Color Color::WHITE        = { 1.f, 1.f, 1.f, 1.f };
const Color Color::GRAY         = { 0.5f, 0.5f, 0.5f, 0.5f };
const Color Color::BLACK        = { 0.f, 0.f, 0.f, 1.f };
const Color Color::RED          = { 1.f, 0.f, 0.f, 1.f};
const Color Color::GREEN        = { 0.f, 1.f, 0.f, 1.f };
const Color Color::BLUE         = { 0.f, 0.f, 1.f, 1.f };
const Color Color::CYAN         = { 0.f, 1.f, 1.f, 1.f };
const Color Color::MAGENTA      = { 1.f, 0.f, 1.f, 1.f };
const Color Color::YELLOW       = { 1.f, 1.f, 0.f, 1.f };
const Color Color::TRANSPARENT  = { 0.f, 0.f, 0.f, 0.f };

Color::Color(std::initializer_list<float> values)
{
    auto size = values.size();
    auto cursor = values.begin();

    r = g = b = a = 1.0f;
    if( size >= 1 ) r = *cursor++;
    if( size >= 2 ) g = *cursor++;
    if( size >= 3 ) b = *cursor++;
    if( size >= 4 ) a = *cursor++;
}

Color::Color(const Color& rh, float a)
{
    *this = rh;
    a = a;
}

NS_FLOW2D_GFX_END