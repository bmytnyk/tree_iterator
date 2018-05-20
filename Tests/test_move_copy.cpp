//
//  Created by bogdan.mytnyk on 5.01.18.
//  Copyright 2017 bogdan.mytnyk. All rights reserved.
//

#include "tree.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>

typedef tree<int> tree_type;
typedef tree_item<int> tree_item_type;

TEST(TestTreeItem, TestConstructor)
{
	tree_item_type root(3, { new tree_item_type{4}, new tree_item_type{5},
							new tree_item_type{6}, new tree_item_type{3} });

	EXPECT_EQ(root.value(), 3);
	const auto& children = root.children();
	EXPECT_EQ(children.size(), 4);
	EXPECT_EQ(children[0]->value(), 4);
	EXPECT_EQ(children[1]->value(), 5);
	EXPECT_EQ(children[2]->value(), 6);
	EXPECT_EQ(children[3]->value(), 3);
}

class MoveCopyTestTreeItem : public ::testing::Test
{
public:
	virtual void SetUp()
	{
		m_tree_original.reset(new tree_item_type(1));
		m_tree_original->add_child(new tree_item_type(2));
		m_tree_original->add_child(new tree_item_type(4));
	}

	virtual void TearDown()
	{
		m_tree_original.reset();
	}

	void CheckResult(tree_item_type& tree_copy, bool isMove)
	{
		EXPECT_EQ(tree_copy.value(), 1);
		const auto& children = tree_copy.children();
		EXPECT_EQ(children.size(), 2);
		EXPECT_EQ(children[0]->value(), 2);
		EXPECT_EQ(children[1]->value(), 4);

		if (isMove)
		{
			// test for consistency
			EXPECT_TRUE(m_tree_original->children().empty());
		}
	}

protected:
	std::unique_ptr<tree_item_type> m_tree_original;
};

TEST_F(MoveCopyTestTreeItem, TestMoveConstructor)
{
	tree_item_type tree_copy(std::move(*m_tree_original));
	CheckResult(tree_copy, true);
}

TEST_F(MoveCopyTestTreeItem, TestMoveOperator)
{
	tree_item_type tree_copy(0);
	tree_copy = std::move(*m_tree_original);
	CheckResult(tree_copy, true);
}

TEST_F(MoveCopyTestTreeItem, TestCopyConstructor)
{
	tree_item_type tree_copy(*m_tree_original);
	CheckResult(tree_copy, false);
}

TEST_F(MoveCopyTestTreeItem, TestCopyOperator)
{
	tree_item_type tree_copy(0);
	tree_copy = *m_tree_original;
	CheckResult(tree_copy, false);
}