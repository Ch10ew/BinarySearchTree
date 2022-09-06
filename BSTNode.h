#ifndef BST_NODE_H
#define BST_NODE_H

#include <stack>
#include <vector>

template <typename T>
class BSTNode
{
public:
    BSTNode<T>* parent = nullptr;
    BSTNode<T>* left = nullptr;
    BSTNode<T>* right = nullptr;
    T data;

public:
    BSTNode() = delete;
    BSTNode(T);

#pragma region Element Access

    BSTNode<T>* search(const T& data) const;
    BSTNode<T>* min() const;
    BSTNode<T>* max() const;
    BSTNode<T>* successor() const;
    BSTNode<T>* predecessor() const;

#pragma endregion

#pragma region Capacity

    int size() const;
    int immediateChildCount() const;

#pragma endregion

#pragma region Modifiers

    bool insert(T data);
    bool remove(T data);
    BSTNode<T>* clone();

#pragma endregion

#pragma region Lookup

    bool leaf() const;
    bool root() const;
    std::vector<T>* inorder() const;

#pragma endregion

private:
    BSTNode<T>* cloneRecursive(BSTNode<T>* root);
};

/**
 * @brief Construct a new BSTNode<T>::BSTNode object
 *
 * @tparam T Type
 * @param data Data to store in node
 */
template <typename T>
BSTNode<T>::BSTNode(T data)
    : data(data)
{
}

/**
 * @brief Search for match (using operator==) in tree
 *
 * @tparam T type
 * @param data Data to search for
 * @return BSTNode<T>* Node containing data if found, nullptr if not found
 */
template <typename T>
BSTNode<T>* BSTNode<T>::search(const T& data) const
{
    BSTNode<T>* currentVertex = const_cast<BSTNode<T>*>(this);

    if (currentVertex == nullptr) // empty subtree
    {
        return nullptr;
    }

    while (currentVertex)
    {
        if (data == currentVertex->data) // check equavalence
            return currentVertex;

        // transverse
        if (data < currentVertex->data)
            currentVertex = currentVertex->left;

        if (data > currentVertex->data)
            currentVertex = currentVertex->right;
    }

    return nullptr; // not found
}

/**
 * @brief Looks for minimum element in tree
 *
 * @tparam T type
 * @return BSTNode<T>* Node containing data if found, nullptr if tree is empty
 */
template <typename T>
BSTNode<T>* BSTNode<T>::min() const
{
    BSTNode<T>* currentVertex = const_cast<BSTNode<T>*>(this);

    if (currentVertex == nullptr) // empty subtree
    {
        return nullptr;
    }

    while (currentVertex->left) // transverse to leftmost leaf
    {
        currentVertex = currentVertex->left;
    }

    return currentVertex;
}

/**
 * @brief Looks for maximum element in tree
 *
 * @tparam T type
 * @return BSTNode<T>* Node containing data if found, nullptr if tree is empty
 */
template <typename T>
BSTNode<T>* BSTNode<T>::max() const
{
    BSTNode<T>* currentVertex = const_cast<BSTNode<T>*>(this);

    if (currentVertex == nullptr) // empty subtree
    {
        return nullptr;
    }

    while (currentVertex->right) // transverse to rightmost leaf
    {
        currentVertex = currentVertex->right;
    }

    return currentVertex;
}

/**
 * @brief Looks for successor element in tree (next largest value than current element)
 *
 * @tparam T type
 * @return BSTNode<T>* Node containing data if found, nullptr if not found
 */
template <typename T>
BSTNode<T>* BSTNode<T>::successor() const
{
    BSTNode<T>* currentVertex = const_cast<BSTNode<T>*>(this);

    if (currentVertex == nullptr) // empty subtree
    {
        return nullptr;
    }

    if (currentVertex->right) // case 1: node has right subtree
    {
        return currentVertex->right->min(); // get minimum in subtree
    }
    else // case 2: node does not have right subtree
    {
        BSTNode<T>* parent = currentVertex.parent;
        while (parent && currentVertex == parent->right) // transverse up until right turn found
        {
            currentVertex = parent;
            parent = currentVertex->parent;
        }

        if (!parent)
        {
            return nullptr;
        }

        return parent;
    }
}

/**
 * @brief Looks for predecessor element in tree (next smallest value than current element)
 *
 * @tparam T type
 * @return BSTNode<T>* Node containing data if found, nullptr if not found
 */
template <typename T>
BSTNode<T>* BSTNode<T>::predecessor() const
{
    BSTNode<T>* currentVertex = const_cast<BSTNode<T>*>(this);

    if (currentVertex == nullptr) // empty subtree
    {
        return nullptr;
    }

    if (currentVertex->left) // case 1: node has left subtree
    {
        return currentVertex->left->max(); // get maximum in subtree
    }
    else // case 2: node does not have left subtree
    {
        BSTNode<T>* parent = currentVertex.parent;
        while (parent && currentVertex == parent->left) // transverse up until left turn found
        {
            currentVertex = parent;
            parent = currentVertex->parent;
        }

        if (!parent)
        {
            return nullptr;
        }

        return parent;
    }
}

/**
 * @brief Gets the node count of tree
 *
 * @tparam T Type
 * @return int node count
 */
template <typename T>
int BSTNode<T>::size() const
{
    // @TODO: implementation for size
    std::vector<T>* result = this->inorder();

    return result->size();
}

/**
 * @brief Gets the immediate child count for the current node
 *
 * @tparam T Type
 * @return int child count
 */
template <typename T>
int BSTNode<T>::immediateChildCount() const
{
    return (this->left ? 1 : 0) +
           (this->right ? 1 : 0);
}

/**
 * @brief Inserts a node in the tree.
 *
 * Uses operators `operator>` and `operator<`
 *
 * @tparam T Type
 * @param data Data to be inserted
 * @return true Successful insertion
 * @return false Failed insertion
 */
template <typename T>
bool BSTNode<T>::insert(T data)
{
    BSTNode<T>* newVertex = new BSTNode<T>(data);
    BSTNode<T>* currentVertex = const_cast<BSTNode<T>*>(this);

    // case 1: empty subtree, insert now
    if (currentVertex->leaf())
    {
        if (data < currentVertex->data)
        {
            currentVertex->left = newVertex;
        }
        else if (data > currentVertex->data)
        {
            currentVertex->right = newVertex;
        }
        return true;
    }

    // case 2: normal insertion
    BSTNode<T>* parent = currentVertex->parent;

    while (currentVertex)
    {
        parent = currentVertex; // preserve parent before transversal

        if (data < currentVertex->data) // less than, go left
        {
            currentVertex = currentVertex->left;
        }
        else if (data > currentVertex->data) // more than, go right
        {
            currentVertex = currentVertex->right;
        }
    } // at this point, parent holds the node we need to insert to and currentVertex is nullptr

    newVertex->parent = parent;

    if (data < parent->data)
    {
        parent->left = newVertex;
    }
    else if (data > parent->data)
    {
        parent->right = newVertex;
    }

    return true;
}

template <typename T>
bool BSTNode<T>::remove(T data)
{
    BSTNode<T>* result = this->search(data);

    // case 1: not found
    if (!result)
    {
        return false;
    }

    if (result->leaf()) // case 2: leaf vertex removal
    {
        // remove link from parent side
        if (result->parent->left == result)
        {
            result->parent->left = nullptr;
        }
        else if (result->parent->right == result)
        {
            result->parent->right = nullptr;
        }

        // remove result
        delete result;
    }
    else if (result->immediateChildCount() == 1) // case 3: internal vertex with 1 child
    {
        // preserve child
        BSTNode<T>* child;
        if (result->left)
        {
            child = result->left;
        }
        else if (result->right)
        {
            child = result->right;
        }

        // create new link with parent
        if (result->parent->left == result)
        {
            result->parent->left = child;
        }
        else if (result->parent->right == result)
        {
            result->parent->right = child;
        }

        // link child's parent
        child->parent = result->parent;

        // remove result
        delete result;
    }
    else // case 4: internal vertex with 2 children
    {
        BSTNode<T>* successor = result->successor();

        // preserve links from result side
        BSTNode<T>* resultParent = result->parent;
        BSTNode<T>* resultLeftChild = result->left;
        BSTNode<T>* resultRightChild = result->right;

        // preserve links from successor side
        BSTNode<T>* successorParent = successor->parent;
        BSTNode<T>* successorLeftChild = successor->left;
        BSTNode<T>* successorRightChild = successor->right;

        // replace successor old links
        if (successorParent->left == successor)
        {
            // check if successor had child
            if (successorLeftChild)
            {
                successorParent->left = successorLeftChild;
                successorLeftChild->parent = successorParent;
            }
            else if (successorRightChild)
            {
                successorParent->left = successorRightChild;
                successorRightChild->parent = successorParent;
            }
        }

        if (successorParent->right == successor)
        {
            // check if successor had child
            if (successorLeftChild)
            {
                successorParent->right = successorLeftChild;
                successorLeftChild->parent = successorParent;
            }
            else if (successorRightChild)
            {
                successorParent->right = successorRightChild;
                successorRightChild->parent = successorParent;
            }
        }

        // replace result old links
        if (resultParent->left == result)
        {
            resultParent->left = successor;
        }
        else if (resultParent->right == result)
        {
            resultParent->right = successor;
        }

        if (resultLeftChild)
        {
            resultLeftChild->parent = successor;
        }

        if (resultRightChild)
        {
            resultRightChild->parent = successor;
        }

        // remove result
        delete result;
    }

    return true;
}

/**
 * @brief Clones the tree
 *
 * @tparam T Type
 * @return BSTNode<T>* Cloned tree
 */
template <typename T>
BSTNode<T>* BSTNode<T>::clone()
{
    return cloneRecursive(this);
}

/**
 * @brief Checks if the node is a leaf node
 *
 * @tparam T Type
 * @return true Node is a leaf node
 * @return false Node is not a leaf node
 */
template <typename T>
bool BSTNode<T>::leaf() const
{
    return !(this->left || this->right);
}

/**
 * @brief Checks if the node is a root node
 *
 * @tparam T Type
 * @return true Node is a root node
 * @return false Node is not a root node
 */
template <typename T>
bool BSTNode<T>::root() const
{
    return !this->parent;
}

/**
 * @brief Resursive in order transversal
 *
 * @tparam T Type
 * @return int
 */
template <typename T>
std::vector<T>* BSTNode<T>::inorder() const
{
    std::vector<T>* result = new std::vector<T>();
    std::stack<BSTNode<T>*> stack;
    BSTNode<T>* currentVertex = const_cast<BSTNode<T>*>(this);

    while (currentVertex || !stack.empty())
    {
        while (currentVertex)
        {
            stack.push(currentVertex);
            currentVertex = currentVertex->left;
        }

        BSTNode<T>* popped = stack.top();
        stack.pop();
        result->push_back(popped->data);
        currentVertex = popped->right;
    }

    return result;
}

/**
 * @brief [INTERNAL] Recursive BST clone implementation
 *
 * @tparam T Type
 * @return BSTNode<T>* Cloned tree
 */
template <typename T>
BSTNode<T>* BSTNode<T>::cloneRecursive(BSTNode<T>* root)
{
    if (!root) // empty subtree
    {
        return nullptr;
    }

    BSTNode<T>* rootCopy = new BSTNode<T>(root->data);

    rootCopy->left = cloneRecursive(root->left);
    rootCopy->right = cloneRecursive(root->right);

    return rootCopy;
}

#endif // BST_NODE_H
