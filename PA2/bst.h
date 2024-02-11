#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
using namespace std;

// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class BST {
private:
    // Define TreeNode for the second phase (Binary Search Tree)
    /* DO NOT CHANGE */
    struct TreeNode {
        std::string key;
        T data;
        TreeNode* left;
        TreeNode* right;
        TreeNode(const std::string &k, const T& value) : key(k), data(value), left(NULL), right(NULL) {}
        
    };

    TreeNode* root;


public: // Do not change.

    BST();
    ~BST();
    TreeNode* getRoot() { return root; }
    bool isEmpty() { return root == NULL; }

    BST& insert(const std::string key, const T& value);
    bool search(std::string value) const;
    void remove(std::string value);
    BST<T>* merge(BST<T>* bst);
    BST<T>* intersection(BST<T>* bst);
    std::vector<TreeNode> tree2vector(TreeNode* root);
    void print();
    
private:// you may add your own utility member functions here.
    void tree2vector(TreeNode* node, vector<TreeNode>& result) {
        if (node != NULL) {
            tree2vector(node->left, result);
            result.push_back((*node));
            tree2vector(node->right, result);
        }
    }
    struct less_than_key
    {
        inline bool operator() (const TreeNode& struct1, const TreeNode& struct2)
        {
            return (struct1.key < struct2.key);
        }
    };


    bool isEqualVectors(vector<TreeNode> a, vector<TreeNode> b){
        if(a.size() != b.size()){return false;}
        
        else{
            int size = a.size();
            sort(a.begin(), a.end(), less_than_key());
            sort(b.begin(), b.end(), less_than_key());
            for(int i = 0; i < size; i++){
                if(a[i].key != b[i].key){
                    return false;
                }
            }
            return true;
        }
    }


    TreeNode* findMin(TreeNode* node){
        while (node->left != NULL){
            node = node->left;
        }
        return node;
    }
    TreeNode* remove(TreeNode* current, const std::string& key) {
        if(current == NULL){
            return NULL;
        }

        if(key < current->key){
            current->left = remove(current->left, key);
        } 
        
        else if(key > current->key){
            current->right = remove(current->right, key);
        } 
        
        else{
            if(current->left == NULL){
                TreeNode* temp = current->right;
                delete current;
                return temp;
            } 
            
            else if(current->right == NULL){
                TreeNode* temp = current->left;
                delete current;
                return temp;
            }
            
            TreeNode* successor = findMin(current->right);
            current->key = successor->key;
            current->right = remove(current->right, successor->key);
        }
        
        return current;
    }
    
    void removeSubTree(TreeNode* node){
        if(node != NULL){
            if(node->left != NULL){
                removeSubTree(node->left);
            }
            if(node->right != NULL){
                removeSubTree(node->right);
            }
            delete node;
        }
    }

    void print(TreeNode* node, std::string indent, bool last, bool isLeftChild); // Do not change.
    
};

    // Constructor
    template <class T>
    BST<T>::BST() : root(NULL) {}

    // Destructor
    template <class T>
    BST<T>::~BST() {
    /* IMPLEMENT THIS */
        removeSubTree(root);

    }

    // Insert function for BST.    
    template <class T>
    BST<T>& BST<T>::insert(const string key, const T& value) {
        /* IMPLEMENT THIS */
        if(isEmpty()){
            root = new TreeNode(key, value);
            return *this;
        }
        TreeNode* temp = root;
        while(temp){
            if(key < temp->key){
                if(temp->left == NULL){
                    temp->left = new TreeNode(key, value);
                    return *this;
                }
                temp = temp->left;
            }
        
            else if(key > temp->key){
                if(temp->right == NULL){
                    temp->right = new TreeNode(key, value);
                    return *this;
                }
                temp = temp->right;
            }
            
        }    
    
    }
    
    // Search function for BST.
    template <class T>
    bool BST<T>::search(std::string value) const {
        /* IMPLEMENT THIS */   
        TreeNode* temp = root;
        
        while(temp){
            if(value == temp->key){
                return true;
            }
            else if(value < temp->key){
                temp = temp -> left;
            }
            else{
                temp = temp->right;
            }
        }
        return false;
    }
    
    // Remove a node from BST for given key. If key not found, do not change anything.
    template <class T>
    void BST<T>::remove(std::string key) {
        /* IMPLEMENT THIS */
        root = remove(root,key);
    }
    
    // A helper function for converting a BST into vector.
    template <class T>
    vector<typename BST<T>::TreeNode> BST<T>::tree2vector(TreeNode* root) {
        vector<TreeNode> result;
        return result;
    }
    
    // Merge two BST's and return merged BST.
    template <class T>
    BST<T>* BST<T>::merge(BST<T>* bst) {
    /* IMPLEMENT THIS */
        vector<TreeNode> thisVector = tree2vector(root);
        vector<TreeNode> bstVector = bst->tree2vector(bst->root);
        
        tree2vector(root, thisVector);
        tree2vector(bst->root, bstVector);
        
        
        if(thisVector.empty() && bstVector.empty()){
            BST* newBST = new BST();
            return newBST;
        }
        
        if(isEqualVectors(thisVector, bstVector) || thisVector.empty() || bstVector.empty()){
            if(!thisVector.empty()){  
                sort(thisVector.begin(), thisVector.end(), less_than_key());
                BST* newBST = new BST();
                for(int i = 0; i < thisVector.size(); i++){
                    newBST->insert(thisVector[i].key, thisVector[i].data);
                }
                return newBST;
            }
            else if(!bstVector.empty()){
                sort(bstVector.begin(), bstVector.end(), less_than_key());
                BST* newBST = new BST();
                for(int i = 0; i < bstVector.size(); i++){
                    newBST->insert(bstVector[i].key, bstVector[i].data);
                }
                
                
                return newBST;
            }
        }

        else{
            vector<TreeNode> mergedVector = thisVector;
            int size = mergedVector.size();
            int flag;
            for(int i = 0; i < bstVector.size(); i++){
                flag = 0;
                for(int j = 0; j < size; j++){
                    if(mergedVector[j].key == bstVector[i].key){
                        flag = 1;
                    }
                }
                if(flag == 0){
                    mergedVector.push_back(bstVector[i]);
                }
            }
            
            sort(mergedVector.begin(), mergedVector.end(), less_than_key());
            BST* newBST = new BST();
            
            
            for(int i = 0; i < mergedVector.size(); i++){
                newBST->insert(mergedVector[i].key, mergedVector[i].data);
            }
            return newBST;
        }
        
    }
        
    // Intersect two BST's and return new BST.
    template <class T>
    BST<T>* BST<T>::intersection(BST<T>* bst) {
    /* IMPLEMENT THIS */
        vector<TreeNode> thisVector = tree2vector(root);
        vector<TreeNode> bstVector = bst->tree2vector(bst->root);
        
        tree2vector(root, thisVector);
        tree2vector(bst->root, bstVector);
        
        if(thisVector.empty() && bstVector.empty()){
            BST* newBST = new BST();
            return newBST;
        }
        
        if(isEqualVectors(thisVector, bstVector) || thisVector.empty() || bstVector.empty()){
            if(!thisVector.empty()){  
                int size = thisVector.size();
                sort(thisVector.begin(), thisVector.end(), less_than_key());
                BST* newBST = new BST();
                for(int i = 0; i < size; i++){
                    newBST->insert(thisVector[i].key, thisVector[i].data);
                }
                return newBST;
            }
            else if(!bstVector.empty()){
                int size = bstVector.size();
                sort(bstVector.begin(), bstVector.end(), less_than_key());
                BST* newBST = new BST();
                for(int i = 0; i < size; i++){
                    newBST->insert(bstVector[i].key, bstVector[i].data);
                }
                
                
                return newBST;
            }
        }
        
        else{
            vector<TreeNode> intersectVector;
            int bstSize = bstVector.size();
            int size = thisVector.size();
            int flag;
            for(int i = 0; i < bstSize; i++){
                flag = 0;
                for(int j = 0; j < size; j++){
                    if(thisVector[j].key == bstVector[i].key){
                        flag = 1;
                    }
                }
                if(flag == 1){
                    intersectVector.push_back(bstVector[i]);
                }
            }
            
            sort(intersectVector.begin(), intersectVector.end(), less_than_key());
            BST* newBST = new BST();
            
            
            for(int i = 0; i < intersectVector.size(); i++){
                newBST->insert(intersectVector[i].key, intersectVector[i].data);
            }
            return newBST;
        }
        
    
    }
    
    /* DO NOT CHANGE */
    template <class T>
    void BST<T>::print() {
        print(root, "", true, false);
    
    }
    
    /* DO NOT CHANGE */
    template <class T>
    void BST<T>::print(TreeNode* node, string indent, bool last, bool isLeftChild) {
        if (node != NULL) {
            cout << node->key << endl;
            print(node->left, indent, false, true);
            print(node->right, indent, true, false);
        }
    
    }


