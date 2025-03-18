#include <stdio.h>
#include <math.h>

#define LOSE (-5)
// No longer playing/has lost
#define INACTIVE (-10)
// Invalid choice for the round
#define INVALID (-100)
#define ACTIVE 1
#define TOTALPLAYERS 5

void introduction();
// implement additional rules once requirements are met
void ruleOne();
void ruleTwo();
void ruleThree();
// Initialize all players' scores
void initializeScore(int score[]);
// Play rounds of beauty contest until a score reaches -10
void playGame();
// update scores
void updateScores(int score[], int count, int closest, int penalty);
void resetChoice(int playerChoice[], int count);
void checkRuleOne(int playerChoice[], int count);
int checkRuleTwo(double average, int closest);
void checkRuleThree(int playerChoice, int count);
void checkActive(int score[], int count);

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
    printf("A new rule is introduced for every player eliminated.\n");
}

void ruleOne() {
    printf("Here are the added rules:\n");
    printf("RULE 1. If 2 people or more choose the same number, "
           "the number they chose becomes invalid,\nmeaning they will lose a point"
           " even if the number is closest to 4/5ths the average.\n\n");
}

void ruleTwo() {
    printf("RULE 2. If there is a person who chooses the exact correct number, "
           "the loser penalty is doubled.\n\n");
}

void ruleThree() {
    printf("RULE 3. If someone chooses 0, the player who chooses 100 is the winner of the round.\n\n");
}

void initializeScore(int score[]) {
    for (int i=0; i<5; i++) {
        score[i] = 0;
    }
}

void playGame() {
    // initialize players, scores, and round number
    int playerCounter = 5;
    int playerScore[TOTALPLAYERS];
    initializeScore(playerScore);
    int roundNum = 1;

    // initialize rules
    int rule1 = INACTIVE;
    int rule2 = INACTIVE;
    int rule3 = INACTIVE;

    // Initialize each player
    int playerChoice[5];

    while (playerCounter > 1) {
        resetChoice(playerChoice, TOTALPLAYERS);
        double average = 0.0;
        int closestToAvg = -100;
        int closestPlayer = 0;
        // penalty for losing a round
        int penalty = -1;

        printf("\nIt is round %d.\n", roundNum);
        // print rules if active per each round
        if (rule1 == ACTIVE) ruleOne();
        if (rule2 == ACTIVE) ruleTwo();
        if (rule3 == ACTIVE) ruleThree();
        for (int i=0; i<TOTALPLAYERS; i++) {
            // Check to see if player has not lost yet
            if (playerScore[i] > LOSE) {
                printf("Enter a number 0-100, player %d.\n", i+1);
                scanf("%d", &playerChoice[i]);
                // Check to see if number is valid
                while (playerChoice[i] < 0 || playerChoice[i] > 100) {
                    printf("Invalid choice. Please select a number 0-100\n");
                    scanf("%d", &playerChoice[i]);
                }
                average += playerChoice[i];
            }
        }
        // Average according to rules
        average /= playerCounter;
        average *= 0.8;
        // Check to see if rule 1 and rule 3 come into play before closest is decided
        if (rule1 == ACTIVE) checkRuleOne(playerChoice, TOTALPLAYERS);
        if (rule3 == ACTIVE) checkRuleThree(playerChoice, TOTALPLAYERS);

        printf("The average was %.2lf\n", average);
        for (int i=0; i<TOTALPLAYERS; i++) {
            if (playerChoice[i] != INACTIVE) {
                // Find player whose number was closest to the average
                if (fabs(playerChoice[i] - average) < fabs(closestToAvg - average)) {
                    closestToAvg = playerChoice[i];
                    closestPlayer = i;
                }
            }
        }

        // Check if rule 2 comes into play before penalty is added
        if (rule2 == ACTIVE) penalty = checkRuleTwo(average, closestToAvg);
        printf("The closest number was %d.\n", closestToAvg);
        if (penalty == -1) {
            printf("Everyone aside from player %d will lose %d point.\n", closestPlayer+1, penalty);
        } else {
            printf("Everyone aside from player %d will lose %d points.\n", closestPlayer+1, penalty);
        }
        // ensure player choices are accurate to each round, lost players'
        // past choices don't interfere
        updateScores(playerScore, TOTALPLAYERS, closestPlayer, penalty);
        for (int i=0; i<TOTALPLAYERS; i++) {
            if (playerScore[i] == LOSE) {
                printf("Player %d has lost.\n", i+1);
                playerCounter -= 1;
                playerScore[i] = INACTIVE;
                playerChoice[i] = INACTIVE;
                if (playerCounter == 4) {
                    rule1 = ACTIVE;
                }
                if (playerCounter == 3) {
                    rule2 = ACTIVE;
                }
                if (playerCounter == 2) {
                    rule3 = ACTIVE;
                }
            }
        }
        roundNum += 1;
    }
        // Outside of while loop, only one player left.
        for (int i=0; i<TOTALPLAYERS; i++) {
            if (playerScore[i] > LOSE) {
                printf("\nPlayer %d has won!\n", i+1);
            }
        }
}

void updateScores(int score[], int count, int closest, int penalty) {
    printf("\nUpdated scores:\n");
    for (int i = 0; i<count; i++) {
        if (i != closest) {
            score[i] = score[i]+penalty;
        }
        // Check scores to see who has just lost.
        checkActive(score, TOTALPLAYERS);
        if (score[i] >= LOSE) {
            printf("Player %d: %d\n", i+1, score[i]);
        }
    }
}

void resetChoice(int playerChoice[], int count) {
    for (int i=0; i<count; i++) {
        if (playerChoice[i] != INACTIVE) {
            playerChoice[i] = INVALID;
        }
    }
}

void checkRuleOne(int playerChoice[], int count) {
    // placeholder in case of more than 2 choosing the same.
    int placeholder = INVALID;
    for (int i = 0; i<count; i++) {
        for (int j = 0; j<count; j++) {
            if (playerChoice[i] == playerChoice[j] && i != j && playerChoice[i] != INACTIVE && playerChoice[j] != INACTIVE
                && playerChoice[i] != INVALID && playerChoice[j] != INVALID) {
                placeholder = playerChoice[i];
                playerChoice[i] = INVALID;
                playerChoice[j] = INVALID;
            }
            if (placeholder == playerChoice[i]) playerChoice[i] = INVALID;
            if (placeholder == playerChoice[j]) playerChoice[j] = INVALID;
        }
    }
    for (int i = 0; i<count; i++) {
        if (playerChoice[i] == INVALID) {
            printf("Player %d chose the same number as someone else. Their choice is now invalid.\n", i+1);
        }
    }

}
int checkRuleTwo(double average, int closest) {
    int penalty = -1;
    int roundedAvg = round(average);
    if (closest == roundedAvg) {
        printf("A player has chosen the exact number. The penalty will be -2 for this round.\n");
        penalty = -2;
    }
    return penalty;
}
void checkRuleThree(int playerChoice, int count) {
    // Implement
}

//Check if all players are active
// If player's score < -4, they are inactive
// Useful for when rule 2 applies, and -2 brings -4 score to -6.
void checkActive(int score[], int count) {
    for (int i = 0; i<count; i++) {
        if (score[i] < LOSE && score[i] > INACTIVE) {
            score[i] = LOSE;
        }
    }
}
