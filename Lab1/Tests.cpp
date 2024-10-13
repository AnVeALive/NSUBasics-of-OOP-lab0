#include <iostream>
#include "CircularBuffer.h"
#include "gtest/gtest.h"

TEST(CircularBuffer, creating_with_cb)
{
    int cap = 3;
    const value_type elem = '5';

    CircularBuffer cb1(cap, elem);
    CircularBuffer cb2(cb1);

    for (int i = 0; i < cap; i++) {
        ASSERT_EQ(cb1[i], cb2[i]);
    }
    ASSERT_EQ(cb1.GetHead(), cb2.GetHead());
    ASSERT_EQ(cb1.GetHead(), 0);
    ASSERT_EQ(cb1.GetTail(), cb2.GetTail());
    ASSERT_EQ(cb1.GetTail(), 0);
    ASSERT_EQ(cb1.GetCapacity(), cb2.GetCapacity());
    ASSERT_EQ(cb1.GetCapacity(), cap);
    ASSERT_EQ(cb1.GetSize(), cb2.GetSize());
    ASSERT_EQ(cb1.GetSize(), cap);
}

TEST(CircularBuffer, creating_with_capacity)
{
    int cap = 3;
    value_type elem = value_type();

    CircularBuffer cb(cap);

    for (int i = 0; i < cap; i++) {
        ASSERT_EQ(cb[i], elem);
    }
    ASSERT_EQ(cb.GetHead(), 0);
    ASSERT_EQ(cb.GetTail(), 0);
    ASSERT_EQ(cb.GetCapacity(), cap);
    ASSERT_EQ(cb.GetSize(), 0);
}

TEST(CircularBuffer, creating_with_capacity_and_filling)
{
    int cap = 3;
    value_type elem = '5';

    CircularBuffer cb(cap, elem);

    for (int i = 0; i < cap; i++) {
        ASSERT_EQ(cb[i], elem);
    }
    ASSERT_EQ(cb.GetHead(), 0);
    ASSERT_EQ(cb.GetTail(), 0);
    ASSERT_EQ(cb.GetCapacity(), cap);
    ASSERT_EQ(cb.GetSize(), cap);
}

TEST(CircularBuffer, operator_at_correct_ind)
{
    int cap = 3;
    int ind1 = 0, ind2 = -1;
    value_type elem1 = '5';
    value_type elem2 = '1';

    CircularBuffer cb(cap, elem1);
    cb.at(ind1) = elem2;
    cb.at(ind2) = elem2;

    ASSERT_EQ(cb[0], elem2);
    ASSERT_EQ(cb[cap - 1], elem2);
}

TEST(CircularBuffer, operator_at_incorrect_ind)
{
    int cap = 3;
    int ind1 = cap + 1, ind2 = -1 * cap - 1;
    value_type elem1 = '5';
    value_type elem2 = '1';

    CircularBuffer cb(cap, elem1);

    ASSERT_THROW(cb.at(ind1), std::out_of_range);
    ASSERT_THROW(cb.at(ind2), std::out_of_range);
}

TEST(CircularBuffer, operator_assignment_for_different_cbs)
{
    int cap = 3;
    value_type elem = '5';

    CircularBuffer cb1(cap, elem);
    CircularBuffer cb2 = cb1;

    for (int i = 0; i < cb1.GetCapacity(); i++) {
        ASSERT_EQ(cb1[i], cb2[i]);
    }
    ASSERT_EQ(cb1.GetHead(), cb2.GetHead());
    ASSERT_EQ(cb1.GetHead(), 0);
    ASSERT_EQ(cb1.GetTail(), cb2.GetTail());
    ASSERT_EQ(cb1.GetTail(), 0);
    ASSERT_EQ(cb1.GetCapacity(), cb2.GetCapacity());
    ASSERT_EQ(cb1.GetCapacity(), cap);
    ASSERT_EQ(cb1.GetSize(), cb2.GetSize());
    ASSERT_EQ(cb1.GetSize(), cap);
}

TEST(CircularBuffer, operator_assignment_for_identical_cbs)
{
    int cap = 3;
    value_type elem = '5';

    CircularBuffer cb1(cap, elem);
    CircularBuffer &result = cb1 = cb1;
    EXPECT_EQ(&result, &cb1);
}