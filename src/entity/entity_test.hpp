#pragma once

#include "entity.h"
#include "util/test/test2.h"

class Entity_Test : public test2::Test2 {
    EntityManager em;

    void before_each() {
        em = EntityManager();
    }

    void run_tests() {
        EXECUTE_TEST(api_test);
        EXECUTE_TEST(reference_test);
    }

    void api_test() {
        EntityID e1, e2, e3;

        e1 = em.create();
        ASSERT2_EQ(1U, e1);

        em.destroy(e1);
        ASSERT2(em.destroyed(e1));
        ASSERT2_EQ(1, em.count());

        e2 = em.create();
        ASSERT2_EQ(2U, e2);

        em.cleanup();
        ASSERT2_EQ(1, em.count());

        e3 = em.create();
        ASSERT2_EQ(1U, e3);
        ASSERT2(!em.destroyed(e2));
        ASSERT2_EQ(2, em.count());
    }

    void reference_test() {
        EntityID e1, e2;
        EntityRefID r1, r2, r3;

        // [Frame 1]
        e1 = em.create();
        r1 = em.make_ref(e1);

        EntityRef r1_1 = em.get_ref(r1);
        ASSERT2_EQ(e1, r1_1.entity);
        ASSERT2_EQ(0U, r1_1.id);
        ASSERT2_EQ(true, r1_1.valid);

        em.destroy(e1);
        ASSERT2_EQ(true, r1_1.valid);

        // [Frame 2]
        em.cleanup();
        EntityRef r1_2 = em.get_ref(r1);
        ASSERT2_EQ(false, r1_2.valid);
        
        r2 = em.make_ref(e1);
        ASSERT2_EQ(false, r1_2.valid);

        e2 = em.create();
        r3 = em.make_ref(e2);

        EntityRef r1_3 = em.get_ref(r3);
        ASSERT2_EQ(e2, r1_3.entity);
        ASSERT2_EQ(2U, r1_3.id);
        ASSERT2_EQ(true, r1_3.valid);

        // [Frame 3]
        em.cleanup(); // 2/3 invalid refs, 3 refs > 2 entities => flush 'em out
        ASSERT2_EQ((size_t)1, em.refs.size());
    }
};