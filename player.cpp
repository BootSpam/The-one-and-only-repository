#include "player.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <limits.h>     //New for AI

using namespace std;
Play Player::do_turn(
        vector<int>& real_hand, 
        vector<int>& playable_hand, 
        vector<int>& played_cards,
	    vector<int>& your_open, 
        vector<int>& opponents_open,
	    int last_played
    ) {
    
    cout << "Debug: do_turn succesfully called" << endl;
    
    Play play;

    //Display for human player
    cout << "Player " << player_number << endl
        << "Last played " << last_played << endl;
    
    cout << "Playable hand ";
    for (int i = 0; i < playable_hand.size(); i++) {
        cout << playable_hand[i] << " ";
    }
    cout << endl;

    cout << "Real hand ";
    for (int i = 0; i < real_hand.size(); i++) {
        cout << real_hand[i] << " ";
    }
    cout << endl;
    
    cout << "Played cards ";
    for (int i = 0; i < played_cards.size(); i++) {
	    cout << played_cards[i] << " ";
    }
    cout << endl;

    cout << "Your open cards "; 
    for (int i = 0; i < your_open.size(); i++) {
        cout << your_open[i] << " ";
    }
    cout << endl;

    cout << "Opponents open cards ";
    for (int i = 0; i < opponents_open.size(); i++) {
        cout << opponents_open[i] << " ";
    }
    cout << endl;

    //Player input
    cout << "Play a card [0-" << playable_hand.size()-1 << "] " << endl;
    //cin >> this->card_to_play;
    
    /*AI*/
    /*------------------------------------------------------------*/

    //                      A   2   3   4   5   6   7   8   9  10   J   Q   K
    int weights[13] = {    -2, -1,  9,  8,  0,  7,  6,  5,  4, -3,  3,  2,  1};
    int highest_yet = INT_MIN;

    for (int i = 0; i < playable_hand.size(); i++) {
        cout << "AI: card #" << i << " is a " << playable_hand[i] << " and is weighted " << weights[playable_hand[i]-1] << endl;
        if (weights[playable_hand[i]-1] > highest_yet) {
            this->card_to_play = i;
            highest_yet = weights[playable_hand[i]-1];
        }
    }
    cout << "AI: We play #" << this->card_to_play << endl;

    /*------------------------------------------------------------*/

    //Declarations to deal with duplicates
    int duplicates_of_chosen_card = 0;
    int value_of_chosen_card = playable_hand[this->card_to_play];

    this->amount_to_play = 1;

    // Check for duplicates
    for (int i=0; i < playable_hand.size(); i++){
        if (playable_hand[i] == value_of_chosen_card)
            duplicates_of_chosen_card++;
    }

    // Optionally play duplicates
    if (duplicates_of_chosen_card > 1) {
        cout << "How many? [1-" << duplicates_of_chosen_card << "] " << endl;
        //cin >> this->amount_to_play;

        /*AI*/
        /*------------------------------------------------------------*/
        //                           A  2  3  4  5  6  7  8  9  10 J  Q  K 
        int duplicate_weights[13] = {4, 1, 4, 4, 1, 4, 4, 4, 4, 1, 4, 4, 4};

        //Choosing the amount to play based on the array "duplicate_weights"
        this->amount_to_play = duplicate_weights[value_of_chosen_card-1];

        //Adjust for bad duplicate number input
        if (amount_to_play > duplicates_of_chosen_card) {
            amount_to_play = duplicates_of_chosen_card;
        }
        if (amount_to_play < 1) {
            amount_to_play = 1;
        }

        cout << "AI: We play " << this->amount_to_play << " of the " << value_of_chosen_card << " cards" << endl;

        /*------------------------------------------------------------*/
    }

    //Adjust for bad card number input
    if (playable_hand.size() == 0) {
        cout << "Error: No playable cards" << endl;
        return play;
    } else {
        card_to_play = card_to_play % playable_hand.size();
        cout << "Debug: card_to_play = " << card_to_play << endl;
    }

    cout << "Debug: amount_to_play = " << amount_to_play << endl;

    play.card_value = playable_hand[card_to_play];
    play.amount = amount_to_play;

    cout << "Debug: played card value = " << play.card_value << endl;
    cout << "Debug: played amount = " << play.amount << endl;

    return play;
}

int Player::do_early_turn(vector<int>& all_cards) {
    
    cout << "Player " << player_number << endl
    << "Chose an open card: ";
    for (int i = 0; i < all_cards.size(); i++) {
        cout << all_cards[i] << " ";
    }
    cout << endl;
    
    //Player input
    cout << "Pick a card [0-" << all_cards.size()-1 << "] " << endl;
    //cin >> this->card_to_make_open;

    /*AI*/
    /*------------------------------------------------------------*/

    //                      A   2   3   4   5   6   7   8   9  10   J   Q   K
    int weights[13] = {    -2, -1,  9,  8,  0,  7,  6,  5,  4, -3,  3,  2,  1};
    int lowest_yet = INT_MAX;

    for (int i = 0; i < all_cards.size(); i++) {
        cout << "AI: card #" << i << " is a " << all_cards[i] << " and is weighted " << weights[all_cards[i]-1] << endl;
        if (weights[all_cards[i]-1] < lowest_yet) {
            this->card_to_make_open = i;
            lowest_yet = weights[all_cards[i]-1];
        }
    }
    cout << "AI: We choose #" << this->card_to_make_open << endl;

    /*------------------------------------------------------------*/

    return card_to_make_open;
}