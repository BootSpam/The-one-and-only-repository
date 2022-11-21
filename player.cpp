#include "player.h"
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;
Play Player::do_turn(
        vector<int>& real_hand, 
        vector<int>& playable_hand, 
        vector<int>& played_cards,
	    vector<int>& your_open, 
        vector<int>& opponents_open,
	    int last_played
    ) {
    
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
    cout << "Play a card [0-" << playable_hand.size()-1 << "] ";
    cin >> this->card_to_play;

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
        cout << "How many? [1-" << duplicates_of_chosen_card << "] ";
        cin >> this->amount_to_play;
    }

    //Adjust for bad card number input
    if (playable_hand.size() == 0) {
        cout << "Error: No playable cards" << endl;
        return play;
    } else {
        card_to_play = card_to_play % playable_hand.size();
        cout << "Debug: card_to_play = " << card_to_play << endl;
    }

    //Adjust for bad duplicate number input
    if (amount_to_play > duplicates_of_chosen_card) {
        amount_to_play = duplicates_of_chosen_card;
    }
    if (amount_to_play < 1) {
        amount_to_play = 1;
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
    cout << "Pick a card [0-" << all_cards.size()-1 << "] ";
    cin >> this->card_to_make_open;

    return card_to_make_open;
}