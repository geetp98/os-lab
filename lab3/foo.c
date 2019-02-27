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
				}
			}
		}
	}
}

void rm__(char *filename)
{
	char itype;
	int blocks[3], block[3];
	_directory_entry _directory_entries[4], _directory_entrie[4];
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
			// printf("used entry\n");
			e_inode = stoi(_directory_entries[j].MMM, 3); // this is the inode that has more info about this entry

			if (_inode_table[e_inode].TT[0] == 'F')
			{ // entry is for a file
				// printf("entry is a file\n");
				if (strcmp(_directory_entries[j].fname, filename) == 0)
				{
					printf("filename : %s\n", filename);
					which_block = stoi(_inode_table[e_inode].XX, 2);
					if (which_block != 0)
					{
						check = writeSFS(which_block, NULL);
						if (check)
						{
							printf("done block 1!!\t");
						}
						returnBlock(which_block);
					}
					which_block = stoi(_inode_table[e_inode].YY, 2);
					if (which_block != 0)
					{
						check = writeSFS(which_block, NULL);
						if (check)
						{
							printf("done block 2!!\t");
						}
						returnBlock(which_block);
					}
					which_block = stoi(_inode_table[e_inode].ZZ, 2);
					if (which_block != 0)
					{
						check = writeSFS(which_block, NULL);
						if (check)
						{
							printf("done block 3!!\t");
						}
						returnBlock(which_block);
					}
					_directory_entries[j].F='0';
					writeSFS(blocks[i], (char *)_directory_entries);
					returnInode(e_inode);
					printf("\n");
				}
			}
			else if (_inode_table[e_inode].TT[0] == 'D')
			{
				char currdir[1024];
				int l, o;

				strcpy(currdir, current_working_directory);
				currdir[strlen(current_working_directory)] = '\0';

				int cdi = CD_INODE_ENTRY;

				block[0] = stoi(_inode_table[e_inode].XX, 2);
				block[1] = stoi(_inode_table[e_inode].YY, 2);
				block[2] = stoi(_inode_table[e_inode].ZZ, 2);

				// lets traverse the directory entries in all three blocks
				for (l = 0; l < 3; l++)
				{
					if (block[l] == 0)
						continue; // 0 means pointing at nothing
					
					readSFS(block[l], (char *)_directory_entrie); // lets read a directory entry; notice the cast
					// so, we got four possible directory entries now
					for (o = 0; o < 4; o++)
					{
						if (_directory_entrie[o].F == '0')
							continue; // means unused entry
						cd(_directory_entries[j].fname);
						rm__(_directory_entrie[o].fname);
						int m;
						for (m = 0; current_working_directory[m] != '\0'; m++)
							current_working_directory[m] = currdir[m];
						current_working_directory[m] = '\0';
						CD_INODE_ENTRY = cdi;
					}
				}

				_directory_entries[j].F = '0';
				writeSFS(blocks[i], (char *)_directory_entries);
				returnInode(e_inode);
			}
		}
	}
}

void md__(char *filename)
{
	char itype;
	int blocks[3];
	_directory_entry _directory_entries[4];
	int i, j;
	int empty_dblock = -1, empty_dentry = -1;
	int empty_ientry;

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
			if (empty_dblock == -1)
			{
				empty_dblock = i;
			}
			continue;
		}
		readSFS(blocks[i], (char *)_directory_entries); //here we read the directory
		// so, we got four possible directory entries now
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0') //check for unused entry
			{
				if (empty_dentry == -1)
				{
					empty_dentry = j;
					empty_dblock = i;
				}
				continue;
			}
			if (strncmp(filename, _directory_entries[j].fname, 252) == 0)
			{
				printf("%.252s: Already exists.\n", filename);
				return;
			}
		}
	}
	//here we are sure that the directory is new
	// if we did not find an empty directory entry and all three blocks are in use; then no new directory can be made
	if (empty_dentry == -1 && empty_dblock == -1)
	{
		printf("No directories can be made.\n");
		return;
	}
	else
	{
		if (empty_dentry == -1)
		{ // Great! didn't find an empty entry but not all three blocks have been used
			empty_dentry = 0;
			if ((blocks[empty_dblock] = getBlock()) == -1)
			{ // first get a new block using the block bitmap
				printf("Error: Disk is full.\n");
				return;
			}
			writeSFS(blocks[empty_dblock], NULL); // write all zeros to the block (there may be junk from the past!)
			switch (empty_dblock)
			{ // update the inode entry of current dir to reflect that we are using a new block
			case 0:
				itos(_inode_table[CD_INODE_ENTRY].XX, blocks[empty_dblock], 2);
				break;
			case 1:
				itos(_inode_table[CD_INODE_ENTRY].YY, blocks[empty_dblock], 2);
				break;
			case 2:
				itos(_inode_table[CD_INODE_ENTRY].ZZ, blocks[empty_dblock], 2);
				break;
			}
		}
		empty_ientry = getInode();								   // get an empty place in the inode table which will store info about block
		readSFS(blocks[empty_dblock], (char *)_directory_entries); // read block of current directory where info on this new directory
		
		_directory_entries[empty_dentry].F = '1';
		strncpy(_directory_entries[empty_dentry].fname, filename, 252);
		
		itos(_directory_entries[empty_dentry].MMM, empty_ientry, 3); // and the index of the inode that will hold info inside this directory
		writeSFS(blocks[empty_dblock], (char *)_directory_entries);  // write this block back to the disk
		
		int stop = 0, cnt = 0, bid, k, i;
		strncpy(_inode_table[empty_ientry].TT, "FI", 2);
		for (k = 0; k < 3; k++)
		{
			if (!stop)
			{
				bid = getBlock();
				if (bid == -1)
				{
					printf("Disk is Full!\n");
				}
				else
				{
					printf("(Max 3072 characters : hit ESC-ENTER to end )\n");
					char ppm[1024];
					getchar();
					for (i = 0; i < 1024 && !stop; i++)
					{
						char c = getchar();
						if (c == 27)
						{
							stop = 1;
							break;
						}
						ppm[i] = c;
					}
					cnt += i;
					ppm[i] = '\0';
					writeSFS(bid, ppm);
					char nm[3];
					itos(nm, bid, 2);
					if (i == 0)
						strncpy(_inode_table[empty_ientry].XX, nm, 2);
					else if (i == 1)
						strncpy(_inode_table[empty_ientry].YY, nm, 2);
					else
						strncpy(_inode_table[empty_ientry].ZZ, nm, 2);
				}
			}
		}
		printf("%d bytes saved.\n", cnt);
		writeSFS(BLOCK_INODE_TABLE, (char *)_inode_table); // phew!! write the inode table back to the disk
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

int firstwhitespace(char *s)
{
	int i = 0;
	while (i < strlen(s))
	{
		if (s[i] == ' ')
		{
			return i;
		}
		i++;
	}
	return i;
}

int endofstring(char *s)
{
	int i = 0;
	while (i < strlen(s))
	{
		if (s[i] == '\0')
		{
			return i;
		}
		i++;
	}
	return i;
}

int main(void)
{
	mountSFS();
	char command[8 + 1 + 252];
	char c1[8];
	char filename[252];
	while (1)
	{
		printPrompt();
		memset(command, 0, 8 + 1 + 252);
		memset(c1, 0, 8);
		scanf("%[^\n]%*c", command);
		strncpy(c1, command, firstwhitespace(command));
		if (!strcmp(c1, "display"))
		{
			memset(filename, 0, 252);
			strncpy(filename, &command[firstwhitespace(command) + 1], endofstring(command) - firstwhitespace(command));
			if (strlen(filename) >= 1)
			{
				int disp;
				disp = check(filename);
				disp ? disp__(filename) : printf("File not found..\n");
			}
			else
			{
				printf("Enter display <filename>\n");
			}
		}
		else if (!strcmp(c1, "rm"))
		{
			memset(filename, 0, 252);
			strncpy(filename, &command[firstwhitespace(command) + 1], endofstring(command) - firstwhitespace(command));
			if (strlen(filename) >= 1)
			{
				rm__(filename);
			}
			else
			{
				printf("Enter display <filename>\n");
			}
		}
		else if(!strcmp(c1, "md")){
			memset(filename, 0, 252);
			strncpy(filename, &command[firstwhitespace(command) + 1], endofstring(command) - firstwhitespace(command));
			if (strlen(filename) >= 1)
			{
				md__(filename);
			}
			else
			{
				printf("Enter md <filename>\n");
			}
		}
		else if(strcmp(c1, "stats")){
			stats();
		}
		else if(strcmp(c1, "ls")){
			ls();
		}
		else if(strcmp(c1, "rd")){
			rd();
		}
		else if (!strcmp(c1, "exit"))
		{
			break;
		}
		memset(command, 0, 8 + 1 + 252);
		memset(c1, 0, 8);
		printf("\n\n");
	}
	return 0;
}