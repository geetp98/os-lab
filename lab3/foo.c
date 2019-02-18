#include "sfs.h"
#include <stdbool.h>

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

int main(int argc, char** argv){
	mountSFS();
	char command[8];
	char filename[252];
	/*while(1){
		printf("SFS:: /# ");
		memset(command, 0, 8);
		scanf("%s", command);
		if(strcmp(command, "ls") == 0){
			ls();
		}
		else if(strcmp(command, "exit") == 0){
			break;
		}
	}*/
	int disp = check(argv[1]);
	disp?printf("File found!!\n"):printf("File not found..\n");
	disp?disp__(argv[1]):fakefunction();
	return 0;
}
