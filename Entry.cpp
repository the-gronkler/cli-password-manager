#include "Entry.h"
#include "dataManipulation.h"


#include <utility>
#include <stdexcept>
#include <iostream>
#include <iomanip>

using namespace std;

vector<string> Entry::splitEntryParams(const string& str, char del) {
    auto res = vector<string>();
    auto temp = string();

    for (char i : str) {
        if (i != del)
            temp += i;
        else {
            res.push_back(temp);
            temp = "";
        }
    }
    res.push_back(temp);
    return res;
}

Entry::Entry(const string& dataString){
    //dataString format: name;password;category;website;login
    vector<string> data = splitEntryParams(dataString, ';');
    if(data.size() != 5)
        throw invalid_argument(&"parsed invalid number of arguments for constructing entry: Expected 5, found " [ data.size()]);
    this->name = data.at(0);
    this->password = data.at(1);
    this->category = data.at(2);
    this->website = data.at(3);
    this->login = data.at(4);
}
Entry::Entry(string name, string password, string category, string website, string login) :
    name(std::move(name)), password(std::move(password)), category(std::move(category)), website(std::move(website)), login(std::move(login)) {}
Entry::Entry(const Entry& other) {
    this->name = other.name;
    this->password = other.password;
    this->category = other.category;
    this->website = other.website;
    this->login = other.login;
}

bool Entry::operator==(const Entry &other) {
    return strEquals(name, other.name) &&
        strEquals(password, other.password) &&
        strEquals(category, other.category) &&
        strEquals(login, other.login) &&
        strEquals(website, other.website) ;
}
Entry& Entry::operator=(const Entry& other) {
    if (this != &other) {
        this->name = other.name;
        this->password = other.password;
        this->category = other.category;
        this->website = other.website;
        this->login = other.login;
    }
    return *this;
}
ostream &operator<<(ostream &os, const Entry &entry) {
    os  << "name: \'" << entry.name << "\'"
        << ", password: \'" << entry.password << "\'"
        << ", category: \'" << entry.category << "\'"
        << ", website: " << ( entry.website.empty() ? "none" : "\'" + entry.website + "\'" )
        << ", login:'" <<  ( entry.login.empty() ? "none" : "\'" + entry.login + "\'");
    return os;
}

void Entry::printAsTable() const {
    size_t maxNameLength = 4,
            maxPasswordLength = 7,
            maxCategoryLength = 8,
            maxWebsiteLength = 7,
            maxLoginLength = 5;

    maxNameLength = max(maxNameLength, this->getName().length());
    maxPasswordLength = max(maxPasswordLength, this->getPassword().length());
    maxCategoryLength = max(maxCategoryLength, this->getCategory().length());
    maxWebsiteLength = max(maxWebsiteLength, this->getWebsite().length());
    maxLoginLength = max(maxLoginLength, this->getLogin().length());



    cout << setw(maxNameLength) << left << "Name" << " | "
         << setw(maxPasswordLength) << left << "Password" << " | "
         << setw(maxCategoryLength) << left << "Category" << " | "
         << setw(maxWebsiteLength) << left << "Website" << " | "
         << setw(maxLoginLength) << left << "Login" << endl

         << string(maxNameLength, '-') << "-|-"
         << string(maxPasswordLength, '-') << "-|-"
         << string(maxCategoryLength, '-') << "-|-"
         << string(maxWebsiteLength, '-') << "-|-"
         << string(maxLoginLength, '-') << endl

         << setw(maxNameLength) << left << this->getName() << " | "
         << setw(maxPasswordLength) << left << this->getPassword() << " | "
         << setw(maxCategoryLength) << left << this->getCategory() << " | "
         << setw(maxWebsiteLength) << left << this->getWebsite() << " | "
         << setw(maxLoginLength) << left << this->getLogin() << endl;
}

size_t Entry::printRangeAlligned(vector<Entry>::iterator begin, vector<Entry>::iterator end){
    size_t maxNameLength = 4,
        maxPasswordLength = 7,
        maxCategoryLength = 8,
        maxWebsiteLength = 7,
        maxLoginLength = 5;

    // calculate row widths
    for (auto it = begin; it != end; it++) {
        maxNameLength = max(maxNameLength, it->getName().length());
        maxPasswordLength = max(maxPasswordLength, it->getPassword().length());
        maxCategoryLength = max(maxCategoryLength, it->getCategory().length());
        maxWebsiteLength = max(maxWebsiteLength, it->getWebsite().length());
        maxLoginLength = max(maxLoginLength, it->getLogin().length());
    }


    // print header
    cout << "   "
         << setw(maxNameLength) << left << "Name" << " | "
         << setw(maxPasswordLength) << left << "Password" << " | "
         << setw(maxCategoryLength) << left << "Category" << " | "
         << setw(maxWebsiteLength) << left << "Website" << " | "
         << setw(maxLoginLength) << left << "Login" << endl;

    cout << string(
            maxNameLength + maxPasswordLength + maxCategoryLength +maxWebsiteLength + maxLoginLength + 15,
            '-') << endl;

    // print body
    size_t j = 0;
    for (auto it = begin; it != end; it++, j++) {
        cout << j << ". " << setw(maxNameLength) << left << it->getName() << " | "
                  << setw(maxPasswordLength) << left << it->getPassword() << " | "
                  << setw(maxCategoryLength) << left << it->getCategory() << " | "
                  << setw(maxWebsiteLength) << left << it->getWebsite() << " | "
                  << setw(maxLoginLength) << left << it->getLogin() << endl;
    }
    return j+1;
}

string Entry::getConcat(){
    return name+";"+password+";"+category+";"+website+";"+login;
}

const string &Entry::getName() const {
    return name;
}
void Entry::setName(const string &name) {
    Entry::name = name;
}
const string &Entry::getPassword() const {
    return password;
}
void Entry::setPassword(const string &password) {
    Entry::password = password;
}
const string &Entry::getCategory() const {
    return category;
}
void Entry::setCategory(const string &category) {
    Entry::category = category;
}
const string &Entry::getWebsite() const {
    return website;
}
void Entry::setWebsite(const string &website) {
    Entry::website = website;
}
const string &Entry::getLogin() const {
    return login;
}
void Entry::setLogin(const string &login) {
    Entry::login = login;
}

Entry::~Entry() = default;
