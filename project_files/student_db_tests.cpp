#include <vector>
#include <algorithm>
#include <iostream>
#include "student_db.h"

std::ostream& operator<< ( std::ostream& os, const std::pair<int, cop3530::student_info> &pair) {
    os << "{" << pair.first << ": {" << pair.second.get_name() << ", " << pair.second.get_gpa() << "}}";
    return os;
}
std::ostream& operator<< ( std::ostream& os, const std::vector<std::pair<int, cop3530::student_info>> &pairs) {
    if(pairs.empty()) return os << "{}";
    os << "{" << pairs[0];
    std::for_each(begin(pairs)+1, end(pairs), [&](const auto &pair){
        os << ", " << pair;
    });
    os << "}";
    return os;
}

#include "catch.hpp"
#include <exception>

TEST_CASE("student_db insert", "[points:1]") {
    cop3530::student_db database;
    //database.insert(1, "Albert", 3.5);
    //cop3530::student_info student = database.lookup(1);
    //CHECK(student.get_name() == "Albert");
    //CHECK(student.get_gpa() == 3.5);

    database.insert(2, "Alberta", 4.0);
    cop3530::student_info student = database.lookup(2);
    CHECK(student.get_name() == "Alberta");
    CHECK(student.get_gpa() == 4.0);

    //database.insert(1, "Alberto", 3.8);
    //student = database.lookup(1);
    //CHECK(student.get_name() == "Alberto");
    //CHECK(student.get_gpa() == 3.8);
}

TEST_CASE("student_db erase", "[points:1]") {
    cop3530::student_db database;
    database.insert(1, "Albert", 3.5);
    database.insert(2, "Alberta", 4.0);
    
    CHECK(database.lookup(1).get_name() == "Albert");
    CHECK(database.lookup(2).get_name() == "Alberta");

    database.erase(1);
    database.erase(2);

    CHECK_THROWS_AS(database.lookup(1), std::out_of_range);
    CHECK_THROWS_AS(database.lookup(2), std::out_of_range);
}

TEST_CASE("student_db size", "[points:1]") {
    cop3530::student_db database;
    CHECK(database.size() == 0);

    database.insert(1, "Albert", 3.5);
    CHECK(database.size() == 1);
    database.insert(2, "Alberta", 4.0);
    CHECK(database.size() == 2);

    database.erase(1);
    CHECK(database.size() == 1);
    database.erase(2);
    CHECK(database.size() == 0);
}

TEST_CASE("student_db empty", "[points:1]") {
    cop3530::student_db database;
    CHECK(database.empty());

    database.insert(1, "Albert", 3.5);
    database.insert(2, "Alberta", 4.0);
    
    CHECK_FALSE(database.empty());

    database.erase(1);
    database.erase(2);
    
    CHECK(database.empty());
}

TEST_CASE("student_db get_all_students", "[points:1]") {
    cop3530::student_db database;
    CHECK(database.empty());

    //database.insert(1, "Albert", 3.5);
    database.insert(2, "Alberta", 4.0);
    
    //auto students = database.get_all_students();
    //std::vector<std::pair<int, cop3530::student_info>> expected_students = {{1, {"Albert", 3.5}}, {2, {"Alberta", 4.0}}};
   //CHECK(students == expected_students);
}