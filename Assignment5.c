#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct Node {
    float data;
    struct Node* next;

}Node;

typedef struct {
    Node *top;
} stack;

Node* newNode(float data);
stack* initialize ();
int isEmpty(stack *s);
void push (stack *s, float value);
float peek (stack *s);
int priority(char c);
char* infixTopostfix(char *infix);
float evaluatePostfix(char* postfix);

int main(void) {
    char s[100];
    printf("Enter Infix: ");
    fgets(s, 100, stdin);
    s[strcspn(s, "\n")] = 0;

    char* postfix = infixTopostfix(s);
    printf("Postfix: %s\n", postfix);
    char postfixCopy[200];
    strcpy(postfixCopy, postfix);

    printf("Value: %.4f\n", evaluatePostfix(postfixCopy));

    free(postfix);
    return 0;
}

Node* newNode(float data) {
    Node* n = malloc(sizeof(Node));
    n->data = data;
    n->next = NULL;
    return n;
}

stack* initialize () {
    stack* s =  malloc(sizeof(stack));
    s->top = NULL;
    return s;
}

int isEmpty(stack *s) {
    if (s->top == NULL) {
         return 1;
    }
        return 0;
}

float pop (stack *s) {
    if(isEmpty(s)) {
        printf("Stack underflow!\n");
        return -1;
    }
    Node* p = s->top;
    float data = p->data;
    s->top = s->top->next;
    free(p);
    return data;
}

void push (stack *s, float value) {
   Node* n = newNode(value);
    n->next = s->top;
    s->top = n;
}

float peek (stack *s) {
    if(isEmpty(s)) {
        return -1;
    }
    return s->top->data;
}

int priority(char c) {
    switch (c) {
        case '^':
            return 3;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
        case '(': // to prevent stack popping
            return 0;
        default:
            return -1;
    }
}

char* infixTopostfix(char *infix) {
    stack *s = initialize();
    char *post = malloc(strlen(infix) * 2 + 1);
    post[0] = '\0';
    
    char *token = strtok(infix, " ");
    int j = 0;

    while(token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) { // for handling cases
            for (int k = 0; token[k] != '\0'; k++) {
                post[j++] = token[k];
            }
            post[j++] = ' ';
        }
        else if (token[0] == '(') {
            push(s, '(');
        }
        else if (token[0] == ')') {
            while (!isEmpty(s) && (char)peek(s) != '(') {
                post[j++] = (char)pop(s);
                post[j++] = ' ';
            }
            pop(s); 
        }
        else {
            while (!isEmpty(s) && (char)peek(s) != '(' && (priority((char)peek(s)) >priority(token[0])
            ||(priority((char)peek(s)) == priority(token[0]) && token[0] != '^'))) {
                post[j++] = (char)pop(s);
                post[j++] = ' ';
            }
            push(s, token[0]);
        }
        token = strtok(NULL, " ");
    }

    while (!isEmpty(s)) {
        post[j++] = (char)pop(s);
        post[j++] = ' ';
    }
    post[j] = '\0';

    free(s);
    return post;
}

float evaluatePostfix(char* postfix) {
    stack *s = initialize();
    char *token = strtok(postfix, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(s, atof(token)); 
        }
        else {
            float op2 = pop(s);
            float op1 = pop(s);

            switch (token[0]) {
                case '+': push(s, op1 + op2); break;
                case '-': push(s, op1 - op2); break;
                case '*': push(s, op1 * op2); break;
                case '/': push(s, op1 / op2); break;
                case '^': push(s, pow(op1, op2)); break;
                case '%': push(s, (int)op1 % (int)op2); break;
            }
        }
        token = strtok(NULL, " ");
    }

    float result = pop(s);
    free(s);
    return result;
}