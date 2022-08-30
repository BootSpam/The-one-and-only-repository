#include "player.h"
#include <iostream>
using namespace std;

int Player::do_turn(int* hand, int* your_open, int* opponents_open, int last_played) {
    
    //Display for human player
    cout << endl << "Player " << player_number << endl
        << "Your hand " << hand[0] << " " << hand[1] << " " << hand[2] << endl
        << "Your open cards  " << your_open[0] << " " << your_open[1] << " " << your_open[2] << endl
        << "Opponents open cards  " << opponents_open[0] << " " << opponents_open[1] << " " << opponents_open[2]
        << endl;

    //Player input
    cout << "Play a card [0-2/3] ";
    cin >> this->card_to_play;

    //Return card 0-2 or no card 3
    //cout << "Return ";
    return card_to_play;
}
