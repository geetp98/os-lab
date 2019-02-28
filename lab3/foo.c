#include "sfs.h"
#include <stdbool.h>
#include <string.h>

void disp__(char *filename)
{
	char itype;
	int blocks[3];
	_directory_entry _directory_entries[4];
	char buf[1024];
	int which_block;
	int check = 0;

	int i, j;
	int e_inode;

	// read inode entry for current directory
	// in SFS, an inode can point to three blocks at the most
	itype = _inode_table[CD_INODE_ENTRY].TT[0];
	blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
	blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
	blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

	// its a directory; so the following should never happen
	if (itype == 'F')
	{
		printf("Fatal Error! Aborting.\n");
		exit(1);
	}

	// lets traverse the directory entries in all three blocks
	for (i = 0; i < 3; i++)
	{
		if (blocks[i] == 0)
			continue; // 0 means pointing at nothing

		readSFS(blocks[i], (char *)_directory_entries); // lets read a directory entry; notice the cast

		// so, we got four possible directory entries now
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0')
				continue; // means unused entry

			e_inode = stoi(_directory_entries[j].MMM, 3); // this is the inode that has more info about this entry

			if (_inode_table[e_inode].TT[0] == 'F')
			{ // entry is for a file
				if (strcmp(_directory_entries[j].fname, filename) == 0)
				{
					which_block = stoi(_inode_table[e_inode].XX, 2);
					if (which_block != 0)
					{
						check = readSFS(which_block, buf);
						if (check)
						{
							printf("%s", buf);
						}
					}
					which_block = stoi(_inode_table[e_inode].YY, 2);
					if (which_block != 0)
					{
						check = readSFS(which_block, buf);
						if (check)
						{
							printf("%s", buf);
						}
					}
					which_block = stoi(_inode_table[e_inode].ZZ, 2);
					if (which_block != 0)
					{
						check = readSFS(which_block, buf);
						if (check)
						{
							printf("%s", buf);
						}
					}
					printf("\n");
					return;
				}
			}
		}
	}
	printf("File not found!!\n");
}

void rm__(char *filename)
{
	char itype;
	int blocks[3],block[3];
	_directory_entry _directory_entries[4],_directory_entrie[4];		
	int total_files=0, total_dirs=0;
	int i,j;
	int e_inode;
		
	itype = _inode_table[CD_INODE_ENTRY].TT[0];
	blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX,2);
	blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY,2);
	blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ,2);
		
	if (itype=='F') 
	{
		printf("Fatal Error! Aborting.\n");
		exit(1);
	}
	int found=0;
	for (i=0; i<3; i++) 
	{
		if (blocks[i]==0) 
			continue;	// 0 means pointing at nothing		
		readSFS(blocks[i],(char *)_directory_entries);	// read directory entry
		// so, we got four possible directory entries now		
		for (j=0; j<4; j++) 
		{
			if (_directory_entries[j].F=='0') 
				continue;	// means unused entry
			if(strcmp(_directory_entries[j].fname,filename))	
				continue;
			found=1;
			e_inode = stoi(_directory_entries[j].MMM,3);	// this is the inode that has more info about this entry
			if (_inode_table[e_inode].TT[0]=='F')  
			{
				for(int k=0;k<3;k++)
				{
	                int block_id;
	              	if(k==0)
						block_id=stoi(_inode_table[e_inode].XX,2);
					else if(k==1)
						block_id=stoi(_inode_table[e_inode].YY,2);
					else
						block_id=stoi(_inode_table[e_inode].ZZ,2);
					if(block_id)
					{
						writeSFS(block_id,NULL);
						returnBlock(block_id);
					}
			   	}
				_directory_entries[j].F='0';
				writeSFS(blocks[i],(char *)_directory_entries);
				returnInode(e_inode);
			 }
			else if (_inode_table[e_inode].TT[0]=='D')
			{
				char currdir[1024];
				int l;
				strcpy(currdir,current_working_directory);
				int cdi=CD_INODE_ENTRY;
				block[0] = stoi(_inode_table[e_inode].XX,2);
				block[1] = stoi(_inode_table[e_inode].YY,2);
				block[2] = stoi(_inode_table[e_inode].ZZ,2);
				for(l=0;l<3;l++)
				{
					if (block[l]==0) continue;	// 0 means pointing at nothing
					int o;
					readSFS(block[l],(char *)_directory_entrie);	// lets read a directory entry; notice the cast
					for (o=0; o<4; o++) 
					{
						if (_directory_entrie[o].F=='0') continue;	// means unused entry
						cd(_directory_entries[j].fname);
						rm__(_directory_entrie[o].fname);
						int m;
						for(m=0;current_working_directory[m]!='\0';m++)
							current_working_directory[m]=currdir[m];
						current_working_directory[m]='\0';
						CD_INODE_ENTRY=cdi;
					}
				}
				_directory_entries[j].F='0';
				writeSFS(blocks[i],(char *)_directory_entries);
				returnInode(e_inode);
			}
		}
		int yes=0;
		readSFS(blocks[i],(char *)_directory_entries);
		for(j=0;j<4;j++)
		{
			if(_directory_entries[j].F!='0')
				yes=1;
		}
		if(!yes)
			returnBlock(blocks[i]);
	}
	if(!found)
		printf("File/directory not found!!\n");
}

void create__(char *filename)
{
	char itype;
	int blocks[3];
	_directory_entry _directory_entries[4];
	int i, j;
	int empty_dir_block = -1, empty_dir_entry = -1;
	int empty_inode_entry;

	if (free_inode_entries == 0)
	{
		printf("Error: Inode table is full.\n");
		return;
	}
	// read inode entry for current directory
	itype = _inode_table[CD_INODE_ENTRY].TT[0];
	blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
	blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
	blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);
	if (itype == 'F')
	{
		printf("It is not a directory.\n");
		exit(1);
	}
	for (i = 0; i < 3; i++)
	{
		if (blocks[i] == 0)
		{ // 0 means pointing at nothing
			if (empty_dir_block == -1)
			{
				empty_dir_block = i;
			}
			continue;
		}
		readSFS(blocks[i], (char *)_directory_entries); //here we read the directory
		// so, we got four possible directory entries now
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0') //check for unused entry
			{
				if (empty_dir_entry == -1)
				{
					empty_dir_entry = j;
					empty_dir_block = i;
				}
				continue;
			}
			if (strncmp(filename, _directory_entries[j].fname, 252) == 0)
			{
				printf("%.252s: Already exists!!\n", filename);
				return;
			}
		}
	}
	if (empty_dir_entry == -1 && empty_dir_block == -1)
	{	
		// no empty dir entries or blocks
		printf("No directories can be made.\n");
		return;
	}
	else
	{
		if (empty_dir_entry == -1)
		{
			// did not find an empty entry but not all three blocks have been used
			empty_dir_entry = 0;
			if ((blocks[empty_dir_block] = getBlock()) == -1)	// get new block
			{
				printf("Error: Disk is full.\n");
				return;
			}
			writeSFS(blocks[empty_dir_block], NULL); // write all zeros to the block, since it may contain some garbage
			switch (empty_dir_block)	// update the inode entry of current dir to reflect that we are using a new block
			{
			case 0:
				itos(_inode_table[CD_INODE_ENTRY].XX, blocks[empty_dir_block], 2);
				break;
			case 1:
				itos(_inode_table[CD_INODE_ENTRY].YY, blocks[empty_dir_block], 2);
				break;
			case 2:
				itos(_inode_table[CD_INODE_ENTRY].ZZ, blocks[empty_dir_block], 2);
				break;
			}
		}
		empty_inode_entry = getInode();									// get an empty place in the inode table which will store info about block
		readSFS(blocks[empty_dir_block], (char *)_directory_entries);	// read block of current directory where info on this new directory

		_directory_entries[empty_dir_entry].F = '1';
		strncpy(_directory_entries[empty_dir_entry].fname, filename, 252);

		itos(_directory_entries[empty_dir_entry].MMM, empty_inode_entry, 3); // and the index of the inode that will hold info inside this directory
		writeSFS(blocks[empty_dir_block], (char *)_directory_entries);  // write this block back to the disk

		int stop = 0, count = 0, block_id, k, i;
		strncpy(_inode_table[empty_inode_entry].TT, "FI", 2);
		for (k = 0; k < 3; k++)
		{
			if (!stop)
			{
				block_id = getBlock();
				if (block_id == -1)
				{
					printf("Disk is Full!\n");
				}
				else
				{
					printf("(Max 3072 characters : hit ESC-ENTER to end )\n");
					char message[1024];
					getchar();
					for (i = 0; i < 1024 && !stop; i++)
					{
						char c = getchar();
						if (c == 27)
						{
							stop = 1;
							break;
						}
						message[i] = c;
					}
					count += i;
					message[count] = '\0';
					writeSFS(block_id, message);
					char name[3];
					itos(name, block_id, 2);
					if (i == 0)
						strncpy(_inode_table[empty_inode_entry].XX, name, 2);
					else if (i == 1)
						strncpy(_inode_table[empty_inode_entry].YY, name, 2);
					else
						strncpy(_inode_table[empty_inode_entry].ZZ, name, 2);
				}
			}
		}
		printf("%d bytes saved.\n", count);
		writeSFS(BLOCK_INODE_TABLE, (char *)_inode_table); // write the inode table back to the disk
	}
}

bool check(char *filename)
{
	char itype;
	int blocks[3];
	_directory_entry _directory_entries[4];
	char buf[1024];
	int which_block;
	int check;

	int i, j;
	int e_inode;

	// read inode entry for current directory
	// in SFS, an inode can point to three blocks at the most
	itype = _inode_table[CD_INODE_ENTRY].TT[0];
	blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
	blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
	blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

	// its a directory; so the following should never happen
	if (itype == 'F')
	{
		printf("Fatal Error! Aborting.\n");
		exit(1);
	}

	// lets traverse the directory entries in all three blocks
	for (i = 0; i < 3; i++)
	{
		if (blocks[i] == 0)
			continue; // 0 means pointing at nothing

		readSFS(blocks[i], (char *)_directory_entries); // lets read a directory entry; notice the cast

		// so, we got four possible directory entries now
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0')
				continue; // means unused entry

			e_inode = stoi(_directory_entries[j].MMM, 3); // this is the inode that has more info about this entry

			if (_inode_table[e_inode].TT[0] == 'F')
			{ // entry is for a file
				if (strcmp(_directory_entries[j].fname, filename) == 0)
				{
					return true;
				}
			}
		}
	}
	return false;
}

int main(void)
{
	mountSFS();
	while(1)
	{
		printPrompt();
		char str[1024];
		scanf("%s",str);
		if(!strcmp(str,"display"))
		{
			char filename[1024];
			scanf("%s",filename);
			disp__(filename);
			printf("\n");
		}
		else if(!strcmp(str,"rm"))
		{
			char filename[1024];
			scanf("%s",filename);
			rm__(filename);
			printf("\n");
		}
		else if(!strcmp(str,"create"))
		{
			char filename[1024];
			scanf("%s",filename);
			create__(filename);
			printf("\n");
		}
		else if(!strcmp(str,"cd"))
		{
			char filename[1024];
			scanf("%s",filename);
			cd(filename);
			printf("\n");
		}
		else if(!strcmp(str,"md"))
		{
			char filename[1024];
			scanf("%s",filename);
			md(filename);
			printf("\n");
		}
		else if(!strcmp(str,"ls")){
			ls();
			printf("\n");
		}
		else if(!strcmp(str,"stats")){
			stats();
			printf("\n");
		}
		else if(!strcmp(str,"rd")){
			rd();
			printf("\n");
		}
		else if(!strcmp(str,"exit")){
			break;
		};
	}
	return 0;
}