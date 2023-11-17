#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Seat {
private:
    std::string seatNumber;
    bool isBooked;
    int price;

public:
    Seat(const std::string& number, int seatPrice) : seatNumber(number), isBooked(false), price(seatPrice) {}

    void bookSeat() { isBooked = true; }
    void cancelSeat() { isBooked = false; }
    bool isSeatAvailable() const { return !isBooked; }
    std::string getSeatNumber() const { return seatNumber; }
    int getPrice() const { return price; }
};

class Ticket {
private:
    std::string passengerName;
    std::string seatNumber;
    int price;
    bool isConfirmed;

public:
    Ticket(const std::string& name, const std::string& seat, int seatPrice)
        : passengerName(name), seatNumber(seat), price(seatPrice), isConfirmed(false) {}

    void confirmTicket() { isConfirmed = true; }
    void cancelTicket() { isConfirmed = false; }
    bool isTicketConfirmed() const { return isConfirmed; }
    std::string getTicketInfo() const {
        std::stringstream info;
        info << "Passenger: " << passengerName << ", Seat: " << seatNumber << ", Price: $" << price
            << ", Status: " << (isConfirmed ? "Confirmed" : "Not Confirmed");
        return info.str();
    }
};

class Airplane {
private:
    std::string flightNumber;
    std::vector<Seat> seats;
    std::vector<Ticket> tickets;

public:
    Airplane(const std::string& flightNum, const std::vector<Seat>& seatList)
        : flightNumber(flightNum), seats(seatList) {}

    bool bookSeat(const std::string& passengerName, const std::string& seatNumber) {
        for (auto& seat : seats) {
            if (seat.getSeatNumber() == seatNumber && seat.isSeatAvailable()) {
                seat.bookSeat();
                tickets.emplace_back(passengerName, seatNumber, seat.getPrice());
                tickets.back().confirmTicket();
                return true;
            }
        }
        return false;
    }

    void cancelSeat(const std::string& seatNumber) {
        for (auto& seat : seats) {
            if (seat.getSeatNumber() == seatNumber) {
                seat.cancelSeat();
                for (auto& ticket : tickets) {
                    if (ticket.getTicketInfo().find(seatNumber) != std::string::npos) {
                        ticket.cancelTicket();
                        break;
                    }
                }
                break;
            }
        }
    }

    void printTickets() const {
        for (const auto& ticket : tickets) {
            std::cout << ticket.getTicketInfo() << std::endl;
        }
    }
};

int main() {
    std::vector<Seat> seats = { {"1A", 100}, {"1B", 100}, {"2A", 150} };  // Приклад місць
    Airplane airplane("FR123", seats);

    std::string command;
    while (true) {
        std::cout << "Enter command (book, cancel, print, exit): ";
        std::cin >> command;

        if (command == "book") {
            std::string name, seatNumber;
            std::cout << "Enter passenger name: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Enter seat number: ";
            std::cin >> seatNumber;

            if (airplane.bookSeat(name, seatNumber)) {
                std::cout << "Seat booked successfully." << std::endl;
            }
            else {
                std::cout << "Seat booking failed." << std::endl;
            }
        }
        else if (command == "cancel") {
            std::string seatNumber;
            std::cout << "Enter seat number to cancel: ";
            std::cin >> seatNumber;

            airplane.cancelSeat(seatNumber);
            std::cout << "Seat cancellation attempted." << std::endl;
        }
        else if (command == "print") {
            airplane.printTickets();
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "Invalid command." << std::endl;
        }
    }

    return 0;
}
