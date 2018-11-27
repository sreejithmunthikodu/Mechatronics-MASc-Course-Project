#include <stdlib.h>
#include <avr/io.h>

/* Type definitions */
typedef struct {
	int objectID; 	/* unique ID of object starting from 0 */
	int magnetic; 	/* 1: object is magnetic (i.e. steel or aluminum), 0: object is not magnetic (i.e. black or white) */
	uint16_t reflectance; 	/* reflectance sensor value */
	int objectType; /* type of object, 0: aluminum, 1: steel, 2: white, 3: black) */
} object;

typedef struct link{
	object		o;
	struct link *next;
} link;

void	initLink	(link **newLink);
void 	setup		(link **h, link **t);
void 	clearQueue	(link **h, link **t);
void 	enqueue		(link **h, link **t, link **nL);
void 	dequeue		(link **h, link **t, link **deQueuedLink);
object firstValue	(link **h);
char 	isEmpty		(link **h);
int 	size		(link **h, link **t);

struct link *head;			/* The ptr to the head of the queue */
struct link *tail;			/* The ptr to the tail of the queue */
struct link *newLink;		/* A ptr to a link aggregate data type (struct) */
struct link *rtnLink;		/* same as the above */
