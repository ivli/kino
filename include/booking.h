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
    static std::unique_ptr<BookingClient>  New(const Cinema&, const Mouvie&);

protected:

    BookingClient(const BookingClient&);
    BookingClient(BookingClientImpl* aImpl);
private:
    /// @brief movie when selected
    std::unique_ptr<BookingClientImpl> iImpl;
};

};