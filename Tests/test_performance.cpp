//
//  Created by bogdan.mytnyk on 25.11.17.
//  Copyright � 2017 bogdan.mytnyk. All rights reserved.
//

#include "dfs_adaptor.h"
#include "bfs_adaptor.h"

#include <chrono>
#include <iostream>
#include <iomanip>
#include <functional>
#include <algorithm>
#include <gtest/gtest.h>

typedef tree<int> tree_type;
typedef tree_item<int> tree_type_int;

// Generate using DFT  - easier
tree_type_int* GenerateTreeItem(int levelsCount, int childsCount)
{
	tree_type_int* ret_tree_item(new tree_type_int(levelsCount * childsCount));
    
    if (levelsCount > 0)
    {
        for (int i = 0; i < childsCount; ++i)
        {
            auto* child_tree_item = GenerateTreeItem(levelsCount - 1, childsCount);
            ret_tree_item->add_child(child_tree_item);
        };
    }
    
    return ret_tree_item;
};

void DFSTraversing(tree_item<int>* item, const std::function<void(int)>& func)
{
	func(item->value());
    for (size_t i = 0, count = item->count(); i < count; ++i)
    {
        DFSTraversing(item->get_child(i), func);
    };
}

TEST(TestPerformance, TestMain)
{
	// TODO: Create test with parameters
	const uint32_t minLevel = 8;
	const uint32_t maxLevel = 11;
	const uint32_t minChildCount = 2;
	const uint32_t maxChildCount = 4;

    float sumRatio = 0.f;
    
    for (uint32_t curLevel = minLevel; curLevel <= maxLevel; ++curLevel)
    {
        for (uint32_t curChildCount = minChildCount; curChildCount <= maxChildCount; ++curChildCount)
        {
            auto countTraversing = 0;
            
            tree_type test_tree(GenerateTreeItem(curLevel, curChildCount));
            
            auto start = std::chrono::system_clock::now();

			int64_t sumRecursive = 0;
            DFSTraversing(test_tree.top(), [&sumRecursive](int cur) { sumRecursive += cur;});
            
            auto traverseTimeRecursive = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start).count();
            
            start = std::chrono::system_clock::now();
            
			int64_t sumIterative = 0;
			std::for_each(test_tree.dfs_begin(), test_tree.dfs_end(), [&sumIterative](int cur) { sumIterative += cur;});
			
            auto traverseTimeIterative = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start).count();
            
            auto curRatio = static_cast<float>(traverseTimeRecursive) / traverseTimeIterative;
            sumRatio += curRatio;
			
			EXPECT_TRUE(sumIterative == sumRecursive) << "Iterators don't work as expected - we don't get same result as recursive calculation";
        
            std::cout << std::setw(10) << static_cast<float>(traverseTimeRecursive) / traverseTimeIterative;
        }
        
        std::cout << std::endl;
    }

    auto average = sumRatio / ((maxLevel - minLevel + 1) * (maxChildCount - minChildCount + 1));

#ifndef _DEBUG
	// In debug mode we disable all optimizations so results of performance tests are not 
	// relevant. In this mode actual performance test make no sense
	EXPECT_TRUE(average >= 1.f) << "Traversing using iterator is slower then recursive";
#endif

    std::cout << "Average ratio - " << average <<  std::endl;
}

int main(int argc, char** argv)
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
