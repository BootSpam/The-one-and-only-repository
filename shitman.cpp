#include <iostream>
#include <algorithm>
#include <random>
#include "player.h"

using namespace std;

struct Deck {

    //Draw pile
    int deck[52];
    int index;

    //Player one
    int p1_hidden[3];
    int p1_open[3];
    int p1_hand[3];

    //Player two
    int p2_hidden[3];
    int p2_open[3];
    int p2_hand[3];
    
    //Elementary functions
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

    void deal() {
        
        p1_hidden[0] = draw();  p2_hidden[0] = draw();
        p1_hidden[1] = draw();  p2_hidden[1] = draw();
        p1_hidden[2] = draw();  p2_hidden[2] = draw();

        p1_open[0] = draw();    p2_open[0] = draw();
        p1_open[1] = draw();    p2_open[1] = draw();
        p1_open[2] = draw();    p2_open[2] = draw();

        p1_hand[0] = draw();    p2_hand[0] = draw();
        p1_hand[1] = draw();    p2_hand[1] = draw();
        p1_hand[2] = draw();    p2_hand[2] = draw();

    }

};

int main() {
    
    Deck d;
    d.blanda();
    d.deal();
    
    cout << "Deck ";
    for(int i = 0; i < 52; i++) {
        cout << d.deck[i] << " ";
    }
    cout << endl;

    //Player stuff
    int last_played = 0;

    Player p1(1);
    cout << p1.do_turn(d.p1_hand, d.p1_open, d.p2_open, last_played) << endl;
    

    return 0;
}
