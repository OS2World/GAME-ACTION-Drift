#ifndef __LLIST_H
#define __LLIST_H

/* Forward declaration - OBJEKT is fully defined in drift.h */
typedef struct OBJEKT_s OBJEKT;

typedef struct LLIST_NODE {
    OBJEKT           *objekt;
    struct LLIST_NODE *next;
} LLIST;

extern int llist_items;   /* global node count across all lists */

#endif /* __LLIST_H */
