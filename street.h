/* Data structures and functions for street */

#ifndef STREET_H
#define STREET_H

/* represents a point on the map */
struct point {
    int east;   /* x-coordinate of map point */
    int north;  /* y-coordinate of map point */
};
typedef struct point Point;

/* represents a street */
struct street {
    char name[16];  /* street name is up to 15 characters */
    struct point start;
    struct point end;
};
typedef struct street Street;


/* create a new street and initialize */
/* returns pointer to newly-allocated struct street, NULL if error */
Street * newStreet(const char *name, const Point * start, const Point *end);

/* returns the distance from start to end */
int streetLength(const Street *s);

/* returns 0 for west-to-east, 1 for north-to-south */
int streetDirection(const Street *s);


#endif
