#include <iostream>
#include<cstdio>
#include <vector>
#include <list>
#include <cmath>
#include <iomanip>

using namespace std;



/*
 *  class Call
 * 
 */
class Call{
private:
    string phoneNumber;
    int duration; //in seconds
    
public:
    //constructors
    Call(){
    }
    //destructor
    ~Call(){
        
    }
    //used for setting the phoneNumber and duration when created
    Call(string phoneNumber, int duration){
        this->phoneNumber = phoneNumber;
        this->duration = duration;
    }
    
    //setters and getters
    void setPhoneNumber(int num){
        phoneNumber = num;
    }
    
    string getPhoneNumber(){
        return phoneNumber;
    }
    
    void setDuration(int duration){
        duration = duration;
    }
    
    int getDuration(){
        return duration;
    }
};

class Customer{
private:
    string customerName;
    //store the call information as a vertor of Call objects
    vector<Call> calls;
    double balance;
    
public:
    void setName(string name){
        customerName = name;
    }
    
    string getName(){
        return customerName;
    }
    
    void addCall(string number,double duration){
        //Creates a new Call and saves the data into it
        Call newCall(number, duration);
        //saves the data in the vector
        calls.push_back(newCall);
    }
    
    vector<Call> getCall(){
        return calls;
    }  
    
    void setBalance(double e){
        balance = e;
    }
    
    double getBalance(){
        return balance;
    }
    
    virtual void computeBalance() = 0;
    
};

/*
 *  class Regular Customers
 */
class RegularCustomer : public Customer{    
private:
    double monthlyfee;
    double percall;
public:
    RegularCustomer(){
        monthlyfee = 5;
        percall = 1;
    }
    ~RegularCustomer(){}
    void computeBalance(){
        //gets the total calls the customer made 
        //balance = monthlyfee + percall × num_calls
        setBalance(monthlyfee + percall * getCall().size());
    }
};

/*
 *  class Premium Customers
 */
class PremiumCustomer : public Customer{
private:
    double monthlyfee;
    double permin;
    double num_mins;

public:
    PremiumCustomer(){
        monthlyfee = 30;
        permin = 0.05;
        num_mins = 0;
    }
    ~PremiumCustomer(){}
    void computeBalance(){
        //duration of each call in sec
        int sec = 0;
        for (int i=0; i< getCall().size(); i++) {
            //gets the call duration for each call and adds it to seconds
            Call call = getCall()[i];
            sec += call.getDuration();
        }
        num_mins = (int)sec/60; //converting sec to min
        int round = sec%60; //gets the seconds remaining
        if(round != 0)
            num_mins++; //if there are any seconds remaining add 1 to the min
        
        //balance = monthlyfee + permin × num_mins
        setBalance(monthlyfee + permin * num_mins);
        
    }
};

class Simulation{

public:
    Simulation(){}
    ~Simulation(){}
    void printResult(){
        
        srand(time(NULL)); //seeding the random number sequence (makes it fully random)
        
        
        //creates and stores a list of Premium Customers
        int customersRand = (rand() % 100) + 300;
        
        //creates and stores a list of Premium Customers
        list<Customer*> custListPremium; //list of all the pointer to Premium Customer
        for(int i =0; i<customersRand; i++){
            PremiumCustomer *pCust = new PremiumCustomer();
            custListPremium.push_back(generateCustomer(pCust));//saves to list
        }
        
        //creates and stores a list of Regular Customers
        customersRand = (rand() % 100) + 300;
        list<Customer*> custListRegular; //list of all the pointer to Regular Customer
        for(int i =0; i < customersRand; i++){
            RegularCustomer *rCust = new RegularCustomer();
            custListRegular.push_back(generateCustomer(rCust));//saves to list
        } 
        
        //printing results 
        cout << "Simulation run: " << endl;
        cout << "---------------" << endl;
        cout << "Regular Customers:" << endl << endl;
        double avrBalanceReg = printInfo(custListRegular);
        
        cout << "Premium Customers:" << endl << endl;
        double avrBalancePre = printInfo(custListPremium);
        
        if(avrBalancePre >= avrBalanceReg){
            if(avrBalancePre > avrBalanceReg)
                cout << setprecision(2) << fixed << "Regular customers on average save $"<< avrBalancePre - avrBalanceReg << " compared to Premium customers" <<endl;
            else
                cout << "Both Regular customers and Premium customers have the same average budget" << endl;
        }else
            cout << setprecision(2) << fixed << "Premium customers on average save $"<<  avrBalanceReg - avrBalancePre << " compared to Regular customers" <<endl;
        
        //sorting the lists (balance)
        custListRegular.sort(compareBalance); //sorts regular customers list 
        custListPremium.sort(compareBalance); //sorts regular customers list 
        list<Customer*>::iterator lit;//iterator
        
        //reg min balance
        lit = custListRegular.begin();
        Customer *regMinBalance = *lit;
        
        //reg max balance
        lit = custListPremium.end();
        --lit;
        Customer *regMaxBalance = *lit;
        
        
        //pre min balance
        lit = custListPremium.begin();
        Customer *preMinBalance = *lit;
        
        //pre max balance
        lit = custListPremium.end();
        --lit;
        Customer *preMaxBalance = *lit;
        
    }
    
    //sorts the list
    static bool compareBalance(Customer* first, Customer* second)
    {
        return (first->getBalance()  < second->getBalance());
    }
    
    //prints and calculates all the customers info
    double printInfo(list<Customer*> cc){
        //print
        double totalCustomers = cc.size(); //store the total number of customers
        double avrgCalls = 0;
        double avrgduration = 0; //in sec
        double avrgBalance = 0;
        
        double maxBalance = 0;
        double minBalance = 999999999;
        string maxName;
        string minName;
        
        Call call;
        list<Customer*>::iterator it = cc.begin();
        
        while (it !=cc.end()) {
            Customer *cust = *it;//setting up new customer
            double totalCalls = cust->getCall().size();
            
            //gets the call duration for each call and adds it to avrgduration
            double duration = 0;//total duration of the calls
            for (int i=0; i < totalCalls; i++) {
                Call call = cust->getCall()[i];
                duration += call.getDuration();
            }
            
            //total balance of every customer
            avrgBalance += cust->getBalance();
            
            //finds the max balance customer
            if(cust->getBalance() > maxBalance){
                maxBalance = cust->getBalance();
                maxName = cust->getName();        
            }
            //finds the min balance customer
            if(cust->getBalance() < minBalance){
                minBalance = cust->getBalance();
                minName = cust->getName();        
            }
            
            avrgCalls += totalCalls; //gets the total calls of a customer and adds it to avrgCalls
            avrgduration += duration / totalCalls; //avrg duration per call 
            
            ++it;
        }
        //calculating the averages
        avrgCalls = round(avrgCalls / totalCustomers); //calculates the avrg call per cust
        
        avrgduration = avrgduration / totalCustomers; //avrg duration in sec's
        avrgduration = round(avrgduration / 60); //calculating the duration per cust in mins
        
        avrgBalance = avrgBalance / totalCustomers;
            
        //totalCustomers = 
        
        cout << setprecision(0) << fixed << "\tNumber of customers in the group: " << totalCustomers << endl;
        cout << setprecision(0) << fixed << "\tAverage number of calls/per customer: " << avrgCalls << endl;
        cout << setprecision(0) << fixed << "\tAverage duration of the call/per customer: " << avrgduration << " mins" << endl;
        cout << setprecision(2) << fixed << "\tAverage balance/per customer: $" <<  avrgBalance << endl;
        cout << setprecision(2) << fixed << "\tCustomer with largest balance: " << maxName << "($" << maxBalance << ")" << endl;
        cout << setprecision(2) << fixed << "\tCustomer with smallest balance: " << minName << "($" << minBalance << ")" << endl;
        cout << endl;
        
        return avrgBalance;
    }
  
    //creates random customers for premium customers or regular customer and passes a vector
    Customer* generateCustomer(Customer *cu){
        //saving all the values of customer
        int noOfCalls = (rand() % 290) + 10;
        //cout << noOfCalls << endl;
        cu->setName(generateName());
        for(int i = 0; i < noOfCalls; i++)
            cu->addCall(generateNumber(), generateDuration());
        cu->computeBalance();
        
        return cu;
    }
    
    //gets random full name
    string generateName(){
            int i=0;
            string name = "";
            
            //random for first name and last name
            for(int a = 0; a < 2; a++){//fname and lname
                char n[7] = "";//name array
                int lettersRand = (rand() % 3) + 4; //number of letters
                
                n[0] = (rand() % 25) + 65;  //letter for the first letter in the name (capital)
                for (i = 1; i < lettersRand  ; i++)
                    n[i] = (rand() % 25) + 97;  //letters for the rest of the First name
                string fname(n);//store the array to a string
                name += fname + " ";
            }
            return name;   
    }
    
    //gets a random phone number
    string generateNumber(){
        char number[11];
        for(int i = 0; i < 10; i++){ //random 10digit phone number
            int num = (rand() % 10) + 48 ; 
            number[i] = num;
        }
        
        string pNumber(number);
        
        return pNumber;
    }
    
    //gets a random duration
    int generateDuration(){
        int duration = (rand() % 7180) + 20; //random number of duration. from 20 seconds to 2 hours (7200 sec)
        return duration;
    }
};

int main(int argc, char** argv) {
    
    Simulation *sim = new Simulation();
    sim->printResult();
    delete sim;
    
    return 0;
}
