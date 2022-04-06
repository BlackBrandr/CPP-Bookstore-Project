#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;
/*
Our Global variables that we use. The profit variable is to see the daily profit of the bookstore. ManagerID and password
variable is used to see the profit.
*/
int profit;
string ManagerID = "Manager";
int password = 123456;

/*
Our first structure it has private data fields and public methods to access it from the outside.
*/
struct Customer {
private:
    string customer_name;
    string requested_book;
    int needed_copies;
    int money_spent;
public:
    Customer(string cname, string rqbook, int copies){ //It has a constructor method, so when the object is created it takes arguments.
        this->customer_name = std::move(cname);
        this->requested_book = std::move(rqbook);
        this->needed_copies = copies;
    }

public: int getMoneySpent() {  //To get the money_spent variable from outside the struct.
        return this->money_spent;
    }

public: void setMoneySpent(int prc) { //to set money_spent after the object is created.
        this->money_spent = prc;
    }


public: string getcustomername() { //To get the customer_name from outside the struct.
        return this->customer_name;
    }

public: string getrequestedbook() { //To get the requested_book from outside the struct
        return this->requested_book;
    }

public: int getneededcopies() const { //To get the needed_copies from outside the struct
        return this->needed_copies;
    }

};
/*
The second struc we have in this code. It is the Book object it also has private data fields and public methods.
*/
struct Book{
private:
    string book_name;
    int price;
    int number_of_available_copies;
public:
    Book(string bname, int pr, int copies){ //Our constructor method.
        this->book_name = std::move(bname);
        this->price = pr;
        this->number_of_available_copies = copies;
    }

public: string getBookName() { //Get method to see the book_name variable.
        return this->book_name;
    }

public: int getPrice() const { //Get method to see the price variable.
        return this->price;
    }

public: int getCopies() const { //Get method to see the number_of_available_copies variable.
        return this->number_of_available_copies;
    }

public: void removeCopies(int copy) { //A method to subtract copies from the number_of_available_copies variable.
        this->number_of_available_copies -= copy;
    }

public: void addCopies(int copy) { //A method to add copies to the number_of_available_copies
        this->number_of_available_copies += copy;
    }
};


/*
Our main structure which has private vectors and queues, and public methods.
*/
struct BookStore {
private:
    vector<Book> book_list;
    queue<Customer> customer_queue;
    vector<Book> new_deliveries;
    vector<Customer> sold_books_log;

public:
    vector<Book> getBookList(){ //Get method to see the book_list vector.
        return this->book_list;
    }

    queue<Customer> getCustomerQueue(){ //Get method to see the customer_queue queue.
        return this->customer_queue;
    }

    vector<Book> getDeliveries(){ //Get method to see the new_deliveries vector.
        return this->new_deliveries;
    }

    vector<Customer> getLog(){ //Get method to see the sold_books_log vector.
        return this->sold_books_log;
    }

    void addLog(const Customer& CU){ //A method to add a customer to the log of previous customers.
        this->sold_books_log.push_back(CU);
    }

    void addDeliveries(const Book& BO){ //A method to add an incoming delivery into the new_deliveries vector.
        this->new_deliveries.push_back(BO);
    }

    void addCustomer(const Customer& CU){ //A method to add customers to the customer_queue.
        this->customer_queue.push(CU);
    }

    int addBook(Book BO){  //A method to add new books into our inventory than write that inventory into our book_inventory.txt
        for(auto & i : this->book_list){
            if(BO.getBookName() == i.getBookName()) {
                i.addCopies(BO.getCopies());
                writeBookInventory();
                return -1;
            }
        }
        this->book_list.push_back(BO);
        writeBookInventory();
        return -1;
    }

    void rememberInventory(){  //A method which we call the get the inventory from the txt file.
        ifstream file;
        file.open("book_inventory.txt", ios::in);

        if(file.is_open()) {
            while(!file.eof()){
                string bk_nm;
                int bk_cp;
                int bk_pr;
                file >> bk_nm >> bk_cp >> bk_pr;
                Book b1(bk_nm,bk_pr,bk_cp);
                addBook(b1);
            }
        }
    }

    void rememberDelivery() { //A method which we call to remember our waiting deliveries.
        ifstream file2;
        file2.open("delivery_log.txt", ios::in);

        if(file2.is_open()){
            while(!file2.eof()){
                string bk_nm;
                int bk_cp;
                int bk_pr;
                file2 >> bk_nm >> bk_cp >> bk_pr;
                Book b2(bk_nm, bk_pr, bk_cp);
                addDeliveries(b2);
            }
        }
    };

    void rememberCustomer(){ //A method which we call to get the customer log.
        ifstream file3;
        file3.open("cus_log.txt", ios::in);

        if(file3.is_open()){
            while(!file3.eof()){
                string cus_name;
                string bk_name;
                int bk_cp;
                int cus_spent;
                file3 >> cus_name >> bk_name >> bk_cp >> cus_spent;
                Customer c1(cus_name, bk_name, bk_cp);
                c1.setMoneySpent(cus_spent);
                addLog(c1);
            }
        }
    }



    void processDelivery() { //A method to process the waiting delivery.
        if (this->new_deliveries.empty())
            cout << "No deliveries to be delivered" << endl;
        else {
            Book nb = this->new_deliveries.front();
            this->new_deliveries.erase(this->new_deliveries.begin());
            this->addBook(nb);
            profit = profit - (nb.getPrice() * nb.getCopies());
            cout << "Delivery completed" << endl;
            writeBookInventory();
            writeDeliveries();
        }
    }
    void writeDeliveries(){  //A process to write the deliveries into a text file.
        fstream myFile2;
        myFile2.open("delivery_log.txt", ios::out);
        if(myFile2.is_open()){
            for(int i = 0; i < (this->new_deliveries.size()); i++){
                if(i != 0){
                    myFile2 << endl;
                }
                myFile2 << this->getDeliveries()[i].getBookName() << " " << this->getDeliveries()[i].getCopies() << " "
                        << this->getDeliveries()[i].getPrice();
            }
            myFile2.close();
        }
    }

    void writeLog(){ //A method to write the sold books log to a text file.
        fstream myFile;
        myFile.open("cus_log.txt", ios::out);
        if(myFile.is_open()){
            for(int i = 0; i < this->sold_books_log.size(); i++){
                if (i != 0){
                    myFile << endl;
                }
                myFile << this->getLog()[i].getcustomername() << " " << this->getLog()[i].getrequestedbook() <<
                       " " << this->getLog()[i].getneededcopies() <<" " << this->getLog()[i].getMoneySpent();
            }
            myFile.close();
        }
    }
    void writeBookInventory(){ //A method to write the book inventory into a text file.
        fstream myFile3;
        myFile3.open("book_Inventory.txt", ios::out);
        if(myFile3.is_open()){
            for(int i = 0; i < this->book_list.size(); i++){
                if(i != 0){
                    myFile3 << endl;
                }
                myFile3 << this->getBookList()[i].getBookName() << " " << this->getBookList()[i].getCopies() <<
                        " " << this->getBookList()[i].getPrice();
            }
            myFile3.close();
        }
    }

    void processCustomer() {  //To process the customers waiting in the queue the nf variable is used like a checkpoint.
        if (this->customer_queue.empty())  //It has 5 different outcomes.
            cout << "No customers to serve" << endl;
        else {
            int nf = 0;
            Customer obj = this->customer_queue.front();
            this->customer_queue.pop();
            int val2 = this->book_list.size();
            if (val2 == 0) {
                cout << "There are no books in the bookstore" << endl;
            }
            else {
                for (int i = 0; i < this->book_list.size(); i++) {
                    if (this->book_list[i].getBookName() == obj.getrequestedbook()) {
                        if (this->book_list[i].getCopies() >= obj.getneededcopies()) {
                            cout << "Request successfully processed." << endl;
                            obj.setMoneySpent(this->book_list[i].getPrice());
                            profit = profit + (this->book_list[i].getPrice())*obj.getneededcopies();
                            this->addLog(obj);
                            this->writeLog();
                            this->book_list[i].removeCopies(obj.getneededcopies());
                            writeBookInventory();
                            nf = 0;
                            break;

                        }
                        else {
                            nf = 1;
                            cout << "Not enough copies in the bookstore" << endl;
                        }
                    }
                    if(i == (val2-1) && nf == 0){
                            cout << "The book you have requested doesn't exist in the bookstore." << endl;
                    }
                }
            }
        }
    }
};

int main(){ //Our main method.
    BookStore bs; //We create the bookstore object.
    bs.rememberInventory();  //We remember the inventory from the text file.
    bs.rememberDelivery();  //We remember the delivery line from the text file.
    bs.rememberCustomer();  //We remember the sold books log from the text file.

    /*
    We add some customers to the line.
    */
    bs.addCustomer(Customer("Berkin", "Anna_Karenina",1));
    bs.addCustomer(Customer("Burak", "Çalıkuşu", 2));
    bs.addCustomer(Customer("Ibrahim", "Zamir", 3));
    bs.addCustomer(Customer("Hasan", "Denizden", 3));
    bs.addCustomer(Customer("Buse", "Talimatlar", 1));
    bs.addCustomer(Customer("Elif", "Mecburiyet", 2));
    bs.addCustomer(Customer("Mehmet", "Fink", 5));
    bs.addCustomer(Customer("Zehra", "Körlük", 5));
    bs.addCustomer(Customer("Kemal", "Kavgam", 1));
    bs.addCustomer(Customer("Kubra", "Aya_Yolculuk", 2));
    bs.addCustomer(Customer("Buket", "Sisifos_Söyleni", 7));
    bs.addCustomer(Customer("Umut", "Otomatik_Portakal", 9));
    bs.addCustomer(Customer("Serdar", "Gocebeler_Diyari", 1));

    /*
    We add some deliveries into the delivery line.
    */
    bs.addDeliveries(Book("Satranç", 15,2));
    bs.addDeliveries(Book("Babilin_En_Zengin_Adamı", 15,2));
    bs.addDeliveries(Book("Cin_Ali_Okulda", 15,2));
    bs.addDeliveries(Book("Steve_Job", 15,2));
    bs.addDeliveries(Book("Elon_Musk", 15,2));
    bs.addDeliveries(Book("Daha_Adil_Bir_Dünya_Mümkün", 15,2));
    bs.addDeliveries(Book("Nutuk", 15,2));


    //The user interface we create for the user of the program. We use a while loop and a switch case. User must input 7 to break the loop.
    while (true){
        cout << "Main Menu" << endl;
        cout << "1.Add a new customer into the store" << endl;
        cout << "2.Process a customer's request" << endl;
        cout << "3.Add a New Delivery to the store" << endl;
        cout << "4.Process a new delivery" << endl;
        cout << "5.See daily profit" << endl;
        cout << "6.See customer line" << endl;
        cout << "7.Exit" << endl;

        int opt;
        cin >> opt;
        cout << endl;

        if (opt == 7) {
            cout << "Program is closing";
            break;
        } else
            switch (opt) {
                case 1: {
                    cout << "Enter name of customer" << endl;
                    string CU_Name;
                    cin >> CU_Name;
                    cout << "Enter requested book seperated by underscores(_)" << endl;
                    string BO_name;
                    cin >> BO_name;
                    cout << "Enter number of copies needed" << endl;
                    int CO_num;
                    cin >> CO_num;
                    bs.addCustomer(Customer(CU_Name, BO_name, CO_num));
                    break;
                }
                case 2: {
                    bs.processCustomer();
                    break;
                }
                case 3: {
                    cout << "Please enter number of delivery items" << endl;
                    int item;
                    cin >> item;
                    for (int i = 0; i < item; i++) {
                        cout << "Enter book name seperated by underlines(_)" << endl;
                        string BO_N;
                        cin >> BO_N;
                        cout << "Enter price" << endl;
                        int Price;
                        cin >> Price;
                        cout << "Enter number of copies delivering" << endl;
                        int Copy;
                        cin >> Copy;

                        Book BO(BO_N, Price, Copy);             // Burada hata olabilir
                        bs.addDeliveries(BO);
                        bs.writeDeliveries();
                    }
                    break;
                }
                case 4: {
                    bs.processDelivery();
                    break;
                }
                case 5: {
                    string ManagerID_in;
                    int password_in;

                    cout << "Please enter Manager ID" << endl;
                    cin >> ManagerID_in;
                    cout << "Please enter password" << endl;
                    cin >> password_in;

                    if ((ManagerID_in == ManagerID) && (password_in == password)) {
                    cout << "Daily profit is: " << profit << endl;
                }

                    else{
                        cout << "Unauthorized access. Please contact Manager" << endl;
                    }
                    break;
                }
                case 6:{
                    queue<Customer> qCus = bs.getCustomerQueue();
                    int size_of_queue = qCus.size();
                    for(int i = 0; i < size_of_queue; i++){
                        cout << "Name: " << qCus.front().getcustomername() << endl;
                        qCus.pop();

                    }
                }
            }
    }
}
