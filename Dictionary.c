#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define GRAY    "\033[1;37m"
#define RESET   "\033[0m"
int size = 0;


// ************ Dictionary using AVL trees ************

//  ------- Part 0: Helper functions -------
typedef struct Node{
    char word[100];
    int height;
    struct Node* left;
    struct Node* right;
} Node;
int max (int a, int b);
Node* FindMin(Node* root);
Node* FindMax(Node* root);
Node* createNode (char* word);
void printLine();


// ------- Part 1: Functions of AVL tree -------
int height (Node* n);
Node* rightRotate (Node* y);
Node* leftRotate (Node* y);
int balanceFactor (Node* n);
Node* insert(Node* n, char* word);


// ------- Part 2: Dictionary Functions -------
Node* loadFile(Node* root);
void userInput(Node* root);
Node* inorderSuccessor(Node* root, Node* node);
Node* inorderPredecessor(Node* root, Node* node);
Node* search(Node *root, char word[], int *found);

int main() {
    Node* root = NULL;
    root = loadFile(root);
    if(!root) {
        return 1;
    }
    printLine();
    printf(BLUE "Dictionary loaded successfully!" RESET);
    printLine();
    printf(CYAN "Size:"GRAY" %d." RESET, size);
    printLine();
    printf(CYAN "Height:"GRAY" %d." RESET, root->height);
    printLine();
    bool running = true;
    while(running) {
        userInput(root);
        printLine(); printf("\n");
        printf(CYAN"Would you like to enter another sentence?\n" RESET);
        char answer[10];
        fgets(answer, 10, stdin);
        answer[strcspn(answer, "\n")] = '\0';

        for (int i = 0; answer[i]; i++) {
        answer[i] = tolower(answer[i]);
        }
        if (!strcmp(answer, "no")) {
            running = false;
        }
    }
    
    return 0;
}

// ------- Part 0: Helper functions -------
int max (int a, int b) {
    return (a > b) ? a : b;
}

Node* FindMin(Node* root){
    if(root == NULL)
        return NULL;
    else if (root->left == NULL)
        return root;
    else
        return FindMin(root->left);
}

Node* FindMax(Node* root){
    if(root == NULL)
        return NULL;
    else if (root->right == NULL)
        return root;
    else
        return FindMax(root->right);
}

Node* createNode (char* word) {
    Node* n = (Node*)malloc(sizeof(Node));
    if(!n) {
        printf(RED "Error allocating memory!\n" RESET);
        return NULL;
    }
    strcpy(n->word, word);
    n->height = 1;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void printLine() {
    printf(GRAY "\n-------------------------------------\n" RESET);
}


// ------- Part 1: Implementations of AVL tree -------
int height (Node* n) {
    if(!n) {
        return 0;
    }
    return n->height;
}

Node* rightRotate (Node* y) {
    Node* x = y->left;
    Node* t2 = x->right;

    x->right = y;
    y->left = t2;

    // update y height first, x height is dependant on y
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Node* leftRotate (Node* x) {
    Node* y = x->right;
    Node* t2 = y->left;

    x->right = t2;
    y->left = x;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int balanceFactor (Node* n) {
    if(!n) {
        return 0;
    }
    return height(n->left) - height(n->right);
}

Node* insert (Node* n, char* word) {
    if(!n) { // found empty space
        return createNode(word);
    }
    else if(strcasecmp(n->word, word) > 0) {
        n->left = insert(n->left, word);
    }
    else if(strcasecmp(n->word, word) < 0) {
        n->right = insert(n->right, word);
    } else { // equal key values - not permissible
        return n;
    }
    n->height = 1 + max(height(n->left), height(n->right));
    int bf = balanceFactor(n);

    // left left insertion -> right rotation
    if(bf > 1 && strcasecmp(word, n->left->word) < 0) {
        return rightRotate(n);
    }
    // right right insertion -> left rotation
    if(bf < -1 && strcasecmp(word, n->right->word) > 0) {
        return leftRotate(n);
    }
     // left right insertion -> left rotation + right rotation
    if(bf > 1 && strcasecmp(word, n->left->word) > 0) {
        n->left = leftRotate(n->left);
        return rightRotate(n);
    }
    // right left insertion -> right rotation + left rotation
    if(bf < -1 && strcasecmp(word, n->right->word) < 0) {
        n->right = rightRotate(n->right);
        return leftRotate(n);
    }
    return n;
}


// ------- Part 2: Dictionary Application -------
Node* loadFile(Node* root) {
    FILE* file = fopen("Dictionary.txt", "r");
    if (!file) {
        printf(RED "Error opening file!\n" RESET);
        return root;
    }
    char word[100];
    while (fgets(word, sizeof(word), file)) { // read line by line
        word[strcspn(word, "\r\n")] = '\0';
        if (word[0] != '\0') {
            root = insert(root, word);
            size++;
        }
    }
    fclose(file);
    return root;
}

void userInput(Node* root) {
    char sentence[500];
    printf(BLUE "\nEnter the sentence: " RESET); 
    fgets(sentence, sizeof(sentence), stdin);
    sentence[strcspn(sentence, "\n")] = '\0';

    char* token = strtok(sentence, " ");
    while(token) {
        int found;
        Node* last = search(root, token, &found);

        if (found) {
            printf(GREEN "%s is correct\n" RESET, token);
        } else {
            printf(RED "%s is incorrect " YELLOW "| Suggestions: ", token);
            Node* pred = inorderPredecessor(root, last);
            Node* succ = inorderSuccessor(root, last);
             printf("%s", last->word);

            if (pred) printf(" - %s", pred->word);
            if (succ) printf(" - %s" RESET, succ->word);

            printf("\n");
        }
        token = strtok(NULL, " ");
    }
}

Node* inorderSuccessor(Node* root, Node* node) {
    Node* succ = NULL;
    Node* temp = root;
    if(node->right != NULL)
    return FindMin(node->right);
    while(temp != NULL){
        int cmp = strcasecmp(node->word, temp->word);
        if(cmp < 0){
            succ = temp;
            temp = temp->left;
        }
        else if(cmp > 0)
            temp = temp->right;
        else
            break;
    }
        return succ;
}

Node* inorderPredecessor(Node* root, Node* node){
     Node* pred = NULL;
     Node* temp = root;
    if(node->left != NULL)
    return FindMax(node->left);
    while(temp != NULL){
        int cmp = strcasecmp(node->word, temp->word);
        if(cmp < 0)
            temp = temp->left;
        else if(cmp > 0){
            pred = temp;
            temp = temp->right;
        }
        else
            break;
    }
    return pred;
}

Node* search(Node *root, char word[], int *found){
    *found = 0;
    Node* last = NULL;
    Node* temp = root;
    while(temp != NULL){
        int cmp = strcasecmp(word, temp->word);
        if(cmp == 0){
                *found = 1;
                return temp;
        }
        last = temp;
        if (cmp < 0)
            temp = temp->left;
        else
            temp = temp->right;
}
       return last;
}





