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

void DFSTraversing(tree_item<int>* item, const std::function<void()>& func)
{
    func();
    
    for (size_t i = 0, count = item->count(); i < count; ++i)
    {
        DFSTraversing(item->get_child(i), func);
    };
}

void TestPerformance(uint32_t minLevel, uint32_t maxLevel, uint32_t minChildCount, uint32_t maxChildCount)
{
    auto sumRatio = 0.f;
    
    for (uint32_t curLevel = minLevel; curLevel <= maxLevel; ++curLevel)
    {
        for (uint32_t curChildCount = minChildCount; curChildCount <= maxChildCount; ++curChildCount)
        {
            auto countTraversing = 0;
            
            tree_type test_tree(GenerateTreeItem(curLevel, curChildCount));
            
            auto start = std::chrono::system_clock::now();
            
            DFSTraversing(test_tree.top(), [&countTraversing](){ ++countTraversing;});
            
            auto traverseTimeRecursive = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start).count();
            
            start = std::chrono::system_clock::now();
            
            auto countTraverseIter = std::distance(test_tree.dfs_begin(), test_tree.dfs_end());
            //std::for_each(test_tree.dfs_begin(), test_tree.dfs_end(), [&countTraverseIter](){ ++countTraverseIter;});

            auto traverseTimeIterative = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start).count();
            
            auto curRatio = static_cast<float>(traverseTimeRecursive) / traverseTimeIterative;
            sumRatio += curRatio;
        
            /*std::cout << "Cur configuration - (" << curLevel << "," << curChildCount << ")\n"
                      << "\tRecursive - " << traverseTimeRecursive << "\n"
                      << "\tIterative - " << traverseTimeIterative << "\n"
                      << "\tRatio - " << static_cast<float>(traverseTimeRecursive) / traverseTimeIterative << std::endl;*/
            
            std::cout << std::setw(10) << static_cast<float>(traverseTimeRecursive) / traverseTimeIterative;
        }
        
        std::cout << std::endl;
    }

    auto average = sumRatio / ((maxLevel - minLevel + 1) * (maxChildCount - minChildCount + 1));
    
    std::cout << "Average ratio - " << average <<  std::endl;
}

void TestBeginEndCorrectness()
{
    tree_type empty_tree;
    auto beginIter = empty_tree.dfs_begin();
    auto endIter = empty_tree.dfs_end();
    
    if (beginIter != endIter)
        throw std::runtime_error("Test iterator correctness failed");
    
    tree_type one_item_tree(2342);
}

void TestBFSTraversal()
{
    tree_type test_tree(2342);
    
    tree_item<int>* child1 = new tree_item<int>(2834);
    child1->add_child(new tree_item<int>(989));
    child1->add_child(new tree_item<int>(9439));
    
    test_tree.add_child(new tree_item<int>(334));
    test_tree.add_child(child1);
    test_tree.add_child(new tree_item<int>(533));
    
    // Expected items
    int expectedItems[] = {2342, 334, 2834, 533, 989, 9439};
    size_t i = 0;
    for (int value : make_bfs_adaptor(test_tree))
    {
        if (value != expectedItems[i++])
        {
            throw std::runtime_error("BFS Traversal failed");
        }
    }
}

void TestDFSTraversal()
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
    if (expectedCount != std::distance(test_tree.dfs_begin(), test_tree.dfs_end()))
    {
        throw std::runtime_error("BFS Traversal failed - invalid number of nodes");
    }
    
    size_t i = 0;
    for (int value : make_dfs_adaptor(test_tree))
    {
        if (value != expectedItems[i++])
        {
            throw std::runtime_error("BFS Traversal failed");
        }
    }
}

         
void TestFindSTLAlgo()
{
    tree_type test_tree(2342);
    
    tree_item<int>* child1 = new tree_item<int>(2834);
    child1->add_child(new tree_item<int>(989));
    child1->add_child(new tree_item<int>(9439));
    
    test_tree.add_child(child1);
    test_tree.add_child(new tree_item<int>(533));
    
    auto it = std::find(test_tree.dfs_begin(), test_tree.dfs_end(), 312);
    bool has = (it != test_tree.dfs_end());
    if (has)
        throw std::runtime_error("TestFindSTLAlgo failed");
    
    auto it3 = std::find(test_tree.dfs_begin(), test_tree.dfs_begin(), 533);
    has = (it3 != test_tree.dfs_end());
    if (!has)
        throw std::runtime_error("TestFindSTLAlgo failed");
}

int main(int argc, const char * argv[])
{
    try
    {
        TestBeginEndCorrectness();
        
        TestBFSTraversal();
        
        TestDFSTraversal();
        
        TestFindSTLAlgo();
        
        TestPerformance(8, 12, 2, 4);
    }
    catch (const std::exception& exc)
    {
        std::cout << exc.what() << std::endl;
        return -1;
    }
    
	return 0;
}
