#include "dataManipulation.h"

#include <string>
#include <vector>
#include <random>
#include <array>
#include <algorithm>
#include <sstream>

using namespace std;

auto isDigits(const string &s) -> bool {
    return ranges::all_of(s, [](char c) {return isdigit(c);});
}
auto split(const string& str, char del) -> vector<string> {
    vector<string> res;

    stringstream ss(str);
    string item;
    while (getline(ss, item, del))
        if (!item.empty())
            res.push_back(item);

    return res;
}
bool strEquals(const string& s1, const string& s2) {
    if (s1.size() != s2.size())
        return false;
    return equal(s1.begin(), s1.end(), s2.begin(), [](char c1, char c2) {
        return tolower(c1) == tolower(c2);
    });
}
auto encrypt(string const& data, string const& key) -> string {
    auto res = string();
    for( auto dataIt = data.begin(), keyIt = key.begin();
         dataIt != data.end();
         dataIt++, keyIt++ )
    {
        if(keyIt == key.end())
            keyIt = key.begin();

        res += *dataIt + *keyIt;
    }
    return res;
}
auto decrypt(string const& data, string const& key) -> string {
    auto res = string();
    for(auto dataIt = data.begin(), keyIt = key.begin();
        dataIt != data.end();
        dataIt++, keyIt++ )
    {
        if(keyIt == key.end())
            keyIt = key.begin();

        res += *dataIt - *keyIt;
    }
    return res;
}


auto toLowerCase(string s)->string{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}


bool stringBeginMatches(const string& s1, const string& s2){
    return equal(s1.begin(), s1.begin() + min(s1.size(), s2.size()), s2.begin(), [](char c1, char c2) {
        return tolower(c1) == tolower(c2);
    });
}


string getPasswordIssues(const string& password) {
    const string specialChars = "!@#$%^&*()_+-={}[]|:;<>,.?/~";
    
    string res;
    if (password.size() < 9)
        res.append("* Too short. Password should be at least 8 characters long\n");
    
    
    if (!any_of(password.begin(), password.end(),[](char c) { return islower(c); }))
        res.append("* Password should include lowercase letters\n");
    
    
    if (!any_of(password.begin(), password.end(),[](char c) { return isupper(c); }) )
        res.append("* Password should include uppercase letters\n");
    
    
    if (!any_of(password.begin(), password.end(),[](char c) { return isdigit(c); }))
        res.append("* Password should include numbers\n");
    
    
    if (!any_of(password.begin(), password.end(),
                 [&specialChars](char c) { return specialChars.find(c) != string::npos;}))
        res.append("* Password should include special characters\n");

    return res;
}


string generatePassword(int size, bool includeUppercase, bool includeSpecial) {
    const string uppercaseChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string specialChars = R"(#$%&\()*+-./:<=>?@[\]^_`{|}~'")";

    string passwordChars = "abcdefghijklmnopqrstuvwxyz0123456789";
    if (includeUppercase)
        passwordChars.append(uppercaseChars);
    if (includeSpecial)
        passwordChars.append(specialChars);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, passwordChars.size() - 1);

    auto password = string();
    while (true) {
        password.clear();
        sample(passwordChars.begin(), passwordChars.end(), back_inserter(password), size, gen);

        if (!any_of(password.begin(), password.end(), [](char c) {return islower(c);}))
            continue;

        if (includeUppercase &&
        !any_of(password.begin(), password.end(), [](char c) {return isupper(c);}))
            continue;

        if (includeSpecial &&
        !any_of(password.begin(), password.end(), [&specialChars](char c) {
            return specialChars.find(c) != string::npos;
        }))
            continue;

        return password;
    }
}
