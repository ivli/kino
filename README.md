# 
 1. To deal with cross-platformity
    a. use POSIX API
    b. employ docker conainer to deal with almost-POSIX platforms like Windows (don't want to deal with WSL cause it is a proprietary technology) -- container using host network for the sake of simplicity
    c. actually if real crosplatformity is a requirement it'd be better to implement backend in Java
 2. Use RESTful API (if it were more time it'd be better to employ swaggere aka OpenAPI) we'll use HTTP GET that one might use either curl or JS or simply sockets to communicate to the service
 3. Responses are in JSON
 4. since using RDBMS is not allowed (what about in memory DB say sqlite based) we'll use a C++ class composed of stl containers and implement serialization to/from file in JSON format.
 5. We'll use a 3rd party library (TBD) to conwert data to/from JSON(DOM). https://rapidjson.org/??? looks quite mature. And get it from conancenter (https://conan.io/center/recipes/rapidjson)
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

 


