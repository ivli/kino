#include "booking.h"
#include <set>
#include <unordered_set>
#include <array>
#include <thread>
#include <mutex>
#include "rapidjson/document.h"
 
using namespace rapidjson;

namespace booking {

constexpr size_t KCinemaSize=20; 

class BookingBackend
{
public:
    std::vector<Mouvie> GetMouvieList() const;
    std::vector<Cinema> GetCinemaList() const; 
    std::vector<Seat> GetAvailableSeats(const Cinema& aCinema, const Mouvie& aMouvie) const;
    EBookingResult BookTickets(unsigned aNumberOfTickets, const Cinema& aCinema, const Mouvie& aMouvie);
    bool IsValid(const Cinema& aCinema) const{return true;} //TODO: implement harness
    bool IsValid(const Mouvie& aMouvie) const{return true;} //TODO: implement harness
    bool SerializeIn(const std::string& aJsonFile);
private:
    std::mutex iOverbookingGuard;
    bool iInitialised = false;
private: 
     //TODO: it has to be normalized
    struct CRecord {
        Cinema iCinema;
        Mouvie iMouvie; //it's better to have index to a movie list
        std::array<Seat, KCinemaSize> iSeats;
    };

    std::set<CRecord> iRecordSet;
};

bool BookingBackend::SerializeIn(const std::string& aJsonFile)
{
    static const char* KTypeNames[] = 
    { "Null", "False", "True", "Object", "Array", "String", "Number" };
    rapidjson::Document doc;
    doc.Parse(aJsonFile.c_str());

    for (auto& m : doc.GetObject())
        printf("Type of member %s is %s\n",
            m.name.GetString(), KTypeNames[m.value.GetType()]);


    return true;
}

std::vector<Mouvie> BookingBackend::GetMouvieList() const
{
    std::vector<Mouvie> ret;
    
     //so stupid :-) if ther's a time it might have sense to implement caching 
    for (auto &rs : iRecordSet)
        ret.push_back(rs.iMouvie);

    return ret;    
}

std::vector<Cinema> BookingBackend::GetCinemaList() const
{
    std::vector<Cinema> ret;
    
     //so stupid :-) if ther's a time it might have sense to implement caching 
    for (auto &rs : iRecordSet)
        ret.push_back(rs.iCinema);

    return ret;   
} 

std::vector<Seat> BookingBackend::GetAvailableSeats(const Cinema& aCinema, const Mouvie& aMouvie) const
{
    std::vector<Seat> ret;
    return ret;
}

EBookingResult BookingBackend::BookTickets(unsigned aNumberOfTickets, const Cinema& aCinema, const Mouvie& aMouvie)
{


    return EBookingResult::EServerError;
}


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

    std::vector<Seat> GetAvailableSeats() const {
        return gBackend.GetAvailableSeats(iCinema, iMouvie);
    }

    EBookingResult BookTickets(unsigned aNumberOfTickets) {
        return gBackend.BookTickets(aNumberOfTickets, iCinema, iMouvie);
    }

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










