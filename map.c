/*
 * Created by Ram Bala on 11/26/18
 */

#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Helper Functions */
Node * copyList(Node * n);
Node * appendList(Node *h, Street *s);
Node * removeNode(Node *h, Street *S);
Node * nameSort(Node *h);
Node * sortNS(Node *h);
Node * sortWE(Node *h);
int checkPoint(Street * s, int east, int north);
int checkIntersect(Street *given, Street *check);

struct map {
    /* map is a collection of streets */
    char name[16];
    Node * streetList;
};

/* creates new map */
Map createMap(const char *name) {
    Map newMap;
    newMap = (Map) malloc(sizeof(struct map));
    int length;
    length = strlen(name);

    /* Returns NULL if name is too long */
    if (length > 15) return NULL;

    /* Initialize new map */
    strcpy(newMap->name, name);
    newMap->streetList = NULL;

    return newMap;
}

const char * mapName(Map m) {
    return m->name;
}

/* add a new street to Map */
int mapAddStreet(Map m, Street * s) {
    Node * p; /* used to traverse through list */
    Node * newStreet;
    newStreet = (Node *) malloc(sizeof(Node));

    if (m->streetList == NULL) {
        /* if the list is empty, first street is being added */
        m->streetList = newStreet;

        /* initialize Node */
        newStreet->street = s;
        newStreet->next = NULL;
        return 1;
    } else {
        /* initialize Node */
        newStreet->street = s;
        newStreet->next = NULL;
        /* add street to end of list */
        p = m->streetList; /* make p equal the head of the list */
        while (p->next != NULL) { /* traverse through list */
            p = p->next;
            /* check if name clashes */
            if (strcmp(p->street->name, s->name) == 0) return 0;
            /* check for overlap */
            if (p->street->start.north == s->start.north) {
                /* lines go in same direction of east/west */
                if (s->start.east > p->street->start.east && s->start.east < p->street->end.east) return 0;
                if (s->end.east > p->street->start.east && s->end.east < p->street->end.east) return 0;
            }
            if (p->street->start.east == s->start.east) {
                /* lines go in same direction of north/south */
                if (s->start.north > p->street->start.north && s->start.north < p->street->end.north) return 0;
                if (s->end.north > p->street->start.north && s->end.north < p->street->end.north) return 0;
            }

        }
        p->next = newStreet; /* new street added to street list */
        return 1;
    }
}

/* Sorts list of streets alphabetically and stores in new list */
struct streetNode * mapAllStreets(Map m) {
    Node * newList = NULL;
    newList = copyList(m->streetList);

    /* Sort list alphabetically */
    newList = nameSort(newList);

    return newList;
}

/* Stores all streets in NS direction in sorted order from W to E */
struct streetNode * mapNSStreets(Map m) {
    Node *p;
    Node * newList = NULL;
    newList = copyList(m->streetList);

    p = newList; /* traverse through list */
    while (p->next != NULL) {
        if (!streetDirection(p->street)) newList = removeNode(newList, p->street);
        p = p->next;
    }
    if (!streetDirection(p->street)) newList = removeNode(newList, p->street);

    /* SORT LIST from west to east */
    newList = sortWE(newList);

    return newList;
}

/* Stores all streets in WE direction in sorted order from N to S */
struct streetNode * mapWEStreets(Map m) {
    Node *p;
    Node * newList = NULL;
    newList = copyList(m->streetList);

    p = newList; /* traverse through list */
    while (p->next != NULL) {
        if (streetDirection(p->street)) newList = removeNode(newList, p->street);
        p = p->next;
    }
    if (streetDirection(p->street)) newList = removeNode(newList, p->street);

    /* SORT LIST from north to south */
    newList = sortNS(newList);

    return newList;
}

/* Locates the street on the map */
Street * mapFindStreet(Map m, const char *name) {
    Node * p;
    p = m->streetList;

    while (p->next != NULL) {
        if (strcmp(name, p->street->name) == 0) return p->street;
        p = p->next;
    }
    if (strcmp(name, p->street->name) == 0) return p->street;
    return NULL;
}

/* Finds what street a point is located on */
struct streetNode * mapFindPoint(Map m, const Point *p) {
    Node *newList; newList = NULL;
    Node *trav; trav = m->streetList;

    while (trav != NULL) {
        if (checkPoint(trav->street, p->east, p->north) == 1) {
            newList = appendList(newList, trav->street);
        }
        trav = trav->next;
    }
    /* Sort list in alphabetical order */
    newList = nameSort(newList);

    return newList;
}

/* Finds the list of streets that intersect another street */
struct streetNode * mapFindCrossing(Map m, const Street *s) {
    Node *newList; newList = NULL;
    Node *trav; trav = m->streetList;

    while (trav != NULL) {
        if (strcmp(trav->street->name, s->name) == 0) trav = trav->next;
        if (trav == NULL) break; /* if street is last and is skipped, exit loop */
        if (checkIntersect(s, trav->street)) {
            newList = appendList(newList, trav->street);
        }
        trav = trav->next;
    }
    /* Sort list in street direction order */
    if (streetDirection(s)) {
        newList = sortNS(newList);
        newList = sortNS(newList);
    } else {
        newList = sortWE(newList);
        newList = sortWE(newList);
    }

    return newList;
}

/* Finds a list of streets that gets from Point A to Point B */
struct streetNode * mapFindRoute(Map m, const Point *a, const Point *b) {
    Node *newList; newList = NULL;
    Node *trav; trav = m->streetList;

    /* zero turn route: check if points are on same street */
    if (a->north == b->north || a->east == b->east) {
        while (trav != NULL) {
            if (checkPoint(trav->street, a->east, a->north) && checkPoint(trav->street, b->east, b->north)) {
                newList = appendList(newList, trav->street);
            }
            trav = trav->next;
        }
    } else {
        while (trav != NULL) {
            if (checkPoint(trav->street, a->east, a->north)) {
                newList = appendList(newList, trav->street);
            }
            trav = trav->next;
        }
        trav = m->streetList;
        while (trav != NULL) {
            if (checkPoint(trav->street, b->east, b->north)) {
                newList = appendList(newList, trav->street);
            }
            trav = trav->next;
        }
    }

    return newList;
}

/* Copies a list into a new list. Avoids actual map getting deallocated */
Node * copyList(Node * n) {
    Node * p; p = n; /* p is the head of old list */
    Node * newListHead = NULL;
    Node * newList = NULL;

    while (p != NULL) {
        if (newListHead == NULL) {
            /* initialize first node in list */
            newListHead = (Node *) malloc(sizeof(Node));
            newListHead->street = p->street;
            newListHead->next = NULL;
            newList = newListHead;
        } else {
            /* copy rest of the nodes in list to new list */
            newList->next = (Node *) malloc(sizeof(Node));
            newList = newList->next;
            newList->street = p->street;
            newList->next = NULL;
        }
        p = p->next;
    }
    /* return head for new list */
    return newListHead;
}

/* Checks if a point is in a street */
int checkPoint(Street *s, int east, int north) {
    if (east == s->start.east && east == s->end.east) {
        /* check if north point is in between street values */
        if (north <= s->start.north && north >= s->end.north) return 1;
    }
    if (north == s->start.north && north == s->end.north) {
        /* check if north point is in between street values */
        if (east >= s->start.east && east <= s->end.east) return 1;
    }
    return 0;
}

/* Checks if two streets intersect at any point */
int checkIntersect(Street *given, Street *check) {
    int i;
    int stop;
    /* given is the Street given by user */
    /* check is the Street we are checking dor any intersecting points */
    if (streetDirection(given)) {
        stop = given->start.north;
        for (i = given->end.north; i <= stop; i++) {
            if (checkPoint(check, given->start.east, i)) return 1;
        }
    } else {
        stop = given->end.east;
        for (i = given->start.east; i <= stop; i++) {
            if (checkPoint(check, i, given->start.north)) return 1;
        }
    }

    return 0;
}

/* Adds a node to the end of a list */
Node * appendList(Node *h, Street *s) {
    Node *new;
    Node *p;

    if (h == NULL) {
        h = (Node *) malloc(sizeof(Node));
        /* initialize first node in list */
        h->street = s;
        h->next = NULL;
    } else {
        /* create new node */
        new = (Node *) malloc(sizeof(Node));
        new->street = s;
        new->next = NULL;
        p = h;
        while (p->next != NULL) p = p->next; /* go to end of list */
        p->next = new; /* set last pointer to new node to add new node */
    }
    return h;
}

/* Removes a node from a list */
Node * removeNode(Node *h, Street *s) {
    Node *p = h;
    Node *prev = NULL;

    while ((p != NULL) && (p->street != s)) {
        prev = p;
        p = p->next;
    }
    if (p != NULL) {
        if (prev != NULL) prev->next = p->next;
        else h = p->next;
        free(p);
    }

    return h;
}

/* Sorts a list of streets in alphabetical order */
Node * nameSort(Node *h) {
    Node *sorted = NULL;
    Node *n, *p, *prev;

    while (h != NULL) {
        /* remove first element of unsorted list */
        n = h;
        h = n->next;
        n->next = NULL;

        /* add first node from unsorted to sorted list */
        p = sorted;
        prev = NULL;
        while (p != NULL && (p->street->name[0] < n->street->name[0])) {
            prev = p;
            p = p->next;
        }
        if (!prev) {
            n->next = p;
            sorted = n;
        } else {
            n->next = p;
            prev->next = n;
        }
    }

    return sorted;
}

/* Sorts a list of streets in West to East order */
Node * sortWE(Node *h) {
    Node *sorted = NULL;
    Node *n, *p, *prev;

    while (h != NULL) {
        /* remove first element of unsorted list */
        n = h;
        h = n->next;
        n->next = NULL;

        /* add first node from unsorted to sorted list */
        p = sorted;
        prev = NULL;
        while (p != NULL && (p->street->start.east < n->street->start.east)) {
            prev = p;
            p = p->next;
        }
        if (!prev) {
            n->next = p;
            sorted = n;
        } else {
            n->next = p;
            prev->next = n;
        }
    }

    return sorted;
}

/* Sorts a list of streets in North to South order */
Node * sortNS(Node *h) {
    Node *sorted = NULL;
    Node *n, *p, *prev;

    while (h != NULL) {
        /* remove first element of unsorted list */
        n = h;
        h = n->next;
        n->next = NULL;

        /* add first node from unsorted to sorted list */
        p = sorted;
        prev = NULL;
        while (p != NULL && (p->street->start.north > n->street->start.north)) {
            prev = p;
            p = p->next;
        }
        if (!prev) {
            n->next = p;
            sorted = n;
        } else {
            n->next = p;
            prev->next = n;
        }
    }

    return sorted;
}

