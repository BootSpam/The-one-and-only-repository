#include "player.h"
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;
int Player::do_turn(vector<int>& real_hand, vector<int>& hand, int* your_open, int* opponents_open, int last_played) {
    
    //Display for human player
    cout << endl << "Player " << player_number << endl
        << "Last played " << last_played << endl;
    
    cout << "Playable cards ";
    for (int i = 0; i < hand.size(); i++) {
        cout << hand[i] << " ";
    }
    cout << endl;

    cout << "Hand ";
    for (int i = 0; i < real_hand.size(); i++) {
        cout << real_hand[i] << " ";
    }
    cout << endl;

    cout << "Your open cards  " << your_open[0] << " " << your_open[1] << " " << your_open[2] << endl
        << "Opponents open cards  " << opponents_open[0] << " " << opponents_open[1] << " " << opponents_open[2] << endl;

    //Player input
    cout << "Play a card [0-2/3] or quit [4] ";
    cin >> this->card_to_play;
    card_to_play = card_to_play % hand.size();

    /*
switch (card_to_play) {
	case 0:
		return card_to_play;
		break;
	case 1:
		return card_to_play;
		break;
	case 2:
		return card_to_play;
		break;
	case 3:
		return card_to_play;
		break;
	case 4:
		exit;
		break;
	default:
		cout << "Nice try" << endl;
		cout << "Play a card [0-2/3] or quit [4] ";
		cin >> this->card_to_play;
	} */
    return card_to_play;
}
