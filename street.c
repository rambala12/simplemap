/*
 * Created by Ram Bala on 11/26/18
 */

#include "street.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Creates new street struct based on given information */
Street * newStreet(const char *name, const Point * start, const Point * end) {
    int length;
    Street * newStreet;
    newStreet = (Street *) malloc(sizeof(Street));

    /* Check for errors */
    length = strlen(name);
    if (length > 15) return NULL;
    if ((start->east < 0) || (start->north < 0) || (end->east < 0) || (end->north < 0)) return NULL;
    if ((start->east > end->east) || (start->north < end->north)) return NULL;

    /* initialize new street */
    strcpy(newStreet->name, name);
    newStreet->start = *start;
    newStreet->end = *end;

    return newStreet;
}

/* returns the distance from start to end */
int streetLength(const Street *s) {
    int dist;

    dist = (s->end.east - s->start.east) + (s->start.north - s->end.north);
    return dist;
}

/* returns 0 for west-to-east, 1 for north-to-south */
int streetDirection(const Street *s) {
    /* If start and end east values are same, street goes North/South */
    if (s->start.east == s->end.east) return 1;
    /* If start and end north values are the same, street foes East/West */
    if (s->start.north == s->end.north) return 0;
}
