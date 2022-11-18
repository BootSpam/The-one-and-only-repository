/*
TODO
Specialkort
-Early Game
   -Byta öppna kort
Main loop
    Hantera turer
    Dra kort efter tur
    Passa info till do_turn funktion i player.cpp 
        Anpassa info som går ut till player
        Ta emot info från player
    kolla 4 in a row
    kolla om man måste ta upp
    Hantera endgame
        Byta mellan öppna och gömda kort
        spela gömda kort automatiskt
        Kolla vinst
            Inte gå ut på ess eller specialkort
       -Infinite loop failsafe
Byta namn på en Player class
*/
// FATAL ERROR NEEDS A BUG FIX
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

int random(int min, int max) {
  static bool first = true;
  if (first) {
    srand(time(NULL)); // Seeding for the first time only!
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

void removeIndexFromVector(vector<int> vector, int indexValue) {
  auto iterBegin = std::remove(vector.begin(), vector.end(), indexValue);
  vector.erase(iterBegin, vector.end());
}

int determineCardValue(int card) {
  if (card % 13 == 0) {
    return 13;
  } else if (card % 13 == 1) {
    return 14;
  }

  return card % 13;
}

int whoIsPlaying(int playedTurns, int playerCount) {
  if (playedTurns % playerCount == 0) {
    return 0;
  }
  return (playedTurns % playerCount);
}

bool checkFor4(vector<int> cardList) {
  vector<int> fourLastCards;
  for (int i = cardList.size() - 4; i < cardList.size(); i++) {
    fourLastCards.push_back(determineCardValue(cardList[i]));
  }
  if (fourLastCards.size() == 4) {
    return true;
  }
  return false;
}

int tbdWhoPlays(vector<int> allCardsInHands) {
  sort(allCardsInHands.begin(), allCardsInHands.end());
  for (int i = 0; i < allCardsInHands.size(); i++) {
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
  bool isPlayerOut = false;

  bool initialized = false;

  void init(int array[52], int index, int pIndex, int lastPlayed) {
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
      if (checkLegalMove(determineCardValue(this->hand[i]), lastPlayed) ==
          true) {
        this->playableCards.push_back(this->hand[i]);
      }
    }
  }

  void pickUpPile(vector<int> pile) {
    for (int i = 0; i < pile.size(); i++) {
      hand.push_back(pile[i]);
    }
  }

  void layCard(int card) {
    for (int i = 0; i < playableCards.size(); i++) {
      if (determineCardValue(playableCards[i]) == card) {
        cout << "Card is now on table";
        int cardToRemove = playableCards[i];
        removeIndexFromVector(this->hand, cardToRemove);
      }
    }
  }

  bool canPlay() {
    if (this->playableCards.size() == 0) {
      return false;
    }

    return true;
  };
};

class Table {
public:
  vector<int> cardsOut;
  vector<int> pile;
  int lastPlayed = 0;

  void layCard(int card) {
    this->pile.push_back(determineCardValue(card));
    lastPlayed = pile.back();
  }

  void clearThePile() {
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
  int playerCount;
  int startPlayerIndex;
  vector<int> whoToPlay;
  bool isGameInitialized = false;
  int playingPlayer;

  cout << "Please Enter the amount of player, maximum = 4, :";
  cin >> playerCount;
  // This part is for initializing the players
  Player players[4];
  for (int i = 0; i < playerCount; i++) {
    players[i].init(deck.deck, deck.index, i, table.lastPlayed);
    deck.index = deck.index + 9;
  }

  for (int i = 0; i < playerCount; i++) {
    for (int j = 0; j < 3; j++) {
      whoToPlay.push_back(players[i].playableCards[j]);
      cout << endl << players[i].playableCards[j];
    }
  }

  int playingCard = tbdWhoPlays(whoToPlay);
  cout << endl << "Playing card is :    " << playingCard;

  while (true) {
    if (isGameInitialized == false) {
      for (int i = 0; i < playerCount; i++) {
        if (findCardIndex(players[i].playableCards, playingCard) == 100) {
          continue; // Means card is not found in given players hand
        } else {
          playingPlayer = i;
          cout << endl << playingPlayer;
          break;
        }
      }
      break;
    }
  }
}
