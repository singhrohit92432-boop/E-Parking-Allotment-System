#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parking.h"

#define VIP_SLOTS 5
#define NORMAL_SLOTS 20

struct ParkingSlot {
    int slotId;
    char type[10];
    int isOccupied;
};

struct ParkingSlot vip[VIP_SLOTS];
struct ParkingSlot normal[NORMAL_SLOTS];

// Convert string to lowercase
void toLowerCase(char str[]) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void initializeSlots() {
    for (int i = 0; i < VIP_SLOTS; i++) {
        vip[i].slotId = i + 1;
        strcpy(vip[i].type, "VIP");
        vip[i].isOccupied = 0;
    }
    for (int i = 0; i < NORMAL_SLOTS; i++) {
        normal[i].slotId = i + 1;
        strcpy(normal[i].type, "Normal");
        normal[i].isOccupied = 0;
    }
    printf("\nAll parking slots initialized successfully!\n");
}

void displaySlots() {
    printf("\n------ VIP Parking Slots ------\n");
    for (int i = 0; i < VIP_SLOTS; i++) {
        printf("VIP Slot %d : %s\n", vip[i].slotId,
               vip[i].isOccupied ? "Occupied" : "Available");
    }

    printf("\n------ Normal Parking Slots ------\n");
    for (int i = 0; i < NORMAL_SLOTS; i++) {
        printf("Normal Slot %d : %s\n", normal[i].slotId,
               normal[i].isOccupied ? "Occupied" : "Available");
    }
}

int bookSlot(char type[]) {
    char temp[10];
    strcpy(temp, type);
    toLowerCase(temp);

    if (strcmp(temp, "vip") == 0) {
        for (int i = 0; i < VIP_SLOTS; i++) {
            if (vip[i].isOccupied == 0) {
                vip[i].isOccupied = 1;
                printf("VIP Slot %d booked successfully!\n", vip[i].slotId);
                return vip[i].slotId;
            }
        }
        printf("No VIP slots available!\n");
        return -1;
    } 
    else if (strcmp(temp, "normal") == 0) {
        for (int i = 0; i < NORMAL_SLOTS; i++) {
            if (normal[i].isOccupied == 0) {
                normal[i].isOccupied = 1;
                printf("Normal Slot %d booked successfully!\n", normal[i].slotId);
                return normal[i].slotId;
            }
        }
        printf("No Normal slots available!\n");
        return -1;
    } 
    else {
        printf("Invalid slot type! Please enter VIP or Normal.\n");
        return -1;
    }
}

void releaseSlot(char type[], int slotId) {
    char temp[10];
    strcpy(temp, type);
    toLowerCase(temp);

    if (strcmp(temp, "vip") == 0 && slotId >= 1 && slotId <= VIP_SLOTS) {
        vip[slotId - 1].isOccupied = 0;
        printf("VIP Slot %d released!\n", slotId);
    } 
    else if (strcmp(temp, "normal") == 0 && slotId >= 1 && slotId <= NORMAL_SLOTS) {
        normal[slotId - 1].isOccupied = 0;
        printf("Normal Slot %d released!\n", slotId);
    } 
    else {
        printf("Invalid slot type or ID.\n");
    }
}
