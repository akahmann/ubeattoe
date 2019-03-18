#include <iostream>
#include <string>
#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <algorithm>
#include <queue>

void display(char board[][3]);
void personPlay(char board[][3], bool first);
void comPlay(char board[][3], bool first);
bool canIWin(char board[][3], char comChar, int &row, int &col);
bool canPlayerWin(char board[][3], char playerChar, int &row, int &col);
bool playerHasMiddle(char board[][3], char playerChar);
void analyzeBoard(char board[][3], char playerChar, std::queue<char> &scenario);
void handleScenario(char board[][3], bool first, std::queue<char> scenario, int &row, int &col);
bool isValid(char board[][3], std::string coord);
bool gameover(char board[][3], bool first);
void comWon();
void playerWon();
void catsGame();

struct Selection
{
   int r; //row
   int c; //col
};

/**********************************************************
* MAIN FUNCTION
* This a game of tic tac toe vs an unbeatable computer
***********************************************************/
int main()
{
   char board[3][3];
   srand(time(NULL));
   bool first = rand() % 2;
   //first = true; //debugging purposes

   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
         board[i][j] = '.';

   std::cout << "Welcome to the world of tic tac toe!\n"
             << "My name is Unbeat-toe. Have fun not winning!\n"
             << "#savage #youregoingtohaveabadtime\n";

   if (first)
      std::cout << "\nYou go first! (too bad it won't help you in the end...)\n";
   else
   {
      std::cout << "\nI go first! Your efforts are fruitless!\n";
      comPlay(board, first);
   }

   display(board);

   do
   {
      personPlay(board, first);
      comPlay(board, first);
      display(board);
   }
   while(!gameover(board, first));

   return 0;
}

/**********************************************************
* Display the tic tac toe board
***********************************************************/
void display(char board[][3])
{
   std::cout << "    a   b   c";
   for (int i = 0; i < 3; i++)
   {
      std::cout << std::endl;
      std::cout << ' ' << i + 1 << ' ';
      for (int j = 0; j < 3; j++)
      {
         if (j == 1 || j == 2)
            std::cout << '|';

         if (board[i][j] != 'X' && board[i][j] != 'O')
            std::cout << "   ";
         else
            std::cout << ' ' << board[i][j] << ' ';
      }
      std::cout << std::endl;
      if (i != 2)
         std::cout << "   ---+---+---";
   }
}

/**********************************************************
* User picks a space
***********************************************************/
void personPlay(char board[][3], bool first)
{
   char playerChar;
   char comChar;
   std::string coord;
   int row;
   int col;

   if (first)
   {
      playerChar = 'X';
      comChar = 'O';
   }
   else
   {
      playerChar = 'O';
      comChar = 'X';
   }

   do
   {
      std::cout << "Select coordinate: ";
      std::cin  >> coord;
   }
   while(!isValid(board, coord));

   //if you put a letter first
   if (isalpha(coord[0]))
   {
      row = (int)coord[1] - 49;
      col = (int)toupper(coord[0]) - 65;
   }
   //if you put a letter second
   else
   {
      row = (int)coord[0] - 49;
      col = (int)toupper(coord[1]) - 65;
   }

   board[row][col] = playerChar;
}

/**********************************************************
* Unbeat-toe picks a space
***********************************************************/
void comPlay(char board[][3], bool first)
{
   char playerChar;
   char comChar;
   int row = 0;
   int col = 0;

   if (first)
   {
      playerChar = 'X';
      comChar = 'O';
   }
   else
   {
      playerChar = 'O';
      comChar = 'X';
   }

   //Priority #1 - Can I Win?
   if (canIWin(board, comChar, row, col))
   {
      //don't need to do anything here
   }
   //Priority #2 - Can the Player Win?
   else if (canPlayerWin(board, playerChar, row, col))
   {
      //don't need to do anything here
   }
   //Priority #3 - If Player Picked the Middle
   else if (playerHasMiddle(board, playerChar))
   {
      //if Unbeat-toe went first
      if (!first)
      {
         //pick opposite corner
         if (board[0][0] == comChar)
         {
            board[2][2] = comChar;
         }
         else if (board[0][2] == comChar)
            board[2][0] = comChar;
         else if (board[2][0] == comChar)
            board[0][2] = comChar;
         else if (board[2][2] == comChar)
            board[0][0] = comChar;

      }
      //if Unbeat-toe went second
      else
         board[0][0] = comChar;
      return;
   }
   else
   {
      //Priority #4 - Read the Board and find coordinates
      std::queue<char> scenario;
      if (first)
         scenario.push('2');
      else
         scenario.push('1');
      analyzeBoard(board, playerChar, scenario);
      handleScenario(board, first, scenario, row, col);
   }

   board[row][col] = comChar;
}

/**********************************************************
* Find coordinates that would let Unbeat-toe win this turn
***********************************************************/
bool canIWin(char board[][3], char comChar, int &row, int &col)
{
   int count = 0;

   //check horizontal
   for (int i = 0; i < 3; i++)
   {
      for (int j = 0; j < 3; j++)
      {
         if (board[i][j] != '.' && board[i][j] != comChar)
            count = -2;
         if (board[i][j] == comChar)
            count++;
         if (board[i][j] == '.')
         {
            row = i;
            col = j;
         }
      }
      if (count == 2)
         return true;
      count = 0;
   }

   //check vertical
   for (int i = 0; i < 3; i++)
   {
      for (int j = 0; j < 3; j++)
      {
         if (board[j][i] != '.' && board[j][i] != comChar)
            count = -2;
         if (board[j][i] == comChar)
            count++;
         if (board[j][i] == '.')
         {
            row = j;
            col = i;
         }
      }
      if (count == 2)
         return true;
      count = 0;
   }

   //diagonal left to right
   for (int i = 0; i < 3; i++)
   {
      if (board[i][i] != '.' && board[i][i] != comChar)
            count = -2;
      if (board[i][i] == comChar)
         count++;
      if (board[i][i] == '.')
            row = col = i;
   }
   if (count == 2)
      return true;
   count = 0;

   //diagonal right to left
   for (int i = 2; i >= 0; i--)
   {
      if (board[3 - (i + 1)][i] != '.' && board[3 - (i + 1)][i] != comChar)
         count = -2;
      if (board[3 - (i + 1)][i] == comChar)
         count++;
      if (board[3 - (i + 1)][i] == '.')
      {
         row = 3 - (i + 1);
         col = i;
      }
   }
   if (count == 2)
      return true;

   return false;
}

/**********************************************************
* Find coordinates that would stop player from winning
***********************************************************/
bool canPlayerWin(char board[][3], char playerChar, int &row, int &col)
{
   int count = 0;

   //check horizontal
   for (int i = 0; i < 3; i++)
   {
      for (int j = 0; j < 3; j++)
      {
         if (board[i][j] != '.' && board[i][j] != playerChar)
            count = -2;
         if (board[i][j] == playerChar)
            count++;
         if (board[i][j] == '.')
         {
            row = i;
            col = j;
         }
      }
      if (count == 2)
         return true;
      count = 0;
   }

   //check vertical
   for (int i = 0; i < 3; i++)
   {
      for (int j = 0; j < 3; j++)
      {
         if (board[j][i] != '.' && board[j][i] != playerChar)
            count = -2;
         if (board[j][i] == playerChar)
            count++;
         if (board[j][i] == '.')
         {
            row = j;
            col = i;
         }
      }
      if (count == 2)
         return true;
      count = 0;
   }

   //diagonal left to right
   for (int i = 0; i < 3; i++)
   {
      if (board[i][i] != '.' && board[i][i] != playerChar)
            count = -2;
      if (board[i][i] == playerChar)
         count++;
      if (board[i][i] == '.')
            row = col = i;
   }
   if (count == 2)
      return true;
   count = 0;

   //diagonal right to left
   for (int i = 2; i >= 0; i--)
   {
      if (board[3 - (i + 1)][i] != '.' && board[3 - (i + 1)][i] != playerChar)
         count = -2;
      if (board[3 - (i + 1)][i] == playerChar)
         count++;
      if (board[3 - (i + 1)][i] == '.')
      {
         row = 3 - (i + 1);
         col = i;
      }
   }
   if (count == 2)
      return true;


   return false;
}

/**********************************************************
* Check if player only has one token and owns middle
***********************************************************/
bool playerHasMiddle(char board[][3], char playerChar)
{
   int count = 0;
   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
         if (count > 1)
            return false;
         if (board[i][j] == playerChar)
            count++;
      }

   //if we got to this piece then we check if owns middle
   if (board[1][1] == playerChar)
      return true;

   return false;
}

/**********************************************************
* Unbeat-toe analyzes the board by creating a queue
***********************************************************/
void analyzeBoard(char board[][3], char playerChar, std::queue<char> &scenario)
{

   //check for corners
   for(int i = 0; i < 3; i = i + 2)
      for (int j = 0; j < 3; j = j + 2)
         if (board[i][j] == playerChar)
            scenario.push('C');

   //check for sides
   float j = .5;
   for (int i = 1; j < 2.5; i = (i + 1) % 3, j += .5)
      if (board[i][(int)j] == playerChar)
         scenario.push('S');
}

/**********************************************************
* Finds coordinates according to given scenario
***********************************************************/
void handleScenario(char board[][3], bool first, std::queue<char> scenario, int &row, int &col)
{
   char playerChar;
   char comChar;
   std::string cNario = "";

   if (first)
   {
      playerChar = 'X';
      comChar = 'O';
   }
   else
   {
      playerChar = 'O';
      comChar = 'X';
   }

   while (!scenario.empty())
   {
      cNario += scenario.front();
      scenario.pop();
   }

   if (cNario == "1")
   {
      //pick random corner
      srand(time(NULL));
      switch(rand() % 4)
      {
         case 0 :
            row = col = 0;
            break;
         case 1 :
            row = 2;
            col = 0;
            break;
         case 2 :
            row = 0;
            col = 2;
            break;
         default:
            row = col = 2;
      }
      return;
   }

   if (cNario == "1C" || cNario == "1CS")
   {
      //pick opposite corner or any other corner
      //find corner
      for(int i = 0; i < 3; i = i + 2)
         for (int j = 0; j < 3; j = j + 2)
            if (board[i][j] == comChar)
            {
               //locate opposite corner
               if (board[(i + 2) % 4][(j + 2) % 4] == '.')
               {
                  row = (i + 2) % 4;
                  col = (j + 2) % 4;
                  return;
               }
               //locate other corner
               else if (board[i][(j + 2) % 4] == '.')
               {
                  row = i;
                  col = (j + 2) % 4;
                  return;
               }
               //locate other corner
               else
               {
                  row = (i + 2) % 4;
                  col = j;
                  return;
               }
            }
   }
   if (cNario == "1S")
   {
      //pick parrallel corner with no playTokens (not opposite corner)
      //find corner
      for(int i = 0; i < 3; i = i + 2)
         for (int j = 0; j < 3; j = j + 2)
            if (board[i][j] == comChar)
            {
               if (board[1][j] != playerChar)
               {
                  row = (i + 2) % 4;
                  col = j;
                  return;
               }
               else
               {
                  row = i;
                  col = (j + 2) % 4;
                  return;
               }
            }
   }
   if (cNario == "2C" || cNario == "2S"  || cNario == "1SS")
   {
      //pick middle
      row = col = 1;
      return;
   }
   if (cNario == "2CC")
   {
      //pick any side
      row = 1;
      col = 0;
      return;
   }
   if (cNario == "2SS")
   {
      //pick corner adjacent to any playerChar
      //check for sides
      float j = .5;
      int jj;
      for (int i = 1; j < 2.5; i = (i + 1) % 3, j += .5)
         if (board[i][(int)j] == playerChar)
         {
            jj = (int)j;
            //check above
            if (i == 1 && board[i - 1][jj] == '.')
            {
               row = i - 1;
               col = jj;
               return;
            }
            //check right
            if (jj < 2 && board[i][jj + 1] == '.')
            {
               row = i;
               col = jj + 1;
               return;
            }
            //check below
            if (i == 1 && board[i + 1][jj] == '.')
            {
               row = i + 1;
               col = jj;
               return;
            }
            //check left
            if (jj > 0 && board[i][jj - 1] == '.')
            {
               row = i;
               col = jj - 1;
               return;
            }
         }
   }
   if (cNario == "2CS")
   {
      //pick corner adjacent to side and parrallel to corner
      //check for corners
      int cRow;
      int cCol;
      for(int i = 0; i < 3; i = i + 2)
         for (int j = 0; j < 3; j = j + 2)
            if (board[i][j] == playerChar)
            {
               cRow = i;
               cCol = j;
            }
      //check for sides
      int sRow;
      int sCol;
      float j = .5;
      for (int i = 1; j < 2.5; i = (i + 1) % 3, j += .5)
         if (board[i][(int)j] == playerChar)
         {
            sRow = i;
            sCol = (int)j;
         }

      if (sRow == 1)
      {
         row = cRow;
         col = sCol;
         return;
      }
      else
      {
         row = sRow;
         col = cCol;
         return;
      }
   }

   //ELSE find available spot
   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
         if (board[i][j] == '.')
         {
            row = i;
            col = j;
         }
}

/**********************************************************
* Check if coordinate inputted is valid or not
***********************************************************/
bool isValid(char board[][3], std::string coord)
{

   if (coord.length() != 2)
   {
      std::cout << "Invalid coordinates\n";
      return false;
   }

   if (isalpha(coord[0]) && isdigit(coord[1]))
   {
      if (toupper(coord[0]) < 65 || toupper(coord[0]) > 68)
      {
         std::cout << "Invalid coordinates\n";
         return false;
      }
      if (coord[1] < 49 || coord[1] > 51)
      {
         std::cout << "Invalid coordinates\n";
         return false;
      }

      if (board[(int)coord[1] - 49][(int)toupper(coord[0]) - 65] == 'X' ||
          board[(int)coord[1] - 49][(int)toupper(coord[0]) - 65] == 'O')
      {
         std::cout << "Invalid coordinates: Already filled\n";
         return false;
      }

      return true;
   }

   if (isalpha(coord[1]) && isdigit(coord[0]))
   {
      if (toupper(coord[1]) < 65 || toupper(coord[1]) > 68)
      {
         std::cout << "Invalid coordinates\n";
         return false;
      }
      if (coord[0] < 49 || coord[0] > 51)
      {
         std::cout << "Invalid coordinates\n";
         return false;
      }

      if (board[(int)coord[0] - 49][(int)toupper(coord[1]) - 65] != '.')
      {
         std::cout << "Invalid coordinates: Already filled\n";
         return false;
      }

      return true;
   }

   std::cout << "Invalid coordinates\n";
   return false;
}

/**********************************************************
* Check if the game is over or not
***********************************************************/
bool gameover(char board[][3], bool first)
{
   char comChar;
   char playerChar;
   int cCount = 0;
   int pCount = 0;
   int dCount = 0;

   if (first)
   {
      playerChar = 'X';
      comChar = 'O';
   }
   else
   {
      playerChar = 'O';
      comChar = 'X';
   }

   //hor
   for (int i = 0; i < 3; i++)
   {
      for (int j = 0; j < 3; j++)
      {
         if (board[i][j] == comChar)
            cCount++;
         else if (board[i][j] == playerChar)
            pCount++;
         else
            dCount++;
      }
      if (cCount == 3)
      {
         comWon();
         return true;
      }
      if (pCount == 3)
      {
         playerWon();
         return true;
      }

      cCount = 0;
      pCount = 0;
   }

   if (dCount == 0)
   {
      catsGame();
      return true;
   }

   //ver
   for (int i = 0; i < 3; i++)
   {
      for (int j = 0; j < 3; j++)
      {
         if (board[j][i] == comChar)
            cCount++;
         else if (board[j][i] == playerChar)
            pCount++;
         else
            break;
      }
      if (cCount == 3)
      {
         comWon();
         return true;
      }
      if (pCount == 3)
      {
         playerWon();
         return true;
      }

      cCount = 0;
      pCount = 0;
   }

   //diag1
   for (int i = 0; i < 3; i++)
   {
      if (board[i][i] ==  comChar)
         cCount++;
      else if (board[i][i] == playerChar)
         pCount++;
      else
         break;
   }

   if (cCount == 3)
   {
      comWon();
      return true;
   }
   if (pCount == 3)
   {
      playerWon();
      return true;
   }

   cCount = 0;
   pCount = 0;

   //diag2
   for (int i = 3 - 1; i >= 0; i--)
   {
      if (board[3 - (i + 1)][i] ==  comChar)
         cCount++;
      else if (board[3 - (i + 1)][i] == playerChar)
         pCount++;
      else
         break;
   }

   if (cCount == 3)
   {
      comWon();
      return true;
   }
   if (pCount == 3)
   {
      playerWon();
      return true;
   }

   return false;
}

/**********************************************************
* Display that Unbeat-toe won
***********************************************************/
void comWon()
{
   std::cout << "\nHaha! I won!\n";
   std::cout << "Better start practicing noob!\n\n";
}

/**********************************************************
* This function should never be called...
***********************************************************/
void playerWon()
{
   std::cout << "\nWARNING: CHEATER DETECTED\n";
   std::cout << "(aka that's you...)\n\n";
}

/**********************************************************
* Display that its a cat's game
***********************************************************/
void catsGame()
{
   std::cout << "\nIt's a cat's game!\n";
   std::cout << "See? I told you that you couldn't beat me!\n\n";
}