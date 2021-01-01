#include <stdio.h>
#include <stdlib.h>
//
// Declaring all variables/functions that will be included in the game,
// such as the die faces, names of players, finding players to the left
// or right, and player bank array.
//
typedef enum faciem { LEFT, RIGHT, CENTER, PASS } faces;
faces die[] = { LEFT, RIGHT, CENTER, PASS, PASS, PASS };

const char *names[] = { "Happy", "Sleepy", "Sneezy", "Dopey", "Bashful",
  "Grumpy", "Doc", "Mirror Mirror", "Snow White", "Wicked Queen" };

unsigned int left(unsigned int position, unsigned int players);
unsigned int right(unsigned int position, unsigned int players);

int player_money[] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

int main() {

  int seed;
  int n_players = 0;
  int position = 0;
  int pot = 0;
  int roll;
  int n_rolls;
  int n_players_with_money;
  int winner_position;
  //
  // Prompts user to input a number to set it as a seed for
  // dice rolls.
  //
  printf("Random seed: ");
  scanf("%d", &seed);
  srand(seed);
  //
  // Prompts users on how many players there will be; if there is less than 2
  // or more than 10, will tell user invalid input and to try again.
  //
  while (1) {
    printf("How many players? ");
    scanf("%d", &n_players);
    if (n_players > 1 && n_players < 11) {
      n_players_with_money = n_players;
      break;
    }
    printf("Incorrect number of players.\n");
  }
  //
  // While the game has more than one player with money still, game will
  // continue to loop until otherwise.
  //
  // Checks current player's bank to see how many rolls they will have.
  //
  while (n_players_with_money > 1) {
    if (player_money[position] < 3) {
      n_rolls = player_money[position];
    } else {
      n_rolls = 3;
    }
    //
    // For each turn, player will roll a die and give $1 to their left, right,
    // to the pot, or pass, depending on their outcome.
    //
    for (int turn = 0; turn < n_rolls; ++turn) {
      if (turn == 0) {
        printf("%s rolls... ", names[position]);
      }
      roll = die[rand() % 6];
      if (roll == 0) {
        printf("gives $1 to %s", names[left(position, n_players)]);
        --player_money[position];
        ++player_money[left(position, n_players)];
      } else if (roll == 1) {
        printf("gives $1 to %s", names[right(position, n_players)]);
        --player_money[position];
        ++player_money[right(position, n_players)];
      } else if (roll == 2) {
        printf("puts $1 in the pot");
        --player_money[position];
        ++pot;
      } else {
        printf("gets a pass");
      }
      //
      // Will add a space between each turn, otherwise will start a newline.
      //
      if ((turn + 1) < n_rolls) {
        printf(" ");
      } else {
        printf("\n");
      }
    }
    //
    // Moves current game position to player to the right.
    //
    position = right(position, n_players);
    //
    // Sets number of players with money to 0, then checks each players'
    // bank to see if they still have money to check while loop
    // condition.
    //
    n_players_with_money = 0;
    for (int i = 0; i < n_players; ++i) {
      if (player_money[i] > 0) {
        ++n_players_with_money;
      }
    }
  }
  //
  // When while loop is ended and game is over, checks for player with
  // money still and finds their position as the winner.
  //
  for (int winner = 0; winner < n_players; ++winner) {
    if (player_money[winner] > 0) {
      winner_position = winner;
    }
  }
  //
  // Prints the name of the winner and how much they get in the pot with
  // how much they had left in the bank.
  //
  printf("%s wins the $%d pot with $%d left in the bank!\n",
      names[winner_position], pot, player_money[winner_position]);
  return 0;
}
//
// Function to get position of player to the left.
//
unsigned int left(unsigned int position, unsigned int players) {
  return ((position + players - 1) % players);
}
//
// Function to get position of player to the right.
//
unsigned int right(unsigned int position, unsigned int players) {
  return ((position + 1) % players);
}
