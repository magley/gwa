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
        EXECUTE_TEST(addRemAdd__ifNoCleanupThenNewSlot);
        EXECUTE_TEST(addRem_flaggedForDeletion);
        EXECUTE_TEST(entityRef_follow);
        EXECUTE_TEST(entityRef_complex);
    }

    void addRemAdd__sameId_countEq1() {
        EntityID e1 = em.add();
        em.rem(e1);
        em.cleanup(); // e1 will go from "marked for removal" to "removed"
        EntityID e2 = em.add();

        ASSERT2_EQ(e1, e2);
        ASSERT2_EQ(1, em.count());
    }

    void addRemAdd__ifNoCleanupThenNewSlot() {
        EntityID e1 = em.add();
        em.rem(e1);
        // No em.cleanup() -> e2.del == 1  --> won't be overwritten
        EntityID e2 = em.add();

        ASSERT2(e1 != e2);
        ASSERT2_EQ(2, em.count());
    }

    void addRem_flaggedForDeletion() {
        EntityID e1 = em.add();
        em.rem(e1);

        ASSERT2_EQ(true, em.is_rem(e1));
    }

    void entityRef_follow() {
        // Create entities and initialize their positions.
        EntityID e1, e2;
        e1 = em.add();
        e2 = em.add();
        em.add_c(e1, TRANSFORM);
        em.add_c(e2, TRANSFORM);
        {
            transform_c* t1 = em.transform(e1);
            t1->x = 32;
            t1->y = 128;

            transform_c* t2 = em.transform(e2);
            t2->x = 0;
            t2->y = 0;
        }

        // Create a reference to e1.
        EntityRefID ref = em.make_ref(e1);

        // Mock game-loop
        for (int i = 0; i < 10; i++) 
        {
            // Called before any other entity code.
            em.cleanup();

            // Once e1 is destroyed, the reference is no longer valid.
            if (i > 5) {
                EntityRef refdata = em.get_ref(ref);
                ASSERT2_EQ(false, refdata.valid);
            }

            // Simulate e1 being destroyed for some reason. A new entity takes
            // its place, although this doesn't affect the 
            if (i == 5) {
                em.rem(e1);
            }

            // Logic for e2.
            {
                transform_c* my_pos = em.transform(e2);

                // Since EntityID is volatile and recycled, it's not safe to
                // store EntityIDs for entity references between frames. An
                // EntityRef keeps track of major changes in an entity and gets
                // invalidated if such changes occurr.
                // You can store an EntityRefID between frames and fetch the
                // EntityID in each frame repeatedly.
                EntityRef refdata = em.get_ref(ref);

                // If the reference is still valid
                if (refdata.valid) {
                    // do stuff with the other entity

                    transform_c* his_pos = em.transform(refdata.entity);
                    if (my_pos->x < his_pos->x) {
                        my_pos->x++;
                    }
                } 
                // We don't have to do this, but it's for our own sanity.
                else {
                    ref = -1;
                }
            }
        }
    }


    struct B_c {
        EntityRefID player_ref = -1;

        EntityID find_player(EntityManager& em) {
            // mock
            if (em.is_rem(0)) {
                return -1;
            }
            return 0;
        }

        void update(EntityManager& em, int& out_counter) {
            // If stale reference, try to fetch it again.
            if (player_ref == -1) {
                EntityID player = find_player(em);
                player_ref = em.make_ref(player);
            }

            // If reference is valid, use the entity it refers to.
            EntityRef ref = em.get_ref(player_ref);
            if (ref.valid) {
                // Do something with the player.
                out_counter++;
            } else {
                player_ref = -1;
            }
        }
    };

    void entityRef_complex() {
        EntityID e1, e2;
        e1 = em.add();
        e2 = em.add();
        int has_player_cnt = 0;
        B_c e2_b; // pretend e2 has this component

        // Frame
        em.cleanup();
        e2_b.update(em, has_player_cnt);

        // Frame
        em.cleanup();
        e2_b.update(em, has_player_cnt);

        // Frame
        em.cleanup();
        em.rem(e1);
        e2_b.update(em, has_player_cnt); // ref is valid until next cleanup

        // Frame
        em.cleanup();
        e2_b.update(em, has_player_cnt);

        ASSERT2_EQ(3, has_player_cnt);
    }

};