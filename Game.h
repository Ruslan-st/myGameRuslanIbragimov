#ifndef GAME_H
#define GAME_H

typedef struct Room Room;
typedef struct Player Player;
typedef struct Creature Creature;

struct Player {
    int health, strength, inventoryCapacity, countOfInventory;
    char **inventory;
};

struct Creature {
    char *name;
    int health, strength;
};

struct Room {
    char *description;
    Room *north, *east, *south, *west;
    Creature *creature;
    char *item;
};

Player *initPlayer();
Room *initRoom(const char *description, Creature *creature, const char *item);
Creature *initCreature(const char *name, int health, int strength);
void freeThePlayer(Player *player);
void freeTheRoom(Room *room);
void freeTheCreature(Creature *creature);
void takeALook(Room *room);
void move(Room **currentRoom, const char *directionOfPlayer);
void pickUpTheItem(Player *player, Room *room);
void toAttack(Player *player, Room *room);
void saveTheGame(Player *player, Room *currentRoom, const char *filepath);
void loadTheGame(Player *player, Room **currentRoom, const char *filepath);
void showMenu();

#endif
