#include <catch.hpp>
#include <lemon-toolkit.hpp>

USING_NS_LEMON;
USING_NS_LEMON_CORE;
USING_NS_LEMON_MATH;

struct TransformFixture
{
    TransformFixture()
    {
        initialize(0);
    }

    ~TransformFixture()
    {
        dispose();
    }
};

TEST_CASE_METHOD(TransformFixture, "TestConstructor")
{
    Handle e1 = create();
    Transform* t1 = add_component<Transform>(e1, Vector3f{10.f, 10.f});

    Handle e2 = create();
    Transform* t2 = add_component<Transform>(e2, Vector3f{20.f, 20.f});

    Handle e3 = create();
    Transform* t3 = add_component<Transform>(e3, Vector3f{40.f, 40.f}, Vector3f{2.0f, 2.0f});

    Handle e4 = create();
    Transform* t4 = add_component<Transform>(e4, Vector3f{50.f, 50.f}, Vector3f{2.0f, 2.0f}, Quaternion());

    REQUIRE( equals(t2->get_position(), {20.f, 20.f}) );
    REQUIRE( equals(t1->get_scale(), t2->get_scale()) );
}

TEST_CASE_METHOD(TransformFixture, "TestTransform")
{
    Handle e1 = create();
    Transform* t1 = add_component<Transform>(e1, Vector3f{10.f, 10.f, 0.f});
    REQUIRE( t1->transform_point({0.f, 5.f, 0.f}) == (Vector3f{10.f, 15.f, 0.f}) );
    REQUIRE( t1->inverse_transform_point({0.f, 5.f, 0.f}) == (Vector3f{-10.f, -5.f, 0.f}) );
    REQUIRE( t1->transform_vector({0.f, 5.f, 0.f}) == (Vector3f{0.f, 5.f, 0.f}) );
    REQUIRE( t1->inverse_transform_vector({0.f, 5.f, 0.f}) == (Vector3f{0.f, 5.f, 0.f}) );
    REQUIRE( t1->transform_direction({0.f, 5.f, 0.f}) == (Vector3f{0.f, 5.f, 0.f}) );
    REQUIRE( t1->inverse_transform_direction({0.f, 5.f, 0.f}) == (Vector3f{0.f, 5.f, 0.f}) );

    t1->set_position({0.f, 0.f, 0.f});
    t1->set_scale({2.0f, 0.5f, 1.0f});
    REQUIRE( t1->transform_point({10.f, 10.f, 10.f}) == (Vector3f{20.f, 5.f, 10.f}) );
    REQUIRE( t1->inverse_transform_point({10.f, 10.f, 10.f}) == (Vector3f{5.f, 20.f, 10.f}) );
    REQUIRE( t1->transform_vector({10.f, 10.f, 10.f}) == (Vector3f{20.f, 5.f, 10.f}) );
    REQUIRE( t1->inverse_transform_vector({10.f, 10.f, 10.f}) == (Vector3f{5.f, 20.f, 10.f}) );
    REQUIRE( t1->transform_direction({10.f, 10.f, 10.f}) == (Vector3f{10.f, 10.f, 10.f}) );
    REQUIRE( t1->inverse_transform_direction({10.f, 10.f, 10.f}) == (Vector3f{10.f, 10.f, 10.f}) );

    t1->set_position({10.f, 5.f, 0.f});
    REQUIRE( t1->transform_point({10.f, 10.f, 10.f}) == (Vector3f{30.f, 10.f, 10.f}) );
    REQUIRE( t1->inverse_transform_point({10.f, 10.f, 10.f}) == (Vector3f{0.f, 10.f, 10.f}) );
    REQUIRE( t1->transform_vector({10.f, 10.f, 10.f}) == (Vector3f{20.f, 5.f, 10.f}) );
    REQUIRE( t1->inverse_transform_vector({10.f, 10.f, 10.f}) == (Vector3f{5.f, 20.f, 10.f}) );
    REQUIRE( t1->transform_direction({10.f, 10.f, 10.f}) == (Vector3f{10.f, 10.f, 10.f}) );
    REQUIRE( t1->inverse_transform_direction({10.f, 10.f, 10.f}) == (Vector3f{10.f, 10.f, 10.f}) );

    t1->set_position({0.f, 0.f, 0.f});
    t1->set_scale({1.f, 1.f, 1.f});
    t1->set_rotation(from_euler_angles({0.f, 90.f, 0.f}));
    REQUIRE( equals(t1->transform_point({0.f, 0.f, 1.f}), {1.f, 0.f, 0.f}) );
    REQUIRE( equals(t1->inverse_transform_point({1.f, 0.f, 0.f}), {0.f, 0.f, 1.f}, 1e-5f) );

    t1->set_scale({2.f, 1.f, 1.f});
    REQUIRE( equals(t1->transform_point({0.f, 0.f, 1.f}), {2.f, 0.f, 0.f}, 1e-5f) );
    REQUIRE( equals(t1->inverse_transform_point({1.f, 0.f, 0.f}), {0.f, 0.f, 0.5f}, 1e-5f) );
    REQUIRE( equals(t1->transform_direction({0.f, 0.f, 1.f}), {1.f, 0.f, 0.f}, 1e-5f));
    REQUIRE( equals(t1->inverse_transform_direction({1.f, 0.f, 0.f}), {0.f, 0.f, 1.f}, 1e-5f));

    t1->set_position({10.f, 0.f, 0.f});
    REQUIRE( equals(t1->transform_point({0.f, 0.f, 1.f}), {12.f, 0.f, 0.f}, 1e-5f) );
    REQUIRE( equals(t1->inverse_transform_point({1.f, 0.f, 0.f}), {0.f, 0.f, -4.5f}, 1e-5f) );
    REQUIRE( equals(t1->transform_direction({0.f, 0.f, 1.f}), {1.f, 0.f, 0.f}, 1e-5f));
    REQUIRE( equals(t1->inverse_transform_direction({1.f, 0.f, 0.f}), {0.f, 0.f, 1.f}, 1e-5f));
}

TEST_CASE_METHOD(TransformFixture, "TestOpsWithoutHierachy")
{
    Handle e1 = create();
    Transform* t1 = add_component<Transform>(e1, Vector3f{10.f, 10.f});

    t1->set_position({20.f, 20.f});
    REQUIRE( t1->get_position() == Vector3f({20.f, 20.f}) );
    REQUIRE( t1->get_position() == t1->get_position(TransformSpace::WORLD) );

    t1->set_position({30.f, 30.f}, TransformSpace::WORLD);
    REQUIRE( t1->get_position() == Vector3f({30.f, 30.f}) );
    REQUIRE( t1->get_position() == t1->get_position(TransformSpace::WORLD) );

    t1->set_scale({2.f, 2.f});
    REQUIRE( t1->get_scale() == Vector3f({2.f, 2.f}) );
    REQUIRE( t1->get_scale() == t1->get_scale(TransformSpace::WORLD) );

    t1->set_scale({3.f, 3.f}, TransformSpace::WORLD);
    REQUIRE( t1->get_scale() == Vector3f({3.f, 3.f}) );
    REQUIRE( t1->get_scale() == t1->get_scale(TransformSpace::WORLD) );

    t1->set_rotation({60.f, 45.f});
    auto rotation = to_euler_angles(t1->get_rotation());
    REQUIRE( rotation[0] == Approx(60.f) );
    REQUIRE( rotation[1] == Approx(45.f) );
    REQUIRE( t1->get_rotation() == t1->get_rotation(TransformSpace::WORLD) );
}

TEST_CASE_METHOD(TransformFixture, "TestHierachy")
{
    Handle e1 = create();
    Transform* t1 = add_component<Transform>(e1, Vector3f{10.f, 10.f});

    Handle e2 = create();
    Transform* t2 = add_component<Transform>(e2, Vector3f{20.f, 20.f});

    Handle e3 = create();
    Transform* t3 = add_component<Transform>(e3, Vector3f{40.f, 40.f}, Vector3f{2.0f, 2.0f});

    Handle e4 = create();
    Transform* t4 = add_component<Transform>(e4, Vector3f{-50.f, -10.f}, Vector3f{3.0f, 3.0f});

    t1->append_child(*t2);
    REQUIRE( t1->get_children().count() == 1 );
    REQUIRE( t2->get_children().count() == 0 );
    REQUIRE( t2->get_parent() == t1 );
    REQUIRE( equals(t1->get_position(), {10.f, 10.f}) );
    REQUIRE( equals(t2->get_position(), {20.f, 20.f}) );
    REQUIRE( equals(t2->get_position(TransformSpace::WORLD), {30.f, 30.f}) );

    // keep world pose of t3
    t1->append_child(*t3, true);
    REQUIRE( t1->get_children().count() == 2 );
    REQUIRE( equals(t1->get_position(), {10.f, 10.f}) );
    REQUIRE( equals(t3->get_position(), {30.f, 30.f}) );
    REQUIRE( t3->get_parent() == t1 );
    REQUIRE( equals(t3->get_position(TransformSpace::WORLD), {40.f, 40.f}) );

    // nested hierachies
    t3->append_child(*t4);
    REQUIRE( t1->get_children().count() == 2 );
    REQUIRE( t1->get_children(true).count() == 3 );
    REQUIRE( t3->get_children(true).count() == 1 );
    REQUIRE( t4->get_parent() == t3 );
    REQUIRE( equals(t4->get_position(TransformSpace::WORLD), {-10.f, 30.f}) );
    REQUIRE( equals(t4->get_scale(TransformSpace::WORLD), {6.f, 6.f}) );
}

TEST_CASE_METHOD(TransformFixture, "TestHierachyReconstructWhenDispose")
{
    Handle e1 = create_with<Transform>(Vector3f{10.f, 10.f});
    Transform* t1 = get_component<Transform>(e1);

    Handle e2 = create_with<Transform>(Vector3f{10.f, 10.f});
    Transform* t2 = get_component<Transform>(e2);

    Handle e3 = create_with<Transform>(Vector3f{10.f, 10.f});
    Transform* t3 = get_component<Transform>(e3);

    Handle e4 = create_with<Transform>(Vector3f{10.f, 10.f});
    Transform* t4 = get_component<Transform>(e4);

    t1->append_child(*t2);
    t1->append_child(*t3);
    t3->append_child(*t4);

    REQUIRE( t1->is_root() );
    REQUIRE( !t2->is_root() );
    REQUIRE( !t3->is_root() );
    REQUIRE( !t4->is_root() );

    REQUIRE( !t1->is_leaf() );
    REQUIRE( t2->is_leaf() );
    REQUIRE( !t3->is_leaf() );
    REQUIRE( t4->is_leaf() );

    REQUIRE( t1->get_children().count() == 2 );
    REQUIRE( t1->get_children(true).count() == 3 );

    t3->remove_from_parent();

    REQUIRE( t1->is_root() );
    REQUIRE( !t2->is_root() );
    REQUIRE( t3->is_root() );
    REQUIRE( !t4->is_root() );

    REQUIRE( t1->get_children().count() == 1 );
    REQUIRE( t1->get_children(true).count() == 1 );
}

static void dump_heriachy(Transform* t, int index = 1, int level = 0)
{
    if( t->get_children().count() == 0 )
        return;

    for( size_t i = 0; i < level; i++ )
        printf("\t");
    printf("[%d]", index);
    for( auto& transform : t->get_children() )
        printf("%d ", transform.handle.get_index() );
    printf("\n");

    size_t count = 0;
    for( auto& transform : t->get_children() )
        dump_heriachy(&transform, ++count, level+1);
}

TEST_CASE_METHOD(TransformFixture, "TestIteration")
{
    std::vector<Transform*> transforms;
    for( size_t i = 0; i < 255; i++ )
    {
        auto e = create_with<Transform>();
        transforms.push_back(get_component<Transform>(e));
    }

    std::srand(std::time(0));
    std::vector<Transform*> constructed;
    constructed.push_back(transforms.back());
    transforms.pop_back();

    size_t count = 0;
    for( size_t i = 0; i < 255-1; i++ )
    {
        size_t index = std::rand() % transforms.size();
        size_t pindex = std::rand() % constructed.size();

        if( pindex == 0 ) count ++;
        constructed[pindex]->append_child(*transforms[index]);

        // if( constructed[0]->get_children(true).count() != i+ 1)
        //     dump_heriachy(constructed[0]);
        REQUIRE( constructed[0]->get_children(true).count() == i+1 );

        constructed.push_back(transforms[index]);
        transforms.erase(transforms.begin()+index);
    }

    size_t direct = 0;
    constructed[0]->get_children().visit([&](Transform& t)
    {
        direct ++;
    });

    REQUIRE( direct == count );

    direct = 0;
    for( auto& transform : constructed[0]->get_children() )
        direct ++;

    REQUIRE( direct == count );

    size_t total = 0;
    for( auto& transform : constructed[0]->get_children(true) )
        total ++;

    REQUIRE( total == 254 );
}

struct Widget : Component
{};

TEST_CASE_METHOD(TransformFixture, "TestGenericIteration")
{
    std::vector<Transform*> transforms;
    std::vector<Widget*> widgets;

    auto r = create();
    auto root = add_component<Transform>(r);

    for( size_t i = 0; i < 255; i++ )
    {
        auto e = create_with<Transform>();
        transforms.push_back(get_component<Transform>(e));
        root->append_child(*transforms.back());

        if( (i % 2) == 1 )
            widgets.push_back( add_component<Widget>(e) );
    }

    REQUIRE( transforms.size() == root->get_children_with<Transform>(true).count() );
    REQUIRE( widgets.size() == root->get_children_with<Widget>(true).count() );
}
