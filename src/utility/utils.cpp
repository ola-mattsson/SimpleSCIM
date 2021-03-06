//
// Created by Ola Mattsson on 2018-09-09.
//
#include <vector>
#include <iostream>
#include <algorithm>

#include "utils.hpp"
#include "GroupSCIM_config.h"
#include "simplescim_error_string.hpp"

void print_usage(const std::string &app_name) {
    std::cout << app_name
              << " Version "
              << GroupSCIM_VERSION_MAJOR << '.'
              << GroupSCIM_VERSION_MINOR << std::endl;
    std::cout << "Usage: "
              << app_name
              << " supplier.conf supplier.conf" << std::endl;

}

int check_params(int argc, char **argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return -1;
    }
    if (argc >= 2) {
        std::string first_param = argv[1];
        if (first_param == "-v" || first_param == "--version") {
            print_usage(argv[0]);
            return -1;
        } else if (first_param.find("--test_server_url") != std::string::npos) {
            return 1;
        }
    }
    return 0;
}

std::string get_test_server_url(char **argv) {
    std::string s = argv[1];
    return s.substr(s.find('=') + 1);
}

std::vector<std::string> string_to_vector(const std::string &s) {
    if (s.empty())
        return {};
    std::vector<std::string> var_v{};
    auto cur = std::begin(s);
    auto end = std::end(s);
    auto var_end = std::find(cur, end, ' ');
    while (var_end != std::end(s)) {
        std::string variable(cur, var_end);
        std::string::size_type comma = variable.find(',');
        if (comma != std::string::npos)
            variable.erase(comma);
        var_v.emplace_back(variable);
        cur = var_end + 1;
        var_end = std::find(cur, end, ' ');
    }
    var_v.emplace_back(cur, end);
    return var_v;
}

std::pair<std::string, std::string> string_to_pair(const std::string &s) {
    auto cur = std::begin(s);
    auto end = std::end(s);
    auto var_end = std::find(cur, end, '.');
    if (var_end != end) {
        std::string second(var_end + 1, end);
        auto comma = second.find(',');
        if (comma != std::string::npos)
            second.erase(comma);
        return std::make_pair(std::string(cur, var_end), second);
    } else
        return std::make_pair("", "");

}

std::string pair_to_string(const std::pair<std::string, std::string> &pair) {
    return pair.first + '.' + pair.second;
}

void print_error() {
    if (has_errors_to_print())
        std::cerr << simplescim_error_string_get() << std::endl;
}

void print_status(const char *config_file_name) {
    std::cout << "Successfully performed SCIM operations for " << config_file_name << std::endl;
}

std::string unifyurl(const std::string &s) {
    std::string newString;
    auto iter = std::begin(s);
    auto end = std::end(s);
    while (iter != end) {
        switch (*iter) {
            case ' ':
                newString.append("%20");
                break;
            case '!':
                newString.append("%21");
                break;
            case '#':
                newString.append("%23");
                break;
            case '$':
                newString.append("%24");
                break;
            case '&':
                newString.append("%26");
                break;
            case '\'':
                newString.append("%27");
                break;
            case '(':
                newString.append("%28");
                break;
            case ')':
                newString.append("%29");
                break;
            case '*':
                newString.append("%2A");
                break;
            case '+':
                newString.append("%2B");
                break;
            case ',':
                newString.append("%2C");
                break;
            case '/':
                newString.append("%2F");
                break;
            case ':':
                newString.append("%3A");
                break;
            case ';':
                newString.append("%3B");
                break;
            case '=':
                newString.append("%3D");
                break;
            case '?':
                newString.append("%3F");
                break;
            case '@':
                newString.append("%40");
                break;
            case '[':
                newString.append("%5B");
                break;
            case ']':
                newString.append("%5D");
                break;
            default:
                newString += *iter;


        }
        iter++;
    }
    return newString;
}

std::string toUpper(const std::string &s) {
    std::string out(s);
    std::transform(s.begin(), s.end(), out.begin(), ::toupper);
    return out;
}

std::string &toUpper(std::string &s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

std::string uuid_util::generate() {
    boost::uuids::uuid uuid = generator();
    std::string id = boost::uuids::to_string(uuid);
    return toUpper(id);
}

std::string uuid_util::generate(const std::string &a, const std::string &b) {
    boost::uuids::name_generator_latest name_generator(boost::uuids::ns::oid());

    boost::uuids::uuid uuid = name_generator(a + b);
    std::string id = boost::uuids::to_string(uuid);
    return toUpper(id);
}

std::string uuid_util::un_parse_uuid(char *val) {
    boost::uuids::uuid uuid{};
    ::memcpy(&uuid, val, 16);
    auto st = boost::lexical_cast<std::string>(uuid);
    return toUpper(st);
}
