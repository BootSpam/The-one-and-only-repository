struct Player {
    int player_number;
    int card_to_play;
    Player(int n) {this->player_number = n;}
    int do_turn(int* hand, int* your_open, int* opponents_open, int last_played);
};
