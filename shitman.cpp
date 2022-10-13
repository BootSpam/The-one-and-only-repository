#include <iostream>
#include <vector>
#include <algorithm>    //For shuffling deck
#include <random>

#include "player.h"

using namespace std;

//TODO fix p2 tries to find last played instead of correct card
struct Deck {

    //General
    int deck[52];
    int index;
    int last_played;

    //Player one
    int p1_hidden[3];
    int p1_open[3];
    vector<int> p1_hand = {0, 0, 0,};
    vector<int> p1_playable;

    //Player two
    int p2_hidden[3];
    int p2_open[3];
    vector<int> p2_hand = {0, 0, 0,};
    vector<int> p2_playable;
    
    //Functions
    Deck() {
        for(int i = 0; i < 52; i++) {
            deck[i] = i % 13 + 1;
        }
    }
    
    void blanda() {
        shuffle(begin(this->deck), end(this->deck), mt19937{random_device{}()});
        index = 0;
        last_played = 0;    //Represents no card
    }

    int draw() {
        if (index > 51) {
            blanda();
            cout << "Error: Reshuffled" << endl; //Not supposed to happen
        }

        index++;
        return deck[index - 1];
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
    bool legal_move(int card) {
        if (card > last_played) {return true;}
        else if (card == 1 || card == 2 || card == 5 || card == 10) {return true;}
        else {return false;}
    }
    void set_playable(int player) {
        if (player == 1) {
            p1_playable.clear();
            for(int i = 0; i < p1_hand.size(); i++) {
                if (legal_move(p1_hand[i])) {
                    p1_playable.push_back(p1_hand[i]);
                }
            }
        }
        else if (player == 2) {
            p2_playable.clear();
            for(int i = 0; i < p2_hand.size(); i++) {
                if (legal_move(p2_hand[i])) {
                    p2_playable.push_back(p2_hand[i]);
                }
            }
        }
    }

    int find_card_in_hand(int player, int card) {
        cout << "Debug: card to find = " << card << endl;
        if (player == 1) {
            for (int i = 0; i < p1_hand.size(); i++) {
                if (p1_hand[i] == card) {
                    cout << "Debug: card index = " << i << endl;
                    return i;
                }
            }
        } else if (player == 2) {
            for (int i = 0; i < p2_hand.size(); i++) {
                if (p2_hand[i] == card) {
                    cout << "Debug: card index = " << i << endl;
                    return i;
                }
            }
        }
        cout << "Error: Invalid player or card" << endl;
        return 0;  //Not supposed to happen
    }

    void lay_card(int card) {
        cout << "Debug: card layed = " << card << endl;
        switch (card) {
            case 1:
                last_played = 14;
                break;
            case 5:
                break;
            case 10:
                last_played = 10;
                break;
            default:
                last_played = card;
        }
    }

    void play(int player, int card_number) {
        if (player == 1) {
            if (legal_move(p1_hand[card_number])) {
                lay_card(p1_hand[card_number]);
                p1_hand[card_number] = draw();
            }
        } else if (player == 2) {
            if (legal_move(p2_hand[card_number])) {
                lay_card(p2_hand[card_number]);
                p2_hand[card_number] = draw();
            }
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
        d.set_playable(1);
        d.play(1, 
            d.find_card_in_hand(1, 
            d.p1_playable[
            p1.do_turn(
                d.p1_hand,
                d.p1_playable, 
                d.p1_open, 
                d.p2_open, 
                d.last_played)]));

        d.set_playable(2);
        d.play(2, 
            d.find_card_in_hand(2, 
            d.p1_playable[
            p2.do_turn(
                d.p2_hand,
                d.p2_playable, 
                d.p2_open, 
                d.p1_open, 
                d.last_played)]));
    }

    return 0;
}
