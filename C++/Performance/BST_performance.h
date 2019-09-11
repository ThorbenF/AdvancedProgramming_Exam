#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

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


BST() {
        root_node=nullptr;
        MyComparator = Functor;
};

class Iterator;
class ConstIterator;

Iterator begin();
Iterator end() {
        return Iterator{nullptr};
};
ConstIterator cbegin() const;
ConstIterator cend() const {
        return ConstIterator{nullptr};
};

void insert(const key k, value v);
void clear();
void balance();
ConstIterator find(const key k) const;
value& operator[](const key& k);
const value& operator[](const key& k) const;
BST(const BST &bst_rhs);
BST& operator=(const BST &bst_rhs);
BST(BST&& bst_rhs);
BST& operator=(BST&& bst_rhs);

~BST() = default;

};


template <class key, class value, class comparator>
class BST<key, value, comparator>::Iterator {
using node = BST<key, value, comparator>::node;

node* current_node;

public:

Iterator(node* n) : current_node{n}  {}

std::pair<const key, value>& operator*() const {
        return current_node->data_pair;
}

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

Iterator operator++(int){
        Iterator it{*this};
        ++(*this);
        return it;
}

bool operator==(const Iterator& other) {
        return current_node == other.current_node;
}
bool operator!=(const Iterator& other) {
        return !(*this == other);
}

};

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

template <class key, class value, class comparator>
typename BST<key, value, comparator>::ConstIterator BST<key, value, comparator>::find(const key k) const {

        node* current=root_node.get();
        while (current) {
                if(k==current->data_pair.first) {
                        //std::cout << "found a node with the given key" << std::endl;
                        return ConstIterator(current);
                }
                else if (k > current->data_pair.first) {
                        current=current->right.get();
                }
                else{
                        current=current->left.get();
                }
        }
        std::cout << "key is not existing" << std::endl;
        return cend();

}

template <class key, class value, class comparator>
std::ostream& operator<<(std::ostream& os, BST<key, value, comparator>& l) {
        for (auto& data_pair : l)
                os << data_pair.first << ": " << data_pair.second << std::endl;
        return os;
}

template <class key, class value, class comparator>
std::ostream& operator<<(std::ostream& os, const BST<key, value, comparator>& l) {
        BST<int, int>::ConstIterator it  = l.cbegin();
        for(; it!=nullptr; ++it)
                os << (*it).first << ": " << (*it).second << std::endl;
        return os;
}

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

template <class key, class value, class comparator>
const value& BST<key, value, comparator>::operator[](const key& k) const {
        Iterator temp = find(k);
        if(temp != cend()) return (*temp).second;
        throw std::runtime_error("tried accessing not existing key in const BST");
}


//copy semantic
template <class key, class value, class comparator>
BST<key, value, comparator>::BST(const BST &bst_rhs){
        root_node=nullptr;
        MyComparator = Functor;
        deepcopy_recursive(bst_rhs.root_node);
        //std::cout << "copy via constructor" << std::endl;
}

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

// move semantic
template <class key, class value, class comparator>
BST<key, value, comparator>::BST(BST&& bst_rhs) : root_node{std::move(bst_rhs.root_node)} {
        //std::cout << "move via constructor" << std::endl;
}

template <class key, class value, class comparator>
BST<key, value, comparator>& BST<key, value, comparator>::operator=(BST&& bst_rhs){
        root_node = std::move(bst_rhs.root_node);
        //std::cout << "move via assignment" << std::endl;
        return *this;
}

#endif
