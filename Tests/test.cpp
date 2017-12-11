//
//  Created by bogdan.mytnyk on 25.11.17.
//  Copyright � 2017 bogdan.mytnyk. All rights reserved.
//

#include "dfs_adaptor.h"
#include "bfs_adaptor.h"

#include <iostream>

int main(int argc, const char * argv[])
{
	typedef tree<int> tree_type;
	typedef tree_item<int> tree_type_int;
	
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

	return 0;
}
