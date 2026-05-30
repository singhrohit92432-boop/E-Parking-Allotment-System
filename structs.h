#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

struct Guest {
    int guestID;
    char name[50];
    char vehicleNo[20];
    char type[10];       // VIP or Normal
    char inviteCode[20]; // optional
};

struct Slot {
    int slotID;
    int isBooked;
    int guestID;
};

#endif
