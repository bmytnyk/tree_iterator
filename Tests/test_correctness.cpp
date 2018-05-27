//
//  Created by bogdan.mytnyk on 25.11.17.
//  Copyright � 2017 bogdan.mytnyk. All rights reserved.
//

#include "dfs_adaptor.h"
#include "bfs_adaptor.h"

#include <memory>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <functional>
#include <algorithm>
#include <gtest/gtest.h>

typedef tree<int> tree_type;
typedef tree_item<int> tree_type_int;

TEST(TestIterator, TestDfsBeginEnd)
{
    tree_type empty_tree;   
	EXPECT_EQ(empty_tree.dfs_begin(), empty_tree.dfs_end());
    
    tree_type one_item_tree(2342);
	auto beginIter = one_item_tree.dfs_begin();
	auto endIter = one_item_tree.dfs_end();
	EXPECT_NE(beginIter, endIter);

	EXPECT_EQ(*beginIter, 2342);
}

TEST(TestIterator, TestBFS)
{
    tree_type test_tree(2342);
    
    tree_item<int>* child1 = new tree_type_int(2834);
    child1->add_child(new tree_type_int(989));
    child1->add_child(new tree_item<int>(9439));
    
    test_tree.add_child(new tree_item<int>(334));
    test_tree.add_child(child1);
    test_tree.add_child(new tree_item<int>(533));
    
    // Expected items
    int expectedItems[] = {2342, 334, 2834, 533, 989, 9439};
    size_t i = 0;
    for (int value : make_bfs_adaptor(test_tree))
    {
		ASSERT_EQ(value, expectedItems[i++]);
    }
}

TEST(TestIterator, TestDFS)
{
    tree_type test_tree(1);
    
    tree_item<int>* child1 = new tree_item<int>(2);
    child1->add_child(new tree_item<int>(3));
    child1->add_child(new tree_item<int>(4));
    
    tree_item<int>* child2 = new tree_item<int>(5);
    child2->add_child(new tree_item<int>(6));
    child2->add_child(new tree_item<int>(7));
    child2->add_child(new tree_item<int>(8));
    
    tree_item<int>* child3 = new tree_item<int>(9);
    child3->add_child(new tree_item<int>(10));
    child3->add_child(new tree_item<int>(11));
    
    test_tree.add_child(child1);
    test_tree.add_child(child2);
    test_tree.add_child(child3);
    
    // Expected items
    int expectedItems[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    
    size_t expectedCount = sizeof(expectedItems)/sizeof(expectedItems[0]);
	EXPECT_EQ(expectedCount, std::distance(test_tree.dfs_begin(), test_tree.dfs_end()));
    
    size_t i = 0;
    for (int value : make_dfs_adaptor(test_tree))
    {
		EXPECT_EQ(value, expectedItems[i++]);
    }
}

int main(int argc, char * argv[])
{
	int res = -1;

    try
    {
		::testing::InitGoogleTest(&argc, argv);
		res = RUN_ALL_TESTS();
    }
    catch (const std::exception& exc)
    {
        std::cout << exc.what() << std::endl;
    }
	catch (...)
	{
		std::cout << "Unknown exception occured" << std::endl;
	}
    
	return res;
}
