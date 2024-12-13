#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Game.h"

Player *initPlayer() {
    Player *player = (Player *)malloc(sizeof(Player));
    if (player == NULL) {
        printf("Error. Cannot allocate memory for the player...\n");
        exit(1);
    }

    player->health = 100;
    player->strength = 10;
    player->inventoryCapacity = 15;
    player->countOfInventory = 0;
    player->inventory = (char **)malloc(sizeof(char *) * player->inventoryCapacity);
    return player;
}

Room *initRoom(const char *description, Creature *creature, const char *item) {
    Room *room = (Room *)malloc(sizeof(Room));
    if (room == NULL) {
        printf("Error. Cannot allocate memory for the room...\n");
        exit(1);
    }
    room->description = strdup(description);
    room->north = room->south = room->west = room->east = NULL;
    room->creature = creature;
    room->item = strdup(item);
    return room;
}

Creature *initCreature(const char *name, int health, int strength) {
    Creature *creature = (Creature *)malloc(sizeof(Creature));
    if (creature == NULL) {
        printf("Error. Cannot allocate memory for the creature...\n");
        exit(1);
    }
    creature->name = strdup(name);
    creature->health = health;
    creature->strength = strength;
    return creature;
}

void freeThePlayer(Player *player) {
    for (int i = 0; i < player->countOfInventory; i++) {
        free(player->inventory[i]);
    }
    free(player->inventory);
    free(player);
}

void freeTheRoom(Room *room) {
    free(room->description);
    free(room->item);
    free(room);
}

void freeTheCreature(Creature *creature) {
    free(creature->name);
    free(creature);
}

void takeALook(Room *room) {
    printf("Description of the room: %s\n", room->description);
    if (room->creature != NULL) {
        printf("There is a creature in the room: %s.\n", room->creature->name);
    } else {
        printf("There is no creature in the room.\n");
    }
    if (room->item != NULL) {
        printf("There is an item in the room: %s.\n", room->item);
    } else {
        printf("There is no item in the room.\n");
    }
}

void move(Room **currentRoom, const char *directionOfPlayer) {
    Room *newRoom = NULL;
    if (strcmp(directionOfPlayer, "East") == 0) {
        newRoom = (*currentRoom)->east;
    } else if (strcmp(directionOfPlayer, "North") == 0) {
        newRoom = (*currentRoom)->north;
    } else if (strcmp(directionOfPlayer, "West") == 0) {
        newRoom = (*currentRoom)->west;
    } else if (strcmp(directionOfPlayer, "South") == 0) {
        newRoom = (*currentRoom)->south;
    }
    if (newRoom == NULL) {
        printf("Stop, Cannot move in this direction..\n");
    } else {
        *currentRoom = newRoom;
        printf("You have moved to a new room.\n");
    }
}

void pickUpTheItem(Player *player, Room *room) {
    if (room->item != NULL && player->countOfInventory < player->inventoryCapacity) {
        player->inventory[player->countOfInventory++] = strdup(room->item);
        room->item = NULL;
        printf("You have picked up the item.\n");
    } else if (room->item == NULL) {
        printf("There is no item.\n");
    } else {
        printf("Your inventory is full.\n");
    }
}

void toAttack(Player *player, Room *room) {
    if (room->creature != NULL) {
        printf("You have attacked the creature! %s!\n", room->creature->name);
        room->creature->health -= player->strength;
        if (room->creature->health <= 0) {
            printf("You have killed the creature %s.\n", room->creature->name);
            freeTheCreature(room->creature);
            room->creature = NULL;
        } else {
            printf("The health of the creature is: %d\n", room->creature->health);
        }
    } else {
        printf("There is no creature to attack...\n");
    }
}

void saveTheGame(Player *player, Room *currentRoom, const char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Cannot open the file...\n");
        return;
    }
    fprintf(file, "%d %d %d %d\n", player->health, player->strength, player->inventoryCapacity, player->countOfInventory);
    for (int i = 0; i < player->countOfInventory; i++) {
        fprintf(file, "%s\n", player->inventory[i]);
    }
    fclose(file);
    printf("The game is saved to the file %s.\n", filepath);
}

void loadTheGame(Player *player, Room **currentRoom, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Cannot load the game...\n");
        return;
    }

    fscanf(file, "%d %d %d %d\n", &player->health, &player->strength, &player->inventoryCapacity, &player->countOfInventory);
    for (int i = 0; i < player->countOfInventory; i++) {
        player->inventory[i] = (char *)malloc(50 * sizeof(char));
        fscanf(file, "%s", player->inventory[i]);
    }
    fclose(file);
    printf("The game is loaded!\n");
}

void showMenu() {
    printf("Choose an action:\n");
    printf("1. Move: move <direction> (East, North, West, South)\n");
    printf("2. Look around: Look\n");
    printf("3. Show inventory: inventory\n");
    printf("4. Pick up an item: pickup\n");
    printf("5. Attack a creature: attack\n");
    printf("6. Save the game: save <filepath>\n");
    printf("7. Load the game: load <filepath>\n");
    printf("8. Exit the game: exit\n");
}

int main() {
    Player *player = initPlayer();
    Creature *slenderMan = initCreature("SlenderMan", 50, 10);
    Room *roomA = initRoom("BloodyRoom", slenderMan, "chainsaw");
    Room *roomB = initRoom("RoomOfPeace", NULL, "food");

    roomA->east = roomB;
    roomB->north = roomA;

    Room *currentRoom = roomA;
    char commandLine[100];

    printf("Welcome to the game. Press any key to continue...\n");
    getchar();

    showMenu();

    while (1) {
        printf("Enter the command: ");
        fgets(commandLine, sizeof(commandLine), stdin);
        commandLine[strcspn(commandLine, "\n")] = '\0';

        if (strncmp(commandLine, "move ", 5) == 0) {
            move(&currentRoom, commandLine + 5);
        } else if (strcmp(commandLine, "Look") == 0) {
            takeALook(currentRoom);
        } else if (strcmp(commandLine, "inventory") == 0) {
            printf("Inventory: \n");
            for (int i = 0; i < player->countOfInventory; i++) {
                printf("- %s\n", player->inventory[i]);
            }
        } else if (strncmp(commandLine, "pickup ", 7) == 0) {
            pickUpTheItem(player, currentRoom);
        } else if (strcmp(commandLine, "attack") == 0) {
            toAttack(player, currentRoom);
        } else if (strncmp(commandLine, "save ", 5) == 0) {
            saveTheGame(player, currentRoom, commandLine + 5);
        } else if (strncmp(commandLine, "load ", 5) == 0) {
            loadTheGame(player, &currentRoom, commandLine + 5);
        } else if (strcmp(commandLine, "exit") == 0) {
            break;
        } else {
            printf("Invalid command...\n");
        }
    }

    freeThePlayer(player);
    freeTheRoom(roomA);
    freeTheRoom(roomB);
    return 0;
}
