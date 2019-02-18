#include "sfs.h"
#include <stdbool.h>
#include <string.h>

void disp__(char* filename){
	char itype;
	int blocks[3];
	_directory_entry _directory_entries[4];
	char buf[1024];
	int which_block;
	int check;
	
	int total_files=0, total_dirs=0;
	
	int i,j;
	int e_inode;

	// read inode entry for current directory
	// in SFS, an inode can point to three blocks at the most		
	itype = _inode_table[CD_INODE_ENTRY].TT[0];
	blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX,2);
	blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY,2);
	blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ,2);

	// its a directory; so the following should never happen		
	if (itype=='F') {
		printf("Fatal Error! Aborting.\n");
		exit(1);
	}
	
	// lets traverse the directory entries in all three blocks
	for (i=0; i<3; i++) {
		if (blocks[i]==0) continue;	// 0 means pointing at nothing
		
		readSFS(blocks[i],(char *)_directory_entries);	// lets read a directory entry; notice the cast
		
		// so, we got four possible directory entries now		
		for (j=0; j<4; j++) {
			if (_directory_entries[j].F=='0') continue;	// means unused entry

			e_inode = stoi(_directory_entries[j].MMM,3);	// this is the inode that has more info about this entry
			
			if (_inode_table[e_inode].TT[0]=='F')  { // entry is for a file
				if(strcmp(_directory_entries[j].fname, filename) == 0){
					//printf("File found!!\n");
					int check = 0;
					which_block = stoi(_inode_table[e_inode].XX, 2);
					if(which_block != 0){
						check = readSFS(which_block, buf);
						if(check){
							printf("%s", buf);
						}
					}
					which_block = stoi(_inode_table[e_inode].YY, 2);
					if(which_block != 0){
						check = readSFS(which_block, buf);
						if(check){
							printf("%s", buf);
						}
					}
					check = 0;
					which_block = stoi(_inode_table[e_inode].ZZ, 2);
					if(which_block != 0){
						check = readSFS(which_block, buf);
						if(check){
							printf("%s", buf);
						}
					}
					printf("\n");
					return;
				}
				//printf("%.252s\n",_directory_entries[j].fname);
				total_files++;
			}
		}
	}
}

bool check(char* filename){
	char itype;
	int blocks[3];
	_directory_entry _directory_entries[4];
	char buf[1024];
	int which_block;
	int check;
	
	int total_files=0, total_dirs=0;
	
	int i,j;
	int e_inode;

	// read inode entry for current directory
	// in SFS, an inode can point to three blocks at the most		
	itype = _inode_table[CD_INODE_ENTRY].TT[0];
	blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX,2);
	blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY,2);
	blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ,2);

	// its a directory; so the following should never happen		
	if (itype=='F') {
		printf("Fatal Error! Aborting.\n");
		exit(1);
	}
	
	// lets traverse the directory entries in all three blocks
	for (i=0; i<3; i++) {
		if (blocks[i]==0) continue;	// 0 means pointing at nothing
		
		readSFS(blocks[i],(char *)_directory_entries);	// lets read a directory entry; notice the cast
		
		// so, we got four possible directory entries now		
		for (j=0; j<4; j++) {
			if (_directory_entries[j].F=='0') continue;	// means unused entry

			e_inode = stoi(_directory_entries[j].MMM,3);	// this is the inode that has more info about this entry
			
			if (_inode_table[e_inode].TT[0]=='F')  { // entry is for a file
				if(strcmp(_directory_entries[j].fname, filename) == 0){
					return true;
				}
				//printf("%.252s\n",_directory_entries[j].fname);
				total_files++;
			}
		}
	}
	return false;
}

void fakefunction(){
	return;
}

int firstwhitespace(char* s){
	int i = 0;
	while(i < strlen(s)){
		if(s[i] == ' '){
			return i;
		}
		i++;
	}
	return i;
}

int endofstring(char* s){
	int i = 0;
	while(i < strlen(s)){
		if(s[i] == '\0'){
			return i;
		}
		i++;
	}
	return i;
}

int main(int argc, char** argv){
	mountSFS();
	char command[8+1+252];
	char c1[8];
	char filename[252];
	while(1){
		printf("SFS:: /# ");
		memset(command, 0, 8+1+252);
		memset(c1, 0, 8);
		scanf("%[^\n]%*c", command);
		strncpy(c1, command, firstwhitespace(command));
		//printf("command : %s\n", command);
		//printf("firstwhitespace : %d\n", firstwhitespace(command));
		//printf("c1 : %s\n", c1);
		if(!strcmp(c1, "ls")){
			ls();
		}
		if(!strcmp(c1, "display")){
			memset(filename, 0, 252);
			//printf("firstwhitespace : %d\n", firstwhitespace(command));
			//printf("sizeof(command) : %d\n", (int)endofstring(command));
			//printf("diff : %d\n", (int)endofstring(command) - firstwhitespace(command));
			strncpy(filename, &command[firstwhitespace(command)+1], endofstring(command) - firstwhitespace(command));
			//printf("filename : %s\n", filename);
			int disp;
			disp = check(filename);
			disp?disp__(filename):printf("File not found..\n");
		}
		else if(!strcmp(c1, "exit")){
			break;
		}
		memset(command, 0, 8+1+252);
		memset(c1, 0, 8);
		printf("\n\n");
	}
	return 0;
}