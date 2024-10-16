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
    ASSERT_EQ(cb1.Front(), cb2.Front());
    ASSERT_EQ(cb1.Back(), cb2.Back());
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
    ASSERT_EQ(cb1.Front(), cb2.Front());
    ASSERT_EQ(cb1.Back(), cb2.Back());
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
    EXPECT_EQ(&(cb1 = cb1), &cb1);
}

TEST(CircularBuffer, getters_front_and_back)
{
    int cap = 5;

    CircularBuffer cb(cap);
    for (int i = 0; i < 3; i++) {
        cb.PushFront('0' + i);
    }

    ASSERT_EQ(cb.Front(), '2');
    ASSERT_EQ(cb.Back(), '0');
}

TEST(CircularBuffer, setter_capacity_less)
{
    int cap = 5;

    CircularBuffer cb(cap);
    cb.PushBack();
    cb.PushBack();
    cb.PopFront();
    cb.PopFront();
    for (int i = 0; i < 2; i++) {
        cb.PushBack('A' + i);
    }
    cb.SetCapacity(3);

    for (int i = 0; i < cb.GetCapacity(); i++) {
        printf("%d ", (int)cb[i]);
    }
    std::cout << std::endl;

    ASSERT_EQ(cb[2], 'A');
    ASSERT_EQ(cb[0], 'B');
}

TEST(CircularBuffer, setter_capacity_less_and_size_less)
{
    int cap = 5;

    CircularBuffer cb(cap);
    cb.PushBack();
    cb.PushBack();
    cb.PopFront();
    cb.PopFront();
    for (int i = 0; i < 3; i++) {
        cb.PushBack('A' + i);
    }
    cb.SetCapacity(2);

    for (int i = 0; i < cb.GetCapacity(); i++) {
        printf("%d ", (int)cb[i]);
    }
    std::cout << std::endl;

    ASSERT_EQ(cb[0], 'A');
    ASSERT_EQ(cb[1], 'B');
}

TEST(CircularBuffer, setter_capacity_more)
{
    int cap = 5;

    CircularBuffer cb(cap);
    cb.PushBack();
    cb.PushBack();
    cb.PushBack();
    cb.PushBack();
    cb.PopFront();
    cb.PopFront();
    cb.PopFront();
    cb.PopFront();
    for (int i = 0; i < 3; i++) {
        cb.PushBack('A' + i);
    }
    cb.SetCapacity(10);

    for (int i = 0; i < cb.GetCapacity(); i++) {
        printf("%d ", (int)cb[i]);
    }
    std::cout << std::endl;

    ASSERT_EQ(cb[4], 'A');
    ASSERT_EQ(cb[5], 'B');
    ASSERT_EQ(cb[6], 'C');
}

TEST(CircularBuffer, push_back)
{
    int cap = 5;

    CircularBuffer cb(cap);
    for (int i = 0; i < cap + 1; i++) {
        cb.PushBack('1' + i);
    }

    ASSERT_EQ(cb.at(-1), '5');
    ASSERT_EQ(cb[0], '6');
    ASSERT_EQ(cb.GetSize(), cap);
}

TEST(CircularBuffer, push_front)
{
    int cap = 5;

    CircularBuffer cb(cap);
    for (int i = 0; i < cap + 1; i++) {
        cb.PushFront('1' + i);
    }

    ASSERT_EQ(cb[0], '5');
    ASSERT_EQ(cb.at(-1), '6');
    ASSERT_EQ(cb.GetSize(), cap);
}

TEST(CircularBuffer, pop_back)
{
    int cap = 5;
    value_type elem1 = '1';
    value_type elem2 = '5';

    CircularBuffer cb(cap);
    cb.PushBack(elem1);
    cb.PushFront(elem2);
    cb.PopBack();

    ASSERT_EQ(cb[0], value_type());
    ASSERT_EQ(cb.at(-1), elem2);
    ASSERT_EQ(cb.GetSize(), 1);
}

TEST(CircularBuffer, pop_front)
{
    int cap = 5;
    value_type elem1 = '1';
    value_type elem2 = '5';

    CircularBuffer cb(cap);
    cb.PushBack(elem1);
    cb.PushFront(elem2);
    cb.PopFront();

    ASSERT_EQ(cb[0], elem1);
    ASSERT_EQ(cb.at(-1), value_type());
    ASSERT_EQ(cb.GetSize(), 1);
}

TEST(CircularBuffer, linearize)
{
    int cap = 5;

    CircularBuffer cb(cap);
    for (int i = 0; i < 4; i++) {
        cb.PushBack();
        cb.PopFront();
    }
    for (int i = 0; i < 3; i++) {
        cb.PushBack('A' + i);
    }
    value_type *mas = cb.Linearize();

    for (int i = 0; i < cap; i++) {
        printf("%d ", (int)cb[i]);
    }
    std::cout << std::endl;
    ASSERT_EQ(mas[0], 'A');
    ASSERT_EQ(mas[1], 'B');
    ASSERT_EQ(mas[2], 'C');
}

TEST(CircularBuffer, rotate)
{
    int cap = 5;

    CircularBuffer cb(cap);
    for (int i = 0; i < cap; i++) {
        cb.PushBack('A' + i);
    }
    cb.Rotate(2);

    for (int i = 0; i < cap; i++) {
        printf("%d ", (int)cb[i]);
    }
    std::cout << std::endl;
    ASSERT_EQ(cb[0], 'C');
}

TEST(CircularBuffer, resize_smaller)
{
    int cap = 5;
    int new_size = 3;
    value_type item = '5';

    CircularBuffer cb(cap);
    for (int i = 0; i < 2; i++) {
        cb.PushBack();
        cb.PopFront();
    }
    for (int i = 0; i < cap; i++) {
        cb.PushBack('A' + i);
    }
    cb.Resize(new_size, item);

    for (int i = 0; i < cb.GetCapacity(); i++) {
        printf("%d ", (int)cb[i]);
    }
    std::cout << std::endl;
    ASSERT_EQ(cb[0], (char)0);
    ASSERT_EQ(cb[1], (char)0);
}

TEST(CircularBuffer, resize_larger)
{
    int cap = 5;
    int new_size1 = 4, new_size2 = 7;
    value_type item = '5';

    CircularBuffer cb(cap);
    for (int i = 0; i < 3; i++) {
        cb.PushBack('A' + i);
    }

    cb.Resize(new_size1, item);
    for (int i = 0; i < cb.GetCapacity(); i++) {
        printf("%d ", (int)cb[i]);
    }
    std::cout << std::endl;
    ASSERT_EQ(cb[3], '5');

    cb.Resize(new_size2, item);
    for (int i = 0; i < cb.GetCapacity(); i++) {
        printf("%d ", (int)cb[i]);
    }
    std::cout << std::endl;
    for (int i = 4; i < cb.GetCapacity(); i++) {
        ASSERT_EQ(cb[3], '5');
    }
}

TEST(CircularBuffer, swap_correct)
{
    int cap = 3;

    CircularBuffer cb1(cap);
    for (int i = 0; i < cap; i++) {
        cb1.PushBack('A' + i);
    }
    CircularBuffer cb2(cap);
    cb2.PushBack();
    cb2.PopFront();
    for (int i = 0; i < cap - 1; i++) {
        cb2.PushBack('0' + i);
    }
    cb1.Swap(cb2);

    ASSERT_EQ(cb1[0], value_type());
    ASSERT_EQ(cb2[0], 'A');
    ASSERT_EQ(cb1[1], '0');
    ASSERT_EQ(cb2[1], 'B');
    ASSERT_EQ(cb1[2], '1');
    ASSERT_EQ(cb2[2], 'C');
    ASSERT_EQ(cb1.GetSize(), 2);
    ASSERT_EQ(cb2.GetSize(), 3);
}

TEST(CircularBuffer, swap_incorrect)
{
    int cap1 = 3;
    int cap2 = 5;

    CircularBuffer cb1(cap1);
    CircularBuffer cb2(cap2);

    ASSERT_THROW(cb1.Swap(cb2), std::invalid_argument);
}

TEST(CircularBuffer, erase_correct)
{
    int cap = 5;

    CircularBuffer cb(cap);
    for (int i = 0; i < cap; i++) {
        cb.PushBack('A' + i);
    }
    cb.Erase(1, 3);

    ASSERT_EQ(cb[1], value_type());
    ASSERT_EQ(cb[2], value_type());
}

TEST(CircularBuffer, erase_incorrect)
{
    int cap = 5;

    CircularBuffer cb(cap);
    for (int i = 0; i < cap; i++) {
        cb.PushBack('A' + i);
    }

    ASSERT_THROW(cb.Erase(3, 1), std::invalid_argument);
}

TEST(CircularBuffer, is_equal)
{
    int cap1 = 3, cap2 = 5;

    CircularBuffer cb1(cap1);
    CircularBuffer cb2(cap1);
    for (int i = 0; i < cap1; i++)
    {
        cb1.PushBack('A' + i);
        cb2.PushBack('A' + i);
    }
    CircularBuffer cb3(cap2);
    for (int i = 0; i < cap2; i++) {
        cb3.PushBack('0' + i);
    }

    ASSERT_TRUE(cb1 == cb2);
    ASSERT_FALSE(cb1 == cb3);
}

TEST(CircularBuffer, is_unequal)
{
    int cap1 = 3, cap2 = 5;

    CircularBuffer cb1(cap1);
    CircularBuffer cb2(cap1);
    for (int i = 0; i < cap1; i++) {
        cb1.PushBack('A' + i);
        cb2.PushBack('A' + i);
    }
    CircularBuffer cb3(cap2);
    for (int i = 0; i < cap2; i++) {
        cb3.PushBack('0' + i);
    }

    ASSERT_TRUE(cb1 != cb3);
    ASSERT_FALSE(cb1 != cb2);
}