#include <iostream>
#include <vector>
#include <algorithm>
std::ostream& operator<< ( std::ostream& os, const std::pair<int, int> &pair) {
    os << "{" << pair.first << ", " << pair.second << "}";
    return os;
}
std::ostream& operator<< ( std::ostream& os, const std::vector<std::pair<int, int>> &pairs) {
    if(pairs.empty()) return os << "{}";
    os << "{" << pairs[0];
    std::for_each(begin(pairs)+1, end(pairs), [&](const auto &pair){
        os << ", " << pair;
    });
    os << "}";
    return os;
}

#include "catch.hpp"
#include "bst.h"
#include <exception>

template<typename Key, typename T>
struct test_node {
    Key k;
    T data;
    std::unique_ptr<test_node> left;
    std::unique_ptr<test_node> right;
    test_node() {}
    test_node(const Key &k, const T &t): k(k), data(t) {}
};

template<typename Key, typename T>
std::unique_ptr<test_node<Key, T>> rebuild_bst(typename std::vector<std::pair<Key, T>>::const_iterator &begin_preorder_contents,
                                               typename std::vector<std::pair<Key, T>>::const_iterator end_preorder_contents,
                                               typename std::vector<std::pair<Key, T>>::const_iterator begin_inorder_contents,
                                               typename std::vector<std::pair<Key, T>>::const_iterator end_inorder_contents) {
    
    if(begin_preorder_contents >= end_preorder_contents || begin_inorder_contents >= end_inorder_contents) {
        --begin_preorder_contents;
        return nullptr;
    }

    std::unique_ptr<test_node<Key, T>> root = std::make_unique<test_node<Key, T>>(begin_preorder_contents->first, begin_preorder_contents->second);
    auto inorder_pos = std::lower_bound(begin_inorder_contents, end_inorder_contents, *begin_preorder_contents);

    root->left = rebuild_bst<Key, T>(++begin_preorder_contents, end_preorder_contents, begin_inorder_contents, inorder_pos);
    root->right = rebuild_bst<Key, T>(++begin_preorder_contents, end_preorder_contents, inorder_pos+1, end_inorder_contents);

    return root;
}

template<typename Key, typename T>
std::unique_ptr<test_node<Key, T>> rebuild_bst(std::vector<std::pair<Key, T>> preorder_contents, std::vector<std::pair<Key, T>> inorder_contents) {
    auto pre_begin = cbegin(preorder_contents);
    auto pre_end = cend(preorder_contents);
    auto in_begin = cbegin(inorder_contents);
    auto in_end = cend(inorder_contents);
    return rebuild_bst<Key, T>(pre_begin, pre_end, in_begin, in_end);
}

TEST_CASE("bst insert", "[points:1]") {
    cop3530::bst<int, int> map;
    
    map.insert(1, 2);
    std::vector<std::pair<int, int>> expected_inorder_contents = {{1, 2}};
    std::vector<std::pair<int, int>> expected_preorder_contents = {{1, 2}};
    CHECK(map.inorder_contents() == expected_inorder_contents);
    CHECK(map.preorder_contents() == expected_preorder_contents);
    
    map.insert(0, 0);
    expected_inorder_contents = {{0, 0}, {1, 2}};
    expected_preorder_contents = {{1, 2}, {0, 0}};
    CHECK(map.inorder_contents() == expected_inorder_contents);
    CHECK(map.preorder_contents() == expected_preorder_contents);
    
    map.insert(0, 3);
    expected_inorder_contents = {{0, 3}, {1, 2}};
    expected_preorder_contents = {{1, 2}, {0, 3}};
    CHECK(map.inorder_contents() == expected_inorder_contents);
    CHECK(map.preorder_contents() == expected_preorder_contents);

    map.insert(2, 4);
    expected_inorder_contents = {{0, 3}, {1, 2}, {2, 4}};
    expected_preorder_contents = {{1, 2}, {0, 3}, {2, 4}};
    CHECK(map.inorder_contents() == expected_inorder_contents);
    CHECK(map.preorder_contents() == expected_preorder_contents);
    
}

TEST_CASE("bst erase", "[points:1]") {
    cop3530::bst<int, int> map;
    map.insert(1, 2);
    map.insert(0, 0);
    map.insert(2, 4);

    
    map.erase(0);
    std::vector<std::pair<int, int>> expected_inorder_contents = {{1, 2}, {2, 4}};
    std::vector<std::pair<int, int>> expected_preorder_contents = {{1, 2}, {2, 4}};
    CHECK(map.inorder_contents() == expected_inorder_contents);
    CHECK(map.preorder_contents() == expected_preorder_contents);
    
    map.erase(1);
    expected_inorder_contents = {{2, 4}};
    expected_preorder_contents = {{2, 4}};
    CHECK(map.inorder_contents() == expected_inorder_contents);
    CHECK(map.preorder_contents() == expected_preorder_contents);
     
    map.erase(2);
    expected_inorder_contents = {};
    expected_preorder_contents = {};
    CHECK(map.inorder_contents() == expected_inorder_contents);
    CHECK(map.preorder_contents() == expected_preorder_contents);
}
  
TEST_CASE("Erase two-child node", "[points:1]") {
    cop3530::bst<int, int> map;
    std::vector<std::pair<int, int>> vals = {{5, 5}, {4, 4}, {6, 6}};
    for_each(begin(vals), end(vals), [&](const auto &pair){
        map.insert(pair.first, pair.second);
    });
    CHECK(map.contains(5));
    map.erase(5);
    CHECK_FALSE(map.contains(5));
    std::vector<std::pair<int, int>> expected_inorder_contents = {{4, 4}, {6, 6}};
    std::vector<std::pair<int, int>> expected_preorder_contents_opt1 = {{4, 4}, {6, 6}}; // replace w/ inorder predecessor
    std::vector<std::pair<int, int>> expected_preorder_contents_opt2 = {{6, 6}, {4, 4}}; // replace w/ inorder successor
    CHECK(map.inorder_contents() == expected_inorder_contents);
    CHECK( (map.preorder_contents() == expected_preorder_contents_opt1
            || map.preorder_contents() == expected_preorder_contents_opt2) );
}


TEST_CASE("bst at", "[points:1]") {
    cop3530::bst<int, int> map;
    map.insert(1, 2);
    map.insert(0, 0);
    map.insert(2, 4);
    
    CHECK(map.at(1) == 2);
    CHECK(map.at(0) == 0);
    CHECK(map.at(2) == 4);
}

TEST_CASE("bst contains", "[points:1]") {
    cop3530::bst<int, int> map;
    CHECK_FALSE(map.contains(1));
    CHECK_FALSE(map.contains(0));
    CHECK_FALSE(map.contains(2));

    map.insert(1, 2);
    map.insert(0, 0);
    map.insert(2, 4);

    CHECK(map.contains(1));
    CHECK(map.contains(0));
    CHECK(map.contains(2));
    
    map.erase(1);
    map.erase(0);
    map.erase(2);
     
    CHECK_FALSE(map.contains(1));
    CHECK_FALSE(map.contains(0));
    CHECK_FALSE(map.contains(2));
}

TEST_CASE("bst empty", "[points:1]") {
    cop3530::bst<int, int> map;
    CHECK(map.empty());

    map.insert(1, 2);
    map.insert(0, 0);
    map.insert(2, 4);

    CHECK_FALSE(map.empty());

    map.erase(1);
    map.erase(0);
    map.erase(2);

    CHECK(map.empty());
}

TEST_CASE("bst size", "[points:1]") {
    cop3530::bst<int, int> map;
    CHECK(map.size() == 0);

    map.insert(1, 2);
    CHECK(map.size() == 1);
    map.insert(0, 0);
    CHECK(map.size() == 2);
    map.insert(2, 4);
    CHECK(map.size() == 3);

    map.erase(1);
    CHECK(map.size() == 2);
    map.erase(0);
    CHECK(map.size() == 1);
    map.erase(2);
    CHECK(map.size() == 0);
}

TEST_CASE("bst at and erase throw when key not found") {
    cop3530::bst<int, int> map;
    CHECK_THROWS_AS(map.at(1), std::out_of_range);
    CHECK_THROWS_AS(map.erase(1), std::out_of_range);
    CHECK_THROWS_AS(map.at(0), std::out_of_range);
    CHECK_THROWS_AS(map.erase(0), std::out_of_range);
    CHECK_THROWS_AS(map.at(2), std::out_of_range);
    CHECK_THROWS_AS(map.erase(2), std::out_of_range);

    map.insert(1, 2);
    map.insert(0, 0);
    map.insert(2, 4);

    CHECK_THROWS_AS(map.at(4), std::out_of_range);
    CHECK_THROWS_AS(map.erase(4), std::out_of_range);

    map.erase(1);
    map.erase(0);
    map.erase(2);

    CHECK_THROWS_AS(map.at(1), std::out_of_range);
    CHECK_THROWS_AS(map.erase(1), std::out_of_range);
    CHECK_THROWS_AS(map.at(0), std::out_of_range);
    CHECK_THROWS_AS(map.erase(0), std::out_of_range);
    CHECK_THROWS_AS(map.at(2), std::out_of_range);
    CHECK_THROWS_AS(map.erase(2), std::out_of_range);
}



TEST_CASE("custom test", "[points:N/A]") {

    // Extra comprehensive test case that tests functions in a row with a larger tree.

    cop3530::bst<int, int> map;

    CHECK(map.empty());

    map.insert(6, 5);

    map.insert(4, 10);

    map.insert(3, 15);

    map.insert(5, 20);

    map.insert(7, 25);

    map.insert(9, 30);

    map.insert(8, 35);

    CHECK(map.size() == 7);

    std::vector<std::pair<int, int>> expected_inorder_contents = { { 3, 15 },{ 4, 10 },{ 5, 20 },{ 6, 5 },{ 7, 25 },{ 8, 35 },{ 9, 30 } };

    std::vector<std::pair<int, int>> expected_preorder_contents = { { 6, 5 },{ 4, 10 },{ 3, 15 },{ 5, 20 },{ 7, 25 },{ 9, 30 },{ 8, 35 } };

    

    CHECK(map.inorder_contents() == expected_inorder_contents);

    CHECK(map.preorder_contents() == expected_preorder_contents);

    CHECK(map.at(6) == 5);

    CHECK(map.at(4) == 10);

    CHECK(map.at(3) == 15);

    CHECK(map.at(5) == 20);

    

    map.erase(6);
 
    expected_inorder_contents = { { 3, 15 },{ 4, 10 },{ 5, 20 },{ 7, 25 },{ 8, 35 },{ 9, 30 } };

    std::vector<std::pair<int, int>> expected_preorder_contents_succ = { { 7, 25 },{ 4, 10 },{ 3, 15 },{ 5, 20 },{ 9, 30 },{ 8, 35 } };

    std::vector<std::pair<int, int>> expected_preorder_contents_pre = { { 5, 20 },{ 4, 10 },{ 3, 15 },{ 7, 25 },{ 9, 30 },{ 8, 35 } };

    CHECK_THROWS_AS(map.at(6), std::out_of_range);

    CHECK_THROWS_AS(map.erase(6), std::out_of_range);

    CHECK(map.size() == 6);


    CHECK(map.inorder_contents() == expected_inorder_contents);
 
    CHECK( (map.preorder_contents() == expected_preorder_contents_succ || map.preorder_contents() == expected_preorder_contents_pre) );

    map.erase(8);

    expected_inorder_contents = { { 3, 15 },{ 4, 10 },{ 5, 20 },{ 7, 25 },{ 9, 30 } };

    expected_preorder_contents_pre = { { 5, 20 },{ 4, 10 },{ 3, 15 },{ 7, 25 },{ 9, 30 } };

    expected_preorder_contents_succ = { { 7, 25 }, { 4, 10 }, { 3, 15 }, { 5, 20 }, { 9, 30 } };

    CHECK_THROWS_AS(map.at(8), std::out_of_range);

    CHECK_THROWS_AS(map.erase(8), std::out_of_range);

    CHECK(map.size() == 5);

    CHECK(map.inorder_contents() == expected_inorder_contents);
 
    CHECK((map.preorder_contents() == expected_preorder_contents_succ || map.preorder_contents() == expected_preorder_contents_pre));

    CHECK_FALSE(map.empty());

    CHECK_FALSE(map.contains(8));

    CHECK_FALSE(map.contains(6));

    

    CHECK(map.contains(7));

    CHECK(map.contains(9));

    map.insert(8, 100);

    expected_inorder_contents = { { 3, 15 },{ 4, 10 },{ 5, 20 },{ 7, 25 },{ 8, 100 },{ 9, 30 } };

    expected_preorder_contents_succ = { { 7, 25 },{ 4, 10 },{ 3, 15 },{ 5, 20 },{ 9, 30 },{ 8, 100 } };

    expected_preorder_contents_pre = { { 5, 20 },{ 4, 10 },{ 3, 15 },{ 7, 25 },{ 9, 30 },{ 8, 100 } };

    CHECK(map.size() == 6);

    CHECK(map.inorder_contents() == expected_inorder_contents);

    CHECK((map.preorder_contents() == expected_preorder_contents_succ || map.preorder_contents() == expected_preorder_contents_pre));

    CHECK(map.contains(8));

    CHECK(map.at(8) == 100);

    

    map.insert(8, 101);

    map.insert(7, 5);

    map.insert(4, 0);

    CHECK(map.at(8) == 101);

    CHECK(map.size() == 6);

    CHECK_THROWS_AS(map.at(6), std::out_of_range);

    CHECK_FALSE(map.empty());

    expected_inorder_contents = { { 3, 15 },{ 4, 0 },{ 5, 20 },{ 7, 5 },{ 8, 101 },{ 9, 30 } };

    expected_preorder_contents_succ = { { 7, 5 },{ 4, 0 },{ 3, 15 },{ 5, 20 },{ 9, 30 },{ 8, 101 } };

    expected_preorder_contents_pre = { { 5, 20 },{ 4, 0 },{ 3, 15 },{ 7, 5 },{ 9, 30 },{ 8, 101 } };

    CHECK(map.inorder_contents() == expected_inorder_contents);

    CHECK((map.preorder_contents() == expected_preorder_contents_succ || map.preorder_contents() == expected_preorder_contents_pre));

    CHECK_FALSE(map.contains(0));

    CHECK_FALSE(map.contains(1));

    CHECK(map.contains(5));

    CHECK(map.contains(3));

    map.erase(3);

    map.erase(5);

    map.erase(7);

    map.erase(4);

    map.erase(9);

    expected_inorder_contents = { { 8, 101 } };

    expected_preorder_contents_succ = { { 8, 101 } };

    expected_preorder_contents_pre = { { 8, 101 } };

    CHECK(map.inorder_contents() == expected_inorder_contents);

    CHECK((map.preorder_contents() == expected_preorder_contents_succ || map.preorder_contents() == expected_preorder_contents_pre));

    CHECK(map.size() == 1);

    CHECK_THROWS_AS(map.at(3), std::out_of_range);

    CHECK_THROWS_AS(map.at(5), std::out_of_range);

    CHECK_THROWS_AS(map.at(7), std::out_of_range);

    CHECK_THROWS_AS(map.at(4), std::out_of_range);

    CHECK_THROWS_AS(map.at(9), std::out_of_range);

    CHECK_FALSE(map.contains(3));

    CHECK_FALSE(map.contains(7));

    CHECK_FALSE(map.contains(9));

    CHECK(map.contains(8));

    map.erase(8);

    expected_inorder_contents = { };

    expected_preorder_contents_succ = { };

    expected_preorder_contents_pre = { };

    CHECK(map.inorder_contents() == expected_inorder_contents);

    CHECK((map.preorder_contents() == expected_preorder_contents_succ || map.preorder_contents() == expected_preorder_contents_pre));

    CHECK(map.size() == 0);

    CHECK(map.empty());

    map.insert(1, 10);

    map.insert(2, 20);

    map.insert(3, 30);

    expected_inorder_contents = { { 1, 10 },{ 2, 20 },{ 3, 30 } };

    expected_preorder_contents = { { 1, 10 },{ 2, 20 },{ 3, 30 } };

    CHECK(map.size() == 3);

    CHECK_FALSE(map.empty());

    CHECK(map.inorder_contents() == expected_inorder_contents);

    CHECK(map.preorder_contents() == expected_preorder_contents);

    CHECK(map.contains(2));

    CHECK(map.at(1) == 10);

    CHECK(map.at(2) == 20);

    CHECK(map.at(3) == 30);

    map.erase(2);

    CHECK_THROWS_AS(map.at(2), std::out_of_range);

    CHECK_FALSE(map.contains(2));
    

}

