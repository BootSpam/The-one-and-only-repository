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
    vector<int> p1_hand = {0, 0, 0};
    vector<int> p1_open = {0, 0, 0};
    vector<int> p1_hidden = {0, 0, 0};
    vector<int> p1_playable;
    bool p1_open_mode;

    //Player two
    vector<int> p2_hand = {0, 0, 0};
    vector<int> p2_open = {0, 0, 0};
    vector<int> p2_hidden = {0, 0, 0};
    vector<int> p2_playable;
    bool p2_open_mode;
    
    //Functions
    Deck() {
        for(int i = 0; i < 52; i++) {
            deck[i] = i % 1 + 1;
        }
        player_turn = 1;
        is_empty = false;
        p1_open_mode = p2_open_mode = false;
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
        }

        cout << "Debug: drawing a " << deck[index] << " index " << index << endl;
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

    //Also sorts hand
    void set_playable(int player) {
        if (player == 1) {
            sort(p1_hand.begin(), p1_hand.end());
            p1_playable.clear();
            for(int i = 0; i < p1_hand.size(); i++) {
                if (legal_move(p1_hand[i])) {
                    p1_playable.push_back(p1_hand[i]);
                }
            }
        }
        else if (player == 2) {
            sort(p2_hand.begin(), p2_hand.end());
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
        cout << "Player: " << player << endl;
        cout << "Card: " << card << endl;
        
        return 0;  //Not supposed to happen
    }

    bool lay_card(int card) {
        //Add card to played pile
        played_cards.push_back(card);

        cout << "Debug: card layed = " << card << endl;

        // check for four in a row
        std::vector<int>::reverse_iterator rit = played_cards.rbegin();
        int check_value = *rit;
        int i;
        for (rit = played_cards.rbegin(), i = 0; rit!= played_cards.rend(), i < 4, check_value == *rit; ++rit, ++i) {
            if (i == 3) {
                last_played = 0;
                played_cards.clear();
                return true;
            }
        }
        
        switch (card) {
            case 1:
                last_played = 14;
                return false;
            case 2:
                last_played = 2;
                return true;
            case 5:
                return false;
            case 10:
                last_played = 0;
                played_cards.clear();
                return true;
            default:
                last_played = card;
                return false;
        }
    }

    void play(int player, Play play) {
        int card_number;
        bool play_again = false;
        
        if (player == 1) {
            if (p1_hidden.size() == 0 && play.amount == p1_hand.size() && 
                (play.card_value == 1
                || play.card_value == 2
                || play.card_value == 5
                || play.card_value == 10)) {
                    play.amount --;
            }
            if (legal_move(play.card_value)) {
                for (int i = 0; i < play.amount; i++) {
                    card_number = find_card_in_hand(1, play.card_value);
                    play_again = lay_card(play.card_value);
                    p1_hand.erase(p1_hand.begin() + card_number);
                }
                while(p1_hand.size() < 3 && !is_empty) {
                    p1_hand.push_back(draw());
                }
            }
        } 
        else if (player == 2) {
            if (p2_hidden.size() == 0 && play.amount == p2_hand.size() && 
                (play.card_value == 1
                || play.card_value == 2
                || play.card_value == 5
                || play.card_value == 10)) {
                    play.amount --;
            }
            if (legal_move(play.card_value)) {
                for (int i = 0; i < play.amount; i++) {
                    card_number = find_card_in_hand(2, play.card_value);
                    play_again = lay_card(play.card_value);
                    p2_hand.erase(p2_hand.begin() + card_number);
                }
                while(p2_hand.size() < 3 && !is_empty) {
                    p2_hand.push_back(draw());
                }
            }
        } else {
            cout << "Invalid player" << endl;
        }

        //Switch turn
        if (!play_again) {
            switch (player_turn) {
                case 1:
                    player_turn = 2;
                    break;
                case 2:
                    player_turn = 1;
            }
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
            if (p1_open_mode) {
                cout << "Debug: saving current hand to open" << endl;
                p1_open.clear();
                for (int i = 0; i < p1_hand.size(); i++) {
                    p1_open.push_back(p1_hand[i]);
                }
                p1_hand.clear();    //Must clear hand to ensure no residue open cards
                p1_open_mode = false;
                cout << "Debug: p1_open_mode = false" << endl;
            }
            for (int i = 0; i < played_cards.size(); i++) {
                p1_hand.push_back(played_cards[i]);
            }
        } else if (player == 2) {
            if (p2_open_mode) {
                cout << "Debug: saving current hand to open" << endl;
                p2_open.clear();
                for (int i = 0; i < p2_hand.size(); i++) {
                    p2_open.push_back(p2_hand[i]);
                }
                p2_open_mode = false;
                cout << "Debug: p2_open_mode = false" << endl;
            }
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

        //Update open cards
        if (d.p1_open_mode) {
            cout << "Debug: Updating p1_open" << endl;
            d.p1_open.clear();
            for (int i = 0; i < d.p1_hand.size(); i++) {
                d.p1_open.push_back(d.p1_hand[i]);
            }
        }
        if (d.p2_open_mode) {
            cout << "Debug: Updating p2_open" << endl;
            d.p2_open.clear();
            for (int i = 0; i < d.p2_hand.size(); i++) {
                d.p2_open.push_back(d.p2_hand[i]);
            }
        }

        //Player turns
        cout << endl << "Debug: Player " << d.player_turn << endl;
        if (d.player_turn == 1) {
            if (d.is_empty && d.p1_hand.size() == 0) {
                d.p1_open_mode = true;
                cout << "Debug: P1_open_mode = true" << endl;
                for (int i = 0; i < d.p1_open.size(); i++) {
                    d.p1_hand.push_back(d.p1_open[i]);
                }
            }
            d.set_playable(1);
            if (d.can_play(1)) {
                d.play(1, 
                    p1.do_turn(
                        d.p1_hand,
                        d.p1_playable, 
                        d.played_cards,
			            d.p1_open, 
                        d.p2_open, 
                        d.last_played));
            } else {
                if (d.p1_open_mode && d.p1_hand.size() == 0 && d.p1_open.size() == 0) {
                    //Win, play or pick up
                    if (
                        d.p1_hidden.size() > 1
                        && d.legal_move(d.p1_hidden[d.p1_hidden.size()-1])
                    ) {
                        cout << "Debug: Good hidden card picked" << endl;

                        //Switch turn
                        if (d.p1_hidden[d.p1_hidden.size()-1] != 2 && d.p1_hidden[d.p1_hidden.size()-1] != 10) {
                            d.player_turn = 2;
                        }

                        d.lay_card(d.p1_hidden[d.p1_hidden.size()-1]);
                        d.p1_hidden.erase(d.p1_hidden.begin() + d.p1_hidden.size() - 1);
                    
                    }
                    else if (
                        d.p1_hidden.size() == 1
                        && d.legal_move(d.p1_hidden[d.p1_hidden.size()-1])
                        && d.p1_hidden[d.p1_hidden.size()-1] != 1 
                        && d.p1_hidden[d.p1_hidden.size()-1] != 2 
                        && d.p1_hidden[d.p1_hidden.size()-1] != 5
                        && d.p1_hidden[d.p1_hidden.size()-1] != 10
                    ) {
                        cout << "Debug: Good hidden card picked" << endl;

                        //Switch turn
                        if (d.p1_hidden[d.p1_hidden.size()-1] != 2 && d.p1_hidden[d.p1_hidden.size()-1] != 10) {
                            d.player_turn = 2;
                        }

                        d.lay_card(d.p1_hidden[d.p1_hidden.size()-1]);
                        d.p1_hidden.erase(d.p1_hidden.begin() + d.p1_hidden.size() - 1);

                    } else {
                        cout << "Debug: Bad hidden card picked" << endl <<
                        "Debug: You picked a " << d.p1_hidden[d.p1_hidden.size()-1] << endl;

                        d.played_cards.push_back(d.p1_hidden[d.p1_hidden.size()-1]);
                        d.p1_hidden.erase(d.p1_hidden.begin() + d.p1_hidden.size() - 1);
                        d.pick_up_pile(1);
                    }
                } else {
                    d.pick_up_pile(1);
                }
            }
        } else if (d.player_turn == 2) {
            if (d.is_empty && d.p2_hand.size() == 0){
                d.p2_open_mode = true;
                cout << "Debug: P2_open_mode = true" << endl;
                for (int i = 0; i < d.p2_open.size(); i++) {
                    d.p2_hand.push_back(d.p2_open[i]);
                }
            }
            d.set_playable(2);
            if (d.can_play(2)) {
                d.play(2, 
                    p2.do_turn(
                        d.p2_hand,
                        d.p2_playable, 
                        d.played_cards,
			            d.p2_open, 
                        d.p1_open, 
                        d.last_played));
            } else {
                if (d.p2_open_mode && d.p2_hand.size() == 0 && d.p2_open.size() == 0) {
                    //Win, play or pick up
                    if (
                        d.p2_hidden.size() > 1
                        && d.legal_move(d.p2_hidden[d.p2_hidden.size()-1])
                    ) {
                        cout << "Debug: Good hidden card picked" << endl;

                        //Switch turn
                        if (d.p2_hidden[d.p2_hidden.size()-1] != 2 && d.p2_hidden[d.p2_hidden.size()-1] != 10) {
                            d.player_turn = 1;
                        }

                        d.lay_card(d.p2_hidden[d.p2_hidden.size()-1]);
                        d.p2_hidden.erase(d.p2_hidden.begin() + d.p2_hidden.size() - 1);
                    }
                    else if (
                        d.p2_hidden.size() == 1
                        && d.legal_move(d.p2_hidden[d.p2_hidden.size()-1])
                        && d.p2_hidden[d.p2_hidden.size()-1] != 1 
                        && d.p2_hidden[d.p2_hidden.size()-1] != 2 
                        && d.p2_hidden[d.p2_hidden.size()-1] != 5
                        && d.p2_hidden[d.p2_hidden.size()-1] != 10
                    ) {
                        cout << "Debug: Good hidden card picked" << endl;

                        //Switch turn
                        if (d.p2_hidden[d.p2_hidden.size()-1] != 2 && d.p2_hidden[d.p2_hidden.size()-1] != 10) {
                            d.player_turn = 1;
                        }

                        d.lay_card(d.p2_hidden[d.p2_hidden.size()-1]);
                        d.p2_hidden.erase(d.p2_hidden.begin() + d.p2_hidden.size() - 1);

                    } else {
                        cout << "Debug: Bad hidden card picked" << endl <<
                        "Debug: You picked a " << d.p2_hidden[d.p2_hidden.size()-1] << endl;

                        d.played_cards.push_back(d.p2_hidden[d.p2_hidden.size()-1]);
                        d.p2_hidden.erase(d.p2_hidden.begin() + d.p2_hidden.size() - 1);
                        d.pick_up_pile(2);
                    }
                } else {
                    d.pick_up_pile(2);
                }
            }
        } else {
            cout << "Error: player_turn = " << d.player_turn << endl;
        }

        //See if anyone has won
        if (d.p1_hand.size() == 0 && d.p1_hidden.size() == 0 && d.p1_open.size() == 0) {
            cout << "P1 wins!" << endl;
            return 0;
        } else if (d.p2_hand.size() == 0 && d.p2_hidden.size() == 0 && d.p2_open.size() == 0) {
            cout << "P2 wins!" << endl;
            return 0;
        }

        vector<int> total_cards;

        total_cards.insert( total_cards.end(), d.p1_hand.begin(), d.p1_hand.end() );
        total_cards.insert( total_cards.end(), d.p2_hand.begin(), d.p2_hand.end() );
        total_cards.insert( total_cards.end(), d.played_cards.begin(), d.played_cards.end());

        if (d.p1_hidden.size() == 0 && d.p2_hidden.size() == 0 && total_cards.size() < 10) {
            std::vector<int>::reverse_iterator rit = total_cards.rbegin();
            int i;
            for (rit = total_cards.rbegin(), i = 0; rit!= total_cards.rend(), i < 9, 1 == *rit || 2 == *rit || 5 == *rit; ++rit, ++i) {
                if (i == total_cards.size()) {
                    cout << "Infinite loop yay" << endl;
                    return 0;
                }
            }
        }
    }

    return 0;
}