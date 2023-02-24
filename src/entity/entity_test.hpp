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
    }

    void api_test() {
        EntityID e1, e2, e3;

        e1 = em.create();
        ASSERT2_EQ(1U, e1);

        em.destroy(e1);
        ASSERT2(em.destroyed(e1));
        ASSERT2_EQ(0U, em.entity_slots.size());

        e2 = em.create();
        ASSERT2_EQ(2U, e2);

        em.cleanup();
        ASSERT2_EQ(1U, em.entity_slots.size());

        e3 = em.create();
        ASSERT2_EQ(1U, e3);
        ASSERT2(!em.destroyed(e2));
        ASSERT2_EQ(0U, em.entity_slots.size());
    }
};