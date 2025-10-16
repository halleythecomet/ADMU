#define MANAGER_PASSCODE 123
#define MAX_ATTEMPTS 3
 
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;
 
string int_to_fcn(int cn_int);
bool ask_passcode();
string choice_cin(const string options[], int options_len);
int get_current_cn();
string task1_ask_payment_method();
void task1_get_order();
void task2_check_availability(string ac_arr[], string meals_arr[][4], int num_ac, int num_meals, string cn_str, string payment_method);
void task2_add_order(string ac_arr[], string meals_arr[][4], int num_ac, int num_meals, string cn_str, string payment_str, string payment_method);
void task2_display_one_order();
void task2_display_all_orders();
int task3_find_line_num(string cn_str);
void task3_remove_line(int line_to_delete);
void task3_delete_order();
void bonus_display_total_payment();
void swap_str(string *s1, string *s2);
int char_index(char c);
void sort_str_array(string* arr, int arr_len);


const string payment_options[3] = {"Cash", "Credit Card", "GCash"};
const string size_options[3] = {"Regular", "Medium", "Large"};
const string yesno_options[2] = {"Yes", "No"};

const int payment_options_len = 3;
const int size_options_len = 3;
const int yesno_options_len = 2;

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
 
bool ask_passcode(){
    string passcode_attempt;
    for(int i = 0; i < MAX_ATTEMPTS; i++){
        cout << "Input manager passcode: (" << 3-i << " attempt/s remaining): ";
        getline(cin >> ws, passcode_attempt);
        if(stoi(passcode_attempt) == MANAGER_PASSCODE){
            return true;
        }
    }
    cout << "Manager login failed. Returning.\n";
    return false;
}

string choice_cin(const string options[], int options_len){
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
            cout << "Invalid option, try again" << endl;
        }
    }   
    
    return choice;
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

void task1_get_order(){
    string cn_str = int_to_fcn(get_current_cn());
    int num_ac = 0;
    int num_meals = 0;
    int num_items = 0;
 
    fstream temp_file;
    temp_file.open("temp.txt", ios::out);
    if(temp_file.is_open()){
        cout << "Hello, welcome to Jolibee! May I take your order?\n";
  
        cout << "What items would you like to add?\nType N if you have nothing more to add\n";
        string item_input = "";
        string meal_string = "";
        while(item_input != "N"){
            cout << num_items+1 << ": ";
            getline(cin >> ws, item_input);
            
            if(item_input != "N"){
                string drink = "";
                cout << "Do you want a drink with that order? ";
                if(choice_cin(yesno_options, yesno_options_len) == "Yes"){
                    cout << "What drink would you like? ";
                    getline(cin >> ws, drink);
                    cout << "What size? ";
                    drink = choice_cin(size_options, size_options_len) + " " + drink;
                }
                
                string fries = "";
                cout << "Do you want fries with that order? ";
                if(choice_cin(yesno_options, yesno_options_len) == "Yes"){
                    cout << "What size? ";
                    fries += choice_cin(size_options, size_options_len);
                    fries += " Fries";
                }
                
                if(drink != "" && fries != ""){
                    meal_string = item_input + " with " + fries + " and " + drink;
                }else if(drink == ""){
                    meal_string = item_input + " with " + fries;
                }else if(fries == ""){
                    meal_string = item_input + " with " + drink;
                }
                
                if(drink == "" && fries == ""){
                    meal_string = item_input;
                    num_ac++;
                }else{
                    num_meals++;
                } 
                
                temp_file << meal_string << "\n";
                temp_file << item_input << "\n";
                temp_file << fries << "\n";
                temp_file << drink << "\n";
                
                num_items++;
            }
        }
        temp_file.close();
    }
    
    string ac_arr[num_ac];
    string meals_arr[num_meals][4];
    string order_arr[num_items][4];
    temp_file.open("temp.txt", ios::in);
    if(temp_file.is_open()){
        for(int i = 0; i < num_items; i++){
            for(int j = 0; j < 4; j++){
                getline(temp_file, order_arr[i][j]);
            }
        }
        temp_file.close();
    }
    
    int cur_ac_index = 0;
    int cur_meals_index = 0;
    for(int i = 0; i < num_items; i++){
        if(order_arr[i][2] == "" && order_arr[i][3] == ""){
            ac_arr[cur_ac_index] = order_arr[i][0];
            cur_ac_index++;
        }else{
            meals_arr[cur_meals_index][0] = order_arr[i][0];
            meals_arr[cur_meals_index][1] = order_arr[i][1];
            meals_arr[cur_meals_index][2] = order_arr[i][2];
            meals_arr[cur_meals_index][3] = order_arr[i][3];
            cur_meals_index++;
        }
    }
    


    cout << "How would you like to pay? ";
    string payment_method = choice_cin(payment_options, payment_options_len);

    task2_check_availability(ac_arr, meals_arr, num_ac, num_meals, cn_str, payment_method);
}

void task2_check_availability(string ac_arr[], string meals_arr[][4], int num_ac, int num_meals, string cn_str, string payment_method){
    cout << "Checking availability of all food items.\n";
    if(!ask_passcode()){
        return;
    }
    int payment = 0;
    bool ac_availablility[num_ac];
    bool meals_availability[num_meals];
    bool all_available = true;
    
    int input_stock;
    cout << "Ala carte order: \n";
    if(num_ac == 0){
        cout << "No ala carte orders were inputted\n";
    }
    for(int i = 0; i < num_ac; i++){
        cout << "How much stock is available for the " << ac_arr[i] << "?\n";
        cin >> input_stock;
        
        if(input_stock != 0){
            ac_availablility[i] = true;
        }else{
            ac_availablility[i] = false;
            all_available = false;
        }
        //temp pricing for items
        payment += 100;
        
        cout << "\n";
    }
    
    
    cout << "Meal order: \n";
    if(num_meals == 0){
        cout << "No meal orders were inputted\n";
    }
    for(int i = 0; i < num_meals; i++){
        for(int j = 1; j <= 3; j++){
            bool flag = false;
            if(meals_arr[i][j] != ""){
                cout << "How much stock is available for the " << meals_arr[i][j] << "?\n";
                cin >> input_stock;
        
                if(input_stock != 0 && !flag){
                    meals_availability[i] = true;
                }else{
                    meals_availability[i] = false;
                    all_available = false;
                    flag = true;
                }
                           
                //temp pricing for items 
                if(j == 1){
                    payment += 100;
                }else{
                //temp pricing for sides
                    payment += 50;
                }
                cout << "\n";
            }
        }
    }
    
    if(!all_available){
        cout << "Some meals were not available! \n\n";
        
        cout << "Your order:\n";
        cout << "Ala carte order: \n";
        for(int i = 0; i < num_ac; i++){
            cout << ac_arr[i];
            if(ac_availablility[i] == false){
                cout << " - NOT AVAILABLE";
            }
            cout << "\n";
        }
        
          
        cout << "Meal order: \n";
        for(int i = 0; i < num_meals; i++){
           cout << meals_arr[i][0];
            if(meals_availability[i] == false){
                cout << " - NOT AVAILABLE";
            }
            cout << "\n";
        }
        
        cout << "Please order again. \n\n";
        
        task1_get_order();
    }else{
        cout << "Order was successful!\n\n";
        
        string payment_str = to_string(payment) + ".00";
        task2_add_order(ac_arr, meals_arr, num_ac, num_meals, cn_str, payment_str, payment_method);
    }
}


void task2_add_order(string ac_arr[], string meals_arr[][4], int num_ac, int num_meals, string cn_str, string payment_str, string payment_method){
    fstream ol_file;
    
    ol_file.open("Order List.txt", ios::app);
    if(ol_file.is_open()){
        ol_file << cn_str << " " << payment_str << '\n';
        ol_file.close();
    }
    
    fstream cn_file;
    cn_file.open(cn_str+".txt", ios::out);
    if(cn_file.is_open()){
        cn_file << "Type of payment: " << payment_method << "\n";
        
        cn_file << "Ala carte order: \n";
        for(int i = 0; i < num_ac; i++){
            cn_file << i+1 << ". " << ac_arr[i] << "\n";    
        }
        
        cn_file << "Meal order: \n";
        for(int i = 0; i < num_meals; i++){
            cn_file << i+1 << ". " << meals_arr[i][0] << "\n";    
        }
        
        cn_file << cn_str << "\n";
        cn_file << "Total payment: " << payment_str;
        cn_file.close();
    }
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
        while(cn_file.get(c)){
            cout << c;
        }
        
        
        ol_file.close();
        cn_file.close();
    }else{
        cout << "No orders available \n";
    }
    
    cout << endl;
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
    
    cout << endl;
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
    }
    ol_file.close();
    
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
                temp_str += (s + "\n");
            }
            cur_line_num++;
        }
    }
    ol_file.close();
    
    ol_file.open("Order List.txt", ios::out);
    ol_file << temp_str;
    ol_file.close();
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
                cout << "Invalid customer number, try again \n";
            }
        }
        cn_file.close();
        
        cout << "Are you sure you want to delete this file? \n";
        bool is_sure = choice_cin(yesno_options, yesno_options_len) == "Yes";

        if(is_sure){
            if(remove((cn_str + ".txt").c_str()) == 0){
                cout << "File deleted successfully. \n";
            }else{
                cout << "Error when deleting file. \n";
            }
        }
        ol_file.close();
        task3_remove_line(task3_find_line_num(cn_str));
    }else{
        cout << "No orders available\n";
    }
}

void bonus_display_total_payment(){
    fstream ol_file;
    double total_payment = 0;
            
    ol_file.open("Order List.txt", ios::in);
    if(ol_file.is_open()){
        string s;
        while(getline(ol_file, s)){
            total_payment += stod(s.substr(6, s.length()-3-5));
        }
    }
    ol_file.close();
    
    cout << "Total payment: " << total_payment << endl;
}

void swap_str(string *s1, string *s2){
    string temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}
 
int char_index(char c){
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
 
void sort_str_array(string* arr, int arr_len){
    for(int i = 0; i < arr_len; i++){
        for(int j = i+1; j < arr_len; j++){
            string i_str = arr[i];
            string j_str = arr[j];
 
            int min_len = i_str.length() < j_str.length() ? i_str.length() : j_str.length();
 
            bool flag = false;
            int k = 0;
            while(!flag && k < min_len){
                int i_index = char_index(i_str[k]);
                int j_index = char_index(j_str[k]);
                if(i_index > j_index){
                    swap_str(&arr[i], &arr[j]);
                    flag = true;
                }else if(i_index < j_index){
                    flag = true;
                }
                k++;
            }
        }
    }
}
 
