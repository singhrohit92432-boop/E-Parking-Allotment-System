#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

struct guestNode {
    struct Guest g;
    struct guestNode *next;
};

static struct guestNode *head = NULL;

void Filesave(struct Guest g);

void loadGuestsFromFile() {
    FILE *fp = fopen("guests.txt", "r");
    if (!fp) {
        printf("No guest records found.\n");
        return;
    }

    struct Guest g;
    while (fscanf(fp, "%d %s %s %s %s",
                  &g.guestID, g.name, g.vehicleNo, g.type, g.inviteCode) == 5) {
        struct guestNode *node = malloc(sizeof(struct guestNode));
        node->g = g;
        node->next = head;
        head = node;
    }
    fclose(fp);
    printf("Guests loaded successfully.\n");
}

int generateGuestID() {
    FILE *fp = fopen("last_id.txt", "r+");
    int id = 1000;
    if (fp) {
        fscanf(fp, "%d", &id);
        rewind(fp);
    } else {
        fp = fopen("last_id.txt", "w");
    }
    fprintf(fp, "%d", id + 1);
    fclose(fp);
    return id + 1;
}
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void registerGuest() {
    struct Guest g;
    g.guestID = generateGuestID();
    clearInputBuffer();
    printf("\n--- Register New Guest ---\n");
    printf("Enter Name: ");
    fgets(g.name, sizeof(g.name), stdin);
    g.name[strcspn(g.name, "\n")] = 0;
    
    printf("Enter Vehicle No: ");
    fgets(g.vehicleNo, sizeof(g.vehicleNo), stdin);
    g.vehicleNo[strcspn(g.vehicleNo, "\n")] = 0;
    
    printf("Enter Type (VIP/Normal): ");
    fgets(g.type, sizeof(g.type), stdin);
    g.type[strcspn(g.type, "\n")] = 0;
    
    printf("Enter Invite Code: ");
    fgets(g.inviteCode, sizeof(g.inviteCode), stdin);
    g.inviteCode[strcspn(g.inviteCode, "\n")] = 0;
    
    Filesave(g);
    printf("Guest registered successfully!\n");
   
}


void Filesave(struct Guest g) {
    FILE *fp = fopen("guests.txt", "a");
    fprintf(fp, "%d %s %s %s %s\n",
            g.guestID, g.name, g.vehicleNo, g.type, g.inviteCode);
    fclose(fp);
}

void displayGuestList() {
    struct guestNode *temp = head;
    if (!temp) {
        printf("No guests loaded.\n");
        return;
    }
    printf("\n--- Guests ---\n");
    while (temp) {
        printf("%d | %s | %s | %s | %s\n",
               temp->g.guestID, temp->g.name, temp->g.vehicleNo,
               temp->g.type, temp->g.inviteCode);
        temp = temp->next;
    }
}

int authenticateGuest(char *inviteCode) {
    struct guestNode *temp = head;
    while (temp) {
        if (strcmp(temp->g.inviteCode, inviteCode) == 0) {
            printf("Welcome %s!\n", temp->g.name);
            return 1;
        }
        temp = temp->next;
    }
    printf("Invalid code.\n");
    return 0;
}

void guestLogin() {
    char code[20];
    printf("Enter Invite Code: ");
    fgets(code, sizeof(code), stdin);
    code[strcspn(code, "\n")] = 0;
    authenticateGuest(code);
}
