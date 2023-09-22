//
//  main.cpp
//  project1
//
//  Created by Yizhou Li on 12/25/22.
//

#include <iostream>
#include "imgDisplay.hpp"
#include "vidDisplay.hpp"

using namespace std;

int main() {
    cout << "Please, input the task number:" << endl;
    
    int choice;
    cin >> choice;
    
    if (choice == 1) {
        startImg();
    } else if (2 <= choice && choice <= 11) {
        startVid();
    }
    
    return 0;
}
