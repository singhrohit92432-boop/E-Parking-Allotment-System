#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parking.h"
#include "registration.h"
#include "admin.h"
#include "booking_queue.h"
#include "structs.h"

#define MAX_SLOTS 10

int main(int argc, char *argv[]) {
    int choice, slotId;
    char type[10];
    struct Slot slots[MAX_SLOTS];

    initializeSlots();
    initializeQueueSlots(slots, MAX_SLOTS);
    loadGuestsFromFile();

    // ✅ If called from Flask (with command-line arguments)
    if (argc == 5) {
        char *name = argv[1];
        char *vehicleNo = argv[2];
        char *guestType = argv[3];
        char *inviteCode = argv[4];

        // Directly register guest (non-interactive)
        struct Guest g;
        strcpy(g.name, name);
        strcpy(g.vehicleNo, vehicleNo);
        strcpy(g.type, guestType);
        g.guestID = rand() % 1000 + 1; // random ID
        saveGuestToFile(g);

        printf("Guest %s registered successfully with vehicle %s as %s.\n",
               g.name, g.vehicleNo, g.type);
        return 0;
    }

    // ✅ Otherwise: show interactive menu
    while (1) {
        printf("\n===== PARKING MANAGEMENT SYSTEM =====\n");
        printf("1. Display Slots\n2. Book Slot\n3. Release Slot\n4. Register Guest\n");
        printf("5. Admin Login\n6. Add to Queue\n7. Remove from Queue\n8. View Queue\n9. Process Booking\n10. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                displaySlots();
                break;
            case 2:
                printf("Enter Type (VIP/Normal): ");
                scanf("%s", type);
                bookSlot(type);
                break;
            case 3:
                printf("Type: ");
                scanf("%s", type);
                printf("Slot ID: ");
                scanf("%d", &slotId);
                releaseSlot(type, slotId);
                break;
            case 4:
                registerGuest();
                printf("\nPress Enter to return to Main Menu...");
                getchar();
                break;
            case 5:
                admin_module();
                break;
            case 6: {
                struct Guest g;
                printf("Guest ID: ");
                scanf("%d", &g.guestID);
                getchar();
                printf("Name: ");
                fgets(g.name, sizeof(g.name), stdin);
                g.name[strcspn(g.name, "\n")] = 0;
                printf("Vehicle: ");
                fgets(g.vehicleNo, sizeof(g.vehicleNo), stdin);
                g.vehicleNo[strcspn(g.vehicleNo, "\n")] = 0;
                enqueue(g, slots, MAX_SLOTS);
                break;
            }
            case 7: {
                struct Guest g = dequeue();
                if (g.guestID != 0)
                    printf("Removed %s (ID: %d)\n", g.name, g.guestID);
                break;
            }
            case 8:
                displayQueue();
                break;
            case 9:
                processBooking(slots, MAX_SLOTS);
                break;
            case 10:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid.\n");
        }
    }
}

