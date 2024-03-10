#include "gtest/gtest.h"
#include "../include/booking.h"
#include <string>
#include <thread>
#include <chrono>
#include "testdata.h"


namespace {
using namespace booking;
using namespace testdata;

TEST(Backend, InitializeBadData) 
{
    EXPECT_EQ(false, booking::Booking::Initialize(BADJSON));
}

TEST(Backend, InitializeOK) 
{
    booking::Booking::Initialize(JSON);
    std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();
    EXPECT_EQ(KTheaters.size(), b->GetTheaterList().size());
    EXPECT_EQ(KMovies.size(), b->GetMovieList().size());
}

TEST(Backend, InitializeEmpty) 
{
    booking::Booking::Initialize("{}");
    std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();
    EXPECT_EQ(0, b->GetTheaterList().size());
    EXPECT_EQ(0, b->GetMovieList().size());
}

TEST(Backend, CheckLists) 
{
    booking::Booking::Initialize(JSON);
    std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();
     
    auto cl = b->GetTheaterList();
    auto ml = b->GetMovieList();

    // very very silly but
    std::set<booking::Theater> cs(std::make_move_iterator(cl.begin()), std::make_move_iterator(cl.end()));

    for (auto &t : KTheaters)
        EXPECT_NE(cs.end(), cs.find({t}));

    EXPECT_EQ(cs.end(), cs.find({"Acapulco"}));

    std::set<booking::Movie> ms(std::make_move_iterator(ml.begin()), std::make_move_iterator(ml.end()));

    for (auto &t : KMovies)
        EXPECT_NE(ms.end(), ms.find({t}));

    EXPECT_EQ(ms.end() , ms.find({"Die Another Day"}));
}


TEST(Backend, SelectMovie) 
{
    booking::Booking::Initialize(JSON);
    std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();

    EXPECT_EQ(KTheaters.size(), b->GetTheaterList().size());
      //select a movie shown in more than one theaters (2)
    EXPECT_EQ(true, b->SelectMovie({KMovies[2]}));   
    EXPECT_EQ(KTerminators, b->GetTheaterList().size());
}


TEST(Backend, SelectTheater) 
{
    booking::Booking::Initialize(JSON);
    std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();

    EXPECT_EQ(KTheaters.size(), b->GetTheaterList().size());
      //select a movie shown in more than one theaters (2)
    EXPECT_EQ(true, b->SelectMovie({KMovies[2]}));   
    EXPECT_EQ(KTerminators, b->GetTheaterList().size());
}


TEST(Backend, BookingTest) 
{
    booking::Booking::Initialize(JSON);
    std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();
     
    auto cl = b->GetTheaterList();
    auto ml = b->GetMovieList();

    EXPECT_EQ(true, b->SelectTheater({"D'Abruzo"}));  
    EXPECT_EQ(0, b->GetMovieList().size());

    EXPECT_EQ(true, b->SelectTheater({KTheaters[0]}));
    EXPECT_EQ(1, b->GetMovieList().size());

    EXPECT_EQ(true, b->SelectMovie(b->GetMovieList()[0]));

    EXPECT_EQ(20, b->GetAvailableSeats().size());

    EXPECT_EQ(EBookingResult::ESuccess, b->BookTickets(5));
    EXPECT_EQ(15, b->GetAvailableSeats().size());

    std::unique_ptr<booking::BookingClient> b2 = booking::BookingClient::New();

    EXPECT_EQ(true, b2->SelectTheater({KTheaters[0]}));
    EXPECT_EQ(true, b2->SelectMovie(b->GetMovieList()[0]));
    EXPECT_EQ(EBookingResult::ESuccess, b2->BookTickets(5));
    EXPECT_EQ(10, b2->GetAvailableSeats().size());

    EXPECT_EQ(EBookingResult::ENoSeatsAvailable, b2->BookTickets(15));
    EXPECT_EQ(10, b2->GetAvailableSeats().size());

    EXPECT_EQ(EBookingResult::ESuccess, b2->BookTickets(10));
    EXPECT_EQ(0, b2->GetAvailableSeats().size());    
}


std::atomic<int>  i{0};


void booking_agent() {
    std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();
    auto cl = b->GetTheaterList();
    auto ml = b->GetMovieList();
    EXPECT_EQ(true, b->SelectTheater({KTheaters[1]}));
    EXPECT_EQ(true, b->SelectMovie(b->GetMovieList()[0]));


    while(EBookingResult::ESuccess == b->BookTickets(1)) {
        ++i;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

TEST(Backend, BookingTestMt) 
{
    booking::Booking::Initialize(JSON);
    std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();
    auto cl = b->GetTheaterList();
    auto ml = b->GetMovieList();
    EXPECT_EQ(true, b->SelectTheater({KTheaters[1]}));
    EXPECT_EQ(true, b->SelectMovie(b->GetMovieList()[0]));
    EXPECT_EQ(20, b->GetAvailableSeats().size());

    std::thread jt1(booking_agent);
    std::thread jt2(booking_agent);

    jt1.join();
    jt2.join();

    EXPECT_EQ(20, i);
    EXPECT_EQ(0, b->GetAvailableSeats().size());
}


}