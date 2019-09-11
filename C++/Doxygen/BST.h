#ifndef DATE_H
#define DATE_H

/** @mainpage Binary Search Tree
 * @author Thorben Fröhlking
 * @date 30 August 2019
 * @section Abstract
 * A Binary Search Tree (BST) class was developed. A BST is a node-based ordered data structure. Left subtrees of a node contain only nodes with keys lesser and right subtrees only contain nodes with keys greater than the node’s key. The left and right subtree are also BSTs. The framework offers storing pairs of key and value, iterating from smallest to largest key through nodes, balancing the tree such that lookup complexity reduces to approximately Olog(N), finding the value of a given key, copy and move semantic as well as printing the tree traversed in ascending order.
 * @subsection Files
 * The documented files are the BST class 'BST.h' containing the entire functionality, the 'Test.cpp' being the main method for demonstration purposes and the 'Performance.cpp' is the method needed for the performance run regarding average lookup times.
 * @file BST.h
 * The file contains the entire functionality of the BST. For demonstration purposes additional output stream statements can be enabled.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

/**
 * A templated functor is setup for comparison taking references to two std::pairs
 * @param[in]  lhs,rhs  Reference to the two data pairs that shall be compared.
 * @param[out] int 0 if larger, 1 if smaller and 2 if equal.
 */
template <class key, class value>
int Functor(std::pair<const key,value>& lhs, std::pair<const key,value>& rhs)
{
        if(lhs.first > rhs.first)
                return 0;
        else if (lhs.first < rhs.first)
                return 1;
        else
                return 2;
}

/**
 * The BST class is templated on the type of the const key and the type of the value associated
   with it allowing for variability in input data and the Functor which template type is derived via decltype().
   Nodes are realised as nested struct containing data, smart pointers to the next left and right node
   as well as a pointer to the local root.
 */

template <class key, class value, class comparator = decltype(& Functor<const key,value>) >
class BST
{
private:
struct node
{
        std::pair<const key, value> data_pair;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;
        node* local_root;
        node(const std::pair<const key, value>&p, node* l, node* r, node* lr) :
                data_pair{p},left{l},right{r}, local_root{lr} {
        }

        ~node() = default;

};

std::unique_ptr<node> root_node;
comparator MyComparator;

void add_node_recursive(std::pair<const key, value> p, node* current);
void balance_recursive(std::vector<std::pair<const key, value> >& vec, std::size_t start, std::size_t end);
void deepcopy_recursive(const std::unique_ptr<node>& node);

public:

/** @brief Each BST class is constructed with a smart pointer to the root node and a comparison functor. */
BST() {
        root_node=nullptr;
        MyComparator = Functor;
};

class Iterator;
class ConstIterator;

/** @brief Iterator to the node with lowest key. */
Iterator begin();
/** @brief Iterator Returns nullptr. */
Iterator end() {
        return Iterator{nullptr};
};
/** @brief ConstIterator to the node with lowest key. */
ConstIterator cbegin() const;
/** @brief ConstIterator Returns nullptr. */
ConstIterator cend() const {
        return ConstIterator{nullptr};
};

/** @param[in] k, v arbitrary input key and value.
   @brief Making use of the comparison functor allows adding a pair
   of key and value as soon as a nullptr is encountered in the next smart pointer of the current node. */
void insert(const key k, value v);
/** @brief Resets the root-node back to nullptr. */
void clear();
/** @brief Not in-place balancing of the tree in a recursive manner. */
void balance();
/** @brief Searches for a given key.
   @param[in] k target key.
   @param[out] ConstIterator to the corresponding node. */
ConstIterator find(const key k) const;
/** @brief Operator allowing value access via key.
   @param[in] k target key.
   @param[out] value& reference to the corresponding value. */
value& operator[](const key& k);
/** @brief Const operator allowing value access via key.
   @param[in] k target key.
   @param[out] value& constant reference to the corresponding value. */
const value& operator[](const key& k) const;
/** @brief Copy constructor which constructs a deepcopy with new root-node
   following the original node structure in a recursive manner and inserting
   new nodes with the same std::pair as the original.
   @param[in] bst_rhs BST object to copy from. */
BST(const BST &bst_rhs);
/** @brief Copy assignment.
   @param[in] =bst_rhs BST object to copy from.
   @param[out] lhs& Reference to lhs BST object which is a deepcopy.*/
BST& operator=(const BST &bst_rhs);
/** @brief Move constructor which allows that the ownership of the root-node is transferred to the lvalue.
   @param[in] bst_rhs BST object to acquire ownership from. */
BST(BST&& bst_rhs);
/** @brief Move assignment.
   @param[in] =bst_rhs BST object to obtain ownership from.
   @param[out] lhs& Reference to lhs BST object. */
BST& operator=(BST&& bst_rhs);
/** @brief Default destructor. */
~BST() = default;

};

/**  @brief Iterator uses struct node of BST() class.  */
template <class key, class value, class comparator>
class BST<key, value, comparator>::Iterator {
using node = BST<key, value, comparator>::node;

node* current_node;

public:

/**  @brief Iterator is constructed with a raw pointer to the current node. */
Iterator(node* n) : current_node{n}  {}

/**  @brief operator*() is overloaded to return reference to the data-pair. */
std::pair<const key, value>& operator*() const {
        return current_node->data_pair;
}

/**  @brief Pre-increment operator++() is overloaded to move to the next right node and if that is not possible to keep moving up in the tree
   until the next largest key is encountered.
   @param[out] *this Iterator reference of current node. */
Iterator& operator++() {
        if (current_node->right != nullptr) {
                current_node = current_node->right.get();
                while(current_node->left!=nullptr) {
                        current_node=current_node->left.get();
                }
                return *this;
        }
        else {
                node* temp_root= current_node->local_root;
                while (temp_root != nullptr && current_node == temp_root->right.get()) {
                        current_node = temp_root;
                        temp_root=current_node->local_root;
                }
                current_node=temp_root;
                return *this;
        }
}

/**  @brief Post-increment operator++(int) is enabled. */
Iterator operator++(int){
        Iterator it{*this};
        ++(*this);
        return it;
}

/**  @brief Operator for equality check between two iterators.
   @param[out] bool true if equal. */
bool operator==(const Iterator& other) {
        return current_node == other.current_node;
}

/**  @brief Operator for equality check between two iterators.
   @param[out] bool true if not equal. */
bool operator!=(const Iterator& other) {
        return !(*this == other);
}

};


/**  @brief ConstIterator is using the functionality of the Iterator class, but is preventing the
   accessed elements to be manipulated by returning operator*() const. */
template <class key, class value, class comparator>
class BST<key, value, comparator>::ConstIterator : public BST<key, value, comparator>::Iterator {
public:
using parent = const BST<key, value, comparator>::Iterator;
using parent::Iterator;
std::pair<const key, value> operator*() const {
        return parent::operator*();
}
};

template <class key, class value, class comparator>
typename BST<key, value, comparator>::Iterator BST<key, value, comparator>::begin() {
        node* current = root_node.get();
        while (current->left != nullptr) {
                current = current->left.get();
        }
        return Iterator{current};
}


template <class key, class value, class comparator>
typename BST<key, value, comparator>::ConstIterator BST<key, value, comparator>::cbegin() const {
        node* current = root_node.get();
        while (current->left != nullptr) {
                current = current->left.get();
        }
        return ConstIterator{current};
}



/** When the tree is empty a first node is constructed and the smart pointer to root-node is moved.
Adding additional nodes is allowed via a the private member function add_node_recursive(std::pair<const key, value> p, node* current).
It uses the functor for comparison and moves along the branches accordingly until a nullptr is encountered.
If the key was not existing a new node is contructed and
previous left or right smart pointer is moved to the new node. if the key was existing the value is overwritten.
 */
template <class key, class value, class comparator>
void BST<key, value, comparator>::insert(const key k, value v){

        std::pair<const key, value> p(k, v);

        if (root_node==nullptr) {
                std::unique_ptr<node> elem (new node{p, nullptr, nullptr, nullptr});
                root_node=std::move(elem);
        }
        else{
                add_node_recursive(p,root_node.get());
        }
        //std::cout << "inserted node successfully" << std::endl;
}

template <class key, class value, class comparator>
void BST<key, value, comparator>::add_node_recursive(std::pair<const key, value> p, node* current){
        if (MyComparator(p, current->data_pair)==2) {
                current->data_pair.second=p.second;
                return;
        }

        if (MyComparator(p, current->data_pair)==1) {
                if (current->left == nullptr) {
                        std::unique_ptr<node> elem (new node{p, nullptr, nullptr, current});
                        current->left=std::move(elem);
                        return;
                }
                current= current->left.get();
        }
        if (MyComparator(p, current->data_pair)==0) {
                if (current->right == nullptr) {
                        std::unique_ptr<node> elem (new node{p, nullptr, nullptr, current});
                        current->right=std::move(elem);
                        return;
                }
                current= current->right.get();
        }
        add_node_recursive(p, current);
}


template <class key, class value, class comparator>
void BST<key, value, comparator>::clear() {
        root_node=nullptr;
        //std::cout << "root_node reset" << std::endl;
}

/** Upon calling the function stores all std:pairs contained in the tree inside a std::vector in ascending order according to key.
The tree is set to empty via clear() and rebuild in a recursive manner using the private member function balance_recursive(std::vector<std::pair<const key, value> >& vec, std::size_t start, std::size_t end).
This function recursively determines a median and inserts its data-pair back into the tree via insert().
Initially the entire left side of the tree is rebuild with the call to balance_recursive().
As soon as this half of the vector is down to a single data-pair the same procedure is carried out for the right side of the vector.
*/
template <class key, class value, class comparator>
void BST<key, value, comparator>::balance() {
        if (root_node == nullptr) {
                //std::cout << "attempted balancing empty BST" << std::endl;
                return;
        }
        std::vector<std::pair<const key, value> > temp_container;
        for (ConstIterator it = cbegin(); it != nullptr; ++it)
                temp_container.push_back(*it);
        clear();
        balance_recursive(temp_container,0,temp_container.size());

}

template <class key, class value, class comparator>
void BST<key, value, comparator>::balance_recursive(std::vector<std::pair<const key, value> >& vec, std::size_t start, std::size_t end){
        if(end-start==0) return;
        std::size_t temp_mid = (start + end) / 2;
        insert(vec[temp_mid].first,vec[temp_mid].second);
        balance_recursive(vec, start, temp_mid);
        balance_recursive(vec, temp_mid+1, end);
}

/** The search starts with a temporary raw pointer to the root-node and continues until either the target k is found and returned as ConstIterator
to the corresponding nodes or a nullptr is met and returned.
*/
template <class key, class value, class comparator>
typename BST<key, value, comparator>::ConstIterator BST<key, value, comparator>::find(const key k) const {

        node* current=root_node.get();
        while (current) {
                if(k==current->data_pair.first) {
                        return ConstIterator(current);
                }
                else if (k > current->data_pair.first) {
                        current=current->right.get();
                }
                else{
                        current=current->left.get();
                }
        }
        //std::cout << "key is not existing" << std::endl;
        return cend();

}

/** @brief Override to allow printing of the BST in the format 'key : value'
*/
template <class key, class value, class comparator>
std::ostream& operator<<(std::ostream& os, BST<key, value, comparator>& l) {
        for (auto& data_pair : l)
                os << data_pair.first << ": " << data_pair.second << std::endl;
        return os;
}

/** @brief Override to allow printing of a const BST in the format 'key : value' via a ConstIterator handling.
*/
template <class key, class value, class comparator>
std::ostream& operator<<(std::ostream& os, const BST<key, value, comparator>& l) {
        BST<int, int>::ConstIterator it  = l.cbegin();
        for(; it!=nullptr; ++it)
                os << (*it).first << ": " << (*it).second << std::endl;
        return os;
}

/** find() is used to search and return the key. If the key can not be found a new key is inserted with value{} and returned. */
template <class key, class value, class comparator>
value& BST<key, value, comparator>::operator[](const key& k){
        Iterator temp = find(k);
        if(temp != end()) return (*temp).second;
        else{
                insert(k, value{});
                //std::cout << "inserted missing key with value{}" << std::endl;
                temp = find(k);
                return (*temp).second;
        }

}

/** find() is also used for the const override. However if the key can not be found a std::runtime error is thrown. */
template <class key, class value, class comparator>
const value& BST<key, value, comparator>::operator[](const key& k) const {
        Iterator temp = find(k);
        if(temp != cend()) return (*temp).second;
        throw std::runtime_error("Attempt of accessing not existing key in const BST.");
}


/** a root-node and a functor are initialised. The private member function deepcopy_recursive(), which is taking a reference to the current node of the original tree, is used
to build a new tree with the same components as the original by using insert().
First the entire left tree side is copied recursively until a nullptr is encountered in rhs tree and then the same is repeated recursively
for the right half of the tree.
*/
template <class key, class value, class comparator>
BST<key, value, comparator>::BST(const BST &bst_rhs){
        root_node=nullptr;
        MyComparator = Functor;
        deepcopy_recursive(bst_rhs.root_node);
        //std::cout << "copy via constructor" << std::endl;
}

/** First the copy assignement clears the lhs and proceeds like the copy constructor afterwards.
It also contains a handling for self assignment.
*/
template <class key, class value, class comparator>
BST<key, value, comparator>& BST<key, value, comparator>::operator=(const BST &bst_rhs){
        if (this == &bst_rhs) {
                //std::cout << "self assignment" << std::endl;
                return *this;
        }
        clear();
        deepcopy_recursive(bst_rhs.root_node);
        //std::cout << "copy via assignment" << std::endl;
        return *this;
}

template <class key, class value, class comparator>
void BST<key, value, comparator>::deepcopy_recursive(const std::unique_ptr<node>& node){
        if(node!=nullptr)
        {
                insert(node->data_pair.first,node->data_pair.second);
                deepcopy_recursive(node->left);
                deepcopy_recursive(node->right);
        }
}

/** Takes rvalue reference && and uses std::move() to obtain ownership over the rhs root-node.*/
template <class key, class value, class comparator>
BST<key, value, comparator>::BST(BST&& bst_rhs) : root_node{std::move(bst_rhs.root_node)} {
        //std::cout << "move via constructor" << std::endl;
}

/** Takes rvalue reference && and uses std::move() to obtain ownership over the rhs root-node.*/
template <class key, class value, class comparator>
BST<key, value, comparator>& BST<key, value, comparator>::operator=(BST&& bst_rhs){
        root_node = std::move(bst_rhs.root_node);
        //std::cout << "move via assignment" << std::endl;
        return *this;
}

#endif
