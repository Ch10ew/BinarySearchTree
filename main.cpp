#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "BSTNode.h"

void printLevelNodes(BSTNode<int>* root);

int main()
{
    BSTNode<int>* root = new BSTNode<int>(123);

    root->insert(65);
    root->insert(78);
    root->insert(126);
    root->insert(125);
    root->insert(234);

    // Example: Inorder transversal
    {
        std::cout << "Inorder: ";
        std::vector<int>* result = root->inorder();
        for (int i = 0; i < result->size(); ++i)
        {
            std::cout << (*result)[i] << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;

        delete result;
    }

    // Example: Cloning of subtree
    {
        BSTNode<int>* subtreeClone = root->search(126)->clone();
        std::cout << "Inorder: ";
        std::vector<int>* result = subtreeClone->inorder();
        for (int i = 0; i < result->size(); ++i)
        {
            std::cout << (*result)[i] << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;

        delete result;
    }

    // Example: Printing level nodes
    {
        printLevelNodes(root);
    }

    return 0;
}

void printLevelNodes(BSTNode<int>* root)
{
    // Here, we make a 2D vector but only 2 elements will be used

    // The concept that will be adopted is similar to double buffers
    // in graphics programming, where the 2 vectors are swapped
    // and used in an alternate pattern
    std::array<std::vector<BSTNode<int>*>, 2> levelNodes;

    levelNodes[0].push_back(root); // genesis

    int level = 1;
    bool isUseFirst = false;
    while (!levelNodes[0].empty() || !levelNodes[1].empty())
    {
        std::cout << "Level " << level << " nodes: ";
        for (BSTNode<int>* node : levelNodes[(int)isUseFirst])
        {
            std::cout << node->data << " ";

            if (node->left)
            {
                levelNodes[(int)!isUseFirst].push_back(node->left);
            }

            if (node->right)
            {
                levelNodes[(int)!isUseFirst].push_back(node->right);
            }

            levelNodes[(int)isUseFirst].pop_back();
        }
        std::cout << std::endl;
        isUseFirst = !isUseFirst;
        ++level;
    }
}
