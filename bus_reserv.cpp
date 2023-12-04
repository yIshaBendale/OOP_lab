
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

class Reservation 
{
public:
    char name[50];
    char address[50];
    char phone[15];
    int seatNumber;
};

void display_available(int availableseats[]) 
{
    cout << "Available Seats: ";
    for (int i = 0; i < 10; i++) 
    {
        if (availableseats[i] == 0)     //0 for available and 1 for reserved
        {
            cout << i + 1 << " ";
        }
    }
    cout << endl;
    cout << endl;
}

void reserve_seat(int availableseats[]) 
{
    int seatNumber;
    cout << "Enter the seat number you want to reserve (1-10): ";
    cin >> seatNumber;

    if (seatNumber < 1 || seatNumber > 10) 
    {
        cout << "Invalid !\n Please choose a seat between 1 and 10." << endl;
        return;
    }

    if (availableseats[seatNumber - 1] == 0) 
    {
        Reservation reservation;     //reservation is object of Reservtion class

        cout << "Enter your name: ";
        cin.ignore();   // Ignore the newline character in the buffer
        cin.getline(reservation.name, 50);

        cout << "Enter your address: ";
        cin.getline(reservation.address, 50);

        cout << "Enter your phone number: ";
        cin.getline(reservation.phone, 15);

        reservation.seatNumber = seatNumber; //seat numberis assigned to the seatNumber attribute of the reservation object

        ofstream file("reservations.txt", ios::app | ios::binary);
        file.write(reinterpret_cast<char*>(&reservation), sizeof(Reservation));
        file.close();

        availableseats[seatNumber - 1] = 1;  // update array
        cout << endl;
        cout << "Seat reserved successfully!" << endl;
        cout << endl;
    } else 
    {
        cout << "Seat already reserved. Please choose another seat." << endl;
        cout<<endl;
    }
}

void cancel_reservation(int availableseats[]) 
{
    int seatNumber;
    cout << "Enter the seat number you want to cancel (1-10): ";
    cin >> seatNumber;

    if (seatNumber < 1 || seatNumber > 10) 
    {
        cout << "Invalid !!\n Please choose a seat between 1 and 10." << endl;
        return;
    }

    if (availableseats[seatNumber - 1] == 1) 
    {
        Reservation reservation;

        ifstream file("reservations.txt", ios::binary);
        ofstream tempFile("temp.txt", ios::binary);

        while (file.read(reinterpret_cast<char*>(&reservation), sizeof(Reservation))) {
            if (reservation.seatNumber != seatNumber) {
                tempFile.write(reinterpret_cast<char*>(&reservation), sizeof(Reservation));
            }
        }

        file.close();
        tempFile.close();

        remove("reservations.txt");
        rename("temp.txt", "reservations.txt");

        availableseats[seatNumber - 1] = 0;  //uptade array

        cout << "Reservation canceled successfully!" << endl;
    } else 
    {
        cout << "No reservation found for the specified seat." << endl;
    }
}

void display_reservations() 
{
    ifstream file("reservations.txt", ios::binary);
    if (!file) 
    {
        cerr << "No seat is reserved" << endl; //  err = standard error // = cout
        return;
    }

    cout << "Passenger Reservations: " << endl;
    cout << "------------------------" << endl;
    cout<<endl;

    Reservation reservation;
    while (file.read(reinterpret_cast<char*>(&reservation), sizeof(Reservation))) 
    {
        cout << "Passenger Name: " << reservation.name << endl;
        cout << "Seat Number: " << reservation.seatNumber << endl;
        cout << "------------------------" << endl;
    }

    file.close();
}

int main() 
{
    char ch;
    
    //initialise array with 0  i.e. all seats are available
    int availableSeats[10] = {0}; // 0 means seat is available, 1 means seat is reserved

    int choice;
    do {
        cout << "\n:::::::: Bus Reservation System ::::::::" << endl;
        cout << "1. Display Available Seats" << endl;
        cout << "2. Reserve a Seat" << endl;
        cout << "3. Cancel Reservation" << endl;
        cout << "4. Display Reservations" << endl;
        cout <<endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout<<endl;

        switch (choice) 
        {
            case 1:
                display_available(availableSeats);
                break;

            case 2:
                reserve_seat(availableSeats);
                break;

            case 3:
                cancel_reservation(availableSeats);
                break;

            case 4:
                display_reservations();
                break;
            cout<<endl;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        cout<<"Do you want to continue ? (y/n)"<<endl;
        cin>>ch;
    } 
    while (ch=='y');
}