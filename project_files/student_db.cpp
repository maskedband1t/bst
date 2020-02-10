#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include "student_db.h"

namespace cop3530 {

student_info::student_info() {}
student_info::student_info(std::string name, double gpa): name(name), gpa(gpa) {
}

std::string student_info::get_name() const {
    return name;
}

double student_info::get_gpa() const {
    return gpa;
}

bool student_info::operator==(const student_info &other) const {
    return name == other.name && abs(gpa-other.gpa) < 0.000000001;
}

student_db::student_db() {
}

student_db::~student_db() {
    tree.~bst();
}

void student_db::insert(const int student_id, const std::string &student_name, const double gpa) {
    tree.insert(student_id,student_info(student_name,gpa));
}

void student_db::erase(const int student_id) {
    tree.erase(student_id);
}

student_info& student_db::lookup(const int student_id) {
    return tree.at(student_id);
}

bool student_db::empty() const {
    return tree.empty();
}

size_t student_db::size() const {
    return tree.size();
}

std::vector<std::pair<int, student_info>> student_db::get_all_students() {
    return tree.inorder_contents();
}

}