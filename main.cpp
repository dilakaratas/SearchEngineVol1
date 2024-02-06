#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

template <class Key, class Value>
class AVLNode {
public:
    Key key;
    Value value;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const Key& k, const Value& v, int h = 1)
            : key(k), value(v), height(h), left(nullptr), right(nullptr) {}
};

template <class Key, class Value>
class AVLSearchTree {
private:
    AVLNode<Key, Value>* root;
    int getHeight(AVLNode<Key, Value>* node);
    int balanceFactor(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* leftRotate(AVLNode<Key, Value>* y);
    AVLNode<Key, Value>* rightRotate(AVLNode<Key, Value>* x);
    AVLNode<Key, Value>* rightLeftRotate(AVLNode<Key, Value>* z);
    AVLNode<Key, Value>* leftRightRotate(AVLNode<Key, Value>* z);
    AVLNode<Key, Value>* insertRecursive(AVLNode<Key, Value>* root, Key key, Value value);
    AVLNode<Key, Value>* removeRecursive(AVLNode<Key, Value>* root, Key key);
    AVLNode<Key, Value>* searchRecursive(AVLNode<Key, Value>* root, Key key);

public:
    AVLSearchTree();
    ~AVLSearchTree();
    AVLNode<Key, Value>* getRoot();
    void insert(const Key& key, const Value& value);
    void remove(const Key& key);
    Value search(const Key& key);
    void destroyTree(AVLNode<Key, Value>* node);
};


//constructor
template <class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree() : root(nullptr) {}

//destructor
template <class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree() {
    destroyTree(root);
}

//get root
template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::getRoot() {
    return root;
}

//delete tree
template <class Key, class Value>
void AVLSearchTree<Key, Value>::destroyTree(AVLNode<Key, Value>* node) {
    if (node != nullptr) {
        destroyTree(node->left); // delete left
        destroyTree(node->right); // delete right
        delete node;
    }
}

// get height
template <class Key, class Value>
int AVLSearchTree<Key, Value>::getHeight(AVLNode<Key, Value>* node) {
    if (node != nullptr){
        return node->height;
    } else{
        return 0;
    }
}

// left subtree - right subtree gives balance factor of AVLTree
template <class Key, class Value>
int AVLSearchTree<Key, Value>::balanceFactor(AVLNode<Key, Value>* node) {
    return getHeight(node->left) - getHeight(node->right);
}


template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::leftRotate(AVLNode<Key, Value>* y) {
    AVLNode<Key, Value>* x = y->right;
    AVLNode<Key, Value>* T2 = x->left;

    x->left = y; // left child node y
    y->right = T2; // right child of y to left subtree T2

    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    return x; // new root
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::rightRotate(AVLNode<Key, Value>* x) {
    AVLNode<Key, Value>* y = x->left;
    AVLNode<Key, Value>* T2 = y->right;

    y->right = x; // moves x to right sbtree of y
    x->left = T2; // x to rightsubtree T2

    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    return y;
}

// balance factor z is -2
template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::rightLeftRotate(AVLNode<Key, Value>* z) {
    z->right = rightRotate(z->right);
    return leftRotate(z);
}

//balance factor z is 2
template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::leftRightRotate(AVLNode<Key, Value>* z) {
    z->left = leftRotate(z->left);
    return rightRotate(z);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key& key, const Value& value) {
    root = insertRecursive(root, key, value);
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::insertRecursive(AVLNode<Key, Value>* root, Key key, Value value) {

    if (root == nullptr) {
        return new AVLNode<Key, Value>(key, value);
    }

    if (root->key > key) { // if smaller than left subtree
        root->left = insertRecursive(root->left, key, value);
    }
    else if (root->key < key) { // if larger than right subtree
        root->right = insertRecursive(root->right, key, value);
    }
    else {
        root->value->Update(value);
    }

    root->height = 1 + (getHeight(root->left) > getHeight(root->right) ? getHeight(root->left) : getHeight(root->right));
    int bFactor = balanceFactor(root);

    //-1, 0, 1 => balanced AVL Tree
    // if not do rotations for balancing
    if (bFactor > 1 && key < root->left->key)
        return rightRotate(root);

    if (bFactor < -1 && key > root->right->key)
        return leftRotate(root);

    if (bFactor > 1 && key > root->left->key)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (bFactor < -1 && key < root->right->key)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key& key) {
    root = removeRecursive(root, key);
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::removeRecursive(AVLNode<Key, Value>* root, Key key) {
    if (root == nullptr) {
        return root;
    }
    if (key < root->key) {
        root->left = removeRecursive(root->left, key); // remove from left

    } else if (key > root->key) {
        root->right = removeRecursive(root->right, key); // remove from right

    } else {
        if (root->left == nullptr || root->right == nullptr) {
            AVLNode<Key, Value>* temp = (root->left != nullptr) ? root->left : root->right;

            // no child
            if (temp == nullptr) {
                temp = root;
                root = nullptr;

                // 1 child
            } else {
                *root = *temp;
            }
            delete temp;

            // 2 child
        } else {
            AVLNode<Key, Value>* temp = root->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            root->key = temp->key;
            root->value = temp->value;
            root->right = removeRecursive(root->right, temp->key);
        }
    }

    if (root == nullptr) {
        return root;
    }

    // updating heights
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int bFactor = balanceFactor(root);


    // after removing checking if it is balanced if not rebalance it by rotation same as above
    if (bFactor > 1 && balanceFactor(root->left) >= 0) {
        return rightRotate(root);
    }

    if (bFactor > 1 && balanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (bFactor < -1 && balanceFactor(root->right) <= 0) {
        return leftRotate(root);
    }

    if (bFactor < -1 && balanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

template <class Key, class Value>
Value AVLSearchTree<Key, Value>::search(const Key& key) {
    AVLNode<Key, Value>* node = searchRecursive(root, key);
    if (node != nullptr) {
        return node->value;
    } else {
        return Value();
    }
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::searchRecursive(AVLNode<Key, Value>* root, Key key) {
    if (root == nullptr) return nullptr;
    if (root->key > key) return searchRecursive(root->left, key);
    if (root->key < key) return searchRecursive(root->right, key);
    return root;
}

struct DocumentItem {
    string documentName;
    int count;
    DocumentItem* next;
    DocumentItem(const string& name, int c) : documentName(name), count(c), next(nullptr) {}
};

class WordItem {

public:
    WordItem() {}

    string word;
    DocumentItem* documents;
    WordItem(const string& w) : word(w) {
        documents = nullptr;
    }
    void Update(WordItem* item) {
        if (documents == nullptr) {
            documents = new DocumentItem(item->documents->documentName, 1);
            return;
        }
        DocumentItem* iterator = documents; // initalizing iterator with first word document
        while (true) {
            if (iterator->documentName == item->documents->documentName) {
                iterator->count++;
                return;
            }
            if (iterator->next == nullptr) break; // check if there is no next document
            iterator = iterator->next; // next iterator
        }
        iterator->next = new DocumentItem(item->documents->documentName, 1);
    }
};

bool operator<(const DocumentItem& left, const DocumentItem& right) {
    return left.documentName < right.documentName;
}

bool operator<(const WordItem& left, const WordItem& right) {
    return left.word < right.word;
}

bool operator>(const DocumentItem& left, const DocumentItem& right) {
    return left.documentName > right.documentName;
}

bool operator>(const WordItem& left, const WordItem& right) {
    return left.word > right.word;
}

int main() {

    AVLSearchTree<string, WordItem*> myTree;

    cout << "Enter number of input files: ";
    int inputFileCount;
    cin >> inputFileCount;

    vector<string> inputFileNames; // store files
    for (int i = 1; i <= inputFileCount; i++) {
        string fileName;
        cout << "Enter " << i << ". file name: ";
        cin >> fileName;
        inputFileNames.push_back(fileName);
    }

    // Read each document
    for (int i = 0; i < inputFileCount; i++) {
        string fileName = inputFileNames[i];

        ifstream file;
        file.open(fileName, ios::in);
        if (!file.is_open()) continue;

        WordItem* currentWordItem = nullptr;

        while (!file.eof()) {
            string line;
            getline(file, line);

            istringstream iss(line);
            string word;

            while (iss >> word) {
                string processedWord = "";
                for (char ch : word) {
                    if (isalpha(ch)) {
                        processedWord += tolower(ch);
                    }
                }
                if (!processedWord.empty()) { // after processing if it still word then start loop
                    if (currentWordItem == nullptr || currentWordItem->word != processedWord) {
                        currentWordItem = new WordItem(processedWord); // memory allocation
                        currentWordItem->documents = new DocumentItem(fileName, 1); // document item is shows word'S place
                        myTree.insert(processedWord, currentWordItem); // inserting to AVL tree
                    }
                }
            }
        }

        file.close();
    }
    cin.ignore(); // preperation for next input

    // enter word in single line
    bool exit = false; // start loop
    while (!exit) { // continue until exit
        cout << "Enter queried words in one line: ";
        string query;
        getline(cin, query);

        vector<WordItem*> results; // storing results in vector
        istringstream iss(query);
        string queryWord;

        while (iss >> queryWord) {
            if (queryWord == "ENDOFINPUT") {
                exit = true;
                break;
            }
            if (queryWord == "REMOVE") {
                while (iss >> queryWord && queryWord != "ENDOFINPUT") {
                    string processedQueryWord;
                    for (char ch : queryWord) {
                        if (isalpha(ch)) {
                            processedQueryWord += tolower(ch); // case sensitive
                        }
                    }
                    myTree.remove(processedQueryWord);
                    cout << processedQueryWord << " has been REMOVED" << endl;
                }
                break;
            }

            string processedQueryWord;
            for (char ch : queryWord) {
                if (isalpha(ch)) {
                    processedQueryWord += tolower(ch); // case sensitive
                }
            }

            WordItem* queryResult = myTree.search(processedQueryWord);
            if (queryResult != nullptr) {
                results.push_back(queryResult);
            } else {
                cout << "No document contains the given query" << endl; // if not found
            }
        }

        if (!exit) {
            for (int i = 0; i < inputFileCount; i++) {
                string line = "";

                for (int j = 0; j < results.size(); j++) {
                    WordItem* queryResult = results[j];
                    DocumentItem* iterator = queryResult->documents;

                    while (iterator != nullptr) {
                        if (iterator->documentName == inputFileNames[i]) {
                            line += ", " + queryResult->word + " found " + to_string(iterator->count) + " times";
                        }
                        iterator = iterator->next;
                    }
                }
                if (!line.empty()) {
                    cout << "in Document " << inputFileNames[i] << line << endl;
                }
            }
            cout << endl;
        }
    }
    return 0;
}
