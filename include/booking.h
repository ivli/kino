#pragma once

#include <set>
#include <vector>
#include <string>
#include <map>
#include <memory>

namespace booking{
/**
 * @brief represents a cinema 
 * in future we might want to add other specialization such as address, phone, etc 
 */
struct Cinema{
    std::string iName;
};

/**
 * @brief represents a mouvie 
 * in future we might want to add other specialization such as lenght, director, etc 
 */
struct Mouvie{
    std::string iName;
};

inline int operator < (const Mouvie& aLhs, const Mouvie& aRhs) {return aLhs.iName < aRhs.iName;}
inline int operator < (const Cinema& aLhs, const Cinema& aRhs) {return aLhs.iName < aRhs.iName;}
/**
 * @brief represents a mouvie 
 * in future we might want to add other specialization such as price 
 */
struct Seat{
    unsigned iNo;
};

/// @brief result of booking operation
enum class EBookingResult
{
    ESuccess,    ///seats successfully booked
    EServerError, ///http 5xx
    ENoSeatsAvailable, /// no seats available
    EBadArg,       /// bad argument say wrong number of seats requested
    ENoSuchMouvie, /// wrong mouvie
    ENoSuchCinema  /// wrong cinema name
};

//forward declaration of an implementation class
class BookingClientImpl;

/**
 * @brief represents a booking service client 
 * 
 */
class BookingClient
{
public:
     /// @brief get a list of movies currently available in cinemas 
     /// @return 
    static std::vector<Mouvie> GetMouvieList();

     /// @brief get a list of cinemas
     /// @return 
    static std::vector<Cinema> GetCinemaList(); 

     /// @brief select Cinema for further operations
     /// @return 
    bool SelectCinema(const Cinema& aCinema);
     /// @brief 
     /// @return 
    bool SelectMovie(const Mouvie& aMovie);
     /// @brief returns a list of available seats for previously set Cinema and Mouvie
     /// @return 
    std::vector<Seat> GetAvailableSeats() const;
     /// @brief actually books tickets
     /// @param aNumberOfTickets number of tickets to book
     /// @return see \@EBookingResult
    EBookingResult BookTickets(unsigned aNumberOfTickets);
     /// @brief factory method
     /// @return  new client, bound to a backend
    static std::unique_ptr<BookingClient> New();
    /// @brief 
    ~BookingClient();
protected:
    BookingClient(const BookingClient&) = delete;
    BookingClient(BookingClientImpl* aImpl);
private:
    /// @brief  hidden implementation class
    BookingClientImpl* iImpl=nullptr;
};

/**
 * @brief a static class to initialize service 
 * 
 */
class Booking {
public:
    static bool Initialize(const std::string& json);
};

};