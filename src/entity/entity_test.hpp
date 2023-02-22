#pragma once

#include "entity.h"
#include "util/test/test2.h"

class Entity_Test : public test2::Test2 {
    EntityManager em;
    void before_each() {
        em = EntityManager();
    }

    void run_tests() {
        EXECUTE_TEST(addRemAdd__sameId_countEq1);
        EXECUTE_TEST(addRem_flaggedForDeletion);
    }

    void addRemAdd__sameId_countEq1() {
        EntityID e1 = em.add();
        em.rem(e1);
        EntityID e2 = em.add();

        ASSERT2_EQ(e2, e1);
        ASSERT2_EQ(1, em.count());
    }

    void addRem_flaggedForDeletion() {
        EntityID e1 = em.add();
        em.rem(e1);

        ASSERT2_EQ(true, em.is_rem(e1));
    }
};