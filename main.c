#include <stdio.h>
#include <string.h>
#include <math.h>

void introduction();
// Implement rule 1
void fourPlayers();
// Implement rule 2
void threePlayers();
// Implement rule 3
void twoPlayers();
// Initialize all players' scores
void initializeScore(int score[]);
// Play rounds of beauty contest until a score reaches -10
int playGame(int playerCount, int playerScore[]);

int main(void) {
    int playerCount = 5;
    int playerScore[5];
    initializeScore(playerScore);
    introduction();
    playerCount = playGame(playerCount, playerScore);
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
    printf("A player is eliminated once their score reaches -10\n");
    printf("A new rule is introduced for every player eliminated.\n");
}

void fourPlayers() {
    printf("\nIf there are 2 people or more choose the same number, "
           "the number they choose becomes invalid, meaning they will lose a point"
           " even if the number is closest to 4/5ths the average\n");
}

void threePlayers() {
    printf("\nIf there is a person who chooses the exact correct number, "
           "the loser penalty is doubled.\n");
}

void twoPlayers() {
    printf("If someone chooses 0, the player who chooses 100 is the winner of the round.\n");
}

void initializeScore(int score[]) {
    for (int i=0; i<5; i++) {
        score[i] = 0;
    }
}

int playGame(int playerCount, int playerScore[]) {
    int roundNum = 1;
    while (playerCount == 5) {
        double average = 0.0;
        int closestToAvg = -100;
        int playerChoice[5];
        printf("\nIt is round %d.\n", roundNum);
        for (int i=0; i<playerCount; i++) {
            printf("Enter a number 0-100, player %d.\n", i+1);
            scanf("%d", &playerChoice[i]);
            while (playerChoice[i] < 0 || playerChoice[i] > 100) {
                printf("Invalid choice. Please select a number 0-100\n");
                scanf("%d", &playerChoice[i]);
            }
            average += playerChoice[i];
        }
        average /= playerCount;
        average *= 0.8;
        printf("The average was %.2lf\n", average);
        for (int i=0; i<playerCount; i++) {
            if (fabs(playerChoice[i] - average) < fabs(closestToAvg - average)) {
                closestToAvg = playerChoice[i];
            }
        }
        printf("The closest number was %d.\n", closestToAvg);
        for (int i=0; i<playerCount; i++) {
            if (playerScore[i] == -10) {
                playerCount -=1;
            }
        }
        roundNum += 1;
    }
    return playerCount;
}
