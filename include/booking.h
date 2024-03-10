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
struct Theater{
    std::string iName;
};

/**
 * @brief represents a mouvie 
 * in future we might want to add other specialization such as lenght, director, etc 
 */
struct Movie{
    std::string iName;
};

inline int operator < (const Movie& aLhs, const Movie& aRhs) {return aLhs.iName < aRhs.iName;}
inline int operator < (const Theater& aLhs, const Theater& aRhs) {return aLhs.iName < aRhs.iName;}
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
    ENoSuchMovie, /// wrong mouvie
    ENoSuchTheater  /// wrong cinema name
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
     /// @brief if no mouvie selected yet it returns a list of all theatres,
     ///        otherwise only ones showing the mouvie 
     /// @return a list of theathres
    std::vector<Theater> GetTheaterList(); 

     /// @brief get a list of movies currently available in all cinemas, 
     ///        if cinema is selectad it returns a list of mouvies shown in this particular cinema
     /// @return a list of mouvies
    std::vector<Movie> GetMovieList();

     /// @brief select Theater for further operations
     /// @return 
    bool SelectTheater(const Theater& aTheater);

     /// @brief selects mouvie for further operations
     /// @param aMovie movie to select
     /// @return true if success otherwise false
    bool SelectMovie(const Movie& aMovie);

     /// @brief returns a list of available seats for previously set Theater and Movie
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