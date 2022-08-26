#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

struct Deck {
    int deck[52];
    int index;
    
    Deck() {
        for(int i = 0; i < 52; i++) {
            deck[i] = i % 13;
        }
    }
    
    void blanda() {
        shuffle(begin(this->deck), end(this->deck), mt19937{random_device{}()});
        index = 0;
    }

    int draw() {
        if (index > 51) {
            blanda();
            cout << "Reshuffled" << endl; //Not supposed to happen
        }

        index++;
        return deck[index -1];
    }


};

int main() {
    
    Deck deck1;
    deck1.blanda();
    
    for(int i = 0; i < 52; i++) {
        //cout << deck1.deck[i] << " ";
        cout << deck1.draw() << " ";
    }
    cout << endl;

    return 0;
}
