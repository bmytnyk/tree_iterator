//
//  Created by bogdan.mytnyk on 25.11.17.
//  Copyright � 2017 bogdan.mytnyk. All rights reserved.
//

#include "../dfs_adaptor.h"
#include "../bfs_adaptor.h"

#include <chrono>
#include <iostream>
#include <functional>

typedef tree<int> tree_type;
typedef tree_item<int> tree_type_int;

// Generate using DFT  - easier
tree_item<int>* GenerateTreeItem(int levelsCount, int childsCount)
{
    tree_item<int>* ret_tree_item(new tree_item<int>(levelsCount * childsCount));
    
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

void DFSTraversing(tree_item<int>* item, const std::function<void(int&)>& func)
{
    func(item->value());
    
    for (size_t i = 0; i < item->count(); ++i)
    {
        DFSTraversing(item->get_child(i), func);
    };
}

void TestPerformance(uint32_t minLevel, uint32_t maxLevel, uint32_t minChildCount, uint32_t maxChildCount)
{
    auto averageOfAverage = 0.f;
    
    for (int i = 0; i < 10; ++i)
    {
        auto sumRatio = 0.f;
        
        for (uint32_t curLevel = minLevel; curLevel <= maxLevel; ++curLevel)
        {
            for (uint32_t curChildCount = minChildCount; curChildCount <= maxChildCount; ++curChildCount)
            {
                tree_type test_tree(GenerateTreeItem(curLevel, curChildCount));
                
                auto start = std::chrono::system_clock::now();
                
                DFSTraversing(test_tree.top(), [](int& value){ ++value;});
                
                auto traverseTimeRecursive = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start).count();
                
                start = std::chrono::system_clock::now();
                
                std::for_each(test_tree.dfs_begin(), test_tree.dfs_end(), [](int& value){ ++value;});

                auto traverseTimeIterative = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start).count();
                
                auto curRatio = static_cast<float>(traverseTimeRecursive) / traverseTimeIterative;
                sumRatio += curRatio;
            
                std::cout << "Cur configuration - (" << curLevel << "," << curChildCount << ")\n"
                          << "\tRecursive - " << traverseTimeRecursive << "\n"
                          << "\tIterative - " << traverseTimeIterative << "\n"
                          << "\tRatio - " << static_cast<float>(traverseTimeRecursive) / traverseTimeIterative << std::endl;
                
            }
        }
    
        auto average = sumRatio / ((maxLevel - minLevel + 1) * (maxChildCount - minChildCount + 1) * 10);
        averageOfAverage += average;
        
        std::cout << "Average ratio - " << average <<  std::endl;
    }
    
    std::cout << "Average of average - " << averageOfAverage;
}

int main(int argc, const char * argv[])
{
    TestPerformance(8, 12, 2, 4);
    
    tree_type::bfs_iterator it1, it2;
	
	tree_type test_tree(2342);

	tree_item<int>* child1 = new tree_item<int>(2834);
	child1->add_child(new tree_item<int>(989));
	child1->add_child(new tree_item<int>(9439));

	test_tree.add_child(new tree_item<int>(334));
	test_tree.add_child(child1);
	test_tree.add_child(new tree_item<int>(533));

	auto beginIter = test_tree.bfs_begin();
	auto endIter = test_tree.bfs_end();
	bool equal = (beginIter == endIter);

	std::cout << "DFS traversing : \n";
	for (const auto& item : make_dfs_adaptor(test_tree))
		std::cout << item << " ; ";
	std::cout << "\n";
    
    const auto& test_tree_ref = test_tree;
	std::cout << "BFS traversing : \n";
	for (const auto& item : make_bfs_adaptor(test_tree_ref))
		std::cout << item << " ; ";
	std::cout << std::endl;
    
    //auto dfs_tree = make_dfs_adaptor(test_tree);
    
    auto it = std::find(test_tree.bfs_begin(), test_tree.bfs_end(), 312);
    bool has = (it != test_tree.bfs_end());
    
    it = std::find(test_tree.bfs_begin(), test_tree.bfs_end(), 533);
    has = (it != test_tree.bfs_end());
    
	return 0;
}
