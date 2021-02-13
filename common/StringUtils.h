//
// Created by l2pic on 13.02.2021.
//

#ifndef DBGATEWAY_COMMON_STRINGUTILS_H_
#define DBGATEWAY_COMMON_STRINGUTILS_H_

#include <string>
#include <string_view>
#include <algorithm>

namespace String {

inline void toUpper(std::string *str) {
    std::transform(str->begin(), str->end(), str->begin(), ::toupper);
}

inline std::string toUpper(std::string_view str) {
    std::string upperStr(str);
    toUpper(&upperStr);
    return upperStr;
}

inline void toLower(std::string *str) {
    std::transform(str->begin(), str->end(), str->begin(), ::tolower);
}

inline std::string toLower(std::string_view str) {
    std::string upperStr(str);
    toLower(&upperStr);
    return upperStr;
}

}

#endif //DBGATEWAY_COMMON_STRINGUTILS_H_
