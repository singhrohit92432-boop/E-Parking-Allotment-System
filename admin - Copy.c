#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"
#include "structs.h"

void admin_module() {
    if (admin_login()) admin_menu();
    else printf("Access denied.\n");
}

int admin_login() {
    char pass[20];
    printf("Enter Admin Password: ");
    scanf("%s", pass);
    getchar();
    return strcmp(pass, "teamseven") == 0;
}

void admin_menu() {
    int choice;
    do {
        printf("\n1. View All Guests\n2. Search Guest\n3. Exit\nEnter choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: view_all_guests(); break;
            case 2: search_guest(); break;
            case 3: break;
            default: printf("Invalid.\n");
        }
    } while (choice != 3);
}

void view_all_guests() {
    FILE *fp = fopen("guests.txt", "r");
    if (!fp) {
        printf("No records.\n");
        return;
    }

    struct Guest g;
    printf("\n%-5s %-20s %-15s %-10s %-15s\n", "ID", "Name", "Vehicle", "Type", "InviteCode");
    while (fscanf(fp, "%d %s %s %s %s",
                  &g.guestID, g.name, g.vehicleNo, g.type, g.inviteCode) == 5) {
        printf("%-5d %-20s %-15s %-10s %-15s\n",
               g.guestID, g.name, g.vehicleNo, g.type, g.inviteCode);
    }
    fclose(fp);
}

void search_guest() {
    FILE *fp = fopen("guests.txt", "r");
    if (!fp) {
        printf("No records.\n");
        return;
    }

    char term[50];
    printf("Enter name or vehicle no: ");
    scanf("%s", term);

    struct Guest g;
    int found = 0;
    while (fscanf(fp, "%d %s %s %s %s",
                  &g.guestID, g.name, g.vehicleNo, g.type, g.inviteCode) == 5) {
        if (strcasecmp(g.name, term) == 0 || strcasecmp(g.vehicleNo, term) == 0) {
            printf("Found: %s | %s | %s\n", g.name, g.vehicleNo, g.type);
            found = 1;
            break;
        }
    }
    if (!found) printf("Guest not found.\n");
    fclose(fp);
}
