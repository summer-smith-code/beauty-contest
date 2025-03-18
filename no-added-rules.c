#include <stdio.h>
#include <string.h>
#include <math.h>

#define LOSE (-5)

void introduction();
// Initialize all players' scores
void initializeScore(int score[]);
// Play rounds of beauty contest until a score reaches -10
void playGame();
// update scores
void updateScores(int score[], int count, int closest);
void resetChoice(int playerChoice[], int count);

int main(void) {
    introduction();
    playGame();
    return 0;
}

void introduction() {
    printf("Welcome to Beauty Contest.\n");
    printf("All players will begin with a score of 0.\n");
    printf("All players will select a number between 0 and 100.\n");
    printf("The average of the values will be multiplied by 0.8\n");
    printf("The person who chooses the number closest to the calculated number wins.\n");
    printf("Everyone else will have -1 added to their scores.");
    printf("This constitutes one round.\n");
    printf("A player is eliminated once their score reaches %d\n", LOSE);
}

void initializeScore(int score[]) {
    for (int i=0; i<5; i++) {
        score[i] = 0;
    }
}

void playGame() {
    // initialize players, scores, and round number
    int players = 5;
    int playerCounter = 5;
    int playerScore[players];
    initializeScore(playerScore);
    int roundNum = 1;

    // Initialize each player

    while (playerCounter > 1) {
        double average = 0.0;
        int closestToAvg = -100;
        int closestPlayer = 0;
        int playerChoice[5];
        printf("\nIt is round %d.\n", roundNum);
        for (int i=0; i<players; i++) {
            // Check to see if player has not lost yet
            if (playerScore[i] > LOSE) {
                printf("Enter a number 0-100, player %d.\n", i+1);
                scanf("%d", &playerChoice[i]);
                // Check to see if number is valid
                while (playerChoice[i] < 0 || playerChoice[i] > 100) {
                    printf("Invalid choice. Please select a number 0-100\n");
                    scanf("%d", &playerChoice[i]);
                }
            }
            average += playerChoice[i];
        }
        // Average according to rules
        average /= players;
        average *= 0.8;
        printf("The average was %.2lf\n", average);
        for (int i=0; i<players; i++) {
            // Find player whose number was closest to the average
            if (fabs(playerChoice[i] - average) < fabs(closestToAvg - average)) {
                closestToAvg = playerChoice[i];
                closestPlayer = i;
            }
        }
        printf("The closest number was %d.\n", closestToAvg);
        printf("Everyone aside from player %d will lose one point.\n", closestPlayer+1);
        // ensure player choices are accurate to each round, lost players'
        // past choices don't interfere
        resetChoice(playerChoice, players);
        updateScores(playerScore, players, closestPlayer);
        // Check scores to see if any are -10
        for (int i=0; i<players; i++) {
            if (playerScore[i] == LOSE) {
                printf("Player %d has lost.\n", i+1);
                playerCounter -= 1;
                playerScore[i] -= 1;
            }
        }
        roundNum += 1;
    }
    // Outside of while loop, only one player left.
    for (int i=0; i<players; i++) {
        if (playerScore[i] > -5) {
            printf("\nPlayer %d has won!\n", i+1);
        }
    }
}

void updateScores(int score[], int count, int closest) {
    printf("\nUpdated scores:\n");
    for (int i = 0; i<count; i++) {
        if (i != closest) {
            score[i] = score[i]-1;
        }
        if (score[i] >= -5) {
            printf("Player %d: %d\n", i+1, score[i]);
        }
    }
}

void resetChoice(int playerChoice[], int count) {
    for (int i=0; i<count; i++) {
        playerChoice[i] = 101;
    }
}