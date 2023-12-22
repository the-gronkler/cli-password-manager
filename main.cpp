#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <array>
#include <regex>
#include <set>
#include <algorithm>

#include "Entry.h"
#include "dataManipulation.h"
#include "main.h"
auto getFilePath() -> std::filesystem::path{
    using namespace std;
    auto line = string();
    while(true){
        cout<<"Would you like to:\n (1) Use default file\n (2) Select file from current directory\n (3) Provide path to file\n> ";
        getline(cin, line);

        if(line == "1") {
            auto path = filesystem::path(filesystem::current_path().string().append(R"(\passwords.txt)"));
            if (exists(path))
                return path;
            else
                cout << "File not found\n\n";
        }
        else if(line == "2") {
            auto path = filesystem::current_path();
            auto files = vector<filesystem::directory_entry>();
            auto dirIter = std::filesystem::recursive_directory_iterator(path);
            for (auto const& entry : dirIter)
                if (entry.is_regular_file() && entry.path().extension() == ".txt")
                    files.push_back(entry);
            if(files.empty()){
                cout << "No valid files found in " << path << ", try again\n\n";
                continue;
            }
            cout << path << ":\n";
            for(int i = 0; i < files.size(); i++){
                cout << i << ". " << files[i].path().string().erase(0, path.string().size()+1) << "\n";
            }
            cout << "enter index of file from selection(\'e\' to go back): ";
            while(true){
                getline(cin, line);
                if (line == "e")
                    break;
                if(isDigits(line)) {
                    auto index = stoi(line);
                    if (index >= 0 && index < files.size())
                        return files[index].path();
                    else
                        cout << "enter valid index\n";
                }
                else
                    cout << "enter valid index\n";
            }
        }
        else if(line == "3") {
            cout << "Enter source file path: ";
            getline(cin, line);
            auto file = filesystem::directory_entry(line);
            if( exists(file) && file.is_regular_file() && file.path().extension() == ".txt" )
                return file.path();
            else
                cout << "Invalid path, try again\n\n";
        }
        else
            cout << "invalid answer. please enter number in range [ 1 - 3 ]\n>";
        line = "";
    }
}

auto parseFileToMemory() -> void{
    auto line = string();
    auto infile = ifstream(filePath);

    fileParsed = chrono::system_clock::to_time_t(chrono::system_clock::now());

    if (infile.peek() == ifstream::traits_type::eof()){
        cout << "empty source file successfully selected\n";
        return;
    }

    int count = 0, hour, minute, second;
    while(getline(infile, line)){
        if (count == 11) {
            hour = stoi(line.substr(0, 2));
            line = line.substr(2);
        }
        else if (count == 22) {
            minute = stoi(line.substr(0, 2));
            line = line.substr(2);
        }
        else if (count == 33) {
            second = stoi(line.substr(0, 2));
            line = line.substr(2);
        }

        if(!line.empty())
            try{
                auto tmp = Entry(decrypt(line, globalPassword));
                categories.insert(tmp.getCategory());
                entries.push_back(tmp);
            }
            catch(const invalid_argument& e){
                cerr << "Error while reading file: "
                        "Either the file is not formatted correctly or a wrong password was provided.\n";
                exit(3);
            }
        count++;
    }

    fileParsed = chrono::system_clock::to_time_t(chrono::system_clock::now());


    cout << "Last decryption attempt was at " << hour << ":" << minute << ":" << second << endl;
    cout << "Source file successfully loaded\n";
}

auto saveEdits() -> void{
    ofstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Cannot save changes, failed to open the file.\n";
        return;
    }

    std::tm* lastDecryptionAttempt = std::localtime(&fileParsed);
    int lineCount = 0;
    for(auto it = entries.begin(); it != entries.end() || lineCount < 34; lineCount++) {
        if (lineCount == 11)
            file << setfill('0') << setw(2) << lastDecryptionAttempt->tm_hour;
        else if (lineCount == 22)
            file << setfill('0') << setw(2) << lastDecryptionAttempt->tm_min;
        else if (lineCount == 33)
            file << setfill('0') << setw(2) << lastDecryptionAttempt->tm_sec;

        if(lineCount < entries.size())
            file << encrypt(it->getConcat(), globalPassword) << endl;
        else
            file << endl;

        if(it != entries.end())
            it++;
    }
    file.close();
    cout << "Changes saved\n";
}


auto getPasswordFromUser() -> string {
    cout << endl;
    string password, line;
    while (true){
        cout << "would you like to:\n    1. get an automatically generated password\n    2. enter a password yourself\n>";
        getline(cin, line);

        if(line == "1"){
            cout << "you may select settings for generating your password\n"<<
            "the settings are [size] [include Uppercase letters] [include special characters]\n" <<
            R"(for example "11 y n" or "15 n y")" << endl << ">";
            getline(cin, line);

            auto params = split(line, ' ');
            auto size = int();
            auto includeUppercase = bool(), includeSpecial = bool();

            if(params.size() < 3)
                continue;

            if (isDigits(params[0]))
                size = stoi(line);
            else {
                cout << "entered size parameter is not a number, try again\n";
                continue;
            }

            if(params[1] == "y")
                includeUppercase = true;
            else if(params[1] == "n")
                includeUppercase = false;
            else{
                cout << R"(entered include uppercase letters parameter is not 'y' or 'n', try again)" << endl;
                continue;
            }

            if(params[2] == "y")
                includeSpecial = true;
            else if(params[2] == "n")
                includeSpecial = false;
            else{
                cout << R"(entered include special characters parameter is not 'y' or 'n', try again)" << endl;
                continue;
            }

            password = generatePassword(size, includeUppercase, includeSpecial);

            cout << "do you want to proceed with password \'" << password << "\'?(y/n): ";
            while (true) {
                getline(cin, line);
                if (line == "y")
                    return password;
                else if (line == "n")
                    break;
                else
                    cout << "invalid answer, please enter \'y\' or \'n\'.\n>";
                line.clear();
            }
        }
        else if (line == "2"){
            cout << "Please enter password: ";
            getline(cin, password);
            if (password.find(';') != string::npos)
                cout << "Forbidden character: \';\'\n";
            else if (!password.empty()) {
                string issues = getPasswordIssues(password);
                if (!issues.empty())
                    cout << "issues found with password: \n" << issues;
                if(any_of(entries.begin(), entries.end(), [&](const Entry& e) { return e.getPassword() == password;}))
                    cout << "this password has been used before\n";
                cout << "do you want to proceed with password \'" << password << "\'?(y/n): ";
                while (true) {
                    getline(cin, line);
                    if (line == "y")
                        return password;
                    else if (line == "n")
                        break;
                    else
                        cout << "invalid answer, please enter \'y\' or \'n\'.\n>";
                    line.clear();
                }
            }
        }
        else
            cout << "invalid answer. please enter number in range [ 1 - 2 ]\n>";
        line.clear();
    }
}

auto getCategoryIndexFromUser(const string& message) -> int{
    cout << endl;
    string line;
    while (true) {
        cout << message << "(you may enter 'exit' to cancel):\n";
        for (int i = 0; i < categories.size(); i++)
            cout << *next(categories.begin(), i) << " (" << i << ")\n";
        getline(cin, line);
        if (isDigits(line)) {
            int tmp = stoi(line);
            if (tmp < categories.size())
                return tmp;
            else
                cout << "index must be in range\n";
        }
        else if(stringBeginMatches("exit", line))
            return -1;
        else
            cout << "index must be a number\n";
        line.clear();
    }
}



auto addEntry() -> void {
    cout << endl;
    if(categories.empty()){
        cout << "0 categories recorded. Add at least 1 category before adding passwords\n";
        return;
    }

    string name, password, category, website, login;
    string line;
    cout << "Writing new entry:\n";

    while (true){
        cout << "Please enter name: ";
        getline(cin, name);
        if (name.find(';') != string::npos)
            cout << "Forbidden character: \';\', try again\n";
        else if(!name.empty())
            break;
        else
            cout << "name cannot be empty. try again\n";
    }

    password = getPasswordFromUser();

    int categoryIndex = getCategoryIndexFromUser("select valid category by entering index");
    if(categoryIndex == -1)
        return;
    category = *next(categories.begin(), categoryIndex);

    while (true){
        cout << "Enter name of website/service (optional): ";
        getline(cin, website);
        if (website.find(';') != string::npos)
            cout << "Forbidden character: \';\'\n";
        else
            break;
    }
    while(true){
        cout << "Enter login (optional): ";
        getline(cin, login);
        if (login.find(';') != string::npos)
            cout << "Forbidden character: \';\'\n";
        else
            break;
    }

    cout <<
         "Your entry is:" <<
         "\nname: " << name <<
         "\npassword: " << password <<
         "\ncategory: " << category <<
         "\nwebsite/service: " << (website.empty() ? "none" : website) <<
         "\nlogin: " << (login.empty() ? "none" : login) <<
         "\nIs that correct?(y/n): ";
    while (true){
        getline(cin, line);
        if(line == "y"){
            entries.emplace_back(name, password, category, website, login);
            cout << "Entry recorded.\n";
            return;
        }
        else if(line == "n"){
            cout << "no entries recorded.\n";
            return;
        }
        else
            cout << "invalid answer, please enter \'y\' or \'n\'.\n>";
        line = "";
    }
}

auto deleteCategory() -> void {
    cout << endl;
    string line;
    int categoryIndex = getCategoryIndexFromUser("select category to delete by entering index");
    if (categoryIndex == -1)
        return;
    auto categoryToDelete = next(categories.begin(), categoryIndex);
    while (true) {
        line.clear();
        cout << "are you sure you want to delete category\'" << *categoryToDelete
             << "\'? This will delete ALL associated passwords. (y/n): ";
        getline(cin, line);
        if (line == "y") {
            categories.erase(categoryToDelete);
            entries.erase(remove_if(entries.begin(), entries.end(),[&](const Entry &entry) {
                              return strEquals(entry.getCategory(), *categoryToDelete);
                          }),
                          entries.end());

            cout << "category deleted\n";
            return;
        }
        if (line == "n") {
            cout << "no categories deleted\n";
            return;
        }
        else
            cout << "invalid answer, please enter \'y\' or \'n\'.\n>";
    }
}

auto addCategory() -> void{
    cout << endl;
    auto newCategory = string();
    while(true){
        newCategory.clear();
        cout << "Enter category name: ";
        getline(cin, newCategory);
        if(newCategory.find(';') != string::npos)
            cout << "Forbidden character: \';\'\n";
        else if (!newCategory.empty()){
            categories.insert(newCategory);
            return;
        }
        else
            cout << "category name cannot be empty. try again\n";
    }
}

auto modifyEntry(Entry& entry) -> void{
    string line;
    while(true){
        cout << endl;
        cout << "Modifying entry:\n";
        entry.printAsTable();
        cout << "What would you like to do?\n" <<
               "Change Name..............(1)\n" <<
               "Change Password..........(2)\n" <<
               "Change Category..........(3)\n" <<
               "Change Website/Service...(4)\n" <<
               "Change Login.............(5)\n" <<
               "Delete password..........(6)\n" <<
               "Exit.....................(7)\n>";
        getline(cin, line);
        // change name
        if(line == "1") {
            cout << "enter new name: ";
            getline(cin, line);
            entry.setName(line);
        }
        // change password
        else if(line == "2"){
            entry.setPassword(getPasswordFromUser());
        }
        // change category
        else if(line == "3"){
            if (categories.empty()){
                cout << "No categories recorded, please add new category\n";
                addCategory();
                continue;
            }
            for (int i = 0; i < categories.size(); i++)
                cout << *next(categories.begin(), i) << " (" << i << ")\n";
            int index;
            while (true) {
                line.clear();
                cout << "Please select new category by entering index:\n";
                getline(cin, line);

                if (isDigits(line)) {
                    index = stoi(line);
                    if (index < categories.size()){
                        entry.setCategory(*next(categories.begin(), index));
                        break;
                    }
                }
            }
        }
        // change website
        else if(line == "4"){
            cout << "enter new website: ";
            getline(cin, line);
            entry.setWebsite(line);
        }
        // change login
        else if(line == "5"){
            cout << "enter new login: ";
            getline(cin, line);
            entry.setLogin(line);
        }
        // delete
        else if(line == "6")
            while(true){
                line.clear();
                cout << "Are you sure you want to delete this password?(y/n): ";
                getline(cin, line);
                if(line == "y"){
                    entries.erase(find(entries.begin(), entries.end(), entry));
                    return;
                }
                else if(line == "n")
                    break;
                else
                    cout << "invalid answer, please enter \'y\' or \'n\'.\n>";
            }
        else if (line == "7")
            return;
        else
            cout << "invalid answer. please enter number in range [ 1 - 7 ]\n>";
        line.clear();
    }
}

auto deleteMultipleEntries(size_t size, const string& line) -> void{
    cout << endl;
    // extract indices from line
    vector<int> indices;
    stringstream ss(line);

    string num;
    getline(ss, num, ' ');

    while (getline(ss, num, ' ')) {
        if (ss.fail() || !isDigits(num)) {
            cout << "Invalid input. Expected valid indices after 'delete'.\n";
            return;
        }
        auto tmp = stoi(num);
        if(tmp < size)
            indices.push_back(stoi(num));
    }

    // delete entries
    sort(indices.rbegin(), indices.rend());
    for (auto index : indices)
        entries.erase(entries.begin() + index);

    cout << indices.size() << " password entries deleted\n";
}



auto sortEntries(vector<Entry>:: iterator end, string sortingQuery) -> void{
    cout << endl;
    auto sortCriteriaPattern = regex(R"((name|password|category|website|login)\s+(asc|desc))");
    sregex_iterator sortingRegexIt(sortingQuery.begin(), sortingQuery.end(), sortCriteriaPattern);
    sregex_iterator endIt;

    vector<pair<function<const string&(const Entry&)>, int>> sortParameters; // int is direction, 0 for ascending, 1 for descending

    while(sortingRegexIt != endIt) {
        int dir;
        string direction = (*sortingRegexIt)[2];
        if (strEquals(direction, "asc"))
            dir = 0;
        else if (strEquals(direction, "desc"))
            dir = 1;
        else {
            sortingRegexIt++;
            continue; // no valid direction means not valid sorting command
        }

        string parameters = (*sortingRegexIt)[1];
        if(strEquals(parameters, "name"))
            sortParameters.emplace_back(&Entry::getName, dir);
        else if(strEquals(parameters, "password"))
            sortParameters.emplace_back(&Entry::getPassword, dir);
        else if(strEquals(parameters, "category"))
            sortParameters.emplace_back(&Entry::getCategory, dir);
        else if(strEquals(parameters, "website"))
            sortParameters.emplace_back(&Entry::getWebsite, dir);
        else if(strEquals(parameters, "login"))
            sortParameters.emplace_back(&Entry::getLogin, dir);
        else{
            sortingRegexIt++;
            continue; // no valid criteria means not valid sorting command
        }
        sortingRegexIt++;
    }

    ranges::sort(entries.begin(), end, [&sortParameters](Entry& e1, Entry& e2) -> bool{
        for(const auto& i : sortParameters){
            auto val1 = i.first(e1), val2 = i.first(e2);
            if(strEquals(val1, val2))
                continue;
            if(i.second == 0) return
                !lexicographical_compare(val1.begin(), val1.end(), val2.begin(), val2.end());
            else return
                lexicographical_compare(val1.begin(), val1.end(), val2.begin(), val2.end());
        }
        return false;
    });
}

auto viewAllEntries(const string& query) -> void;
auto searchEntries(const string& query) -> void;

auto operationsOnDisplayedRange(size_t size) -> void {
    cout << "\n- to edit password enter 'edit [index of password you want to edit]'\n" <<
         "- to delete passwords enter 'delete [indices of passwords you want to delete]'\n" <<
         "- to view all passwords enter \'viewall\'" <<
         "- to search enter \'search [query]\'\n" <<
         "- to go back enter 'back'\n>";

    string line;
    smatch match;
    while (true) {
        getline(cin, line);

        if (regex_search(line, match, editPattern)) {
            int index = stoi(match[1].str());
            if (index < size) {
                modifyEntry(entries.at(index));
                return;
            } else
                cout << "invalid index in \'edit\' command\n";
        } else if (stringBeginMatches("search", line)) {
            searchEntries(line);
            return;
        } else if (stringBeginMatches("viewall", line)) {
            viewAllEntries(line);
            return;
        } else if (stringBeginMatches("delete", line)) {
            deleteMultipleEntries(size, line);
            return;
        } else if (stringBeginMatches("back", line))
            return;
        else
            cout << "command not recognised, try again.\n>";
        line.clear();
    }
}
auto searchEntries(const string& query) -> void{
    cout << endl;
    if(entries.empty()){
        cout << "No passwords recorded\n";
        return;
    }
    smatch match;
    string nameSearch, passwordSearch, categorySearch, websiteSearch, loginSearch;

    regex_search(query, match, regex("name:\\{(.*?)\\}"));
    nameSearch = match[1].str();
    regex_search(query, match, regex("password:\\{(.*?)\\}"));
    passwordSearch = match[1].str();
    regex_search(query, match, regex("category:\\{(.*?)\\}"));
    categorySearch = match[1].str();
    regex_search(query, match, regex("website:\\{(.*?)\\}"));
    websiteSearch = match[1].str();
    regex_search(query, match, regex("login:\\{(.*?)\\}"));
    loginSearch = match[1].str();

    vector<Entry>::iterator endOfSearchResults;
    if (nameSearch.empty() && passwordSearch.empty() && categorySearch.empty() && loginSearch.empty() && websiteSearch.empty())
        endOfSearchResults = entries.end();
    else {
        endOfSearchResults = partition(entries.begin(), entries.end(), [&](const Entry &e) {
            return (!nameSearch.empty() && toLowerCase(e.getName()).find(toLowerCase(nameSearch)) != string::npos) ||
                   (!passwordSearch.empty() && toLowerCase(e.getPassword()).find(toLowerCase(passwordSearch)) != string::npos) ||
                   (!categorySearch.empty() && toLowerCase(e.getCategory()).find(toLowerCase(categorySearch)) != string::npos) ||
                   (!loginSearch.empty() && toLowerCase(e.getLogin()).find(toLowerCase(loginSearch)) != string::npos) ||
                   (!websiteSearch.empty() && toLowerCase(e.getWebsite()).find(toLowerCase(websiteSearch)) != string::npos);
        });
        if(endOfSearchResults == entries.begin()) {
            cout << "No passwords found for your query\n";
            return;
        }
    }

    //sort search results
    if(regex_search(query, match, sortPattern)) {
        sortEntries(endOfSearchResults, match[1]);
    }

    operationsOnDisplayedRange(Entry::printRangeAlligned(entries.begin(), endOfSearchResults));
}
auto viewAllEntries(const string& query) -> void {
    cout << endl;
    if(entries.empty()){
        cout << "No passwords recorded\n";
        return;
    }

    smatch match;
    if(regex_search(query, match, sortPattern))
        sortEntries(entries.end(), match[1]);

    Entry::printRangeAlligned(entries.begin(), entries.end());
    operationsOnDisplayedRange(entries.size());
}


int main(){
    auto line = string();

    cout << "Welcome to PasswordManager :)\n";
    filePath = getFilePath();

    // get password to file
    do{
        cout << "enter password to source file: ";
        getline(cin, globalPassword);
        cout << "do you want to proceed with password \'" << globalPassword << "\'? (y/n)\n>";
        while (true){
            getline(cin, line);
            if (line == "y")
                break;
            else if (line == "n"){
                globalPassword = "";
                break;
            }
            else
                cout << "invalid answer, please enter \'y\' or \'n\'.\n>";
        }
    }while(globalPassword.empty());

    parseFileToMemory();

    const string mainMenuText = "\nPassword manager is ready to use. You may manage your passwords by entering the commands listed below."
                          "\n-> \'viewall\' - view all passwords in no particular order.\n"
                          
                          "-> \'search name:{your search} category:{your search} website:{your search} login:{your search}\'\n"
                          "   you may enter these in any order and omit any of them, invalid syntax will be ignored.\n"

                          "-> you may also add \'sortby:{parameter asc/desc, parameter asc/desc, ...}\' to commands \'viewall\' and \'search\'\n\n"
                          
                          "-> \'add password\' - add new password\n"
                          
                          "-> \'add category\' - add new category\n"
                          
                          "-> \'delete category\' - delete category\n\n"

                          "-> 'save' - Save changes\n"

                          "-> 'quit' - quit\n";

    cout << mainMenuText;
    while (true){
        cout << ">";
        getline(cin, line);
        // get command
        if (stringBeginMatches("viewall", line)){
            viewAllEntries(line);
            cout << mainMenuText;
        }
        else if (stringBeginMatches("search", line)){
            searchEntries(line);
            cout << mainMenuText;
        }
        else if (stringBeginMatches("add password", line)){
            addEntry();
            cout << mainMenuText;
        }
        else if (stringBeginMatches("add category", line)){
            addCategory();
            cout << mainMenuText;
        }
        else if (stringBeginMatches("delete category", line)){
            deleteCategory();
            cout << mainMenuText;
        }
        else if (stringBeginMatches("save", line)){
            saveEdits();
        }
        else if (stringBeginMatches("quit", line)){
            cout << "Thank you for using Password manager. Goodbye\n";
            return 0;
        }
        else{
            cout << "command not recognised, try again.\n";
        }
        line.clear();
    }
    return 0;
}
