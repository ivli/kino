#include "booking.h"
#include <thread>
#include <mutex>

namespace booking{


enum class EBookingResult
{
    ESuccess,
    ENoSeatsAvailable,
    EBadArg,
    ENoSuchMouvie,
    ENoSuchCinema
};


class BookingBackend
{

public:
    std::vector<Mouvie> GetMouvieList() const;
    std::vector<Cinema> GetCinemaList() const; 
    std::vector<Seat> GetAvailableSeats(const Cinema& aCinema, const Mouvie& aMouvie) const;

    bool BookTickets(unsigned aNumberOfTickets, const Cinema& aCinema, const Mouvie& aMouvie);

    bool IsValid(const Cinema& aCinema) const{return true;} //TODO: implement harness
    bool IsValid(const Mouvie& aMouvie) const{return true;} //TODO: implement harness
private:
    std::mutex iOverbookingGuard;
};


static BookingBackend gBackend;
 

class BookingClientImpl
{
public:

    static std::vector<Mouvie> GetMovieList() {return gBackend.GetMouvieList();}

    static std::vector<Cinema> GetCinemaList() {return gBackend.GetCinemaList();}

    bool SelectCinema(const Cinema& aCinema) {
        if (!gBackend.IsValid(aCinema))
            return false;
        iCinema=aCinema;
        return true;
        }

    bool SelectMovie(const Mouvie& aMovie) {
        if (!gBackend.IsValid(aMovie))
            return false;
        iMouvie=aMovie;
        return true;
        }

    std::vector<Seat> GetAvailableSeats() const;

    bool BookTickets(unsigned aNumberOfTickets);

private:
    /// @brief movie when selected
    Mouvie iMouvie;

    /// @brief a cinema when selected
    Cinema iCinema;

};


BookingClient::BookingClient(BookingClientImpl* aImpl):iImpl(aImpl){}

std::unique_ptr<BookingClient> BookingClient::New()
{
   return std::unique_ptr<BookingClient>(new BookingClient(new BookingClientImpl())); 
}



};










