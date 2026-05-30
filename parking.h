#ifndef PARKING_SLOTS_H
#define PARKING_SLOTS_H

void initializeSlots();
void displaySlots();
int bookSlot(char type[]);
void releaseSlot(char type[], int slotId);

#endif