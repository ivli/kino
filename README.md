some generic thoughts
 1. To deal with cross-platformity
    a. use POSIX API
    b. employ docker conainer to deal with almost-POSIX platforms like Windows (don't want to deal with WSL cause it is a proprietary technology) -- container using host network for the sake of simplicity
    c. actually if real crosplatformity is a requirement it'd be better to implement backend in Java
 2. Use RESTful API (if it were more time it'd be better to employ swaggere aka OpenAPI) we'll use HTTP GET that one might use either curl or JS or simply sockets to communicate to the service
 3. Responses are in JSON
 4. since using RDBMS is not allowed (what about in memory DB say sqlite based) we'll use a C++ class composed of stl containers and implement serialization to/from file in JSON format.
 5. We'll use a 3rd party library (TBD) to convert data to/from JSON. https://rapidjson.org/??? looks quite mature. And get it from conancenter (https://conan.io/center/recipes/rapidjson)
 6. Employ gtest (https://conan.io/center/recipes/gtest?version=1.14.0)
 7. Use doxygen-style comments
 8. since requirements state no owerbooking is allowed use simple booking strategy ::TODO ()
 9. service of defining the set of cinemas, list of movies and schedules is out of scope so just hardcode'em but in reality thre'll be a similar interface
 10. AAA is also out of scope so deal with any HTTP GET
 11. Requests:
    1. GET list of movies
    2. POST movie (select a movie)
    3. GET theaters showing the movie
    4. POST Select a theater
    5. GET available seats
    6. GET book one or more seats

results
0. the solution consists of 3 parts:
   a. library -- implements memory-based storage, capable of handling all kinds of request of #11 above through public API (include/booking.h), and it can be initialized by means of json
   b. unittests  -- that covers all aspects of public API
   c. simple application (kino, mouvie in Russian) to show how to deal with the library (this is quick&dirty made only for demonstration purpose so don't take it seriously)
1. to install prerequisites and prepare cmake/conan environment run from project root folder $>conan install conanfile.txt --build=missing
2. then run: $>cmake . -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./build/Release/generators/conan_toolchain.cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release
3. then $>make -- to build programm and tests
4. to execute tests: $>make tests
5. there is very-very naive RESTFul server application based on the library, to run: $>./kino [-d] 
6. this application is capable of handling requests of 2 types $>curl -Xget "http://localhost:42024/theaters"  to get a list of theatres and curl -Xget "http://localhost:42024/movies" to get a list of movies
7. To run the same application in docker: 
   a. $>docker build . -t kino:v2
   b. $>running in docker: docker run -p 42024:42024 -t kino:v2
   c. then it can handle requests described in #5
 


