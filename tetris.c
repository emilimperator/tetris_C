// Starting point for Tetris project.

#include <SDL2/SDL.h>
#include <stdbool.h> // For bool type
#include <stdio.h>   // For printf error messages

// --- Game Constants ---
// Screen dimensions
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720

// Tetris Board dimensions (in terms of blocks)
#define BLOCK_SIZE 30
#define COLUMNS 10
#define ROWS 20

#define LOCKED 2
#define EMPTY 0

#define MAX_LEADERBOARD_ENTRY 10
#define LEADERBOARD_FILE "leaderboard.dat"

int leaderboard[MAX_LEADERBOARD_ENTRY];

// Calculated board dimensions in pixels (useful for drawing board boundaries)
const int BOARD_DRAW_WIDTH = COLUMNS * BLOCK_SIZE;
const int BOARD_DRAW_HEIGHT = ROWS * BLOCK_SIZE;

// Offset to draw the board (e.g., to center it or place UI next to it)
const int BOARD_OFFSET_X = 200; // Example: Start board 50px from left
const int BOARD_OFFSET_Y = 50; // Example: Start board 50px from top

// --- Global SDL Variables ---
SDL_Window *window = NULL;     // The window we'll be rendering to
SDL_Renderer *renderer = NULL; // The renderer for the window

// --- Variables for Tetris logic ---
// The main game board: a 2D array representing the grid.
// Each cell stores whether it's EMPTY (0) or FILLED (1).
int board[ROWS][COLUMNS]; // Will be initialized in this project.

// *** Project #1: Define different types and shapes of each Tetromino block ***
const int shapes[7][4][4][4] = {

  // I
  {
      {{0,0,0,0},
       {1,1,1,1},
       {0,0,0,0},
       {0,0,0,0}},

      {{0,0,1,0},
       {0,0,1,0},
       {0,0,1,0},
       {0,0,1,0}},

      {{0,0,0,0},
       {0,0,0,0},
       {1,1,1,1},
       {0,0,0,0}},

      {{0,1,0,0},
       {0,1,0,0},
       {0,1,0,0},
       {0,1,0,0}}
  },

  
  // J
  {
      {{0,0,1,0},
       {0,0,1,0},
       {0,1,1,0},
       {0,0,0,0}},

      {{0,0,0,0},
       {0,1,0,0},
       {0,1,1,1},
       {0,0,0,0}},

      {{0,0,0,0},
       {0,1,1,0},
       {0,1,0,0},
       {0,1,0,0}},

      {{0,0,0,0},
       {1,1,1,0},
       {0,0,1,0},
       {0,0,0,0}}
  },

  // L
  {
      {{0,1,0,0},
       {0,1,0,0},
       {0,1,1,0},
       {0,0,0,0}},

      {{0,0,0,0},
       {0,1,1,1},
       {0,1,0,0},
       {0,0,0,0}},

      {{0,0,0,0},
       {0,1,1,0},
       {0,0,1,0},
       {0,0,1,0}},

      {{0,0,0,0},
       {0,0,1,0},
       {1,1,1,0},
       {0,0,0,0}}
  },

  // O
  {
      {{0,0,0,0},
       {0,1,1,0},
       {0,1,1,0},
       {0,0,0,0}},

      {{0,0,0,0},
       {0,1,1,0},
       {0,1,1,0},
       {0,0,0,0}},

      {{0,0,0,0},
       {0,1,1,0},
       {0,1,1,0},
       {0,0,0,0}},

      {{0,0,0,0},
       {0,1,1,0},
       {0,1,1,0},
       {0,0,0,0}}
  },

  // S
  {
      {{0,0,0,0},
       {0,1,1,0},
       {1,1,0,0},
       {0,0,0,0}},

      {{0,0,1,0},
       {0,0,1,1},
       {0,0,0,1},
       {0,0,0,0}},

      {{0,0,0,0},
       {0,1,1,0},
       {1,1,0,0},
       {0,0,0,0}},

      {{0,0,1,0},
       {0,0,1,1},
       {0,0,0,1},
       {0,0,0,0}}
  },

  // T
  {
      {{0,0,0,0},
       {0,1,1,1},
       {0,0,1,0},
       {0,0,0,0}},

      {{0,0,1,0},
       {0,1,1,0},
       {0,0,1,0},
       {0,0,0,0}},

      {{0,0,1,0},
       {0,1,1,1},
       {0,0,0,0},
       {0,0,0,0}},

      {{0,0,1,0},
       {0,0,1,1},
       {0,0,1,0},
       {0,0,0,0}}
  },

  // Z
  {
      {{0,0,0,0},
       {1,1,0,0},
       {0,1,1,0},
       {0,0,0,0}},

      {{0,0,1,0},
       {0,1,1,0},
       {0,1,0,0},
       {0,0,0,0}},
      {{0,0,0,0},
       {1,1,0,0},
       {0,1,1,0},
       {0,0,0,0}},

      {{0,0,1,0},
       {0,1,1,0},
       {0,1,0,0},
       {0,0,0,0}}
  }
};

  

// Global variables to store the state of the currently falling Tetromino piece
// These variables logically describe the current Tetromino piece.
int currentPieceX;    // X-coordinate on the board (in block units)
int currentPieceY;    // Y-coordinate on the board (in block units)
int currentPieceType; // Index (0-6) for the shape type from the 'shapes' array
int currentPieceRotation; // Index (0-3) for the current rotation

// --- Function Declarations ---
bool InitializeSDL(void); // Sets up SDL, window, and renderer
void CloseSDL(void);      // Cleans up SDL resources
void DrawBlock(int pixelX,
             int pixelY); // Helper function to draw current piece.
void InitializeGame(void);      // *** Project #1
void DrawCurrentPiece(void);    // *** Project #1

// movements
void CheckForOneRight(int type, int rot); // checks for horizontal right
void CheckForOneLeft(int type, int rot);
bool CanMoveDown(int type, int rot);
void CheckForRot(int type, int rot);
void LockThePiece(int type, int rot);
void DrawTheLockedPiece(void);

void CreateNewElement(void);

bool IsLocked(int block);

void ClearTheRow(int row);
void IncreaseTheSpeed(int level);

void LoadLeaderboard();
void SaveLeaderboard();
void AddToLeaderboard(int finalScore);

void PrintGameOverInfo();
int compare(const void* a, const void* b);

int recent_element=0;
bool gameOver=false;

int total_score=0;
int your_current_level=0;

int fallDelay=350;

// --- Main Function ---
int main(int argc, char *args[]) {
  srand((unsigned)time(NULL));
  // Start up SDL and create window/renderer
  if (!InitializeSDL()) {
      printf("Failed to initialize SDL!\n");
      // CloseSDL(); // Ensure cleanup even on partial init failure
      return 1; // Indicate an error
  }
  
  InitializeGame(); // Initialize the game (board, current block)
  
  LoadLeaderboard();
  // Main loop flag
  bool running = true;
  
  // Event handler
  SDL_Event e;
  
  int lastFalltime=0;
  
  // Game loop
  while (running) {
      // Handle events on queue
      while (SDL_PollEvent(&e) != 0) {
          // User requests quit (e.g., clicks the X button)
          if (e.type == SDL_QUIT) {
              running = false;
          }
          // User presses a key
          else if (e.type == SDL_KEYDOWN) {
              if(e.key.keysym.sym==SDLK_ESCAPE){
                  running=false;
              }
              else if(!gameOver && e.key.keysym.sym==SDLK_RIGHT){
                  CheckForOneRight(currentPieceType, currentPieceRotation);
              }
              else if(!gameOver && e.key.keysym.sym==SDLK_LEFT){
                  CheckForOneLeft(currentPieceType, currentPieceRotation);
              }
              else if(!gameOver && e.key.keysym.sym==SDLK_UP){
                  if(currentPieceRotation==3)
                      CheckForRot(currentPieceType, 1);
                  else
                      CheckForRot(currentPieceType, currentPieceRotation+1);
              }
              else if(!gameOver && e.key.keysym.sym==SDLK_DOWN){
                  if(CanMoveDown(currentPieceType, currentPieceRotation)){
                      currentPieceY++;
                  }
                  else{
                      LockThePiece(currentPieceType, currentPieceRotation);
                      if(!gameOver){
                          CreateNewElement();
                      }
                  }
              }
          }
      }
      
      
      if(!gameOver){
          int currentTime=SDL_GetTicks();
          if(currentTime-lastFalltime>fallDelay){
              lastFalltime=currentTime;
              if(CanMoveDown(currentPieceType, currentPieceRotation)){
                  currentPieceY++;
              }
              else{
                  LockThePiece(currentPieceType, currentPieceRotation);
                  if(!gameOver){
                      CreateNewElement();
                  }
              }
          }
      }
      
      
      // --- Game Logic Updates would go here in later sessions ---
      // For now, it's static.
      
      // --- Drawing ---
      //    Set draw color (arguments are R, G, B, Alpha)
      SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Dark gray background
      SDL_RenderClear(
                      renderer); // Fill the entire screen with the current draw color
      
      SDL_Rect boardRect = {
          BOARD_OFFSET_X,   // X position
          BOARD_OFFSET_Y,   // Y position
          BOARD_DRAW_WIDTH, // Width
          BOARD_DRAW_HEIGHT // Height
      };
      SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                             255); // Black for the board playable area
      SDL_RenderFillRect(renderer, &boardRect);
      
      SDL_SetRenderDrawColor(renderer, 100, 100, 100,
                             255); // Lighter gray for border
      SDL_RenderDrawRect(renderer, &boardRect);
      
      
      if(!gameOver){
          DrawCurrentPiece();
      }
      
      DrawTheLockedPiece();  // drawing the locked piece here
      
      SDL_RenderPresent(renderer);
      
      // Frame rate cap (optional, to prevent using 100% CPU)
      SDL_Delay(1000 / 60); // Aim for roughly 60 FPS
  }
      // Free resources and close SDL
  CloseSDL();
return 0;
}

void ClearTheRow(int row) {
  for(int y=row;y>0;y--) {
      for(int x=0;x<COLUMNS;x++) {
          board[y][x]=board[y-1][x];
      }
  }
  for(int x=0;x<COLUMNS;x++) {
      board[0][x]=EMPTY;
  }
}

void IncreaseTheSpeed(int level){
  if(level==1){
      fallDelay=300;
  }
  else if(level==2){
      fallDelay=250;
  }
  else if(level==3){
      fallDelay=200;
  }
}

bool IsLocked(int block){
  if(block==2 || block==3 || block==4 || block==5 || block==6 || block==7 || block==8) return true;
  return false;
}

void CreateNewElement(void){
  currentPieceType=rand()%7;
  currentPieceRotation=rand()%4;
  currentPieceX=(COLUMNS-4)/2;
  currentPieceY=0;
  recent_element=currentPieceType;
}

void LockThePiece(int type, int rot){
  for(int row=0;row<4;row++){
      for(int col=0;col<4;col++){
          if(shapes[type][rot][row][col]==1){
              int y=currentPieceY+row;
              int x=currentPieceX+col;
              if(recent_element==0) board[y][x]=2;
              if(recent_element==1) board[y][x]=3;
              if(recent_element==2) board[y][x]=4;
              if(recent_element==3) board[y][x]=5;
              if(recent_element==4) board[y][x]=6;
              if(recent_element==5) board[y][x]=7;
              if(recent_element==6) board[y][x]=8;
          }
      }
  }
  int clearedLines=0;
  if(!gameOver){
      for(int col=0;col<COLUMNS;col++){
          if(board[2][col]!=EMPTY){
              gameOver=true;
              printf("Blocks reached the top. Press ESC to exit.\n");
              AddToLeaderboard(total_score);
              PrintGameOverInfo();
              break;
          }
      }
  }
  for(int row=ROWS-1;row>=0;row--){
      bool isFull=true;
      for(int col=0;col<COLUMNS;col++){
          if(board[row][col]==EMPTY){
              isFull=false;
              break;
          }
      }
      if(isFull){
          ClearTheRow(row);
          row++;
          clearedLines++;
      }
  }
  if(clearedLines>0){
      switch(clearedLines){
          case 1:
              total_score+=100;
              printf("1 Line Cleared: +100 points\n");
              break;
          case 2:
              total_score+=300;
              printf("2 Lines Cleared: +300 points\n");
              break;
          case 3:
              total_score+=500;
              your_current_level=3;
              printf("3 Lines Cleared: +500 points\n");
              break;
          case 4:
              total_score+=800;
              printf("A Tetris! +800 points\n");
              break;
      }
      if(total_score>=1000) your_current_level=3;
      else if(total_score>=500) your_current_level=2;
      else if(total_score>=100) your_current_level=1;
      IncreaseTheSpeed(your_current_level);
      printf("Total Score: %d & Your Current Level: %d\n", total_score, your_current_level);
      clearedLines=0;
  }
}

// Prints final game over information and leaderboard to the console.
void PrintGameOverInfo() {
  int leaderboardCount = sizeof(leaderboard) / sizeof(int);

  printf("\n\n=====================================\n");
  printf("              GAME OVER\n");
  printf("=====================================\n");
  printf("            Leaderboard:\n");

  for (int i = 0; i < leaderboardCount; ++i) {
      printf("%2d. %d\n", i + 1, leaderboard[i]);
  }

  printf("=====================================\n");
}


bool CanMoveDown(int type, int rot){
  for(int row=0;row<4;row++){
      for(int col=0;col<4;col++){
          if(shapes[type][rot][row][col]==1){
              int y=currentPieceY+row+1;
              int x=currentPieceX+col;
              if(y>=ROWS) return false;
              if(IsLocked(board[y][x])) return false;
          }
      }
  }
  return true;
}

void CheckForRot(int type, int rot){
  for(int row=0;row<4;row++){
      for(int col=0;col<4;col++){
          if(shapes[type][rot][row][col]==1){
              int x=currentPieceX+col;
              int y=currentPieceY+row;
              if(x<0 || x>=COLUMNS) return;
              if(y<0 || y>=ROWS) return;
              if(IsLocked(board[y][x])) return;
          }
      }
  }
  if(currentPieceRotation==3) currentPieceRotation=0;
  else{
      currentPieceRotation++;
  }
}

void CheckForOneRight(int type, int rot){
  for(int row=0;row<4;row++){
      for(int col=3;col>=0;col--){
          if(shapes[type][rot][row][col]==1){
              int x=currentPieceX+col+1;
              int y=currentPieceY+row;
              if(x>=COLUMNS) return;
              if(IsLocked(board[y][x])) return;
          }
      }
  }
  currentPieceX++;
}
void CheckForOneLeft(int type, int rot){
  for(int row=0;row<4;row++){
      for(int col=0;col<4;col++){
          if(shapes[type][rot][row][col]==1){
              int x=currentPieceX+col-1;
              int y=currentPieceY+row;
              if(x<0) return;
              if(IsLocked(board[y][x])) return;
          }
      }
  }
  currentPieceX--;
}

void LoadLeaderboard() {
  FILE* file = fopen(LEADERBOARD_FILE, "rb"); // "rb" = read binary
  if (file == NULL) {
      printf("No leaderboard file found. A new one will be created.\n");
      return;
  }

  // Read up to MAX_ENTRIES structs from the file into our array.
  // fread returns the number of full elements it successfully read.
  fread(leaderboard, sizeof(int), MAX_LEADERBOARD_ENTRY, file);
  fclose(file);

  printf("Loaded scores from %s.\n", LEADERBOARD_FILE);
}

void SaveLeaderboard() {
  FILE* file = fopen(LEADERBOARD_FILE, "wb"); // "wb" = write binary
  if (file == NULL) {
      printf("Error: Could not save leaderboard to file!\n");
      return;
  }

  // Write the raw binary data of the leaderboard array to the file.
  int leaderboardCount = sizeof(leaderboard) / sizeof(int);

  fwrite(leaderboard, sizeof(int), leaderboardCount, file);
  fclose(file);

  printf("Leaderboard saved.\n");
}

void AddToLeaderboard(int finalScore) {
  int leaderboardCount = sizeof(leaderboard) / sizeof(int);

  if (leaderboardCount < MAX_LEADERBOARD_ENTRY ||
      finalScore > leaderboard[MAX_LEADERBOARD_ENTRY - 1])
  {
      leaderboard[MAX_LEADERBOARD_ENTRY - 1] = finalScore;
  }
  else {
      return; // Score wasn't high enough
  }

  qsort(leaderboard, leaderboardCount, sizeof(int), compare);

  SaveLeaderboard();
}

int compare(const void* a, const void* b)
{
  int int_a = *( (int*)a );
  int int_b = *( (int*)b );

  if (int_a == int_b) return 0;
  else if (int_a < int_b) return 1;
  else return -1;
}
// --- Function Implementations ---
bool InitializeSDL(void) {
// Initialize SDL video subsystem
if (SDL_Init(SDL_INIT_VIDEO) < 0) {
  printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  return false;
}

// Create window
window = SDL_CreateWindow("SKKU Tetris",           // Window title
                          SDL_WINDOWPOS_UNDEFINED, // Initial x position
                          SDL_WINDOWPOS_UNDEFINED, // Initial y position
                          SCREEN_WIDTH,            // Width, in pixels
                          SCREEN_HEIGHT,           // Height, in pixels
                          SDL_WINDOW_SHOWN         // Flags
);

if (window == NULL) {
  printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
  SDL_Quit(); // Cleanup SDL initialization
  return false;
}

// Create renderer for window
// Using -1 for index to initialize the first one supporting the requested
// flags SDL_RENDERER_ACCELERATED uses hardware acceleration
// SDL_RENDERER_PRESENTVSYNC synchronizes rendering with the refresh rate
renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
if (renderer == NULL) {
  printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
  SDL_DestroyWindow(window); // Cleanup window
  SDL_Quit();                // Cleanup SDL initialization
  return false;
}

// If everything initialized successfully
printf("SDL Initialized Successfully!\n");
return true;
}

void CloseSDL(void) {
// Destroy renderer
if (renderer) { // Check if it was actually created
  SDL_DestroyRenderer(renderer);
  renderer = NULL; // Set to NULL to prevent double deletion
  printf("Renderer destroyed.\n");
}

// Destroy window
if (window) { 
  SDL_DestroyWindow(window);
  window = NULL; // Set to NULL
  printf("Window destroyed.\n");
}

// Quit SDL subsystems
SDL_Quit();
printf("SDL quit.\n");
}

void InitializeGame(void) {
for(int i=0;i<ROWS;i++){
  for(int j=0;j<COLUMNS;j++){
    board[i][j]=0;
  }
}


  currentPieceType=rand()%7;
  currentPieceRotation=rand()%4;
  recent_element=currentPieceType;
currentPieceX=(COLUMNS-4)/2;
currentPieceY=0;

printf("Game Initialized. Current piece type: %d at board coordinates (%d, "
       "%d), rotation: %d\n",
       currentPieceType, currentPieceX, currentPieceY, currentPieceRotation);
}

void DrawTheLockedPiece(void){
  for(int y=0;y<ROWS;y++){
      for(int x=0;x<COLUMNS;x++){
          if(IsLocked(board[y][x])){
              if(board[y][x]==2) SDL_SetRenderDrawColor(renderer, 128, 255, 0, 255);
              else if(board[y][x]==3) SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
              else if(board[y][x]==4) SDL_SetRenderDrawColor(renderer, 255, 255, 151, 255);
              else if(board[y][x]==5) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
              else if(board[y][x]==6) SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
              else if(board[y][x]==7) SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
              else if(board[y][x]==8) SDL_SetRenderDrawColor(renderer, 255, 51, 153, 255);
              
              int screenX = BOARD_OFFSET_X + x * BLOCK_SIZE;
              int screenY = BOARD_OFFSET_Y + y * BLOCK_SIZE;
              
              SDL_Rect r={
                  screenX,
                  screenY,
                  BLOCK_SIZE-1,
                  BLOCK_SIZE-1
              };
              
              SDL_RenderFillRect(renderer, &r);
          }
      }
  }
}

// Project #1: Implement this function.
void DrawCurrentPiece(void) {
// Function to draw the current Tetromino piece on the board.

int type=currentPieceType;
int rotation=currentPieceRotation;

for(int row=0;row<4;row++){
  for(int column=0;column<4;column++){
    if(shapes[type][rotation][row][column]==1){
      DrawBlock(column, row);
    }
  }
}
// To-do: Calculate which block in the Tetromino piece[4][4] should be filled
// based on the "shapes[7][4][4][4]" array and the global variables
// currentPieceX, currentPieceY, currentPieceType, and currentPeiceRotation
// that you initialized in InitalizeGame() function.
// Then, draw the block using DrawBlock function below.
}

void DrawBlock(int blockX, int blockY) {
// Set the color for the piece. For now, let's use a single color (e.g., Red).

if(recent_element==0) SDL_SetRenderDrawColor(renderer, 128, 255, 0, 255);
  else if(recent_element==1) SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
  else if(recent_element==2) SDL_SetRenderDrawColor(renderer, 255, 255, 151, 255);
  else if(recent_element==3) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  else if(recent_element==4) SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  else if(recent_element==5) SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  else if(recent_element==6) SDL_SetRenderDrawColor(renderer, 255, 51, 153, 255);

int screenX = BOARD_OFFSET_X + (currentPieceX + blockX) * BLOCK_SIZE;
int screenY = BOARD_OFFSET_Y + (currentPieceY + blockY) * BLOCK_SIZE;

// Define the rectangle for a single block on the screen
SDL_Rect blockRect = {
    screenX, screenY,
    BLOCK_SIZE - 1, // Width of the block (slightly smaller for a grid line effect)
    BLOCK_SIZE - 1 // Height of the block (slightly smaller for a grid line effect)
};

// Draw the filled rectangle for this block
SDL_RenderFillRect(renderer, &blockRect);
}
