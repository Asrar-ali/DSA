#pragma GCC diagnostic ignored "-Wformat" 
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC optimize ("", off)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define INIT -128 
#define UNDERFLOW (0x80 + 0x01)
#define OVERFLOW (0x80 + 0x02)
#define BADPTR (0x80 + 0x03)
#define PQLIMIT (unsigned long) 1.20E1

// Defines the structure for a priority queue element
struct QElement {
    char key;
    unsigned long prio;
};

// Element of a doubly linked list
struct LElement {
    struct QElement element;
    struct LElement *next, *prev;
};

// Doubly linked list structure with a sentinel
struct DLLS {
    struct LElement *sentinel;
};

// Priority queue structure containing the list and tracking size
struct PQ {
    struct DLLS *L;
    unsigned long max_size, element_num;
};

// Function prototypes
struct LElement* LISTSEARCH(struct DLLS *L, int k);
void LISTINSERT(struct DLLS *L, struct QElement x);
struct LElement* LISTDELETE(struct DLLS *L, struct LElement *x);
struct LElement* LISTDELETE_LAST(struct DLLS *L);
void iterate(struct DLLS *L);
void enqueue(struct PQ * pq, struct QElement e);
char dequeue(struct PQ* pq);
char dequeue_max(struct PQ *pq);
struct PQ* build(unsigned long maxlen);
struct PQ* myQ;

// Search for a key in the list
struct LElement* LISTSEARCH(struct DLLS *L, int k) {
    struct LElement *x;
    if (!L || !L->sentinel) return NULL;  // Return if list is empty

    x = L->sentinel->next;
    while (x != L->sentinel) {  // Search through the list
        if (x->element.key == k) return x;  // Return if found
        x = x->next;
    }
    return NULL;  // Return NULL if not found
}

// Insert an element at the end of the list
void LISTINSERT(struct DLLS *L, struct QElement x) {
    struct LElement *newElem = (struct LElement *) malloc(sizeof(struct LElement)); 
    if (!newElem || !L || !L->sentinel) return;  // Handle errors

    newElem->element = x;
    newElem->next = L->sentinel;  // Insert at the end
    newElem->prev = L->sentinel->prev;
    L->sentinel->prev->next = newElem;
    L->sentinel->prev = newElem;
}

// Remove a specific element from the list
struct LElement* LISTDELETE(struct DLLS *L, struct LElement *x) {
    if (x && x->prev && x->next) {  // If element exists and is properly linked
        x->prev->next = x->next;  // Remove from the list
        x->next->prev = x->prev;
        return x;
    }
    return NULL;
}

// Remove the last element from the list
struct LElement* LISTDELETE_LAST(struct DLLS *L) {
    struct LElement *x = NULL;
    if (!L || !L->sentinel || L->sentinel->next == L->sentinel) return NULL;  // Check if list is empty

    x = L->sentinel->prev;  // Get the last element
    x->prev->next = L->sentinel;  // Remove it
    L->sentinel->prev = x->prev;
    return x;
}

// Print all elements in the list
void iterate(struct DLLS *L) {
    struct LElement *x;
    if (!L || !(L->sentinel)) exit(-1);  // Exit if list is not set up correctly

    x = L->sentinel->next;
    while (x != L->sentinel) {
        printf("\n key %d and priority %lu", x->element.key, x->element.prio);  // Print key and priority
        x = x->next;
    }
}

// Build a priority queue with a given maximum size
struct PQ* build(unsigned long maxlen) {
    struct PQ* pq = NULL;
    if (maxlen > 0 && maxlen <= PQLIMIT) {  // Check size is within limit
        pq = (struct PQ*) malloc(sizeof(struct PQ));  // Allocate memory for the queue
        if (pq) {
            pq->max_size = maxlen;
            pq->element_num = 0;
            pq->L = (struct DLLS*) malloc(sizeof(struct DLLS));  // Allocate memory for the list
            if (pq->L) {
                pq->L->sentinel = (struct LElement*) malloc(sizeof(struct LElement));  // Set up the sentinel
                if (pq->L->sentinel) {
                    pq->L->sentinel->next = pq->L->sentinel;
                    pq->L->sentinel->prev = pq->L->sentinel;
                    pq->L->sentinel->element.key = 0;
                    pq->L->sentinel->element.prio = 0;
                } else {
                    printf("build()>> Could not allocate memory for sentinel - Bad pointer %x\n", BADPTR);
                    return NULL;
                }
            } else {
                printf("build()>> Could not allocate memory for list - Bad pointer %x\n", BADPTR);
                return NULL;
            }
        } else {
            printf("build()>> Could not allocate memory for priority queue - Bad pointer %x\n", BADPTR);
            return NULL;
        }
    }
    return pq;
}

// Add an element to the queue
void enqueue(struct PQ *pq, struct QElement e) {
    if (pq && pq->element_num < pq->max_size) {  // Ensure the queue isn't full
        LISTINSERT(pq->L, e);  // Insert element into the list
        pq->element_num++;
    } else {
        printf("enqueue()>> Overflow prevented at %p.\n", (void*)pq);  // Print error if full
    }
}

// Remove the last element from the queue
char dequeue(struct PQ* pq) {
    char val = INIT;
    if (!pq || pq->element_num == 0) {  // Ensure queue isn't empty
        printf("dequeue()>> Underflow prevented at %p.\n", (void*)pq);
        return UNDERFLOW;
    }
    struct LElement *ptr = LISTDELETE_LAST(pq->L);  // Remove last element
    if (ptr) {
        val = ptr->element.key;
        free(ptr);
        pq->element_num--;
    }
    return val;
}

// Remove the element with the highest priority
char dequeue_max(struct PQ *pq) {
    if (!pq || pq->element_num == 0) {  // Ensure queue isn't empty
        printf("dequeue_max()>> Underflow prevented at %p.\n", (void*)pq);
        return UNDERFLOW;
    }

    struct LElement *maxElem = NULL, *current = pq->L->sentinel->next;

    // Find the element with the highest priority
    while (current != pq->L->sentinel) {
        if (!maxElem || current->element.prio > maxElem->element.prio) {
            maxElem = current;
        }
        current = current->next;
    }

    if (maxElem) {
        LISTDELETE(pq->L, maxElem);  // Remove the element with max priority
        pq->element_num--;
        return maxElem->element.key;
    }
    return 0;
}

// Main function to test the priority queue
int main(int argc, const char * argv[]) {
    int test = PQLIMIT; 
    myQ = build(test);  // Create the queue
    if (myQ == NULL) {
        printf("\nBad pointer. \n");
        exit(-1);
    }

    struct QElement e;
    srand(time(NULL));  // Seed random number generator

    // Enqueue elements
    for (int i = 0; i < test; i++) {
        e.key = rand() % 127;
        e.prio = rand() % 4;  // Assign random key and priority
        enqueue(myQ, e);  // Add to queue
        printf("After Enqueue(%d, %lu) counter takes %lu\n", e.key, e.prio, myQ->element_num);
    }

    // Attempt to overflow the queue
    e.key = rand() % 127;
    e.prio = rand() % 4;
    enqueue(myQ, e);  // This should trigger overflow message

    iterate(myQ->L);  // Display the queue's contents
    printf("\n\n");

    // Search for a random key in the queue
    int key_to_search = rand() % 127;
    struct LElement* result = LISTSEARCH(myQ->L, key_to_search);  // Search for key

    printf("\nSearching for key %d in the list...\n", key_to_search);

    if (result != NULL) {
        printf("Key %d found in the list at address: %p\n", key_to_search, (void*)result);
    } else {
        printf("Key %d not found in the list.\n", key_to_search);
    }

    // Dequeue all elements
    for (int i = test; i > 0; i--) {
        dequeue(myQ);  // Remove elements from the queue
        printf("After Dequeue() -> the counter takes %lu\n", myQ->element_num);
    }

    // Attempt to dequeue when the queue is empty (should trigger underflow message)
    dequeue(myQ);

    printf("\n\nYou can store a maximum of %lu elements in your PQ (PQ->max_size), whereas max_size of a PQ is capped at PQLIMIT, which is currently set to %lu in your program.\n\n\n", myQ->max_size, PQLIMIT);

    return 0;
}
