#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#define SIZE 100

typedef struct
{
int items[SIZE];
int top;
} Stack;

Stack* initialize ();
bool isEmpty (Stack* s);
bool isFull (Stack* s);
void push (Stack* s, int value);
int pop (Stack* s);
int peek (Stack* s);
void display (Stack* s);
char* reverseStr (char* string);
bool sortedS (Stack* s);
void deleteEven (Stack* s);
Stack* mergedS (Stack* s1, Stack* s2);
bool balancedParent(char* string);
void emptyS (Stack* s);
void menu(Stack* s1, Stack* s2);

int main() {
    Stack* s1 = initialize();
    Stack* s2 = initialize();

    menu(s1, s2);

    free(s1);
    free(s2);
    return 0;
}

Stack* initialize () {
    Stack* s = malloc(sizeof(Stack));
    if(!s) {
        printf("Error Allocating!\n");
        return NULL;
    }
    s->top = -1;
    return s;
}

bool isEmpty (Stack* s) {
    return s->top == -1;
}

bool isFull (Stack* s) {
    return s->top == SIZE - 1;
}

void push (Stack* s, int value) {
    if(isFull(s)) {
        printf("Stack Overflow!\n");
        return;
    }
    s->items[++s->top] = value;
}

int pop (Stack* s) {
    if(isEmpty(s)) {
        printf("Stack Underflow!\n");
        return -1;
    }
    return s->items[s->top--];
}

int peek (Stack* s) {
    if(isEmpty(s)) {
        printf("Stack is Empty!\n");
        return -1;
    }
    return s->items[s->top];
}

void display (Stack* s) {
    Stack* copy = initialize();
    printf("top -> ");
    while(!isEmpty(s)) {
        int value = pop(s);
        printf("%d ", value);
        push(copy, value);
    }
    while(!isEmpty(copy)) {
        push(s, pop(copy));
    }
    free(copy);
}

char* reverseStr (char* string) {
    Stack* s = initialize();
    int len = strlen(string);
    for(int i = 0; i < len; i++) {
        push(s, string[i]);
    }
    int j = 0;
    while(!isEmpty(s)) {
        string[j++] = (char)pop(s);
    }
    free(s);
    return string;
}

bool sortedS (Stack* s) { // assume s is sorted (min on top)
    if(isEmpty(s) || s->top == 0) {
        return true;
    }
    Stack* copy = initialize();
    while(!isEmpty(s)) {
        push(copy, pop(s)); // copy is now max sorted
    }

    bool sorted = true;
    int current = pop(copy);
    push(s, current);

    while(!isEmpty(copy)) {
        int it = pop(copy);
        push(s, it);
        if(current < it) {
            sorted = false;
        }
        current = it;
    }

    free(copy);
    return sorted;
}

void deleteEven (Stack* s) {
    Stack* copy = initialize();

    while(!isEmpty(s)) {
        int value = pop(s);
        if(value % 2) { // if odd push
            push(copy, value);
        }
    }
    while(!isEmpty(copy)) {
        push(s, pop(copy));
    }
}

Stack* mergedS (Stack* s1, Stack* s2) { // assume s1, s2 are sorted (min on top)
    Stack* reversed = initialize();

    while(!isEmpty(s1) && !isEmpty(s2)) {
        if(peek(s1) <= peek(s2)) {
            push(reversed, pop(s1));
        } else {
            push(reversed, pop(s2));
        }
    }
    while(!isEmpty(s1)) {
        push(reversed, pop(s1));
    }
    while(!isEmpty(s2)) {
        push(reversed, pop(s2));
    }

    Stack* merged = initialize();
    while(!isEmpty(reversed)) {
        push(merged, pop(reversed));
    }

    free(reversed);
    return merged;
}

bool balancedParent(char* str) {
    Stack* s =  initialize();
    int len = strlen(str);

    for(int i = 0; i < len; i++) {
        if(str[i] == '(') {
            push(s, str[i]);
        }
        else if (str[i] == ')') {
            if(isEmpty(s)) {
                free(s);
                return false;
            }
            pop(s);
        }
    }
    bool result = isEmpty(s);
    free(s);
    return result;
}

void emptyS (Stack* s) {
    while(!isEmpty(s)) {
        pop(s);
    }
}

void menu(Stack* s1, Stack* s2) {
    int choice, value, n;
    char str[10];

    printf("\n===== MENU =====\n");
    printf("1. Reverse String\n");
    printf("2. Check Sorted Stack\n");
    printf("3. Delete Even\n");
    printf("4. Merge Two Stacks\n");
    printf("5. Check Balanced Parentheses\n");
    printf("6. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            printf("Enter string: ");
            scanf("%s", str);
            printf("Reversed: %s\n", reverseStr(str));
            break;

        case 2:
            printf("Enter number of elements: ");
            scanf("%d", &n);
            printf("Enter your values: ");
            for(int i = 0; i < n; i++) {
                scanf("%d", &value);
                push(s1, value);
            }
            if(sortedS(s1)) {
                printf("Stack is sorted (min on top)\n");
            } else {
                printf("Not sorted\n");
            }
            emptyS(s1);
            break;

        case 3:
            printf("Enter number of elements: ");
            scanf("%d", &n);
            printf("Enter your values: ");
            for(int i = 0; i < n; i++) {
                scanf("%d", &value);
                push(s1, value);
            }
            deleteEven(s1);
            printf("After deleting even:\n");
            display(s1);
            emptyS(s1);
            break;

        case 4: {
            printf("Enter number of elements for stack 1: ");
            scanf("%d", &n);
            printf("Enter your values: ");
            for(int i = 0; i < n; i++) {
                scanf("%d", &value);
                push(s1, value);
            }

            int m;
            printf("Enter number of elements for stack 2: ");
            scanf("%d", &m);
            printf("Enter your values: ");
            for(int i = 0; i < m; i++) {
                scanf("%d", &value);
                push(s2, value);
            }

            Stack* merged = mergedS(s1, s2);
            printf("Merged stack:\n");
            display(merged);
            free(merged);
            emptyS(s1);
            emptyS(s2);
            break;
        }

        case 5:
            printf("Enter parentheses string: ");
            scanf("%s", str);
            if(balancedParent(str))
                printf("Balanced\n");
            else
                printf("Not Balanced\n");
            break;

        case 6:
            printf("Exiting the system...\n");
            return;

        default:
            printf("Invalid choice\n");
    }

    sleep(1);
    menu(s1, s2);
}

