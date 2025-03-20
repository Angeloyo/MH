cmake_minimum_required(VERSION 3.28)

PROJECT(mh_p1 CXX)

file(GLOB C_SOURCES
  "src/*.cpp"
)
INCLUDE_DIRECTORIES("common" "inc")

ADD_EXECUTABLE(main "main.cpp" ${C_SOURCES})

ADD_EXECUTABLE(test_mdd "test_mdd.cpp" ${C_SOURCES})

ADD_EXECUTABLE(test_random "test_random.cpp" ${C_SOURCES})

ADD_EXECUTABLE(test_greedy "test_greedy.cpp" ${C_SOURCES})

ADD_EXECUTABLE(test_localsearch "test_localsearch.cpp" ${C_SOURCES})
