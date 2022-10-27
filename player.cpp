#include "player.h"
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;
int Player::do_turn(
        vector<int>& real_hand, 
        vector<int>& hand, 
        vector<int>& played_cards,
	    vector<int>& your_open, 
        vector<int>& opponents_open,
	    int last_played
    ) {
    
    //Display for human player
    cout << endl << "Player " << player_number << endl
        << "Last played " << last_played << endl;
    
    cout << "Playable hand ";
    for (int i = 0; i < hand.size(); i++) {
        cout << hand[i] << " ";
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

    cout << "Your open cards  " << your_open[0] << " " << your_open[1] << " " << your_open[2] << endl
        << "Opponents open cards  " << opponents_open[0] << " " << opponents_open[1] << " " << opponents_open[2] << endl;

    //Player input
    cout << "Play a card [0-" << hand.size()-1 << "] ";
    cin >> this->card_to_play;

    //Adjust for bad input
    if (hand.size() == 0) {
        cout << "Error: No playable cards" << endl;
        return 0;
    } else {
        card_to_play = card_to_play % hand.size();
        cout << "Debug: card_to_play = " << card_to_play << endl;
    }

    return card_to_play;
}
