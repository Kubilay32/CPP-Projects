#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;


template <class T>
class Trie {    
private:
    // Define TrieNode for the first phase (Trie)
    /* DO NOT CHANGE */
    struct TrieNode {
        static const int ALPHABET_SIZE = 128;
        TrieNode* children[ALPHABET_SIZE];
        char keyChar;
        bool isEndOfKey;
        T* data;

        TrieNode(char c) : keyChar(c), isEndOfKey(false), data(NULL) {

                for (int i = 0; i< ALPHABET_SIZE; i++) {
                children[i] = NULL;
                
            }
            
            }
    };

    TrieNode* root;

public: // Do not change.

    Trie(); 
    ~Trie();
    
    Trie& insert(const string& username); 
    void remove(std::string username);
    T* search(std::string username); 
    void findStartingWith(std::string prefix, std::vector<T*> &results); 
    void wildcardSearch(const std::string &wildcardKey, std::vector<T*> &results); 
    void print(); 

private: // you may add your own utility member functions here.
    void wildcardSearchHelper(TrieNode* node, const string &wildcardKey, int index, vector<T*> &results) {
        if (node == NULL) {
            return;
        }
        
        if (wildcardKey[index] == '?' || wildcardKey[index] == node->keyChar) {
            if (index == wildcardKey.length() - 1 && node->isEndOfKey) {
                results.push_back(node->data);
            }

            for (int i = 0; i < 128; i++) {
                if (node->children[i] != NULL) {
                    wildcardSearchHelper(node->children[i], wildcardKey, index + 1, results);
                }
            }
        }    
        
        else if (wildcardKey[index] == '*') {
            wildcardSearchHelper(node, wildcardKey, index + 1, results);
            for (int i = 0; i < 128; i++) {
                if (node->children[i] != NULL) {
                    wildcardSearchHelper(node->children[i], wildcardKey, index, results);
                }
            }
        } 
                            
                                           
    }                                       

    void findStartingWithHelper(TrieNode* node, std::vector<T*>& results) {
        if (node == NULL) {
            return;
        }
        
        if(node->isEndOfKey && node->data != NULL){
            results.push_back(node->data);
        }
        for(int i = 0; i < 128; i++){
            if(node->children[i]){
                findStartingWithHelper(node->children[i], results);
            }
        }
    }
    
    void deleteTrieNode(TrieNode* node) {
        if (node != NULL) {
            for (int i = 0; i < TrieNode::ALPHABET_SIZE; i++) {
                deleteTrieNode(node->children[i]);
            }
            delete node->data;
            delete node;
        }
    }
    void print(const std::string& primaryKey); // Do not change this line.
    void printTrie(TrieNode* node, const std::string& currentKey); // Do not change this line.
    
};
/* DO NOT CHANGE */
template <class T>
Trie<T>::Trie() : root(new TrieNode('\0')) {}

template <class T>
Trie<T>::~Trie() {
    /* IMPLEMENT THIS */
    deleteTrieNode(root);
}

template <class T>
Trie<T>& Trie<T>::insert(const string& key) {
    /* IMPLEMENT THIS */
    TrieNode* temp = root;
    
    for(int i = 0; i < key.length(); i++){
        int index = key[i];
        if(temp->children[index] == NULL){
            TrieNode* newNode = new TrieNode(key[i]);
            temp->children[index] = newNode;
        }
        temp = temp->children[index];
    }
    if(temp->keyChar != '\0'){
        temp->isEndOfKey = true;
        temp->data = new T(key);
    }
    
    return *this;
}

template <class T>
T* Trie<T>::search(std::string username) {
    /* IMPLEMENT THIS */
    TrieNode* temp = root;
    int index;
    for(int i = 0; i < username.length(); i++){
        index = username[i];
        if(temp->children[index] == NULL){
            return NULL;
        }
        temp = temp->children[index];
    }
    
    if(temp->keyChar != '\0' && temp->isEndOfKey){
        return temp->data;
    }
    return NULL;
}

template <class T>
void Trie<T>::remove(std::string username) {
    /* IMPLEMENT THIS */
    TrieNode* temp = root;
    int index;
    for(int i = 0; i < username.length(); i++){
        index = username[i];
        if(temp->children[index] == NULL){
            return;
        }
        temp = temp->children[index];
    }
    
    if(temp->isEndOfKey == true){
        temp->isEndOfKey = false;
    }
    
}

template <class T>
void Trie<T>::findStartingWith(string prefix,vector<T*> &results) {
/* IMPLEMENT THIS */
    TrieNode* temp = root;
    
    for(int i = 0; i < prefix.length(); i++){
        int index = prefix[i];
        if(temp->children[index] == NULL){
            return;
        }
        temp = temp->children[index];
    }
    
    
    findStartingWithHelper(temp, results);
    
}

template <class T>
void Trie<T>::wildcardSearch(const std::string &wildcardKey, std::vector<T*> &results) {
/* IMPLEMENT THIS */
    wildcardSearchHelper(root, wildcardKey, 0, results);
}

/* DO NOT CHANGE */
template<class T>
void Trie<T>::print() {
    print("");
}
/* DO NOT CHANGE */
template <class T>
void Trie<T>::print(const std::string& primaryKey) {
    if (primaryKey.empty()) {
        // Print the entire tree.
        printTrie(root, "");
    } else {
        // Print specific keys.
        TrieNode* primaryNode = root;
        for (int i = 0; i < primaryKey.length(); i++) {
            
            int index = primaryKey[i];
            if (!primaryNode->children[index]) {
                std::cout << "{}" << std::endl; // Primary key not found, nothing to print.
                return ;
            }
            primaryNode = primaryNode->children[index];
        }

        }
    }
/* DO NOT CHANGE */
template <class T>
void Trie<T>::printTrie(TrieNode* node, const std::string& currentKey) {
    if (!node) {
        return;
    }
    if (node->isEndOfKey) {
        std::cout << currentKey <<std::endl;
        std::cout << "Watched Movies:" << std::endl;
        if(node->data) {
            node->data->printMovies();
        }
    }
    for (int i = 0; i < 128; ++i) {
        if (node->children[i]) {
            printTrie(node->children[i], currentKey + static_cast<char>(i));
        }
    }
}

