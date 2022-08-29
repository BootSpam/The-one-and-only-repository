#include <iostream>
#include <algorithm>
#include <random>
#include "player.h"

using namespace std;

struct Deck {
    int deck[52];
    int index;
    
    Deck() {
        for(int i = 0; i < 52; i++) {
            deck[i] = i % 13;
        }
    }
    
    void blanda() {
        shuffle(begin(this->deck), end(this->deck), mt19937{random_device{}()});
        index = 0;
    }

    int draw() {
        if (index > 51) {
            blanda();
            cout << "Reshuffled" << endl; //Not supposed to happen
        }

        index++;
        return deck[index -1];
    }


};

int main() {
    
    Deck deck1;
    deck1.blanda();
    
    for(int i = 0; i < 52; i++) {
        cout << deck1.draw() << " ";
    }
    cout << endl;

    //Player stuff
    int hand[3] = {1, 2, 3};
    int open[3] = {4, 5, 6};
    int last_played = 0;

    Player p1(1);
    cout << p1.Do_Turn(hand, open, last_played) << endl;
    

    return 0;
}
