#include "gtest/gtest.h"
#include "../include/booking.h"
#include <string>


namespace {
using namespace booking;

TEST(Backend, Initialize) {

    std::string JSON = "";

    booking::Booking::Initialize(JSON);

}


TEST(Backend, Initialize2) {

    std::string JSON = "{\n"\
            "\"Kolliseum\"   : { \"mouvie\" : \"Once upon a time in America\"},\n"\
            "\"Atrium\"      : { \"mouvie\" : \"9 1/2\"},\n"\
            "\"Neva\"        : { \"mouvie\" : \"Terminator\"},\n"\
            "\"Parisianna\"  : { \"mouvie\" : \"Les Parapluies de Cherbourg\"},\n"\
            "\"Picadilly\"   : { \"mouvie\" : \"The Game of Thrones\"},\n"\
            "\"Lichtspiele\" : { \"mouvie\" : \"Der Herr der Ringe\"},\n"\
            "\"Roma\"        : { \"mouvie\" : \"Terminator\"},\n"\
            "\"Istanbul\"    : { \"mouvie\" : \"The Game of Thrones\"},\n"\
            "\"Athens\"      : { \"mouvie\" : \"Throy\"}\n"\
            "}\n";

    booking::Booking::Initialize(JSON);

    std::unique_ptr<booking::BookingClient> b = booking::BookingClient::New();

    EXPECT_EQ(9, b->GetCinemaList().size());

    EXPECT_EQ(7, b->GetMouvieList().size());

}




}  // namespace