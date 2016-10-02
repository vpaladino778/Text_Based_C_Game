#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct room room;
struct room{
	int state;
	room *north;
	room *south;
	room *east;
	room *west;
	int roomID;
};
struct creature{
	int type;
	int id;
	room *location;
};

int respect = 40;
void moveCreature(struct room *r, struct creature *c, int id, int numCrea, int cRoom, struct room *rooms);
/* Gets input from user between 'a' and 'b', exclusive */
int getIntInput(int a,int b){
	int input;
	scanf("%d",&input);
	while(input < a || input > b){
		printf("Please input a number between %d and %d\n",a,b);
		scanf("%d",&input);
	}
	return input;
}
/* Finds last specified  room for PC to start in */
int startingRoom(struct creature c[], int s){
	int i, lastRoom;
	for(i = 0; i < s; i++){
		if(c[i].type == 0){
			lastRoom = i;
		}
	}
	return lastRoom;
}	
void grumbleGrowl(int ty, int cID){
	if(ty == 1){
		respect--;
		printf("Animal %d growls at you. Your respect is now %d. \n",cID,respect);
	}else if(ty == 2){
		respect--;
		printf("NPC %d grumbles at you. Your respect is now %d. \n",cID,respect);

	}
	
}
void lickSmile(int ty, int cID){
	if(ty == 1){
		respect++;
		printf("Animal %d licks you. Your respect is now %d. \n",cID,respect);
	}else if(ty == 2){
		respect++;
		printf("NPC %d smiles at you. Your respect is now %d. \n",cID,respect);
	}
	
}
/*Print out creatues in specified room */
void printCreatures(int roomNum, struct creature *c, int s){
	printf("Room contains: \n");
	int f;
	printf("PC\n");
	for(f = 0; f < s; f++){
		if(c[f].type >=0 && c[f].location->roomID == roomNum){
			if(c[f].type == 1){
				printf("animal %d \n", c[f].id);
			}else if(c[f].type == 2){
				printf("NPC %d \n", c[f].id);
			}
		}	
	}
}

char* getState(int state){
	if(state == 0){ return "clean";}
	else if(state == 1){ return "half-dirty"; }
	else if(state == 2){ return "dirty";  }
}
/* Displays information about surrounding rooms */
void printRoom( struct room *r){
	printf("You are in room %d and it is currently %s .\n", r->roomID, getState(r->state));	
	if(r->north){
		printf("To the north is room number %d. \n",r->north->roomID); 	
	}
	if(r->south){
		printf("To the south is room number %d. \n", r->south->roomID);
	}
	if(r->east){
		printf("To the east is room number %d. \n", r->east->roomID);
	}
	if(r->west){
		printf("To the west is room number %d. \n", r->west->roomID);
	}
}
/* Determines number of creatures in a room,  return -1 if full */
int checkFull(int roomNum, struct creature *c, int cSize){
	int i, count;	
	count = 0;
	for(i = 0; i < cSize; i++){
		if(c[i].location->roomID == roomNum){
			count++;
		}	
	}
	if(count < 10 && count >= 0){
		return count;
	}
	return -1;
}

/* Returns pointer of room with inputted Id */
struct room* getRoom(int id, struct room *r, int rSize){
	int i;
	for(i = 0; i < rSize; i++){
		if(r[i].roomID == id){
			return &r[i];
		}
	} 
} 
/* Creates the room with the specified surrounding rooms */
void createRoom(struct room *r,int current,int nRooms, int no, int so, int ea, int we){
	r[nRooms].roomID = current;
	if(no != -1){
		r[current].north = getRoom(no,r,nRooms);
	}else {
		r[current].north = NULL;
	} 
	if(so != -1){
		r[current].south = getRoom(so,r,nRooms);
	}else {
		r[current].south = NULL;
	}
	if(ea != -1){
		r[current].east = getRoom(ea,r,nRooms);
	} else{
		r[current].east = NULL;
	}
	if(we != -1){
		r[current].west = getRoom(we,r,nRooms);
	}else {
		r[current].west = NULL;
	}
}
/* Check if user can move, then moves the PC */
int movePC(char loc, struct room *r){
	switch(loc){
		case 'n':
			if(r->north != NULL){
				printf("You moved north to room %d \n ", r->north->roomID);
				return r->north->roomID;
			}
			printf("There is nothing to the north \n");	
			return -1;	
			break;
		case 's':
			if(r->south != NULL){
				printf("You moved south to room %d \n", r->south->roomID);
				return r->south->roomID;
			}	
			printf("There is nothing to the south \n");	
			return -1;	
			break;
		case 'e':
			if(r->east != NULL){
				printf("You moved east to room %d \n", r->east->roomID);
				return r->east->roomID;
			}	
			printf("There is nothing to the east \n");	
			return -1;	
			break;

		case 'w':
			if(r->west != NULL){
				printf("You moved west to room %d \n", r->west->roomID);
				return r->west->roomID;
			}	
			printf("There is nothing to the west \n");	
			return -1;	
			break;
		default:
			break;	
	}
}
int cleanRoom(struct room *r, struct creature *c, int numCrea,int cRoom, struct room *rooms){
	if(r->state == 0){
		printf("This room is already clean.\n");
		return -1;
	}
	else if(r->state == 2 || r->state == 1){
		r->state -= 1;
		if(r->roomID == rooms[cRoom].roomID){
		printf("The room is cleaned. It is now %s.\n",getState(r->state));
		}
	}		
		int i;
		for(i = 0; i < numCrea; i++){
			if(c[i].type > 0 && c[i].location->roomID == r->roomID){
				if(c[i].type == 2){
					if(r->roomID == rooms[cRoom].roomID){
					grumbleGrowl(c[i].type,i);
					}
					if(c[i].location->state == 0){
						moveCreature(r,c,i,numCrea,cRoom, rooms);
					}
				} else if(c[i].type == 1){
					if(c[i].location->state == 1 || c[i].location->state == 0){ 
						if(r->roomID == rooms[cRoom].roomID){
							lickSmile(c[i].type,i);
						}
					}
			
				}
			}
			

		
	}
	return 1;
}
int dirtyRoom(struct room *r, struct creature *c, int numCrea, int cRoom, struct room *rooms){
	if(r->state == 2){
		printf("This room is already dirty.\n");
		return -1;
	}
	else if(r->state == 0 || r->state == 1){
		r->state += 1;
		if(r->roomID == rooms[cRoom].roomID){
			printf("The room is dirtied. It is now %s.\n",getState(r->state));
		}
	}
	if(r->roomID == rooms[cRoom].roomID){
	int i;
	for(i = 0; i < numCrea; i++){
		if(c[i].type > 0 && c[i].location->roomID == r->roomID){
			if(c[i].type == 2){
				if(c[i].location->state == 1 || c[i].location->state == 2){ 
					if(r->roomID == rooms[cRoom].roomID){
						lickSmile(c[i].type,i);
					}
				} 
			} else if(c[i].type == 1 ){
				if(r->roomID == rooms[cRoom].roomID){
					grumbleGrowl(c[i].type,i);
				}
				if(c[i].location->state == 2){
					moveCreature(r,c,i,numCrea,cRoom, rooms);
				}
			}	
	
		}
	}	
	
	}
	return 1;

}
void moveFinal(struct room *r, struct creature *c, int id, int numCrea, char dir, int cRoom, struct room *rooms){
	switch(dir){
		case 'n':
		if(r[0].north != NULL){
			if(c[id].type == 1 && r[0].north->state == 2){
				c[id].location = r[0].north;
				cleanRoom(c[id].location,c,numCrea, cRoom, rooms);	
				printf("Animal %d moves to the north and cleans the room.\n",c[id].id);
			}else if(c[id].type == 2 && r[0].north->state == 0){
				c[id].location = r[0].north;
				dirtyRoom(c[id].location,c,numCrea,cRoom, rooms);	
				printf("NPC %d moves to the north and dirties the room.\n",c[id].id);
			} else{
				c[id].location = r[0].north;
				if(c[id].type == 2){
					printf("NPC %d", c[id].id);
				}
				if(c[id].type == 1){
					printf("Animal %d",c[id].id);
				}
				printf(" moves to the north.\n");
			}
		}
		
			break;
		case 's':
		if(r[0].south != NULL)
			if(c[id].type == 1 && r[0].south->state == 2){
				c[id].location = r[0].south;
				cleanRoom(c[id].location,c,numCrea,cRoom, rooms);	
				printf("Animal %d moves to the south and cleans the room.\n",c[id].id);
			}else if(c[id].type == 2 && r[0].south->state == 0){
				c[id].location = r[0].south;
				dirtyRoom(c[id].location,c,numCrea,cRoom, rooms);	
				printf("NPC %d moves to the south and dirties the room.\n",c[id].id);
			} else{
				c[id].location = r[0].south;
				if(c[id].type == 2){
					printf("NPC %d", c[id].id);
				}
				if(c[id].type == 1){
					printf("Animal %d",c[id].id);
				}
				printf(" moves to the south.\n");
			}
		}
			break;
		case 'e':
		if(r[0].east != NULL){
			if(c[id].type == 1 && r[0].east->state == 2){
				c[id].location = r[0].east;
				cleanRoom(c[id].location,c,numCrea,cRoom, rooms);	
				printf("Animal %d moves to the east and cleans the room.\n",c[id].id);
			}else if(c[id].type == 2 && r[0].east->state == 0){
				c[id].location = r[0].east;
				dirtyRoom(c[id].location,c,numCrea,cRoom, rooms);	
				printf("NPC %d moves to the east and dirties the room.\n",c[id].id);
			} else{
				c[id].location = r[0].east;
				if(c[id].type == 2){
					printf("NPC %d", c[id].id);
				}
				if(c[id].type == 1){
					printf("Animal %d",c[id].id);
				}
				printf(" moves to the east.\n");
			}
		}
			break;
		case 'w':
		if(r[0].west != NULL){

			if(c[id].type == 1 && r[0].west->state == 2){
				c[id].location = r[0].west;
				cleanRoom(c[id].location,c,numCrea, cRoom, rooms);	
				printf("Animal %d moves to the west and cleans the room.\n",c[id].id);
			}else if(c[id].type == 2 && r[0].west->state == 0){
				c[id].location = r[0].west;
				dirtyRoom(c[id].location,c,numCrea,cRoom, rooms);	
				printf("NPC %d moves to the west and dirties the room.\n",c[id].id);
			} else{
				c[id].location = r[0].west;
				if(c[id].type == 2){
					printf("NPC %d", c[id].id);
				}
				if(c[id].type == 1){
					printf("Animal %d",c[id].id);
				}
				printf(" moves to the west.\n");
			}
			break;
		}
		default:
		
			break;
	}
}
void moveCreature(struct room *r, struct creature *c, int id, int numCrea, int cRoom, struct room *rooms){
/* Checks if a random room is full, if it is, check the surrounding rooms. If not, the creature moves there and performs any needed action */
int rng;
rng = rand() % 4;
	if(rng == 0){
		       if(r->north != NULL && checkFull(r[0].north->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'n',cRoom, rooms);
		} else if(r->east != NULL && checkFull(r[0].east->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'e',cRoom, rooms);
		} else if(r->south != NULL && checkFull(r[0].south->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'s',cRoom, rooms);
		} else if(r->west != NULL && checkFull(r[0].west->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'w',cRoom, rooms);
		} else{
			/* Kill Creature, no adjacent rooms to move to */
			printf("Creature %d drills a hole in the ceiling and disapears from the simulation.\n" ,c[0].id);
			c[0].id = -1;
		}
	} else if(rng == 1){
		       if(r->east != NULL && checkFull(r[0].east->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'e',cRoom, rooms);
		} else if(r->west != NULL && checkFull(r[0].south->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'s',cRoom, rooms);
		} else if(r->west != NULL && checkFull(r[0].west->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'w',cRoom, rooms);
		} else if(r->north != NULL && checkFull(r[0].north->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'n',cRoom, rooms);
		}else{
			/* Kill Creature, no adjacent rooms to move to */
			c[0].id = -1;
		}
	} else if(rng == 2){
			   if(r->south != NULL && checkFull(r[0].south->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'s',cRoom, rooms);
		} else if(r->west != NULL && checkFull(r[0].west->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'w',cRoom, rooms);
		} else if(r->north != NULL && checkFull(r[0].north->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'n',cRoom, rooms);
		} else if(r->east != NULL && checkFull(r[0].east->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'e',cRoom, rooms);
		} else{
			/* Kill Creature, no adjacent rooms to move to */
			c[0].id = -1;
		}
	}else if(rng == 3){
		       if(r->west != NULL && checkFull(r[0].west->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'w',cRoom, rooms);
		} else if(r->north != NULL && checkFull(r[0].north->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'n',cRoom, rooms);
		} else if(r->east != NULL && checkFull(r[0].east->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'e',cRoom, rooms);
		} else if(r->south != NULL && checkFull(r[0].south->roomID, c, numCrea) != -1){
			moveFinal(r,c,id,numCrea,'s',cRoom, rooms);
		} else{
			/* Kill Creature, no adjacent rooms to move to */
			c[0].id = -1;
		}
	}
}


main(){
	int numRooms, roomLoc,n, i,k, numCreatures, j;
	int currentRoom;
	char command[15];
	roomLoc = 0;
	numCreatures = 0;
	numRooms = 0;
/* Gets number of rooms and input for each room from user and puts it in array of room structs */
	printf("Enter number of rooms\n");
	numRooms = getIntInput(1,100);

	struct room *rooms = (struct room*)malloc(sizeof(struct room)*numRooms);
	for(k = 0; k < numRooms; k++){
		rooms[k].roomID = k;
	}
	printf("Input 5 numbers for each row, state of the current room followed by the Ids of the surrounding rooms \n");
	printf("e.g state north south east west (ex 0 3 4 2 1)\n");
	for(i = 0; i < numRooms; ++i){
		int n,s,e,w;
		scanf("%d %d %d %d %d",&rooms[i].state,&n,&s,&e,&w);
		if( n >= numRooms){
			n = -1;
		}
		if( s >= numRooms){
			s = -1;
		}
		if( e >= numRooms){
			e = -1;
		}
		if( w >= numRooms){
			w = -1;
		}
		createRoom(rooms,i,numRooms,n,s,e,w);
	}		

/* Gets number of creatures and creates creature array and take input from user and fills array */
	printf("Enter number of creatures: \n");
	numCreatures = getIntInput(1,100);
	struct creature *creatures = (struct creature *)malloc(sizeof(struct creature)*numCreatures);
	printf("For each creature, input creature type (0 for PC, 1 for animals, or 2 for NPC) followed by a space then the room number\n");
	for(j = 0; j < numCreatures; j++){
		scanf("%d %d",&creatures[j].type,&roomLoc);
		creatures[j].location = getRoom(roomLoc,rooms,numRooms);
		creatures[j].id = j;
	}
/* Sets the starting room for the PC */
	currentRoom = startingRoom(creatures,numRooms);
/* User Commands */
	printf("Enter a command (look, n, s, w, e, creatureID:action, help,  or exit)\n");
	scanf("%s", &command);
	while (strcmp(command,"exit") != 0){
		if(strcmp(command,"n") == 0 || strcmp(command,"N") == 0 || strcmp(command,"north") == 0){
			int nextRoom;
			nextRoom = movePC('n',&rooms[currentRoom]); 	
			if(nextRoom != -1){
				currentRoom = nextRoom;
			}
		} else if(strcmp(command,"s") == 0 || strcmp(command,"S") == 0 || strcmp(command,"south") == 0){
			int nextRoom;
			nextRoom = movePC('s',&rooms[currentRoom]); 	
			if(nextRoom != -1){
				currentRoom = nextRoom;
			}
		} else if(strcmp(command,"e") == 0 || strcmp(command,"e") == 0 || strcmp(command,"east") == 0){

			int nextRoom;
			nextRoom = movePC('e',&rooms[currentRoom]); 	
			if(nextRoom != -1){
				currentRoom = nextRoom;
			}
		} else if(strcmp(command,"w") == 0 || strcmp(command,"w") == 0 || strcmp(command,"west") == 0){
			int nextRoom;
			nextRoom = movePC('w',&rooms[currentRoom]); 	
			if(nextRoom != -1){
				currentRoom = nextRoom;
			}
		} else if(strcmp(command,"look") == 0){
			printRoom(&rooms[currentRoom]);
			printCreatures(currentRoom,creatures,numCreatures);	
		} else if(strstr(command,":") != NULL){
			char* cmd = strtok(command,":");
			int intID = atoi(cmd);
			cmd = strtok(NULL,":");
			if(intID  >= numCreatures && creatures[intID].location->roomID != currentRoom){
				printf("Creature %d is not in this room. \n", intID);
			}else{
				
				if(strcmp(cmd,"north") == 0 || strcmp(cmd,"n") == 0){
					if(rooms[currentRoom].north != NULL && checkFull(rooms[currentRoom].north->roomID, creatures, numCreatures) != -1){
						moveFinal(&rooms[currentRoom],creatures,intID,numCreatures,'n',currentRoom, rooms);
					}else {
						printf("That room is full or isn't there. ");
						grumbleGrowl(creatures[intID].type,intID);
					}
				}else if(strcmp(cmd,"south") == 0 || strcmp(cmd,"s") == 0){
					if(rooms[currentRoom].south != NULL && checkFull(rooms[currentRoom].south->roomID, creatures, numCreatures) != -1){
						moveFinal(&rooms[currentRoom],creatures,intID,numCreatures,'s',currentRoom, rooms);
					}else {
						printf("That room is full or isn't there. ");
						grumbleGrowl(creatures[intID].type,intID);
					}
				}else if(strcmp(cmd,"east") == 0 || strcmp(cmd,"e") == 0){
					if(rooms[currentRoom].east != NULL && checkFull(rooms[currentRoom].east->roomID, creatures, numCreatures) != -1){
						moveFinal(&rooms[currentRoom],creatures,intID,numCreatures,'e',currentRoom, rooms);
					}else {
						printf("That room is full or isn't there. ");
						grumbleGrowl(creatures[intID].type,intID);
					}
				}else if(strcmp(cmd,"west") == 0 || strcmp(cmd,"w") == 0){
					if(rooms[currentRoom].west != NULL && checkFull(rooms[currentRoom].west->roomID, creatures, numCreatures) != -1){
						moveFinal(&rooms[currentRoom],creatures,currentRoom,numCreatures,'w',currentRoom, rooms);
					}else {
						printf("That room is full or isn't there. ");
						grumbleGrowl(creatures[intID].type,intID);
					}
				}else if(strcmp(cmd,"clean") == 0){
					if(cleanRoom(&rooms[currentRoom],creatures,numCreatures,currentRoom, rooms) != -1){
						if(creatures[intID].type == 2){
							grumbleGrowl(creatures[intID].type, intID);
							grumbleGrowl(creatures[intID].type, intID);
							printf("NPC %d cleaned the room. \n", intID);
						} else if(creatures[intID].type ==1){
							lickSmile(creatures[intID].type,intID);
							lickSmile(creatures[intID].type,intID);
							printf("Animal %d cleaned the room. \n", intID);
						}
					}	
				}else if (strcmp(cmd,"dirty") == 0){
					if(dirtyRoom(&rooms[currentRoom],creatures,numCreatures,currentRoom, rooms) != -1){
						if(creatures[intID].type == 2){
							lickSmile(creatures[intID].type,intID);
							lickSmile(creatures[intID].type,intID);
							printf("NPC %d dirtied the room. \n", intID);
						} else if(creatures[intID].type ==1){
							grumbleGrowl(creatures[intID].type, intID);
							grumbleGrowl(creatures[intID].type, intID);
							printf("Animal %d dirtied the room. \n", intID);
						}
					}				
				}
			}
		} else if(strcmp(command,"dirty") == 0){
			dirtyRoom(&rooms[currentRoom],creatures,numCreatures, currentRoom,rooms);
		} else if(strcmp(command,"clean") == 0){
			cleanRoom(&rooms[currentRoom],creatures,numCreatures, currentRoom,rooms);			
		} else if (strcmp(command,"help") == 0){
			printf("Commands: \n");
			printf("look - Prints out information regarding your current room. \n");
			printf("clean - Cleans the current room, animals like clean rooms, NPCs do not");
			printf("dirty - Dirties the room, NPCs like dirty rooms, animals do not \n");
			printf("n/s/e/w/ or north/south/east/west - Use to move north, south, east, west respectively. \n");
			printf("creatureID:action - NPCs and animals can perform the actions clean,dirty,north,south,east, and west. \n Type the creatures id followed by a colon and then the action. (e.g 4:north, 1:clean etc. \n");
		} else {
			printf("Command not found, type 'help' for detailed instructions.\n");
		}
		
/* Check if game is Over */
		if(respect <= 0){
			printf("Game Over! You lost. \n");
			break;
		}
		if(respect >= 80){
			printf("Congratz! You won. \n");
			break;
		}
		printf("Enter a command (look, n, s, w, e, creatureID:action, help,  or exit)\n");
		scanf("%s",&command);
	}	

	free(rooms);
	free(creatures);

}
