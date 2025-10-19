#define MANAGER_PASSCODE 123
#define MAX_ATTEMPTS 3

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;

bool ask_passcode();
int get_current_cn();
string int_to_fcn(int cn_int);
int num_cin();
int num_choice_cin(int start_num, int end_num);
string str_choice_cin(const string options[], int options_len);

int task1_get_rows();
int task1_get_cols();
void task1_get_order();

void task2_check_availability(int num_ac, int num_meals, string payment_method, string cn_str);
int task2_compute_order_payment();
void task2_add_order(string ac_arr[], int num_ac, string meals_arr[], int num_meals, string payment_str, string payment_method, string cn_str);
void task2_display_one_order();
void task2_display_all_orders();

int task3_find_line_num(string cn_str);
void task3_remove_line(int line_to_delete);
void task3_delete_order();

void bonus_a_swap_str(string *s1, string *s2);
int bonus_a_char_index(char c);
void bonus_a_sort_str_array(string* arr, int arr_len);

void bonus_c_display_total_payments();

const string MENU_FILENAME = "menu.txt";
const string TEMP_ITEMS_FILENAME = "temp_items.txt";
const string TEMP_AC_FILENAME = "temp_ac.txt";
const string TEMP_MEAL_FILENAME = "temp_meal.txt";
const string TEMP_COST_FILENAME = "temp_cost.txt";

const string PAYMENT_OPTIONS[] = {"Cash", "Credit Card", "GCash"};
const int PAYMENT_OPTIONS_LEN = 3;

const string DRINK_OPTIONS[] = {"Coke", "Royal", "Sprite"};
const int DRINK_OPTIONS_LEN = 3;

bool ask_passcode(){
    string passcode_attempt;
    for(int i = 0; i < MAX_ATTEMPTS; i++){
        cout << "Input manager passcode: (" << 3-i << " attempt/s remaining): ";
        getline(cin >> ws, passcode_attempt);
        if(stoi(passcode_attempt) == MANAGER_PASSCODE){
            return true;
        }
    }
    cout << "Manager login failed. Returning.";
    cout << "\n";
    return false;
}

int get_current_cn(){
    fstream ol_file;
    int cn = 1;

    ol_file.open("Order List.txt", ios::in);
    if(ol_file.is_open()){
        string cl = "a";
        string pl = "";
        while(getline(ol_file, cl) && cl != ""){
            pl = cl;
        }

        if(pl != ""){
            cn = stoi(pl.substr(3, 3)) + 1;
        }
        ol_file.close();
    }
    return cn;
}

string int_to_fcn(int cn_int){
    string unf_cn_str = to_string(cn_int);
    if(cn_int < 10){
        return "JC-00" + unf_cn_str;
    }else if(cn_int < 100){
        return "JC-0" + unf_cn_str;
    }else{
        return "JC-" + unf_cn_str;
    }
}

int num_cin(){
    bool valid_input = false;
    int num_input;
    string str_input;

    while(!valid_input){
        cout << "Please input a number: ";
        try{
            getline(cin >> ws, str_input);
            num_input = stoi(str_input);
            valid_input = true;
        }
        catch(...){
            cout << "Invalid input. \n";
        }
    }

    return num_input;
}

int num_choice_cin(int start_num, int end_num){
    bool valid_input = false;
    string input = "";
    int choice = 0;
    while(!valid_input){
        cout << "Choose a number between " << start_num << " and " << end_num << ": ";
        try{
            getline(cin >> ws, input);
            choice = stoi(input);
            if(choice >= start_num && choice <= end_num) {
                valid_input = true;
            }else{
                cout << "Number is out of range. \n";
            }
        }
        catch(...){
            cout << "Invalid input. \n";
        }
    }
    return choice;
}

string str_choice_cin(const string options[], int options_len){
    string choice = "";
    bool valid_option = false;

    while(!valid_option){
        cout << "Please choose between:\n";
        for(int i = 0; i < options_len; i++){
            cout << "-" << options[i] << "\n";
        }

        cout << "Your choice: ";
        getline(cin >> ws, choice);
        for(int i = 0; i < options_len; i++){
            if(choice == options[i]){
                valid_option = true;
            }
        }

        if(!valid_option){
            cout << "Invalid option, try again. \n";
        }
    }

    return choice;
}

int task1_get_rows(){
    int rows = 0;
    fstream f;
    f.open(MENU_FILENAME, ios::in);
    if(f.is_open()){
        string s = "";
        while(getline(f, s)){
            rows++;
        }
        f.close();
    }
    return rows;
}

int task1_get_cols(){
    int cols = 0;
    fstream f;
    f.open(MENU_FILENAME, ios::in);
    if(f.is_open()){
        char c = ' ';
        bool is_not_newline = true;
        while(f.get(c) && is_not_newline){
            if(c == ','){
                cols++;
            }
            if(c == '\n'){
                is_not_newline = false;
            }
        }
        f.close();
    }
    return cols;
}

void task1_get_order(){
    string cn_str = int_to_fcn(get_current_cn());
    int rows = task1_get_rows();
    int cols = task1_get_cols();

    string raw_table[rows][cols+1];
 
    fstream f;
    f.open(MENU_FILENAME, ios::in);
    if(f.is_open()){
        char c;
        int cur_row = 0;
        int cur_col = 0;
        while(f.get(c)){
            if(c == '\n'){
                cur_row++;
                cur_col = 0;
            }else if(c == ','){
                cur_col++;
            }else{
                raw_table[cur_row][cur_col] += c;
            }
        }
        f.close();
    }

    fstream temp_items, temp_ac, temp_meal, temp_cost;
    temp_items.open(TEMP_ITEMS_FILENAME, ios::out);
    temp_ac.open(TEMP_AC_FILENAME, ios::out);
    temp_meal.open(TEMP_MEAL_FILENAME, ios::out);
    temp_cost.open(TEMP_COST_FILENAME, ios::out);

    int num_ac = 0;
    int num_meals = 0;
    int order_payment = 0;
    bool order_complete = false;

    if(temp_items.is_open() && temp_ac.is_open() && temp_meal.is_open()){
        while(!order_complete){
            int c1, c2 = 0;

            cout << "What would you like to add to your order? \n";
            for(int i = 1; i < cols; i++){
                cout << i << ": ";
                cout << raw_table[0][i];
                cout << "\n";
            }
            cout << cols << ": End order\n";
            c1 = num_choice_cin(1, cols);
            if(c1 == cols){
                order_complete = true;
            }else{
                string item = raw_table[0][c1];
                int num_valid_options = 0;
                for(int i = 1; i < rows; i++){
                    if(stoi(raw_table[i][c1]) > 0){
                        num_valid_options++;
                    }
                }
                if(num_valid_options > 1){
                    cout << "\n";
                    cout << "How would you like it? \n";
                    for(int i = 1; i < rows; i++){
                        if(stoi(raw_table[i][c1]) > 0){
                            cout << i << ": " << item << " ";
                            if(i > 1){
                                cout << "with ";
                            }
                            cout << raw_table[i][0];
                            if(i > 1 && i % 2 != 0){
                                cout << " and " << raw_table[i-1][0];
                            }
                            cout << "\n";
                        }
                    }
                    c2 = num_choice_cin(1, num_valid_options);
                }else{
                    c2 = 1;
                }

                temp_cost << raw_table[c2][c1] << "\n";
                string full_meal = item;
                if(c2 == 1){
                    temp_ac << item << "\n";
                    temp_items << item << "\n";
                    num_ac++;
                }else{
                    cout << "\n";
                    cout << "What drink do you want? ";

                    string fries = "";
                    string drink = str_choice_cin(DRINK_OPTIONS, DRINK_OPTIONS_LEN);

                    if(c2 % 2 == 0){
                        drink += " " + raw_table[c2][0];
                        full_meal += " with " + drink;
                    }else{
                        fries += raw_table[c2][0];
                        drink += " " + raw_table[c2-1][0];
                        full_meal += " with " + fries + " and " + drink;
                    }
                    temp_meal << full_meal << "\n";
                    temp_meal << item << "\n";
                    temp_meal << fries << "\n";
                    temp_meal << drink << "\n";

                    temp_items << full_meal << "\n";

                    num_meals++;
                }
                cout << "\nAdded " << full_meal << " to order.\n";
            }
            cout << "\n";
        }
        temp_items.close();
        temp_meal.close();
        temp_ac.close();
        temp_cost.close();
    }

    if(num_ac == 0 && num_meals == 0){
        cout << "Meal order is empty. Returning to main menu. \n";
        cout << "\n";
    }else{
        cout << "How would you like to pay? ";
        string payment_method = str_choice_cin(PAYMENT_OPTIONS, PAYMENT_OPTIONS_LEN);
        cout << "\n";
        task2_check_availability(num_ac, num_meals, payment_method, cn_str);
    }
}

void task2_check_availability(int num_ac, int num_meals, string payment_method, string cn_str){
    cout << "Checking availability of all food items. \n";
    if(!ask_passcode()){
        return;
    }
    cout << "\n";

    string ac_arr[num_ac];
    string meals_arr[num_meals];
    bool ac_availablility[num_ac];
    bool meals_availability[num_meals];
    bool all_available = true;

    cout << "Ala carte order: \n";
    if(num_ac > 0){
        fstream temp_ac;
        temp_ac.open(TEMP_AC_FILENAME, ios::in);
        if(temp_ac.is_open()){
            string s = "";
            int i = 0;
            int stock = 0;

            while(getline(temp_ac, s)){
                cout << "How much stock is available for the " << s << "?\n";
                stock = num_cin();
                if(stock > 0){
                    ac_availablility[i] = true;
                }else{
                    ac_availablility[i] = false;
                    all_available = false;
                }
                ac_arr[i] = s;
                i++;
            }
            temp_ac.close();
        }
    }else{
        cout << "No ala carte orders were added. \n";
    }
    cout << "\n";

    cout << "Meal order: \n";
    if(num_meals > 0){
        fstream temp_meal;
        temp_meal.open(TEMP_MEAL_FILENAME, ios::in);
        if(temp_meal.is_open()){
            bool all_parts_available = true;
            string s = "";
            int i = 0;
            int stock = 0;

            while(getline(temp_meal, s)){
                if(i % 4 > 0 && s != ""){
                    cout << "How much stock is available for the " << s << "?\n";
                    stock = num_cin();
                    if(stock > 0 && all_parts_available){
                        meals_availability[i/4] = true;
                    }else{
                        meals_availability[i/4] = false;
                        all_available = false;
                        all_parts_available = false;
                    }
                }
                if(i % 4 == 0){
                    meals_arr[i/4] = s;
                }
                i++;
            }
            temp_meal.close();
        }
    }else{
        cout << "No meal orders were added. \n";
    }
    cout << "\n";

    if(!all_available){
        cout << "Some items were not available! \n\n";

        cout << "Your order:\n";
        cout << "Ala carte order: \n";
        if(num_ac == 0){
            cout << "No ala carte orders were added. \n\n";
        }
        for(int i = 0; i < num_ac; i++){
            cout << i+1 << ". ";
            cout << ac_arr[i];
            if(ac_availablility[i] == false){
                cout << " - NOT AVAILABLE";
            }
            cout << "\n";
        }


        cout << "Meal order: \n";
        if(num_meals == 0){
            cout << "No meal orders were added. \n";
        }
        for(int i = 0; i < num_meals; i++){
            cout << i+1 << ". ";
            cout << meals_arr[i];
            if(meals_availability[i] == false){
                cout << " - NOT AVAILABLE";
            }
             cout << "\n";
        }
        cout << "\nPlease order again.\n\n";
        task1_get_order();
    }else{
        cout << "Order was successful!\n\n";

        string payment_str = to_string(task2_compute_order_payment()) + ".00";
        bonus_a_sort_str_array(ac_arr, num_ac);
        bonus_a_sort_str_array(meals_arr, num_meals);
        task2_add_order(ac_arr, num_ac, meals_arr, num_meals, payment_str, payment_method, cn_str);
    }
}

int task2_compute_order_payment(){
    int order_payment = 0;
    fstream temp_cost;
    temp_cost.open(TEMP_COST_FILENAME, ios::in);
    if(temp_cost.is_open()){
        string s = "";
        while(getline(temp_cost, s)){
            order_payment += stoi(s);
        }
        temp_cost.close();
    }
    return order_payment;
}


void task2_add_order(string ac_arr[], int num_ac, string meals_arr[], int num_meals, string payment_str, string payment_method, string cn_str){
    fstream ol_file;

    ol_file.open("Order List.txt", ios::app);
    if(ol_file.is_open()){
        ol_file << cn_str << " " << payment_str << "\n";
        ol_file.close();
    }

    fstream cn_file;
    cn_file.open(cn_str+".txt", ios::out);
    if(cn_file.is_open()){
        cout << "Type of payment: " << payment_method << "\n";
        cn_file << "Type of payment: " << payment_method << "\n";

        cout << "Ala carte order: \n";
        cn_file << "Ala carte order: \n";
        for(int i = 0; i < num_ac; i++){
            cout << i+1 << ". " << ac_arr[i] << "\n";
            cn_file << i+1 << ". " << ac_arr[i] << "\n";
        }

        cout << "Meal order: \n";
        cn_file << "Meal order: \n";
        for(int i = 0; i < num_meals; i++){
            cout << i+1 << ". " << meals_arr[i] << "\n";
            cn_file << i+1 << ". " << meals_arr[i] << "\n";
        }

        cn_file << cn_str << "\n";

        cout << "Total payment: " << payment_str << "\n";
        cn_file << "Total payment: " << payment_str;

        cn_file.close();
    }
    cout << "\n";
}


void task2_display_one_order(){
    if(!ask_passcode()){
        return;
    }

    fstream ol_file;
    ol_file.open("Order List.txt", ios::in);
    if(ol_file.is_open() && get_current_cn() != 1){
        cout << "List of stored orders: \n";
        char c;
        while(ol_file.get(c)){
            cout << c;
        }

        string cn_str = "";
        fstream cn_file;
        while(!cn_file.is_open()){
            cout << "Type customer number of order you want to display (e.g. JC-001): ";
            getline(cin >> ws, cn_str);
            cn_file.open(cn_str+".txt", ios::in);
            if(!cn_file.is_open()){
                cout << "Invalid customer number, try again \n";
            }
        }

        cout << "\n";
        while(cn_file.get(c)){
            cout << c;
        }
        cout << "\n";

        ol_file.close();
        cn_file.close();
    }else{
        cout << "No orders available. \n";
    }
    cout << "\n";
}

void task2_display_all_orders(){
    if(!ask_passcode()){
        return;
    }

    fstream ol_file;

    ol_file.open("Order List.txt", ios::in);
    if(ol_file.is_open() && get_current_cn() != 1) {
        char c;
        while(ol_file.get(c)){
            cout << c;
        }
        ol_file.close();
    }else{
        cout << "No orders available \n";
    }
    cout << "\n";
}

int task3_find_line_num(string cn_str){
    fstream ol_file;
    int line_to_delete = 0;

    ol_file.open("Order List.txt", ios::in);
    if(ol_file.is_open()){
        string s;
        bool found = false;
        while(getline(ol_file, s) && !found){
            if(s.substr(0, 6) == cn_str){
                found = true;
            }else{
                line_to_delete++;
            }
        }
        ol_file.close();
    }

    return line_to_delete;
}

void task3_remove_line(int line_to_delete){
    fstream ol_file;
    string temp_str = "";
    string s;

    ol_file.open("Order List.txt", ios::in);
    if(ol_file.is_open()){
        int cur_line_num = 0;
        while(getline(ol_file, s)){
            if(cur_line_num != line_to_delete){
                temp_str += (s + '\n');
            }
            cur_line_num++;
        }
        ol_file.close();
    }

    ol_file.open("Order List.txt", ios::out);
    if(ol_file.is_open()){
        ol_file << temp_str;
        ol_file.close();
    }
}

void task3_delete_order(){
    if(!ask_passcode()){
        return;
    }

    fstream ol_file;

    ol_file.open("Order List.txt", ios::in);
    if(ol_file.is_open() && get_current_cn() != 1){
        cout << "List of stored orders: \n";
        char c;
        while(ol_file.get(c)){
            cout << c;
        }

        string cn_str = "";
        fstream cn_file;
        while(!cn_file.is_open()){
            cout << "Type customer number of order you want to delete (e.g. JC-001): ";
            getline(cin >> ws, cn_str);
            cn_file.open(cn_str+".txt", ios::in);
            if(!cn_file.is_open()){
                cout << "Invalid customer number, try again. \n";
            }
        }
        cn_file.close();

        cout << "Are you sure you want to delete this file? \n";
        cout << "Retype customer number to confirm: ";
        string s;
        getline(cin >> ws, s);

        if(s == cn_str){
            if(remove((cn_str + ".txt").c_str()) == 0){
                cout << "File deleted successfully. \n";
            }else{
                cout << "Error when deleting file. \n";
            }
        }else{
            cout << "Deletion cancelled. \n";
        }

        ol_file.close();
        task3_remove_line(task3_find_line_num(cn_str));
    }else{
        cout << "No orders available. \n";
    }
    cout << "\n";
}

void bonus_a_swap_str(string *s1, string *s2){
    string temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}

int bonus_a_char_index(char c){
    string alphabet_capital = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string alphabet_lowercase = "abcdefghijklmnopqrstuvwxyz";

    int c_index = -1;

    for(int i = 0; i < 26; i++){
        if(c == alphabet_capital[i] || c == alphabet_lowercase[i]){
            c_index = i;
        }
    }

    return c_index;
}

void bonus_a_sort_str_array(string* arr, int arr_len){
    for(int i = 0; i < arr_len; i++){
        for(int j = i+1; j < arr_len; j++){
            string i_str = arr[i];
            string j_str = arr[j];

            int min_len = i_str.length() < j_str.length() ? i_str.length() : j_str.length();

            bool flag = false;
            int k = 0;
            while(!flag && k < min_len){
                int i_index = bonus_a_char_index(i_str[k]);
                int j_index = bonus_a_char_index(j_str[k]);
                if(i_index > j_index){
                    bonus_a_swap_str(&arr[i], &arr[j]);
                    flag = true;
                }else if(i_index < j_index){
                    flag = true;
                }
                k++;
            }
        }
    }
}

void bonus_c_display_total_payments(){
    if(!ask_passcode()){
        return;
    }
    fstream ol_file;
    int total_payment = 0;
    ol_file.open("Order List.txt");
    if(ol_file.is_open()){
        string s;
        while(getline(ol_file, s)){
            total_payment += stoi(s.substr(7, s.length()-7));
        }
    }
    cout << "Total payments received: PHP " << total_payment << ".00 \n";
    cout << "\n";
}
