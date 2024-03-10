#include "booking.h"
#include <set>
#include <map>
#include <unordered_set>
#include <array>
#include <thread>
#include <mutex>
#include <iostream>
#include "rapidjson/document.h"
 
using namespace rapidjson;


namespace booking {

constexpr size_t KTheaterSize=20; 
/**
 * @brief backend class, containing memory-based booking table and provides methods to manipulate with
 * 
 */
class BookingBackend
{
public:
    std::vector<Theater> GetTheaterList(const Movie& aMovie) const; 
    std::vector<Movie> GetMovieList(const Theater& aTheater) const;
    std::vector<Seat> GetAvailableSeats(const Theater& aTheater, const Movie& aMovie) const;
    EBookingResult BookTickets(unsigned aNumberOfTickets, const Theater& aTheater, const Movie& aMovie);
    bool IsValid(const Theater& aTheater) const{return true;} //TODO: implement harness
    bool IsValid(const Movie& aMovie) const{return true;} //TODO: implement harness
    bool SerializeIn(const std::string& aJsonFile);
private:
    std::mutex iBookingGuard;

public:
     //TODO: extract cinema and movie lists upon parsing json and store indexes
     /// otherwise boost::multiindex might have been used but simple rdbms like sqlite is the best
    struct CRecord {
            std::map<Movie, std::array<bool, KTheaterSize>> iShows;
        };
private: 
     //assume there's no two cinema with the same name
    std::map<Theater, CRecord> iRecordSet;
};

bool BookingBackend::SerializeIn(const std::string& aJson)
{
    using namespace rapidjson;

    Document doc;
    ParseResult ok = doc.Parse(aJson.c_str());

    if (!ok) {
        std::cout << "Failed to parse input data, error code = " << ok.Code() << std::endl;
        return false;
    }

    iRecordSet.clear();

    for (auto& cinema : doc.GetObject()) {
        if (cinema.value.HasMember("mouvie")) {
            const auto& a = cinema.value.FindMember("mouvie");
            if (a->value.IsString()) {
                CRecord rec;
                rec.iShows.insert({{a->value.GetString()},{}});
                iRecordSet.insert({{cinema.name.GetString()}, rec});
            } else if (a->value.IsArray()) {
                CRecord rec;
                for (SizeType i = 0; i < a->value.Size(); ++i)
                    rec.iShows.insert({{a->value[i].GetString()}, {}});
                iRecordSet.insert({{cinema.name.GetString()}, rec});
            } else {
                //TODO: it looks like JSON has smth unexpected
            }
        } 
    }

    return true;
}

std::vector<Movie> BookingBackend::GetMovieList(const Theater& aTheater) const
{
    std::vector<Movie> ret;
    std::set<Movie>    tmp;
     
    if (!aTheater.iName.length()) { 
        for (auto &rs : iRecordSet) {
            for (auto &s : rs.second.iShows) {
                tmp.insert(s.first);
            }
        }
    } else {
        if(auto rs = iRecordSet.find(aTheater); iRecordSet.end() != rs)
            for (auto &s : rs->second.iShows) {
                tmp.insert(s.first);
            }

    }

    for (auto &it : tmp)
        ret.emplace_back(it);

    return ret;    
}

std::vector<Theater> BookingBackend::GetTheaterList(const Movie& aMovie) const
{
    std::vector<Theater> ret;

    for (auto &rs : iRecordSet)
        if (!aMovie.iName.length() ||  rs.second.iShows.end() != rs.second.iShows.find(aMovie))
            ret.push_back(rs.first);

    return ret;   
} 

std::vector<Seat> BookingBackend::GetAvailableSeats(const Theater& aTheater, const Movie& aMovie) const
{
    std::vector<Seat> ret;

    if (auto r = iRecordSet.find(aTheater); iRecordSet.end() != r)
    {
        if (auto m = r->second.iShows.find(aMovie); r->second.iShows.end() != m) {
            for (unsigned i = 0; i < m->second.size(); ++i)
              if (!m->second[i]) 
                ret.push_back({i});
        }
    }

    return ret;
}

EBookingResult BookingBackend::BookTickets(unsigned aNumberOfTickets, const Theater& aTheater, const Movie& aMovie)
{
    const std::lock_guard<std::mutex> lock(iBookingGuard); //abit pessimistic but guarantees no race 

    if (auto r = iRecordSet.find(aTheater); iRecordSet.end() != r)
    {
        if (auto m = r->second.iShows.find(aMovie); r->second.iShows.end() != m) {
            size_t cnt = 0;
             //estimate free seats count, to ensure the entire order can be placed 
            for (unsigned i = 0; i < m->second.size(); ++i)
                if (!m->second[i]) 
                    ++cnt;

            if (aNumberOfTickets > cnt)
                return EBookingResult::ENoSeatsAvailable;

            cnt = aNumberOfTickets;
            for (unsigned i = 0; i < m->second.size() && cnt; ++i)
                if (!m->second[i]) {
                    m->second[i] = true;
                    --cnt;
                }    

            return EBookingResult::ESuccess;
        } else {
            return EBookingResult::ENoSuchMovie;
        }
    } else {
        return EBookingResult::ENoSuchTheater;
    }

    return EBookingResult::EServerError;
}


static BookingBackend gBackend;
 
bool Booking::Initialize(const std::string& json)
{
    return gBackend.SerializeIn(json);
}

class BookingClientImpl
{
public:
    std::vector<Theater> GetTheaterList() const {return gBackend.GetTheaterList(iMovie);}

    std::vector<Movie> GetMovieList() const {return gBackend.GetMovieList(iTheater);}

    bool SelectTheater(const Theater& aTheater) {
        if (!gBackend.IsValid(aTheater))
            return false;
        iTheater = aTheater;
        return true;
        }

    bool SelectMovie(const Movie& aMovie) {
        if (!gBackend.IsValid(aMovie))
            return false;
        iMovie = aMovie;
        return true;
        }

    std::vector<Seat> GetAvailableSeats() const 
    {
        return gBackend.GetAvailableSeats(iTheater, iMovie);
    }

    EBookingResult BookTickets(unsigned aNumberOfTickets) 
    {
        return gBackend.BookTickets(aNumberOfTickets, iTheater, iMovie);
    }

private:
    /// @brief movie when selected
    Movie iMovie;

    /// @brief a cinema when selected
    Theater iTheater;

};


BookingClient::BookingClient(BookingClientImpl* aImpl):iImpl(aImpl){}

std::unique_ptr<BookingClient> BookingClient::New()
{
   return std::unique_ptr<BookingClient>(new BookingClient(new BookingClientImpl())); 
}

BookingClient::~BookingClient()
{
    delete iImpl;
}

std::vector<Movie> BookingClient::GetMovieList() 
{
    return iImpl->GetMovieList();
}

std::vector<Theater> BookingClient::GetTheaterList() 
{
    return iImpl->GetTheaterList();
}

bool BookingClient::SelectTheater(const Theater& aTheater) 
{
    return iImpl->SelectTheater(aTheater);
}

bool BookingClient::SelectMovie(const Movie& aMovie)
{
    return iImpl->SelectMovie(aMovie);
}

std::vector<Seat> BookingClient::GetAvailableSeats() const
{
    return iImpl->GetAvailableSeats();
}

EBookingResult BookingClient::BookTickets(unsigned aNumberOfTickets)
{
    return iImpl->BookTickets(aNumberOfTickets);
}


};










