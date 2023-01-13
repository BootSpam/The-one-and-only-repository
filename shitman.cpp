#include <iostream>
#include <vector>
#include <algorithm>    //For shuffling deck
#include <random>
#include <chrono>

#include "player.h"

using namespace std;

#define NUMBER_OF_WEIGHTS 40
#define NUMBER_OF_ROUNDS 10

struct Deck {

    //General
    int deck[52];
    int index;
    int last_played;
    int player_turn;
    vector<int> played_cards;
    bool is_empty;
    int batch = 0;
    int winners = 0;

    //Weights
    int all_weights[NUMBER_OF_WEIGHTS][13];
    int all_weights_wins[NUMBER_OF_WEIGHTS];

    int winning_weights[NUMBER_OF_WEIGHTS*(NUMBER_OF_WEIGHTS+1)/2][13];
    int counting_winning_weights[NUMBER_OF_WEIGHTS*(NUMBER_OF_WEIGHTS+1)/2][13];

    int standard_early_weights[13] = {-2, -1, 9, 8, 0, 7, 6, 5, 4, -3, 3, 2, 1};
    int standard_mid_weights[13] = {-2, -1, 9, 8, 0, 7, 6, 5, 4, -3, 3, 2, 1};
    int standard_duplicate_weights[13] = {4, 1, 4, 4, 1, 4, 4, 4, 4, 1, 4, 4, 4};

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
            deck[i] = i % 13 + 1;
        }
    }
    
    void blanda() {
        shuffle(begin(this->deck), end(this->deck), mt19937{random_device{}()});
        index = 0;
        last_played = 0;    //Represents no card
    }

    int draw() {
        if (index >= 51) {
            is_empty = true;
            //std::cout << "Debug: index >= 51, deck is empty after this" << endl;
        }

        //std::cout << "Debug: drawing a " << deck[index] << " index " << index << endl;
        index++;
        return deck[index - 1];
    }

    void deal() {

        //Set length of vectors
        p1_hand = {0, 0, 0};
        p1_open = {0, 0, 0};
        p1_hidden = {0, 0, 0};

        p2_hand = {0, 0, 0};
        p2_open = {0, 0, 0};
        p2_hidden = {0, 0, 0};

        //Deal cards
        p1_hidden[0] = draw();  p2_hidden[0] = draw();
        p1_hidden[1] = draw();  p2_hidden[1] = draw();
        p1_hidden[2] = draw();  p2_hidden[2] = draw();

        p1_open[0] = draw();    p2_open[0] = draw();
        p1_open[1] = draw();    p2_open[1] = draw();
        p1_open[2] = draw();    p2_open[2] = draw();

        p1_hand[0] = draw();    p2_hand[0] = draw();
        p1_hand[1] = draw();    p2_hand[1] = draw();
        p1_hand[2] = draw();    p2_hand[2] = draw();
        
        player_turn = 1;
        is_empty = false;
        p1_open_mode = p2_open_mode = false;

    }
    
    bool legal_move(int card) {
        if (player_turn == 1 && p1_hidden.size() == 0 && p1_hand.size() == 1 && (card == 1 || card == 2 || card == 5 || card == 10)){return false;}
        else if (player_turn == 2 && p2_hidden.size() == 0 && p2_hand.size() == 1 && (card == 1 || card == 2 || card == 5 || card == 10)){return false;}
        else if (card >= last_played) {return true;}
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
        //std::cout << "Debug: card to find = " << card << endl;
        if (player == 1) {
            for (int i = 0; i < p1_hand.size(); i++) {
                if (p1_hand[i] == card) {
                    //std::cout << "Debug: card index = " << i << endl;
                    return i;
                }
            }
        } else if (player == 2) {
            for (int i = 0; i < p2_hand.size(); i++) {
                if (p2_hand[i] == card) {
                    //std::cout << "Debug: card index = " << i << endl;
                    return i;
                }
            }
        }
        std::cout << "Error: Invalid player or card" << endl;
        //std::cout << "Player: " << player << endl;
        //std::cout << "Card: " << card << endl;
        
        return 0;  //Not supposed to happen
    }

    bool lay_card(int card) {
        //Add card to played pile
        played_cards.push_back(card);

        //std::cout << "Debug: card layed = " << card << endl;

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
            std::cout << "Invalid player" << endl;
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
            //std::cout << "Debug: Can not play" << endl;
            return false;
        } else if (player == 2 && p2_playable.size() == 0) {
            //std::cout << "Debug: Can not play" << endl;
            return false;
        } else {
            //std::cout << "Debug: Can play" << endl;
            return true;
        }
    }
    
    void pick_up_pile(int player) {
        //std::cout << "Debug: Picking up pile" << endl;
        if (player == 1) {
            if (p1_open_mode) {
                //std::cout << "Debug: saving current hand to open" << endl;
                p1_open.clear();
                for (int i = 0; i < p1_hand.size(); i++) {
                    p1_open.push_back(p1_hand[i]);
                }
                p1_hand.clear();    //Must clear hand to ensure no residue open cards
                p1_open_mode = false;
                //std::cout << "Debug: p1_open_mode = false" << endl;
            }
            for (int i = 0; i < played_cards.size(); i++) {
                p1_hand.push_back(played_cards[i]);
            }
        } else if (player == 2) {
            if (p2_open_mode) {
                //std::cout << "Debug: saving current hand to open" << endl;
                p2_open.clear();
                for (int i = 0; i < p2_hand.size(); i++) {
                    p2_open.push_back(p2_hand[i]);
                }
		p2_hand.clear();
                p2_open_mode = false;
                //std::cout << "Debug: p2_open_mode = false" << endl;
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

    void set_random_weights(int random_type) {
        std::uniform_int_distribution<int> distribution(random_type == 3 ? 1 : 0, random_type == 3 ? 4 : 12);

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator (seed);

        srand(time(NULL));
        for (winners; batch > 1 ? winners < NUMBER_OF_WEIGHTS-3 : winners < NUMBER_OF_WEIGHTS ; winners++) {
            for (int i = 0; i < 13; i++) {

                all_weights[winners][i] = distribution(generator);
                
                //Making sure there are no duplicates
                if(random_type == 1 || random_type == 2){
                    for (int check_unique = 0; check_unique < i; check_unique++)
                    {
                        if (all_weights[winners][check_unique] == all_weights[winners][i])
                        {
                            all_weights[winners][i] = distribution(generator);
                            check_unique = -1;
                        }
                    }
                }
                //std::cout << all_weights[winners][i] << " ";
            }
            //std::cout << endl;
        }
        winners = 0;
    }
};


int main() {

    //Declarations
    Deck d;
    Player p1(1);
    Player p2(2);
    int turn;
    int early_index;
    int early_card;

    int answer;
    string answer_2;
    int last_winners = 0;

    int random_type;

    std::cout << "Choose what of weights to randomize (1 - 3): 1 - early, 2 - mid, 3 - amount" << endl;
    std::cin >> random_type;
    std::cout << "How many batches?" << endl;
    std::cin >> answer;
    //std::cout << "How many different weights do you want?" << endl;
    //std::cin >> NUMBER_OF_WEIGHTS;
    //std::cout << "How many rounds do you want to play each time?" << endl;
    //std::cin >> NUMBER_OF_ROUNDS;

    do {
        d.batch++;
        try {

        d.set_random_weights(random_type);

        for (int i = 0; i < NUMBER_OF_WEIGHTS; i++)
            d.all_weights_wins[i] = 0;

        for (int round = 0; round < NUMBER_OF_ROUNDS; round++) {
        int completion = round * 10;
        cout << completion << "% done" << endl;
        int wins = 0;
        int game = 0;
        for (int weight_player_1 = 0; weight_player_1 < NUMBER_OF_WEIGHTS; weight_player_1 ++) {
            for (int weight_player_2 = weight_player_1+1; weight_player_2 < NUMBER_OF_WEIGHTS; wins == game ? weight_player_2 ++ : game--) {
            //Game loop
            while (true) {
                
                game++;
                //std::cout << endl << "Game " << game << endl;
                //std::cout << endl << "Wins " << wins << endl;

                //Set AI weights
                p1.set_weights(
                    random_type == 1 ? d.all_weights[weight_player_1] : d.standard_early_weights, 
                    random_type == 2 ? d.all_weights[weight_player_1] : d.standard_mid_weights, 
                    random_type == 3 ? d.all_weights[weight_player_1] : d.standard_duplicate_weights
                    );
                p2.set_weights(
                    random_type == 1 ? d.all_weights[weight_player_2] : d.standard_early_weights, 
                    random_type == 2 ? d.all_weights[weight_player_2] : d.standard_mid_weights, 
                    random_type == 3 ? d.all_weights[weight_player_2] : d.standard_duplicate_weights
                    );

                //Set up game
                d.blanda();
                d.deal();
                turn = 0;
                
                //Print deck
                //std::cout << "Deck ";
                //for(int i = 0; i < 52; i++) {
                    //std::cout << d.deck[i] << " ";
                //}
                //std::cout << endl;

                //p1 set early_cards
                vector<int> p1_early_cards = {};
                for(int i = 0; i < 3; i++) {
                    p1_early_cards.push_back(d.p1_hand[i]);
                }
                for(int i = 0; i < 3; i++) {
                    p1_early_cards.push_back(d.p1_open[i]);
                }

                //p1 early game loop
                //std::cout << endl;
                for(int i = 0; i < 3; i++) {
                    early_index = p1.do_early_turn(p1_early_cards);
                    early_card = p1_early_cards[early_index];
                    d.p1_open[i] = early_card;
                    p1_early_cards.erase(p1_early_cards.begin() + early_index);
                }
                //set hand
                for(int i = 0; i < 3; i++) {
                    d.p1_hand[i] = p1_early_cards[i];
                }
                
                //p2 set early_cards
                vector<int> p2_early_cards = {};
                for(int i = 0; i < 3; i++) {
                    p2_early_cards.push_back(d.p2_hand[i]);
                }
                for(int i = 0; i < 3; i++) {
                    p2_early_cards.push_back(d.p2_open[i]);
                }

                //p2 early game loop
                //std::cout << endl;
                for(int i = 0; i < 3; i++) {
                    early_index = p2.do_early_turn(p2_early_cards);
                    early_card = p2_early_cards[early_index];
                    d.p2_open[i] = early_card;
                    p2_early_cards.erase(p2_early_cards.begin() + early_index);
                }
                //set hand
                for(int i = 0; i < 3; i++) {
                    d.p2_hand[i] = p2_early_cards[i];
                }

                //Mid-Game
                while(true) {

                    //Debug -------------------------------------------
                    for (auto i : d.played_cards) {
                        if (i > 13) {
                            return 0;
                        }
                    }
                    //end of Debug -------------------------------------------

                    turn++;
                    //std::cout << "Debug: Turn = " << turn << endl;

                    //Update open cards
                    if (d.p1_open_mode) {
                        //std::cout << "Debug: Updating p1_open" << endl;
                        d.p1_open.clear();
                        for (int i = 0; i < d.p1_hand.size(); i++) {
                            d.p1_open.push_back(d.p1_hand[i]);
                        }
                    }
                    if (d.p2_open_mode) {
                        //std::cout << "Debug: Updating p2_open" << endl;
                        d.p2_open.clear();
                        for (int i = 0; i < d.p2_hand.size(); i++) {
                            d.p2_open.push_back(d.p2_hand[i]);
                        }
                    }

                    //Player turns
                    //std::cout << endl << "Debug: Player " << d.player_turn << endl;
                    if (d.player_turn == 1) {
                        if (d.is_empty && d.p1_hand.size() == 0) {
                            d.p1_open_mode = true;
                            //std::cout << "Debug: P1_open_mode = true" << endl;
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
                                    //std::cout << "Debug: Good hidden card picked" << endl;

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
                                    //std::cout << "Debug: Good hidden card picked" << endl;

                                    //Switch turn
                                    if (d.p1_hidden[d.p1_hidden.size()-1] != 2 && d.p1_hidden[d.p1_hidden.size()-1] != 10) {
                                        d.player_turn = 2;
                                    }

                                    d.lay_card(d.p1_hidden[d.p1_hidden.size()-1]);
                                    d.p1_hidden.erase(d.p1_hidden.begin() + d.p1_hidden.size() - 1);

                                } else {
                                    //std::cout << "Debug: Bad hidden card picked" << endl <<
                                    //"Debug: You picked a " << d.p1_hidden[d.p1_hidden.size()-1] << endl;

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
                            //std::cout << "Debug: P2_open_mode = true" << endl;
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
                                    //std::cout << "Debug: Good hidden card picked" << endl;

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
                                    //std::cout << "Debug: Good hidden card picked" << endl;

                                    //Switch turn
                                    if (d.p2_hidden[d.p2_hidden.size()-1] != 2 && d.p2_hidden[d.p2_hidden.size()-1] != 10) {
                                        d.player_turn = 1;
                                    }

                                    d.lay_card(d.p2_hidden[d.p2_hidden.size()-1]);
                                    d.p2_hidden.erase(d.p2_hidden.begin() + d.p2_hidden.size() - 1);

                                } else {
                                    //std::cout << "Debug: Bad hidden card picked" << endl <<
                                    //"Debug: You picked a " << d.p2_hidden[d.p2_hidden.size()-1] << endl;

                                    d.played_cards.push_back(d.p2_hidden[d.p2_hidden.size()-1]);
                                    d.p2_hidden.erase(d.p2_hidden.begin() + d.p2_hidden.size() - 1);
                                    d.pick_up_pile(2);
                                }
                            } else {
                                d.pick_up_pile(2);
                            }
                        }
                    } else {
                        std::cout << "Error: player_turn = " << d.player_turn << endl;
                    }

                    //See if anyone has won
                    if (d.p1_hand.size() == 0 && d.p1_hidden.size() == 0 && d.p1_open.size() == 0) {
                        std::copy(std::begin(d.all_weights[weight_player_1]), std::end(d.all_weights[weight_player_1]), std::begin(d.winning_weights[wins]));
                        d.all_weights_wins[weight_player_1]++;
                        wins ++;
                        //std::cout << "P1 wins!" << endl;
                        break;
                    } else if (d.p2_hand.size() == 0 && d.p2_hidden.size() == 0 && d.p2_open.size() == 0) {
                        std::copy(std::begin(d.all_weights[weight_player_2]), std::end(d.all_weights[weight_player_2]), std::begin(d.winning_weights[wins]));
                        d.all_weights_wins[weight_player_2]++;
                        wins++;
                        //std::cout << "P2 wins!" << endl;
                        break;
                    } else if (turn >= 200) {

                        //std::cout << "Both loose" << endl;
                        break;
                    }

                    vector<int> total_cards;

                    total_cards.insert( total_cards.end(), d.p1_hand.begin(), d.p1_hand.end() );
                    total_cards.insert( total_cards.end(), d.p2_hand.begin(), d.p2_hand.end() );
                    total_cards.insert( total_cards.end(), d.played_cards.begin(), d.played_cards.end());

                    if (d.p1_hidden.size() == 0 && d.p2_hidden.size() == 0 && total_cards.size() < 10) {
                        std::vector<int>::reverse_iterator rit = total_cards.rbegin();
                        int i;
                        for (rit = total_cards.rbegin(), i = 0; rit!= total_cards.rend(), i < 9, 1 == *rit || 2 == *rit || 5 == *rit; ++rit, ++i) {
                            if (i == total_cards.size() -1) {
                                //std::cout << "Infinite loop yay" << endl;
                                break;
                            }
                        }
                    }
                }

                break;

            }

            }
        }
    }

        //displays winners from last batch
        std::cout << endl << endl;
        if(d.batch > 1){
            std::cout << "Last winners:" << endl;
            for(int i = 0; i < last_winners; i++){
                for(int j = 0; j < 13; j++){
                    std::cout << d.all_weights[i][j] << " ";
                }
                std::cout << endl;
            }
        }

        //displays loserss from last batch
        std::cout << endl << endl;
        if(d.batch > 1){
            std::cout << "Last losers:" << endl;
            for(int i = NUMBER_OF_WEIGHTS-3; i < NUMBER_OF_WEIGHTS; i++){
                for(int j = 0; j < 13; j++){
                    std::cout << d.all_weights[i][j] << " ";
                }
                std::cout << endl;
            }
        }

        std::cout << "Batch: " << d.batch << endl;

        //Sorts all_weights and all_weights_wins
        int temporary_saved_number_of_wins = 0;
        int temporary_saved_weights[13];
        for(int i = 0; i < 13; i++){
            temporary_saved_weights[i] = 0;
        }

        for(int i = 0; i < NUMBER_OF_WEIGHTS; i++){
            for(int j = 0; j < i; j++){
                if(d.all_weights_wins[i] > d.all_weights_wins[j]){
                    temporary_saved_number_of_wins = d.all_weights_wins[i];
                    for(int k = 0; k < 13; k++){
                        temporary_saved_weights[k] = d.all_weights[i][k];
                    }
                    for(int l = i-1; l > j-1; l--){
                        d.all_weights_wins[l+1] = d.all_weights_wins[l];
                        for(int m = 0; m < 13; m++){
                            d.all_weights[l+1][m] = d.all_weights[l][m];
                        } 
                    }
                    d.all_weights_wins[j] = temporary_saved_number_of_wins;
                    for(int n = 0; n < 13; n++){
                        d.all_weights[j][n] = temporary_saved_weights[n];
                    }
                    j = i;
                }

            }
        }

        std::cout << endl << "Amount of wins for the different sets of weights:" << endl;
        for(int i = 0; i < NUMBER_OF_WEIGHTS; i++){
            std::cout << d.all_weights_wins[i] << " - ";
            for(int j = 0; j < 13; j++) {
                std::cout << d.all_weights[i][j] << " ";
            }
            std::cout << endl;
        }

        //Prepares winners for a new set of games
        d.winners = 3;
        
        for(int i = 3; i < NUMBER_OF_WEIGHTS; i++){
            if(d.all_weights_wins[i] == d.all_weights_wins[2]){
                d.winners++;
            }
        }
        
        last_winners = d.winners;

        for(d.winners; d.winners < NUMBER_OF_WEIGHTS-3; d.winners++){
            for(int i = 0; i < 13; i++){
                d.all_weights[d.winners][i] = 0;
            }
        }

        d.winners = last_winners;

        //std::cout << "Another batch? (yes or no)" << endl;
        //std::cin >> answer;

        } catch (exception& e) {
            std::cout << e.what() << endl;
            return 0;
        }
        //if(d.batch == answer){
        //cout << "Another one? (yes/no)";
        //cin >> answer_2;
        //if (answer_2 == "yes" || answer_2 == "Yes" || answer_2 == "y" || answer_2 == "Y"){
        //    answer++;
        //}
        //}

        if(d.batch == answer){
            d.winners = 0;
            d.batch = 0;

        }

    }
    while(d.batch < answer);
    return 0;
}