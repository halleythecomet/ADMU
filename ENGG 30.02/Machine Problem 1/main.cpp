#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "nalasa_header.h"

using namespace std;

int main()
{
    int choice = 0;
    while(choice != 6){
        cout << "The program can do the following tasks: \n"
        << "1. Take a user's order \n"
        << "2. Display all orders \n"
        << "3. Display one order \n"
        << "4. Delete an order \n"
        << "5. Display total payments received\n"
        << "6. Exit the program\n";
        cout << "Choice: ";
        cin >> choice;
        switch(choice){
            case 1:
                task1_get_order();
                break;
            case 2:
                task2_display_all_orders();
                break;
            case 3:
                task2_display_one_order();
                break;
            case 4:
                task3_delete_order();
                break;
            case 5:
                bonus_display_total_payment();
                break;
            case 6:
                cout << "Program exited.\n";
                remove("temp.txt");
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
        
    }

    return 0;
}