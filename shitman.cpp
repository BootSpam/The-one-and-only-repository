#include <iostream>
#include <vector>
#include <algorithm>    //For shuffling deck
#include <random>

#include "player.h"

using namespace std;

struct Deck {

    //General
    int deck[52];
    int index;
    int last_played;
    int player_turn;
    vector<int> played_cards;
    bool is_empty;

    //Player one
    vector<int> p1_hand = {0, 0, 0,};
    vector<int> p1_open = {0, 0, 0,};
    vector<int> p1_hidden = {0, 0, 0,};
    vector<int> p1_playable;

    //Player two
    vector<int> p2_hand = {0, 0, 0,};
    vector<int> p2_open = {0, 0, 0,};
    vector<int> p2_hidden = {0, 0, 0,};
    vector<int> p2_playable;
    
    //Functions
    Deck() {
        for(int i = 0; i < 52; i++) {
            deck[i] = i % 13 + 1;
        }
        player_turn = 1;
        is_empty = false;
    }
    
    void blanda() {
        shuffle(begin(this->deck), end(this->deck), mt19937{random_device{}()});
        index = 0;
        last_played = 0;    //Represents no card
    }

    int draw() {
        if (index >= 51) {
            is_empty = true;
            cout << "Debug: index >= 51, deck is empty after this" << endl;
            /*
            blanda();
            cout << "Error: Reshuffled" << endl; //Not supposed to happen
            */
        }

        cout << "Debug: drawing card index " << index << endl;
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
        if (card >= last_played) {return true;}
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
                last_played = 0;
                played_cards.clear();
                break;
            default:
                last_played = card;
        }

        //Add card to played pile
        played_cards.push_back(card);


        //Switch turn
        if (card != 2 && card != 10) {
            switch (player_turn) {
                case 1:
                    player_turn = 2;
                    break;
                case 2:
                    player_turn = 1;
            }
        }
    }

    void play(int player, int card_number) {
        if (player == 1) {
            if (legal_move(p1_hand[card_number])) {
                lay_card(p1_hand[card_number]);
                
                if (p1_hand.size() > 3 || is_empty) {
                    p1_hand.erase(p1_hand.begin() + card_number);
                } else {
                    p1_hand[card_number] = draw();
                }
            }
        } else if (player == 2) {
            if (legal_move(p2_hand[card_number])) {
                lay_card(p2_hand[card_number]);

                if (p2_hand.size() > 3 || is_empty) {
                    p2_hand.erase(p2_hand.begin() + card_number);
                } else {
                    p2_hand[card_number] = draw();
                }
            }
        } else {
            cout << "Invalid player" << endl;
        }
    }

    bool can_play(int player) {
        if (player == 1 && p1_playable.size() == 0) {
            cout << "Debug: Can not play" << endl;
            return false;
        } else if (player == 2 && p2_playable.size() == 0) {
            cout << "Debug: Can not play" << endl;
            return false;
        } else {
            cout << "Debug: Can play" << endl;
            return true;
        }
    }
    
    void pick_up_pile(int player) {
        cout << "Debug: Picking up pile" << endl;
        if (player == 1) {
            for (int i = 0; i < played_cards.size(); i++) {
                p1_hand.push_back(played_cards[i]);
            }
        } else if (player == 2) {
            for (int i = 0; i < played_cards.size(); i++) {
                p2_hand.push_back(played_cards[i]);
            }
        }
        played_cards.clear();
        last_played = 0;

        //Switch turn
        switch (player_turn) {
            case 1:
                player_turn = 2;
                break;
            case 2:
                player_turn = 1;
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
        if (d.player_turn == 1) {

            if (d.is_empty && d.p1_hand.size() == 0){
                cout << "P1 late game" << endl;
            }

            d.set_playable(1);
            if (d.can_play(1)) {
                d.play(1, 
                    d.find_card_in_hand(1, 
                    d.p1_playable[
                    p1.do_turn(
                        d.p1_hand,
                        d.p1_playable, 
                        d.played_cards,
			            d.p1_open, 
                        d.p2_open, 
                        d.last_played)]));
            } else {
                d.pick_up_pile(1);
            }
        } else if (d.player_turn == 2) {

            if (d.is_empty && d.p2_hand.size() == 0){
                cout << "P2 late game" << endl;
            }

            d.set_playable(2);
            if (d.can_play(2)) {
                d.play(2, 
                    d.find_card_in_hand(2, 
                    d.p2_playable[
                    p2.do_turn(
                        d.p2_hand,
                        d.p2_playable, 
                        d.played_cards,
			            d.p2_open, 
                        d.p1_open, 
                        d.last_played)]));
            } else {
                d.pick_up_pile(2);
            }
        } else {
            cout << "Error: player_turn = " << d.player_turn << endl;
        }
    }

    return 0;
}
