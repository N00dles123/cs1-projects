/*
Jason Lin COP 3502C 0V01
Assignment 1: Chessland Attacks
1-15-2021
*/
// pre processing directives
#include <stdio.h>
#include <stdlib.h>

// typedefs for structs
typedef struct Piece Piece;
typedef struct List List;
typedef struct Board Board;

//structures
// rank represents row
// file represents column
struct Piece{
	int rank, file, id;
};

// List helps store the information for each row
// List will be the arraylist to store piece structures
struct List{
	int identifier;
	int size, cap;
	Piece ** arr;
};

// rankArrayList is an arraylist of rows
struct Board{
	int size, cap;
	List ** arr;
};

// static variables
#define STARTING_CAP 1

//function prototypes
void addRankToBoard(Board *, List *);
void expandBoard(Board *);
void destroyBoard(Board *, int);
Board * createBoard();

void addPieceToRank(List *, Piece *);
void expandRank(List *);
void destroyRank(List *);
List * createRank();
List * findRank(Board *, int);

void destroyPiece(Piece *, int);
Piece * createPiece(int, int, int);




//main function
int main(){
	// rook is an array of piece pointers
	// will be helpful later when comparing piece elements
	Piece ** rook;
	// board is created to store list of rows
	Board * rowboard = createBoard();

	// board is created to store list of columns
	Board * columnboard = createBoard();
	
	// both lists are temporary variables to use
	List * rankList;
	List * fileList;
	
	int numPieces, counter;
	// rank and file will be used to store the scanf values
	int rank, file;
	scanf("%d", &numPieces);
	// allocates numPieces amount of space for rooks
	rook = (Piece **) malloc(numPieces * sizeof(Piece *));
	// loops numPieces times to scan piece data
	for(counter = 0; counter < numPieces; counter++){
		scanf("%d %d", &rank, &file);
		rankList = findRank(rowboard, rank);
		fileList = findRank(columnboard, file);
		rook[counter] = createPiece(rank, file, counter + 1);
		addPieceToRank(rankList, rook[counter]);
		addPieceToRank(fileList, rook[counter]);
	}
	
	// TA gave this idea during ta session hopefully TA idea isnt cheating?
	for(int c = 0; c < numPieces; c++){
		int row, col;
		//keep track of rows
		row = rook[c]->rank;
		col = rook[c]->file;
		//keep track of lists
		List * curRow = findRank(rowboard, row);
		List * curCol = findRank(columnboard, col);
		// sets all pieces to NULL at beginning for comparison
		Piece * bestBefore = NULL;
		Piece * bestAfter = NULL;
		Piece * bestBelow = NULL;
		Piece * bestAbove = NULL;
		
		// will loop through the row of curRow to compare with pieces to detect risks
		for(int j = 0; j < curRow->size; j++){
			// if the current file of item in row array is less then move on
			if(curRow->arr[j]->file < col) {
				if(bestBelow == NULL || bestBelow->file < curRow->arr[j]->file)
					bestBelow = curRow->arr[j];
			}
			// if the current file of item in row array is greater than move on
			if(curRow->arr[j]->file > col) {
				if(bestAbove == NULL || bestAbove->file > curRow->arr[j]->file)
					bestAbove = curRow->arr[j];
			}
		}
		
		// loops through curCol to detect risks compare with the null pieces to start with
		for(int j = 0; j < curCol->size; j++){
			// if the current rank of the item in column array is less then move on
			if(curCol->arr[j]->rank < row){ 
				if(bestBefore == NULL || bestBefore->rank < curCol->arr[j]->rank)
					bestBefore = curCol->arr[j];
			}		
			// if the current rank of the item in column array is less than row move on
			if(curCol->arr[j]->rank > row){
				if(bestAfter == NULL || bestAfter->rank > curCol->arr[j]->rank)
					bestAfter = curCol->arr[j];
			}
		}
		
		// starts at 0 risks, risks go up for each piece not null
		int risks = 0;
		if(bestBefore != NULL){
			//printf("bestBefore exists\n");
			risks++;
		}
		if(bestAfter != NULL){
			//printf("bestAfter exists\n");
			risks++;
		}
		if(bestBelow != NULL){
			//printf("bestBefore exists\n");
			risks++;
		}
		if(bestAbove != NULL){
			//printf("bestAfter exists\n");
			risks++;
		}
		// number of risks are printed and 
		// if each piece is a risk
		
		printf("%d ", risks);
		if(bestBefore != NULL){
			printf("%d ", bestBefore->id);
		}
		if(bestAfter != NULL){
			printf("%d ", bestAfter->id);
		}
		if(bestBelow != NULL){
			printf("%d ", bestBelow->id);
		}
		if(bestAbove != NULL){
			printf("%d ", bestAbove->id);
		}
		printf("\n");
	}
	
	destroyBoard(rowboard, 0);
	destroyBoard(columnboard, 1);
	
	for(int i = 0; i < numPieces; i++){
		destroyPiece(rook[i], 1);
	}
	
	free(rook);	
	
	return 0;
}

List * createRank(int id){
	// Creates a place for the rank arraylist
	List * rank = calloc(1, sizeof(List));
	
	// creates the array of pieces
	rank->arr = calloc(STARTING_CAP, sizeof(Piece));

	// sets up starting capacity
	rank->cap = STARTING_CAP;

	// amount of starting pieces is 0
	rank->size = 0;

	// the identifier is the row or column number
	rank->identifier = id;

	return rank;
}

// adds a piece type struct to the rank arraylist
void addPieceToRank(List * ranks, Piece * newPiece){

	// first checks whether rank arraylist is full
	if(ranks->cap == ranks->size)
		expandRank(ranks);
	// then adds person to the end of the array
	int tempid = newPiece->rank;
	int listid = ranks->identifier;

	ranks->arr[ranks->size] = newPiece;

	// makes additions to the number of pieces in list
	ranks->size++;
}

//doubles the capacity of the rank arraylist
void expandRank(List * curRank){
	// doubles old capacity so that arraylist will 
	// have no need to expand each insertion
	int newCapacity = curRank->cap * 2;
	// pointers 
	curRank->arr = realloc(curRank->arr, newCapacity * sizeof(Piece *));

	curRank->cap = newCapacity;
	return;
}

// frees rank in memory
void destroyRank(List * curRank){
	// loops through to clean out each piece from memory
	for(int i = 0; i < curRank->size; i++){
		destroyPiece(curRank->arr[i], 0);
	} 

	// Free the array of rank pointers
	free(curRank->arr);

	// Frees the curRank arraylist
	free(curRank);
}

//creates a new piece
Piece * createPiece(int rank, int file, int id){
	// creates piece segment of memory
	Piece * rook = calloc(1, sizeof(Piece));

	// set up the rank of the piece
	rook->rank = rank;

	// set up the file of the piece
	rook->file = file;

	// set up the id of the piece
	rook->id = id;

	// returns created piece
	return rook;
}

// purpose of cleanPiece is to clean up memory created for piece
void destroyPiece(Piece * rook, int ifFreePiece){
	// frees rook pointer
	if(ifFreePiece){
		free(rook);
	}
}

// creates an empty array list of ranks
Board * createBoard(){
	// creates a place for board in the rankArrayList
	Board * board = calloc(STARTING_CAP, sizeof(Board));

	// creates the array of ranks
	board->arr = calloc(STARTING_CAP, sizeof(List));

	//sets capacity of the array of ranks
	board->cap = STARTING_CAP;

	// sets size of the array of ranks
	// sets to 0 because of 0 starting ranks
	board->size = 0;

	return board;
}


// adds a piece to the rankArrayList
void addRankToBoard(Board * board, List * rank){
	if(board->size == board->cap)
		expandBoard(board);

	board->arr[board->size] = rank;

	board->size++;
	//printf("rank was successfully added to board\n");
}

// frees the memory taken up by board
void destroyBoard(Board * board, int shouldFreeBoard){
	// frees each rank in the arraylist
	for(int i = 0; i < board->size; i++){
		if(shouldFreeBoard){
			destroyRank(board->arr[i]);
		}
		else if(!shouldFreeBoard){
			free(board->arr[i]);
		}

	}

	// frees the array of rank pointers
	free(board->arr);

	// frees the board arraylist
	free(board);
}

// expands the rankArrayList
void expandBoard(Board * board){
	// capcity will be the double of old capacity
	// so that arraylist wont need to increase after each insertion
	int newMax = board->cap * 2;

	// this moves the pointers of boards into a new array of pointers
	board->arr = realloc(board->arr, newMax * sizeof(List *));

	// updates capacity
	board->cap = newMax;
}

List * findRank(Board * board, int coordinate){
	List * ifNotfound;
	// this loops through the board arraylist to
	// find the List that has the row identifier
	for(int i = 0; i < board->size; i++) {
		List * curList = board->arr[i];
		if(curList->identifier == coordinate){
			//returns curList once 
			// the identifier is found
			return curList;
		}
	}
	// List couldnt be found so new one is created at the end
	ifNotfound = createRank(coordinate);
	addRankToBoard(board, ifNotfound);
	// returns newly created List
	return ifNotfound;
}
