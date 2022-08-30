#include <iostream>
#include <algorithm>
#include <random>
#include "player.h"

using namespace std;

struct Deck {

    //General
    int deck[52];
    int index;
    int last_played;

    //Player one
    int p1_hidden[3];
    int p1_open[3];
    int p1_hand[3];

    //Player two
    int p2_hidden[3];
    int p2_open[3];
    int p2_hand[3];
    
    //Functions
    Deck() {
        for(int i = 0; i < 52; i++) {
            deck[i] = i % 13;
        }
    }
    
    void blanda() {
        shuffle(begin(this->deck), end(this->deck), mt19937{random_device{}()});
        index = 0;
        last_played = 2;    //Represents a 3 or nothing
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

    void play(int player, int card_number) {
        if (player == 1) {
            last_played = p1_hand[card_number];
            p1_hand[card_number] = draw();
        } else if (player == 2) {
            last_played = p2_hand[card_number];
            p2_hand[card_number] = draw();
        } else {
            cout << "Invalid player" << endl;
        }
    }

};

int main() {
    
    //Setup game
    Deck d;
    d.blanda();
    d.deal();
    
    //Print deck
    cout << "Deck ";
    for(int i = 0; i < 52; i++) {
        cout << d.deck[i] << " ";
    }
    cout << endl;

    //Player stuff
    Player p1(1);
    Player p2(2);

    while(true) {
        d.play(1, p1.do_turn(d.p1_hand, d.p1_open, d.p2_open, d.last_played));
        d.play(2, p2.do_turn(d.p2_hand, d.p2_open, d.p1_open, d.last_played));
    }

    //cout << p1.do_turn(d.p1_hand, d.p1_open, d.p2_open, b.last_played) << endl;
    

    return 0;
}
