#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#define  ll long long
using  namespace std;

struct cosa{
    string cosa1;
    string cosa2;
    int number1;

};

int main() {

    ofstream salida("example1.txt" , ios::out);

    string cosa1;
    string cosa2;
    int number1;
    char sep='|';
    for (int i = 0; i <3 ; ++i)
    {   cin>>cosa1>>cosa2>>number1;
        salida<<cosa1<<sep<<cosa2<<sep<<number1<<"\n";
    }

    salida.close();

    ifstream entrada("example1.txt" ,ios::in );

    if(!entrada)
    {
        cerr<<"not found"<<endl;
        exit(1);
    }

    string alfa;
    string beta;
    int teta;
    char sepa;
    string linea;
    while (getline(entrada , linea)) {

    cout<<linea<<endl;
    }




    return 0;


}