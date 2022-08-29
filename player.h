struct Player {
    int player_number;
    int card_to_play;
    Player(int n) {this->player_number = n;}
    int Do_Turn(int* hand, int* open, int last_played);
};
