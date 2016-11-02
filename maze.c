/**
 * This is a Maze Generating/Solving program
 * Written by me
 * 2013-11-08
 * time spent: about 6 hours
 */

/**
 * Sample Maze:
++  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++          ++                  ++      ++          ++      ++      ++
++  ++++++  ++++++++++++++  ++  ++  ++  ++++++  ++  ++  ++  ++++++  ++
++      ++                  ++      ++  ++      ++      ++      ++  ++
++  ++  ++++++++++++++++++++++++++++++  ++  ++++++++++++++++++  ++  ++
++  ++      ++                      ++  ++  ++  ++          ++  ++  ++
++  ++++++  ++++++++++++++++++  ++  ++  ++  ++  ++  ++  ++++++  ++  ++
++  ++      ++              ++  ++      ++  ++      ++      ++  ++  ++
++  ++  ++++++  ++++++++++  ++++++  ++++++  ++  ++++++++++  ++  ++  ++
++  ++          ++      ++      ++          ++  ++          ++  ++  ++
++  ++++++++++++++  ++  ++++++  ++++++++++++++  ++  ++++++  ++  ++  ++
++              ++  ++  ++  ++              ++  ++      ++  ++      ++
++++++++++++++  ++  ++  ++  ++++++++++++++  ++  ++++++  ++  ++++++  ++
++      ++  ++      ++                  ++  ++  ++      ++  ++      ++
++  ++  ++  ++++++++++  ++++++++++++++++++  ++  ++  ++++++  ++  ++++++
++  ++              ++  ++                  ++  ++      ++  ++  ++  ++
++  ++++++++++++++++++  ++  ++++++++++++++++++  ++++++  ++  ++  ++  ++
++                      ++  ++      ++      ++  ++  ++  ++  ++      ++
++  ++++++++++++++++++++++  ++  ++  ++++++  ++  ++  ++  ++++++++++  ++
++              ++  ++      ++  ++      ++          ++  ++          ++
++++++++++++++  ++  ++  ++++++  ++++++  ++++++++++++++  ++  ++++++++++
++          ++  ++  ++      ++  ++  ++  ++          ++      ++      ++
++  ++  ++++++  ++  ++++++  ++  ++  ++  ++  ++++++  ++  ++++++++++  ++
++  ++      ++  ++      ++      ++  ++      ++  ++  ++              ++
++++++++++  ++  ++  ++  ++++++++++  ++++++++++  ++  ++++++++++++++  ++
++          ++      ++      ++      ++          ++      ++          ++
++  ++++++++++++++++++++++  ++  ++  ++++++  ++++++++++  ++  ++++++++++
++                      ++      ++      ++      ++      ++  ++      ++
++  ++++++++++++++++++  ++++++++++++++  ++++++  ++  ++  ++++++  ++  ++
++      ++          ++      ++      ++          ++  ++  ++      ++  ++
++++++  ++  ++++++  ++  ++++++  ++  ++++++++++  ++  ++  ++  ++++++  ++
++      ++  ++  ++  ++  ++      ++              ++  ++  ++      ++  ++
++  ++++++  ++  ++  ++  ++  ++++++++++++++++++++++  ++++++++++  ++  ++
++          ++      ++                          ++              ++  ++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  ++
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//int size: the default maze size
//the size should be odd, for the sake of making legal mazes
//but: it's graceful handled, so, if it's even, it will become size+1
int size=35;
//a one dimensional char array. But mimic two dimensional array. initilized in makeMaze() or readMaze()
char *Maze;
//used in readMaze()
char* filePath = "./maze.txt";
//define the char for wall, path and solution path
char wall='+';
char path=' ';
char markSolution='O';
//initialized in makeMaze() and readMaze()
int startx, starty;//start position
int endx, endy;//end position


/**
 * handle maze from 1 to size!!! including check boundary
 * @param  x colume: from 1 to size
 * @param  y line:from 1 to size
 * @return   the value of that position, '0' if not in the maze
 */
char getPiece(int x, int y)
{
	//check boundary
	if(x<1||y<1||x>size||y>size) return '0';
	int point = size*(y-1)+x -1;
	return *(Maze+point);
}

//do not check out of boundary, should be used after getPiece()
/**
 * similar to getPiece(), but:
 * get the pointer position instead of the value, use *(Maze + point) to get the pointer
 * @param  x colume: from 1 to size
 * @param  y colume: from 1 to size
 * @return   the position
 */
int getPiecePointer(int x, int y)
{
	int point = size*(y-1)+x -1;
	return point;
}

//won't handle illegal maze: maze has different size than 'size', or maze that don't contain
//the right amount '+' and ' '
/**
 * will read the Maze from file
 */
void readMaze()
{
	if(size%2==0) size++;//graceful handle

	//initialization
	startx=2; starty=1;
	endx=size-1; endy=size;

	//create the Maze
	Maze = (char *)malloc(sizeof(char)*size*size+sizeof(char));//last one char for '\0' or else free(Maze) will crash
	*(Maze+getPiecePointer(size, size)+1)='\0';//last char is eos

	FILE *fp;
	fp=fopen(filePath, "r");

	//handle file not exist error
	if(fp==NULL){
		printf("FILE CAN'T BE READ, PROGRAM TERMINATED.\n");
		exit(1);
	}

	char buffer;
	int pos=0;
	while((buffer=fgetc(fp))!=EOF)
	{
		if(buffer==wall||buffer==path){
			*(Maze+pos) = buffer;
			pos++;
		}
	}
	fclose(fp);
}

/**
 * Maze solving, will mark the path ON Maze array
 * recursive function: solve the Maze by depth search
 * @param  x(trivial) should be startx by default
 * @param  y(trivial) should be starty by default
 * @return   1(trivial) for not solvable
 */
int recursiveSolveMaze(int x, int y)
{
	//this will mark all the walking path O
	*(Maze+getPiecePointer(x,y))=markSolution;

	//base case: reach the finish point
	if(x==endx&&y==endy) return 0;

	//recursive explore all the possible path
	if(getPiece(x+1,y)==path){if(recursiveSolveMaze(x+1,y)==0) return 0;}//right
	if(getPiece(x-1,y)==path){if(recursiveSolveMaze(x-1,y)==0) return 0;}//left
	if(getPiece(x,y-1)==path){if(recursiveSolveMaze(x,y-1)==0) return 0;}//up
	if(getPiece(x,y+1)==path){if(recursiveSolveMaze(x,y+1)==0) return 0;}//down

	//this will unmark wrong path
	*(Maze+getPiecePointer(x,y))=path;
	return 1; //not solvable!

}

/**
 * Helper function of makeMaze()
 * recursive function: random generate the path, until go through all possible path block,
 * the position x,y is not the startpoint, but rather the first step, this is for the sake of the algorithm
 * @param  x should be the second colume(startx)
 * @param  y should be the second row(starty+1)
 * @return
 */
void makeNextBlock(int x, int y)
{
	int i=0;//go up to 3
	int a;
	int pu=0;int pd=0;int pl=0;int pr=0;
	for(;i<4;)
	{
		a=rand()%4+1;
		if(a!=pu&&a!=pd&&a!=pl&&a!=pr)
		{
			i++;
			switch(a)
			{
				case 1://go up
					pu=a;
					if (getPiece(x, y-2)==wall)
					{
						*(Maze+getPiecePointer(x,y-1))=path;
						*(Maze+getPiecePointer(x,y-2))=path;
						makeNextBlock(x, y-2);//move to the next block
					}
					break;
				case 2://go down
					pd=a;
					if (getPiece(x, y+2)==wall)
					{
						*(Maze+getPiecePointer(x,y+1))=path;
						*(Maze+getPiecePointer(x,y+2))=path;
						makeNextBlock(x, y+2);//move to the next block
					}
					break;
				case 3://go left
					pl=a;
					if (getPiece(x-2, y)==wall)
					{
						*(Maze+getPiecePointer(x-1,y))=path;
						*(Maze+getPiecePointer(x-2,y))=path;
						makeNextBlock(x-2, y);//move to the next block
					}
					break;
				case 4://go right
					pr=a;
					if (getPiece(x+2, y)==wall)
					{
						*(Maze+getPiecePointer(x+1,y))=path;
						*(Maze+getPiecePointer(x+2,y))=path;
						makeNextBlock(x+2, y);//move to the next block
					}
					break;
			}
		}
		else continue;
	}
	return; //no where to go
}

/**
 * initializion; graceful handle if size is even number; malloc Maze; initialize Maze; call makeNextBlock().
 * @return
 */
void makeMaze()
{
	if(size%2==0) size++;//graceful handle

	//initialization
	startx=2; starty=1;
	endx=size-1; endy=size;

	Maze = (char *)malloc(sizeof(char)*size*size+sizeof(char));//last one char for '\0' or else free(Maze) will crash
	*(Maze+getPiecePointer(size, size)+1)='\0';//last char is eos

	int i=0;
	for(;i<size*size;i++){*(Maze+i)=wall;}
	*(Maze+getPiecePointer(startx, starty))=path;
	*(Maze+getPiecePointer(startx, starty+1))=path;
	*(Maze+getPiecePointer(endx, endy))=path;
	makeNextBlock(startx, starty+1); //important: not startx starty, but startx starty+1
}

/**
 * does what it said
 */
void printMaze()
{
	int i, j;
	for(i=1; i<=size; i++){
		for(j=1; j<=size; j++){
			printf("%c", getPiece(j,i));
		}
		printf("\n");
	}
}

/**
 * this will expand the row in double length, to make the maze looks nicer
 */
void prettyPrintMaze()
{
	int i, j;
	for(i=1; i<=size; i++){
		for(j=1; j<=size; j++){
			printf("%c", getPiece(j,i));
			printf("%c", getPiece(j,i));
		}
		printf("\n");
	}
}

/**
 * Invert the Maze matrix(end position is now start position), will make the Maze harder
 */
void prettyPrintHardMaze()
{
	int i, j;
	for(i=size; i>=1; i--){
		for(j=size; j>=1; j--){
			printf("%c", getPiece(j,i));
			printf("%c", getPiece(j,i));
		}
		printf("\n");
	}
}

/**
 * can handle one arg(optional) as size; will seed the rand function; will create 'repeat' many mazes,
 * print to the screen, use"maze.exe > a_text_file.txt" to put to a file
 * free the memory of Maze created in makeMaze() or readMaze()
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description]
 */
int main(int argc, char const *argv[])
{
	if(argc==2) size = atoi(argv[1]);
	srand (time(NULL));//set rand seed

	//readMaze();
	//if(recursiveSolveMaze(startx, starty)){printf("NOT SOLVABLE! CAN'T REACH THE FINISH POINT.\n"); return 1;}
	int count=0;
	int repeat=5;
	while(count<repeat){
		makeMaze();

		prettyPrintHardMaze();

		printf("\n\n");
		// recursiveSolveMaze(startx,starty);
		// prettyPrintMaze();
		// printf("\n\n");
		count++;
	}

	free(Maze);
	return 0;
}
