
#ifndef PASSWORDMANAGER_MAIN_H
#define PASSWORDMANAGER_MAIN_H

#endif //PASSWORDMANAGER_MAIN_H

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

/**
 * @brief the set of categories.
 */
std::set<std::string> categories;
/**
 * @brief this vector stores all entries.
 */
std::vector<Entry> entries;
/**
 * @brief Path to the source file.
 */
filesystem::path filePath;
/**
 * @brief Password for the source file.
 */
std::string globalPassword;

/**
 * @brief the time when the source file was parsed.
 */
time_t fileParsed;

const regex
/**
 * @brief regex pattern for the edit command.
 */
editPattern = regex(R"(edit (\d+))"),
/**
 * @brief regex pattern for the sort command.
 */
sortPattern = regex(R"( sortby:\{(.*?)\})");

/**
 * @brief Get the path to source text file from user.
 *
 * This function prompts the user to select the file path from different options:\n\n
 *
 * 1. Use the default file path, which will return path to file "passwords.txt" in the current directory\n\n
 *
 * 2. Select a file from the current directory, which will display all .txt files
 * in the current directory and ask the user to select one of them \n\n
 *
 * 3. Provide the path to the file. user may enter an absolute path to a text file\n
 *
 * @return The selected file path.
 */
auto getFilePath() -> std::filesystem::path;

/**
 * @brief Parse the encrypted source file and load its content into memory.
 *
 * For each non-empty line in the encrypted source file this function will create a new element in the entries vector
 * assuming that the line, after decryption, is formatted like \n
 * "name;password;category;website;login".\n
 * It will also add each category to the categories set.\n\n
 *
 * Lines 11, 22, 33 should start with two numbers that represent the hour, minute, and second of the last decryption attempt respectively.
 * The time of the last decryption attempt will be printed out.\n\n
 *
 * The function will record the time of the current decryption attempt\n\n
 *
 * Upon completion, the function will notify the user.
 *
 * @return void
 */
auto parseFileToMemory() -> void;

/**
 * @brief Overwrite source file with updated data.
 *
 * This function overwrites the data in the source file with data from vector entries
 * in the format "name;password;category;website;login\\n" for each entry.\n\n
 *
 * the time of the last decryption attempt will also be saved to the file with \n
 * hour written to the start of line 11,\n
 * minute written to the start of line 22\n
 * second written to the start of line 33\n
 *
 * @return void
 */
auto saveEdits() -> void;


/**
 * @brief Get a password from the user.
 *
 * This function allows the user to choose between entering a password manually and
 * generating a password with changeable parameters size, whether to include uppercase letters
 * and whether to include special characters.
 *
 * @return A password.
*/
auto getPasswordFromUser() -> string;

/**
 * @brief Get the index of a category selected by the user.
 *
 * This function prints out the categories set and prompts the user to select the index of one of the displayed categories.
 *
 * @param message The message to display when prompting for the index.
 * @return The selected category index.
 */
auto getCategoryIndexFromUser(const string& message) -> int;



/**
 * @brief Add a new entry.
 *
 * This function allows the user to add a new entry to vector entries by providing the values for fields
 * name, password, category, website, and login. website and login may be omitted.
 * @return void
 */
auto addEntry() -> void;

/**
 * @brief Delete a category and its associated passwords.
 *
 * This function allows the user to delete a category and all the passwords associated with it.
 * @return void
 */
auto deleteCategory() -> void ;

/**
 * @brief Add a new category.
 *
 * This function gets the name of the new category from the user and then adds it to the categories set.
 * @return void
 */
auto addCategory() -> void;

/**
 * @brief Modify an existing entry.
 *
 * This function allows the user to delete an existing entry or to change the values of its fields
 *
 * @param entry The entry to modify.
 * @return void
 */
auto modifyEntry(Entry& entry) -> void;

/**
 * @brief Deletes multiple entries
 *
 * This function deletes multiple entries from the password manager based on the provided indices.\n
 * It first extracts the indices from the given line and verifies their validity. \n
 * Then it deletes the corresponding entries.
 *
 * @param size The size of range from which the user is selecting the indices.
 * @param line The line containing the indices of entries to be deleted.
 * @return void
 */
auto deleteMultipleEntries(size_t size, const string& line) -> void;



/**
 * @brief Sorts the entries in range based on the provided query.
 *
 * This function extracts the sorting criteria from the query and then sorts the entries
 * in range from begin of the entries vector to the provided end iterator based on the criteria.
 *
 * @param end The iterator indicating the end of the range to be sorted.
 * @param sortingQuery The query for sorting the entries in the form\n
 * "name|password|category|website|login asc|desc" repeated however many times.
 * @return void
 */
auto sortEntries(vector<Entry>:: iterator end, string sortingQuery) -> void;

/**
 * @brief Displays all entries
 *
 * This function displays all entries, optionally sorted based on the given query.\n
 * It prints out all the entries in no particular order.
 * If a sorting query is provided, it sorts the entries accordingly using the sortEntries function.\n
 * After displaying the entries, it allows the user to enter a command to\n
 * - edit one of the displayed entries\n
 * - delete one or multiple displayed entries\n
 * - search through all entries\n
 *
 * @param query The query for sorting the entries.
 * @return void
 */
auto viewAllEntries(const string& query) -> void;
/**
 * @brief Searches and displays entries based on the provided query.
 *
 * This function searches for entries based on the provided query and displays the results.
 * If a sorting query is also provided, it sorts the search results using the sortEntries function.
 * After displaying the entries, it allows the user to enter a command to\n
 * - edit one of the displayed entries\n
 * - delete one or multiple displayed entries\n
 * - search through all entries again\n
 *
 * @param query The search query. formatted like:
 * "search name:{what to search} password:{what to search} category:{what to search} website:{what to search} login:{what to search}".
 * Any of these criteria may be omitted
 * @return void
 */
auto searchEntries(const string& query) -> void;

/**
 * @brief prompts the user to enter commands to perform operations on range of entries.
 *
 * @param size the size if the displayed range
 * @return void
 */
auto operationsOnDisplayedRange(size_t size) -> void;
auto searchEntries(const string& query) -> void;
auto viewAllEntries(const string& query) -> void;




/**
 * @brief The main function of the program.
 *
 * This function gets path to source file from user using getFilePath() function,
 * gets the password to the source file from user, and then parses the file to memory using parseFileToMemory() function.
 * Then the user may enter these commands: \n
 * - entering "viewall ..." will call viewAllEntries with the entered command passed to the function\n
 * - entering "search ..." will call searchEntries with the entered command passed to the function\n
 * - entering "add password" will call addEntry\n
 * - entering "add category" will call addCategory\n
 * - entering "delete category" will call deleteCategory\n
 * - entering "quit" will call saveEdits and then stop execution\n
 *
 * @return int
 */
int main();

