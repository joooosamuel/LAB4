//headers
#include<iostream>
#include<iomanip>
#include <cstdlib> 
using namespace std;

//function and template declaration
void print_real(float number, int fieldspace, int precision);
template <typename T> //template declaration
void update_scale(T& m1, T& m2, T m3 = 10);

int main(void){
    float  a=0; //initialising the float variables
    float b=0; //initialising the float variables
    cout<<" Please input two real numbers: "; //asks the user to enter 2 real numbers
    cin>>a>>b;	//extract data from standard input device

    print_real(a,7,3); //sending the inputted float and parameters to the function
    print_real(b,7,3); //sending the inputted float and parameters to the function

    // call the function update_scale with ONLY the actual parameters a  and b
    update_scale(a, b);

    print_real(a,7,3); //calling the print_real() fucntion after updating the values 'a' and 'b' store
    print_real(b,7,3); //calling the print_real() fucntion after updating the values 'a' and 'b' store
}

//function definition


void print_real(float number, int fieldspace, int precision){//function to print the float in the specified format
    cout << fixed << setprecision(precision) << setw(fieldspace) << number << endl; 
}

// add the function defintion (header+body) of the template function update_scale
template <typename T> //definition for the update_scal() function   
void update_scale(T& m1, T& m2, T m3){
    T m1_1=m1; //storing the original values temproraily
    T m2_1=m2; //storing the original values temproraily

    m1=(m1_1+m2_1)*m3; //updating m1 using the orignal values and m3(10)
    m2=(m1_1-m2_1)*m3; //updating m2 using the orignal values and m3(10)
}

