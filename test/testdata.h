#pragma once

#include <string>
#include <vector>


namespace testdata {

static const std::vector<std::string> KTheaters{"Kolliseum",  
                                      "Atrium",     
                                      "Neva",     
                                      "Parisianna", 
                                      "Picadilly",  
                                      "Lichtspiele",
                                      "Roma",       
                                      "Istanbul",   
                                      "Athens", 
                                      "Paradise"
                                    };


static const std::vector<std::string> KMovies{"Once upon a time in America",
                                      "9 1/2",
                                      "Terminator",
                                      "Les Parapluies de Cherbourg",
                                      "The Game of Thrones",
                                      "Der Herr der Ringe",
                                      "Throy",
                                      "The world is not enough", "Achilles"};

static const std::string BADJSON = "";

static const std::string JSON = "{\n"\
        "\"Kolliseum\"   : { \"mouvie\" : \"Once upon a time in America\"},\n"\
        "\"Atrium\"      : { \"mouvie\" : \"9 1/2\"},\n"\
        "\"Neva\"        : { \"mouvie\" : \"Terminator\"},\n"\
        "\"Parisianna\"  : { \"mouvie\" : \"Les Parapluies de Cherbourg\"},\n"\
        "\"Picadilly\"   : { \"mouvie\" : \"The Game of Thrones\"},\n"\
        "\"Lichtspiele\" : { \"mouvie\" : [\"Der Herr der Ringe\", \"Terminator\"]},\n"\
        "\"Roma\"        : { \"mouvie\" : \"Terminator\"},\n"\
        "\"Istanbul\"    : { \"mouvie\" : \"The Game of Thrones\"},\n"\
        "\"Athens\"      : { \"mouvie\" : [\"Throy\", \"Achilles\"]},\n"\
        "\"Paradise\"    : { \"mouvie\" : \"The world is not enough\"}\n"\
        "}\n";

constexpr unsigned KTerminators=3;        
}