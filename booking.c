#include <stdio.h>
#include <stdlib.h>
#include "booking_queue.h"

#define MAX 10

static struct Guest queue[MAX];
static int front = -1, rear = -1;

void enqueue(struct Guest g, struct Slot slots[], int n) {
    if ((rear + 1) % MAX == front) {
        printf("Queue full! Cannot add guest %s.\n", g.name);
        return;
    }

    for (int i = 0; i < n; i++) {
        if (slots[i].isBooked && slots[i].guestID == g.guestID) {
            printf("Guest ID %d already booked!\n", g.guestID);
            return;
        }
    }

    if (front == -1) front = 0;
    rear = (rear + 1) % MAX;
    queue[rear] = g;

    printf("Guest %s (ID: %d) added to queue.\n", g.name, g.guestID);
}

struct Guest dequeue() {
    struct Guest empty = {0, "", "", "", ""};
    if (front == -1) {
        printf("Queue is empty!\n");
        return empty;
    }
    struct Guest g = queue[front];
    if (front == rear)
        front = rear = -1;
    else
        front = (front + 1) % MAX;
    return g;
}

void displayQueue() {
    if (front == -1) {
        printf("Queue empty.\n");
        return;
    }
    printf("\n--- Current Queue ---\n");
    int i = front;
    while (1) {
        printf("ID: %d | Name: %s | Vehicle: %s\n",
               queue[i].guestID, queue[i].name, queue[i].vehicleNo);
        if (i == rear) break;
        i = (i + 1) % MAX;
    }
}

void processBooking(struct Slot slots[], int n) {
    if (front == -1) {
        printf("No pending bookings.\n");
        return;
    }

    struct Guest g = dequeue();
    int assigned = 0;

    for (int i = 0; i < n; i++) {
        if (!slots[i].isBooked) {
            slots[i].isBooked = 1;
            slots[i].guestID = g.guestID;
            printf("🎟️ Booking confirmed for %s → Slot %d\n",
                   g.name, slots[i].slotID);
            assigned = 1;
            break;
        }
    }

    if (!assigned) {
        printf("No available slots! Adding back to queue.\n");
        enqueue(g, slots, n);
    }
}

void initializeQueueSlots(struct Slot slots[], int n) {
    for (int i = 0; i < n; i++) {
        slots[i].slotID = i + 1;
        slots[i].isBooked = 0;
        slots[i].guestID = 0;
    }
}
