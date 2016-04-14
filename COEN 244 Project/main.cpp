#include "Controller.h"
#include "LangermannPoint.h"
#include <limits>// For "numeric_limits" used to ensure valid user input for initial population.
using namespace std;

int main()
{
    // Display title.
    cout<<"Genetic Algorithm!"<<endl;
    cout<<"\tBy Ahmad El-Baher"<<endl;
    cout<<"\tDate: 12/4/16"<<endl;
    cout<<"\tSID: 40000968"<<endl;
    char choice = 'O';


    // Display menu.
    cout<<endl<<endl<<"Choose an application to view."<<endl;
    cout<<"O - Optimization"<<endl;
    cout<<"C - Curve Fitting"<<endl;
    cout<<"Enter the character corresponding to your choice (default = O): ";
    cin>>choice;

    // Set the problem type (application) depending on the user input.
    Controller::setProblemType((choice == 'C' || choice == 'c')? Controller::CURVE_FITTING : Controller::OPTIMIZATION);

    // Request initial population size.
    int initPopSize = 0;
    cout<<endl<<endl<<"Enter an initial population size (minimum = "<<Controller::DEFAULT_POP_SIZE<<"): ";
    cin>>initPopSize;

    // To ensure that the user enters only integers greater than 100.
    while(cin.fail() || initPopSize < Controller::DEFAULT_POP_SIZE)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"Invalid input. Please enter an integer greater than "<<Controller::DEFAULT_POP_SIZE<<": ";
        cin>>initPopSize;
    }

    // Set the initial population size.
    Controller::setInitPopSize(initPopSize);

    // Display result of choices, and allowing user to view them before beginning at his/her will.
    cout<<endl<<endl<<"You have chosen to view "<<((Controller::getProblemType() == Controller::OPTIMIZATION)? "Optimization" : "Curve Fitting");
    cout<<" with an initial population size of "<<Controller::getInitPopSize()<<endl;
    cout<<"Enter a character to begin: ";
    cin>>choice; // Re-using choice to store the character to begin the program.

    // The entire algorithm is implemented in Controller, and is executed through this function.
    Controller::run();

    // This is to prevent the executable from exiting immediately
    cout<<endl<<endl<<"Enter character to exit the program: ";
    cin>>choice; // Re-using choice to store the character to exit the program.

    return 0;
}
