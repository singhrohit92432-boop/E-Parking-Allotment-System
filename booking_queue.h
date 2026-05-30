#ifndef BOOKING_QUEUE_H
#define BOOKING_QUEUE_H

#include "structs.h"

void enqueue(struct Guest g, struct Slot slots[], int n);
struct Guest dequeue();
void displayQueue();
void processBooking(struct Slot slots[], int n);
void initializeQueueSlots(struct Slot slots[], int n);

#endif
