#include "sfs.h"

void display(char *cmd2)
{
	char itype;
	int blocks[3];
	_directory_entry _directory_entries[4];
	int total_files = 0, total_dirs = 0;
	int i, j;
	int e_inode;
	// we can read inode entry for current directory,here inode point to can point to max 3 blocks
	itype = _inode_table[CD_INODE_ENTRY].TT[0];
	blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
	blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
	blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

	// As it is a directory it can never be a file i_type checks this thing so if we it is a file we can give error message
	if (itype == 'F')
	{
		printf("It is not a directory\n");
		exit(1);
	}
	int found = 0;
	// lets traverse the directory entries in all three blocks
	for (i = 0; i < 3; i++)
	{
		if (blocks[i] == 0)
			continue; // 0 means it is not pointing anything

		readSFS(blocks[i], (char *)_directory_entries); // here we read the directory entry
		//here there are max of 4 possible entries
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0')
				continue; // it is not used entry
			if (strcmp(_directory_entries[j].fname, cmd2))
				continue;
			found = 1;
			e_inode = stoi(_directory_entries[j].MMM, 3);

			if (_inode_table[e_inode].TT[0] == 'F')
			{
				int bn = stoi(_inode_table[e_inode].XX, 2);
				char buff[1024];
				if (bn)
				{
					readSFS(bn, buff);
					printf("%s", buff);
				}
				bn = stoi(_inode_table[e_inode].YY, 2);
				if (bn)
				{
					readSFS(bn, buff);
					printf("%s", buff);
				}
				bn = stoi(_inode_table[e_inode].ZZ, 2);
				if (bn)
				{
					readSFS(bn, buff);
					printf("%s", buff);
				}
			}
		}
	}
	if (!found)
		printf("File does not exist!");
	printf("\n");
}
void create(char *dname)
{
	char itype;
	int blocks[3];
	_directory_entry _directory_entries[4];
	int i, j;
	int empty_dblock = -1, empty_dentry = -1;
	int empty_ientry;
	if (strlen(dname) == 0)
	{
		printf("Usage: md <directory name>\n");
		return;
	}
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
			if (strncmp(dname, _directory_entries[j].fname, 252) == 0)
			{
				printf("%.252s: Already exists.\n", dname);
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
		strncpy(_directory_entries[empty_dentry].fname, dname, 252);
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
void rm(char *cmd2)
{
	char itype;
	int blocks[3], block[3];
	_directory_entry _directory_entries[4], _directory_entrie[4];
	int total_files = 0, total_dirs = 0;
	int i, j;
	int e_inode;

	itype = _inode_table[CD_INODE_ENTRY].TT[0];
	blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
	blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
	blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

	if (itype == 'F')
	{
		printf("Fatal Error! Aborting.\n");
		exit(1);
	}
	int found = 0;
	for (i = 0; i < 3; i++)
	{
		if (blocks[i] == 0)
			continue;									// 0 means pointing at nothing
		readSFS(blocks[i], (char *)_directory_entries); // read directory entry
		// so, we got four possible directory entries now
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0')
				continue; // means unused entry
			if (strcmp(_directory_entries[j].fname, cmd2))
				continue;
			found = 1;
			e_inode = stoi(_directory_entries[j].MMM, 3); // this is the inode that has more info about this entry
			if (_inode_table[e_inode].TT[0] == 'F')
			{
				for (int k = 0; k < 3; k++)
				{
					int bid;
					if (k == 0)
						bid = stoi(_inode_table[e_inode].XX, 2);
					else if (k == 1)
						bid = stoi(_inode_table[e_inode].YY, 2);
					else
						bid = stoi(_inode_table[e_inode].ZZ, 2);
					if (bid)
					{
						writeSFS(bid, NULL);
						returnBlock(bid);
					}
				}
				_directory_entries[j].F = '0';
				writeSFS(blocks[i], (char *)_directory_entries);
				returnInode(e_inode);
			}
			else if (_inode_table[e_inode].TT[0] == 'D')
			{
				char currdir[1024];
				int l;
				strcpy(currdir, current_working_directory);
				currdir[strlen(current_working_directory)] = '\0';
				int cdi = CD_INODE_ENTRY;
				block[0] = stoi(_inode_table[e_inode].XX, 2);
				block[1] = stoi(_inode_table[e_inode].YY, 2);
				block[2] = stoi(_inode_table[e_inode].ZZ, 2);
				for (l = 0; l < 3; l++)
				{
					if (block[l] == 0)
						continue; // 0 means pointing at nothing
					int o;
					readSFS(block[l], (char *)_directory_entrie); // lets read a directory entry; notice the cast
					for (o = 0; o < 4; o++)
					{
						if (_directory_entrie[o].F == '0')
							continue; // means unused entry
						cd(_directory_entries[j].fname);
						rm(_directory_entrie[o].fname);
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
		int yes = 0;
		readSFS(blocks[i], (char *)_directory_entries);
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F != '0')
				yes = 1;
		}
		if (!yes)
			returnBlock(blocks[i]);
	}
	if (!found)
		printf("File does not exist! %s\n", cmd2);
}
int main(int argc, char **argv)
{
	mountSFS();
	while (1)
	{
		printPrompt();
		char str[1024];
		scanf("%s", str);
		if (strcmp(str, "display") == 0)
		{
			char fl[1024];
			scanf("%s", fl);
			display(fl);
		}
		if (strcmp(str, "rm") == 0)
		{
			char fl[1024];
			scanf("%s", fl);
			rm(fl);
		}
		else if (strcmp(str, "create") == 0)
		{
			char fl[1024];
			scanf("%s", fl);
			create(fl);
		}
		else if (strcmp(str, "cd") == 0)
		{
			char fl[1024];
			scanf("%s", fl);
			cd(fl);
		}
		else if (strcmp(str, "md") == 0)
		{
			char fl[1024];
			scanf("%s", fl);
			md(fl);
		}
		else if (strcmp(str, "ls") == 0)
			ls();

		else if (strcmp(str, "stats") == 0)
			stats();

		else if (strcmp(str, "rd") == 0)
			rd();

		else if (strcmp(str, "exit") == 0)
			exit(0);
	}
	return 0;
}
