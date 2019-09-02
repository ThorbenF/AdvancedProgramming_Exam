/** @file Test.cpp
 * The file contains a demonstration of the functionality inside the Binary Search Tree class.
   In chronological order a BST class is constructed, data-pairs are inserted and the tree is cleared afterwards.
   Again data-pairs get inserted and Iterators begin()and end() are used to print all tree nodes.
   The ConstIterator is investigated regarding pre and post-increment operators.
   Commented lines can be enabled to see the read-only behaviour.
   The tree is cleared again and repopulated via the operator[].
   The operator<< is used to print the tree.
   A const BST instance is constructed and its operator[] and operator<< are tested.
   The overwrite on insert() is checked.
   The functionality of find() is investigated and its returned iterator is checked.
   balance() is called for the non-const BST and it is printed multiple times to additionally test all copy and move semantics provided by the class.
   Commented lines can be enabled to see the causing of std::runtime_error.
 */

#include "BST.h"

int main(){

        /**testing constructor: BST()*/
        BST<int, int> BinarySearchTree{};

        /**testing function: void insert(const key k, value v)*/
        BinarySearchTree.insert(50, 50);
        BinarySearchTree.insert(20, 20);
        BinarySearchTree.insert(10, 10);
        BinarySearchTree.insert(60, 60);
        BinarySearchTree.insert(160, 160);
        BinarySearchTree.insert(360, 360);

        /**testing function: void clear()*/
        BinarySearchTree.clear();

        for (int i=8; i > 3; i--) {
                BinarySearchTree.insert(i, i);
        }
        for (int i=0; i < 4; i++) {
                BinarySearchTree.insert(i, i);
        }
        for (int i=12; i > 8; i--) {
                BinarySearchTree.insert(i, i);
        }

        /**testing iterator functions: begin(), end(), cbegin(), cend()*/
        auto it = BinarySearchTree.begin();
        auto stop = BinarySearchTree.end();

        for(; it!=stop; ++it) {
                if ((*it).second==2) (*it).second =200;
                std::cout << (*it).second << std::endl;
        }


        if (BinarySearchTree.cend()==BST<int, int>::ConstIterator{nullptr}) std::cout << "cend correct" << std::endl;
        if (*BinarySearchTree.cbegin() == std::pair<const int, int>{0,0}) std::cout << "cbegin correct" << std::endl;
        if(*((BinarySearchTree.cbegin())++) == std::pair<const int, int>{0,0}) std::cout << "cbegin operator++ correct" << std::endl;
        if(*(++(BinarySearchTree.cbegin())) == std::pair<const int, int>{1,1}) std::cout << "cbegin ++operator correct" << std::endl;
        BST<int, int>::ConstIterator cit = BinarySearchTree.cbegin();
        BST<int, int>::ConstIterator cstop = BinarySearchTree.cend();
        for(; cit!=cstop; ++cit) {
                //(*cit).second=200; //read-only can not be assigned
                std::cout << (*cit).second << std::endl;
        }

        BinarySearchTree.clear();

        /**testing value& operator[](const keye& k) non-const and const*/
        for (int i=0; i<=10; ++i) {
                BinarySearchTree[i]=i;
        }

        /**testing operator <<*/
        std::cout << BinarySearchTree;

        //const BST<int, int> ConstBST{};
        //std::cout << ConstBST[4] << std::endl; //causes std::runtime_error
        const BST<int, int> ConstBinarySearchTree = BinarySearchTree;
        std::cout <<"found value at key=4: " << ConstBinarySearchTree[4] << std::endl;
        std::cout << ConstBinarySearchTree;

        /**testing function: overwrite in void insert(const key k, value v)*/
        BinarySearchTree.insert(5, 500);

        /**testing function: ConstIterator find(const key k)*/
        auto find_it= BinarySearchTree.find(5);
        std::cout << "Searched for key 5 and found value: " << std::endl;
        std::cout <<  (*find_it).second << std::endl;
        std::cout << "Searched for key 6000 and found value: " << std::endl;
        auto find_it2= BinarySearchTree.find(6000);
        if (find_it2==nullptr) std::cout << "returned iterator is nullptr" << std::endl;

        /**testing function: void balance()*/
        BinarySearchTree.balance();
        std::cout << "original" << std::endl;
        std::cout << BinarySearchTree;

        /**testing copy constructor*/
        BST<int, int> BinarySearchTree_copy_cotr = BinarySearchTree;
        std::cout << BinarySearchTree_copy_cotr;
        /**testing copy assignment*/
        BST<int, int> BinarySearchTree_copy_assignment;
        BinarySearchTree_copy_assignment =BinarySearchTree;
        std::cout << BinarySearchTree_copy_assignment;
        /**testing move constructor*/
        BST<int, int> BinarySearchTree_move_cotr = std::move(BinarySearchTree);
        std::cout << BinarySearchTree_move_cotr;
        /**testing move assignment*/
        BST<int, int> BinarySearchTree_move_assignment;
        BinarySearchTree_move_assignment = std::move(BinarySearchTree_move_cotr);
        std::cout << BinarySearchTree_move_assignment;

        std::cout << "attempt copy self assignment" << std::endl;
        BinarySearchTree_move_assignment= BinarySearchTree_move_assignment;
        std::cout << BinarySearchTree_move_assignment;

        std::cout << "attempt move self assignment" << std::endl;
        BinarySearchTree_move_assignment = std::move(BinarySearchTree_move_assignment);
        std::cout << BinarySearchTree_move_assignment;
}
