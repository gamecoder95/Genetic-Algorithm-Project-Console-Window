#include "Controller.h"
#include "LangermannPoint.h"
using namespace std;

int main()
{
    // 'O' for optimization, 'C' for curve fitting.
    cout<<"Genetic Algorithm!"<<endl;
    cout<<"\tBy Ahmad El-Baher"<<endl;
    cout<<"\tDate: 12/4/16"<<endl;
    cout<<"\tSID: 40000968"<<endl;
    char choice = 'O';

    cout<<endl<<endl<<"Choose an application to view."<<endl;
    cout<<"O - Optimization"<<endl;
    cout<<"C - Curve Fitting"<<endl;
    cout<<"Enter the character corresponding to your choice (default = O): ";
    cin>>choice;

    Controller::setProblemType((choice == 'C' || choice == 'c')? Controller::CURVE_FITTING : Controller::OPTIMIZATION);

    int initPopSize = 0;
    cout<<endl<<endl<<"Enter an initial population size (default = "<<Controller::DEFAULT_POP_SIZE<<"): ";
    cin>>initPopSize;

    Controller::setInitPopSize(initPopSize);

    cout<<endl<<endl<<"You have chosen to view "<<((Controller::getProblemType() == Controller::OPTIMIZATION)? "Optimization" : "Curve Fitting");
    cout<<" with an initial population size of "<<Controller::getInitPopSize()<<endl;
    cout<<"Enter a non-space character to begin: ";
    cin>>choice; // Re-using choice to store the character to enter to begin the program.

    Controller::run();

    return 0;
}
