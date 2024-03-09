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
    static std::vector<Mouvie> GetMovieList();

     /// @brief get a list of cinemas
     /// @return 
    static std::vector<Cinema> GetCinemaList(); 

     /// @brief select Cinema for further operations
     /// @return 
    bool SelectCinema(const Cinema& aCinema);
     /// @brief 
     /// @return 
    bool SelectMovie(const Mouvie& aMovie);
     /// @brief 
     /// @return 
    std::vector<Seat> GetAvailableSeats() const;
     /// @brief 
     /// @return 
    bool BookTickets(unsigned aNumberOfTickets);

    static std::unique_ptr<BookingClient>  New();

protected:
    BookingClient(const BookingClient&) = delete;
    BookingClient(BookingClientImpl* aImpl);
private:
    /// @brief 
    std::unique_ptr<BookingClientImpl> iImpl;
};

};