#include "player.h"
#include <iostream>
using namespace std;

int Player::Do_Turn(int* hand, int* open, int last_played) {
    
    //Display for human player
    cout << endl << "Player " << player_number << endl
        << "Hand " << hand[0] << hand[1] << hand[2] << endl
        << "Open " << open[0] << open[1] << open[2] << endl << endl;

    //Player input
    cout << "Play a card [0-2/3] ";
    cin >> this->card_to_play;

    //Return card 0-2 or no card 3
    cout << "Return ";
    return card_to_play;
}
