#include <catch.hpp>
#include <flow2d.hpp>

USING_NS_FLOW2D;

TEST_CASE("TestVectorInitializerList")
{
    Vector2f v { 1.0f, 2.0f };
    REQUIRE( v[0] == Approx(1.0f) );
    REQUIRE( v[1] == Approx(2.0f) );

    v = Vector2f { 3.0f, 4.0f, 1.0f, 2.0f };
    REQUIRE( v[0] == Approx(3.0f) );
    REQUIRE( v[1] == Approx(4.0f) );

    Vector2f v2 { };
    Vector2f v3 { 1.0f };

    REQUIRE( Vector2f::ZERO == (Vector2f { 0.0f, 0.0f }) );
    REQUIRE( Vector2f::ONE == (Vector2f { 1.0f, 1.0f }) );
    REQUIRE( Vector2f::LEFT == (Vector2f { -1.0f, 0.0f }) );
    REQUIRE( Vector2f::RIGHT == (Vector2f { 1.0f, 0.0f }) );
    REQUIRE( Vector2f::UP == (Vector2f { 0.0f, 1.0f }) );
    REQUIRE( Vector2f::DOWN == (Vector2f { 0.0f, -1.0f }) );
}

TEST_CASE("TestVectorComparisions")
{
    Vector2f v1 = { 1.0f, 1.0f };
    Vector2f v2 = { 2.0f, 1.0f };
    Vector2f v3 = { 1.0f, 2.0f };
    Vector2f v4 = { 1.0f, 1.0f };

    REQUIRE( v1 == v4 );
    REQUIRE( v1 != v2 );
    REQUIRE( v1 < v2 );
    REQUIRE( v1 <= v2 );
    REQUIRE( v1 <= v4 );
    REQUIRE( v1 < v3 );
    REQUIRE( v1 <= v3 );
    REQUIRE( v2 > v1 );
    REQUIRE( v2 >= v1 );
    REQUIRE( v3 >= v1 );
    REQUIRE( v2 > v3 );
    REQUIRE( v3 < v2 );
}

TEST_CASE("TestVectorOtherOperations")
{
    Vector2f v1 = { 1.0f, 1.0f };
    Vector2f v2 = { 2.0f, 3.0f };

    REQUIRE( v1[0] == Approx(1.0f) );
    REQUIRE( v1[1] == Approx(1.0f) );

    REQUIRE( (v1 - v2) == (Vector2f { -1.0f, -2.0f }) );
    REQUIRE( (v1 + v2) == (Vector2f { 3.0f, 4.0f }) );
    REQUIRE( (v2 * 2.0f) == (Vector2f { 4.0f, 6.0f }) );
    REQUIRE( (v2 / 2.0f) == (Vector2f { 1.0f, 1.5f }) );
}

TEST_CASE("TestVectorMethods")
{
    Vector2f v1 = { 3.0f, 4.0f };
    Vector2f v2 = { 4.0f, 5.0f };
    Vector2f v3 = { 3.0f, 3.0f };

    REQUIRE( v1.dot(v2) == Approx(32.f) );
    REQUIRE( v1.length_square() == Approx(v1.dot(v1)) );
    REQUIRE( v1.length() == Approx(5.f) );

    Vector2f v4 = v3.normalize();
    REQUIRE( v4[0] == Approx(0.707107f) );
    REQUIRE( v4[1] == Approx(0.707107f) );
    REQUIRE( v4.length() == Approx(1.f) );

    Vector2f v5 = (Vector2f { 0.0f, 0.0f }).normalize();
    REQUIRE( v5.isnan() );

    Vector2f v6 = { 1.0f, 2.0f };
    Vector2f v7 = { 1.1f, 2.0f };
    REQUIRE( !v6.equals(v7, 0.05f) );
    REQUIRE( v6.equals(v7, 0.11f) );
}

TEST_CASE("TestRectInitializerList")
{
    Rect2f r { 0.0f, 1.0f, 3.0f, 4.0f };
    REQUIRE( r[0][0] == Approx(0.f) );
    REQUIRE( r[0][1] == Approx(1.f) );
    REQUIRE( r[1][0] == Approx(3.f) );
    REQUIRE( r[1][1] == Approx(4.f) );

    Rect2f r2 { 2.0f, 1.0f };
    REQUIRE( r2[0][0] == Approx(2.f) );
    REQUIRE( r2[0][1] == Approx(1.f) );
    REQUIRE( r2[1][0] == Approx(0.f) );
    REQUIRE( r2[1][1] == Approx(0.f) );

    REQUIRE( Rect2f::FULL == (Rect2f { -1.f, -1.f, 1.f, 1.f }) );
    REQUIRE( Rect2f::POSITIVE == (Rect2f { 0.f, 0.f, 1.f, 1.f }) );
}

TEST_CASE("TestRectOperations")
{
    Rect2f r { 0.0f, 0.0f, 1.0f, 1.0f };

    REQUIRE( r.is_inside({ 0.f, 0.f }) );
    REQUIRE( r.is_inside({ 0.f, 0.9f }) );
    REQUIRE( r.is_inside({ 0.5f, 0.5f }) );
    REQUIRE( !r.is_inside({ 1.f, 0.f }) );
    REQUIRE( !r.is_inside({ 1.f, 1.f }) );
    REQUIRE( !r.is_inside({ 2.f, 0.5f }) );

    Rect2f r2 { 2.0f, 2.0f, 3.0f, 3.0f };
    Rect2f r3 = r + r2;
    REQUIRE( r3 == (Rect2f { 0.f, 0.f, 3.f, 3.f }) );
    r2 += r;
    REQUIRE( r3 == r2 );
    REQUIRE( (r + Rect2f { -1.f, 0.f, 2.f, 1.f }) == (Rect2f { -1.f, 0.f, 2.f, 1.f }) );

    Vector2f v { 4.f, -2.f };
    REQUIRE( (r + v) == (Rect2f { 0.f, -2.f, 4.f, 1.f }) );
}

TEST_CASE("TestColor")
{
    Color c { 1.0f, 0.0f, 0.0f };
    REQUIRE( c == Color::RED );
    REQUIRE( c.to_uint32() == 0xFF0000FF );

    Color c2 { 1.0f, 0.0f, 1.0f };
    REQUIRE( c.lerp(c2, 0.5f).equals(Color {1.f, 0.0f, 0.5f}) );

    Color c3 { 2.0f, 4.3f, 0.5f };
    REQUIRE( c3.clip().equals(Color {1.f, 1.f, 0.5f}) );
    REQUIRE( c3.invert(true).equals(Color {0.f, 0.f, 0.5f, 0.f}) );
}

