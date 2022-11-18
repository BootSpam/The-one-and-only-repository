#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <ctime>

using namespace std;

int random(int min, int max) {
    static bool first = true;
    if (first) {
        srand(time(NULL)); //Seeding for the first time only!
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

void shuffle(int array[], int length) {
    int temp = 0;
    int randomIndex;
    for (int i = 0; i < length; i++) {
        randomIndex = random(0, length - 1);
        temp = array[i];
        array[i] = array[randomIndex];
        array[randomIndex] = temp;
    }
}
int tbdWhoPlays (vector<int> allCardsInHands){
    sort(allCardsInHands.begin(), allCardsInHands.end());
    for (int i = 0; i < allCardsInHands.size(); i++){
        if (allCardsInHands[i] == 5) {
            return 5;
        }
    }
    return allCardsInHands[0];
}
int findCardIndex(vector<int> vector, int card) {
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i] == card) {
            return i;
        }
    }
    return 100; // Means no card is found in the given vector
}

class Deck {
public:
    int deck[52];
    int index = 0;

    bool isEmpty;

    void init() {
        for (int i = 0; i < 52; i++) {
            deck[i] = i + 1;
        }
        shuffle(deck, 52);
        isEmpty = false;
        index = 0;
    }


    int draw() {
        if (index >= 52) {
            isEmpty = true;
            return 0;
        }

        int card;
        card = deck[index];
        index++;
        return card;
    }
};

class Player {
public:
    vector<int> hand;
    vector<int> visibleTable;
    vector<int> hiddenTable;
    vector<int> playableCards;
    int playerIndex;

    bool initialized = false;

    void init(int array[52], int index, int pIndex) {
        if (this->initialized == true) {
            return;
            cout << "Debug : Player is already initialized" << endl;
        }

        this->playerIndex = pIndex;

        for (int i = 0; i < 3; i++) {
            this->hand.push_back(array[index]);
            index++;
        }

        for (int i = 0; i < 3; i++) {
            this->visibleTable.push_back(array[index]);
            index++;
        }

        for (int i = 0; i < 3; i++) {
            this->hiddenTable.push_back(array[index]);
            index++;
        }
        cout << endl << "Player initialized" << endl;
    }

    bool checkLegalMove(int card, int lastPlayed) {
        if (card >= lastPlayed) {
            return true;
        } else if (card == 1 || card == 2 || card == 5 || card == 10) {
            return true;
        } else {
            return false;
        }
    }

    void setPlayable(int lastPlayed) {
        this->playableCards.clear();
        sort(this->hand.begin(), this->hand.end());
        for (int i = 0; i < this->hand.size(); i++) {
            if (checkLegalMove(this->hand[i], lastPlayed) == true) {
                this->playableCards.push_back(this->hand[i]);
            }
        }
    }

    void pickUpPile (vector<int> pile) {
        for (int i = 0; i < pile.size(); i++) {
            hand.push_back(pile[i]);
        }
    }

    void layCard (int card) {

    }
};

class Table {
public:
    vector<int> cardsOut;
    vector<int> pile;
    int lastPlayed = 0;

    void layCard(int card) {
        this->pile.push_back(card);
        lastPlayed = pile.back();
    }

    void clearThePile () {
        for (int i = 0; i < pile.size(); i++) {
            cardsOut.push_back(pile[i]);
        }

        pile.clear();
        lastPlayed = 0;
    }

};

int main() {
    Table table;
    Deck deck;
    deck.init();
    int numberOfPlayers;
    int startPlayerIndex;
    vector<int> whoToPlay;

    cout << "Please Enter the amount of player, maximum = 4, :";
    cin >> numberOfPlayers;
    //This part is for initializing the players
    Player players[4];
    for (int i = 0; i < numberOfPlayers; i++) {
        players[i].init(deck.deck, deck.index, i);
        deck.index = deck.index + 9;
    }

    for (int i = 0; i < numberOfPlayers; i++) {
        for (int j = 0; j < 3; j++) {
            whoToPlay.push_back(players[i].hand[j]);
            cout << endl << players[i].hand[j];
        }
    }

    int playingCard = tbdWhoPlays(whoToPlay);
    cout << endl << "Playing card is :    " << playingCard;



    while(true) {
//        for (int i = 0; i < numberOfPlayers; i++) {
//            players[i].setPlayable(table.lastPlayed);
//        }

        break;
    }
}
