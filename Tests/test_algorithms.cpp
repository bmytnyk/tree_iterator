//
//  Created by bogdan.mytnyk on 25.11.17.
//  Copyright � 2017 bogdan.mytnyk. All rights reserved.
//

#include "dfs_adaptor.h"
#include "bfs_adaptor.h"
#include "gtest/gtest.h"

#include <memory>
#include <iostream>
#include <functional>
#include <algorithm>

typedef tree<int> tree_int;
typedef tree_item<int> tree_item_int;

template <typename IteratorAdaptorType>
class TestAlgorithm : public ::testing::Test
{
};

TYPED_TEST_CASE_P(TestAlgorithm);

TYPED_TEST_P(TestAlgorithm, TestFind)
{
	tree_int test_tree(2342);
	TypeParam adaptor(test_tree);

	std::unique_ptr<tree_item_int> child1(new tree_item_int(2834));
	child1->add_child(new tree_item_int(989));
	child1->add_child(new tree_item_int(9439));

	test_tree.add_child(child1.release());
	test_tree.add_child(new tree_item_int(533));

	auto itNotFound = std::find(adaptor.begin(), adaptor.end(), 312);
	EXPECT_EQ(itNotFound, adaptor.end());

	auto itFound = std::find(adaptor.begin(), adaptor.begin(), 533);
	EXPECT_NE(itFound, adaptor.end());
}

TYPED_TEST_P(TestAlgorithm, TestDistance)
{
	tree_int test_tree(2342);
	TypeParam iter_adaptor(test_tree);

	auto distance = std::distance(iter_adaptor.begin(), iter_adaptor.end());
	EXPECT_EQ(distance, 1);

	std::unique_ptr<tree_item_int> child1(new tree_item_int(2834));
	child1->add_child(new tree_item_int(989));
	child1->add_child(new tree_item_int(9439));

	test_tree.add_child(child1.release());
	test_tree.add_child(new tree_item_int(533));
	
	distance = std::distance(iter_adaptor.begin(), iter_adaptor.end());
	EXPECT_EQ(distance, 5);
}

REGISTER_TYPED_TEST_CASE_P(TestAlgorithm, TestFind, TestDistance);

template <typename T>
using TestedTypes = ::testing::Types<dfs_adaptor<T>, bfs_adaptor<T>,
									 const_dfs_adaptor<T>, const_bfs_adaptor<T>>;
INSTANTIATE_TYPED_TEST_CASE_P(My, TestAlgorithm, TestedTypes<int>);
