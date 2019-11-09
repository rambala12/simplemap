/* Definition of Map abstract data type */

#ifndef MAP_H
#define MAP_H

#include "street.h"

/* struct map must be defined in map.c */
typedef struct map * Map;

/* used to build a list of streets */
struct streetNode {
    Street * street;  /* data -- pointer to street data */
    struct streetNode * next;  /* pointer to next item in list */
};
typedef struct streetNode Node;

/* allocate and initialize a new Map */
Map createMap(const char *name);

/* returns map name string */
const char *mapName(Map m);

/* return 1 to acknowledge that street has been added */
/* return 0 if error */
int mapAddStreet(Map m, Street * s);

/* return a list of all streets in the map */

struct streetNode * mapAllStreets(Map m);

/* return a list of all north-south streets */

struct streetNode * mapNSStreets(Map m);

/* return a list of all east-west streets */
struct streetNode * mapWEStreets(Map m);

/* returns NULL if street not in map */
Street * mapFindStreet(Map m, const char *name);

/* return a list of all streets that intersect point p */
struct streetNode * mapFindPoint(Map m, const Point *p);

/* return a list of streets that intersect with given street */
struct streetNode * mapFindCrossing(Map m, const Street *s);

/* return a list of streets that gets from Point A to Point B */
struct streetNode * mapFindRoute(Map m, const Point *a, const Point *b);

#endif
