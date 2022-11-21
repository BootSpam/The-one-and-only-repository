#include <vector>
using namespace std;

struct Play {
    int card_value;
    int amount;
};

struct Player {
    int player_number;
    int card_to_play; // index of card in hand to play
    int amount_to_play; // number of equal cards to play
    int card_to_make_open;
    Player(int n) {this->player_number = n;}

    Play do_turn(
        vector<int>& real_hand, 
        vector<int>& playable_hand, 
        vector<int>& played_cards,
	vector<int>& your_open, 
        vector<int>& opponents_open,
	int last_played
        );

    int do_early_turn(vector<int>& all_cards);
};
