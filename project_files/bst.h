#pragma once
#include <vector>
#include <exception>

namespace cop3530 {

template<typename Key, typename T>
class bst {
    struct node {
        // key
        Key key;
        // value
        T data;
        // left child
        node* left;
        // right child
        node* right;

        node() {}
        node(Key k, T t) {
            key(k);
            data(t);
            left = nullptr;
            right = nullptr;
        }
    };

    // root of tree
    node* root;
    //size
    int siuze;

    public:
        bst();
        ~bst();
        void destroy(node *nodePtr);

        void insert(const Key &key, const T &t);
        void insert(node *&rootNode, const Key &key, const T &t);
        void insert(node *&rootNode, node *newNode);

        void erase(const Key &key);
        typename bst<Key, T>::node* erase(const Key &key, node *&nodePtr);

        T& at(const Key &key);
        T& at(const Key &key, node *&nodePtr);

        bool contains(const Key &key) const;
        bool contains(const Key &key, node *&nodePtr) const;

        bool empty() const;
        size_t size() const;

        std::vector<std::pair<Key, T>> inorder_contents();
        std::vector<std::pair<Key, T>> inorder_contents(node *, std::vector<std::pair<Key, T>> &contents);
        
        std::vector<std::pair<Key, T>> preorder_contents(node *, std::vector<std::pair<Key, T>> &contents);
        std::vector<std::pair<Key, T>> preorder_contents();

};

template<typename Key, typename T>
bst<Key, T>::bst() {
    root = nullptr;
    siuze = 0;
}

template<typename Key, typename T>
bst<Key, T>::~bst() {
   //destroy(root);
}

template<typename Key, typename T>
void bst<Key, T>::destroy(node *nodePtr)
{
    if (nodePtr){
        destroy(nodePtr->left);
        destroy(nodePtr->right);
        delete nodePtr;
    }
}

template<typename Key, typename T>
void bst<Key, T>::insert(const Key &key, const T &t) {
    if(contains(key)){
        if(root->key == key){
            root->data = t;
        }
        else if(root->key < key){
            insert(root->right, key, t);
        }
        else{
            insert(root->left, key, t);
        }
    }
    else{
        struct node* newNode = new node;//(struct node*)malloc(sizeof(struct node));
        siuze++;
        newNode->key = key;
        newNode->data = t;
        newNode->left = nullptr;
        newNode->right = nullptr;
        
        insert(root, newNode);
    }
}

template<typename Key, typename T>
void bst<Key, T>::insert(node *&rootNode, const Key &key, const T &t) {
    if(rootNode != nullptr){
       if(rootNode->key == key){
            rootNode->data = t;
        }
        else if(rootNode->key < key){
            insert(rootNode->right, key, t);
        }
        else{
            insert(rootNode->left, key, t);
        } 
    }
}

template<typename Key, typename T>
void bst<Key, T>::insert(node *&rootNode, node *newNode) {

    if(rootNode == nullptr){
        rootNode = newNode;
    }
    else if(newNode->key < rootNode->key){
        insert(rootNode->left, newNode);
    }
    else{
        insert(rootNode->right, newNode);
    }
}

template<typename Key, typename T>
void bst<Key, T>::erase(const Key &key) {
    if(root == nullptr){
        throw std::out_of_range("out of range");
        return;
    }
    if(key < root->key){
        root->left = erase(key, root->left);
    }
    else if(key > root->key){
        root->right = erase(key, root->right);
    }
    else{
        root = erase(key, root);
    }
    siuze--;
}

template<typename Key, typename T>
typename bst<Key, T>::node* bst<Key, T>::erase(const Key &key, node *&nodePtr) {
    if(nodePtr==nullptr){
       throw std::out_of_range("out of range"); 
    }
    if(key < nodePtr->key){
        nodePtr->left = erase(key,nodePtr->left);
        return nodePtr;
    } 
    else if(key > nodePtr->key){
        nodePtr->right = erase(key, nodePtr->right);
        return nodePtr;
    }
    else{
        if(nodePtr->left==nullptr and nodePtr->right==nullptr){
            return nullptr;
        }
        else if(nodePtr->left!=nullptr xor nodePtr->right!=nullptr){
            if(nodePtr->left !=nullptr){
                return nodePtr->left;
            }
            else{
                return nodePtr->right;
            }
        }
        else{
            if(nodePtr->left->right==nullptr){
                nodePtr->left->right = nodePtr->right;
                return nodePtr->left;
            }
            else{
                struct node* current = nodePtr->left;
                struct node* temp = current;
                while(current->right != nullptr){
                    temp = current;
                    current = current->right;
                }
                nodePtr->data = current->data;
                nodePtr->key = current->key;
                temp->right = current->left;
                delete current;
                return nodePtr;
            }
        }
    }
}    

template<typename Key, typename T>
T& bst<Key, T>::at(const Key &key) {
    if(root == nullptr){
         throw std::out_of_range("out of range");
    }
    else{
       if(root->key == key){return root->data;}
       else if(root->key > key){
           return at(key, root->left);
       }
       else{
           return at(key, root->right);
       }
    }
}

template<typename Key, typename T>
T& bst<Key, T>::at(const Key &key, node *&nodePtr) {
    if(nodePtr == nullptr){
         throw std::out_of_range("out of range");
    }
    else{
       if(nodePtr->key == key){return nodePtr->data;}
       else if(nodePtr->key > key){
           return at(key, nodePtr->left);
       }
       else{
           return at(key, nodePtr->right);
       }
    }
}

template<typename Key, typename T>
bool bst<Key, T>::contains(const Key &key) const {
   if(root == nullptr){
       return false;
   }
   else{
       if(root->key == key){return true;}
       else if(root->key > key){
           return contains(key, root->left);
       }
       else{
           return contains(key, root->right);
       }
       return false;
   }
}

template<typename Key, typename T>
bool bst<Key, T>::contains(const Key &key, node *&nodePtr) const {
    if(nodePtr == nullptr){
       return false;
    }
    else{
        if(nodePtr->key == key){return true;}
        else if(nodePtr->key > key){
            return contains(key, nodePtr->left);
        }
        else{
            return contains(key, nodePtr->right);
        }
        return false;
    }
}

template<typename Key, typename T>
bool bst<Key, T>::empty() const {
    if(root != nullptr){return false;}
    return true;
}

template<typename Key, typename T>
size_t bst<Key, T>::size() const {
    if(empty()){return 0;}
    else{
       return siuze;
    }
}

template<typename Key, typename T>
std::vector<std::pair<Key, T>> bst<Key, T>::inorder_contents() {
    std::vector<std::pair<Key, T>> contents;

    if (root != nullptr){
        contents = inorder_contents(root->left, contents);
        contents.push_back(std::make_pair(root->key,root->data));
        contents = inorder_contents(root->right, contents);
    }

    return contents;
}

template<typename Key, typename T>
std::vector<std::pair<Key, T>> bst<Key, T>::inorder_contents(node *nodePtr, std::vector<std::pair<Key, T>> &contents) {

    if (nodePtr != nullptr){
        inorder_contents(nodePtr->left, contents);
        contents.push_back(std::make_pair(nodePtr->key,nodePtr->data));
        inorder_contents(nodePtr->right, contents);
    }

    return contents;
}

template<typename Key, typename T>
std::vector<std::pair<Key, T>> bst<Key, T>::preorder_contents() {
    std::vector<std::pair<Key, T>> contents;

    if (root != nullptr){
        contents.push_back(std::make_pair(root->key,root->data));
        contents = preorder_contents(root->left, contents);
        contents = preorder_contents(root->right, contents);
    }

    return contents;
}

template<typename Key, typename T>
std::vector<std::pair<Key, T>> bst<Key, T>::preorder_contents(node *nodePtr, std::vector<std::pair<Key, T>> &contents) {

    if (nodePtr != nullptr){
        contents.push_back(std::make_pair(nodePtr->key,nodePtr->data));
        contents = preorder_contents(nodePtr->left, contents);
        contents = preorder_contents(nodePtr->right, contents);
    }

    return contents;
}


}