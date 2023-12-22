
#include <string>
#include <vector>
#include "dataManipulation.h"


#ifndef PASSWORDMANAGER_ENTRY_H
#define PASSWORDMANAGER_ENTRY_H


using namespace std;
/**
 * @brief Class representing an entry in a password manager.
 *
 * An Entry object holds information about a specific entry in a password manager,
 * such as the name, password, category, website, and login information.
 * It provides methods to construct an entry from a serialized string representation,
 * retrieve a concatenated string representation of the entry, and initialize an entry
 * with specific properties.
 */
class Entry {
private:
    string name, password, category, website, login;
 /**
 * @brief Splits a string into substrings based on a delimiter character.
 *
 * This function splits a given string into substrings based on the specified delimiter character.
 * Note that if there are two delimiter characters in a row an empty string will be added to the result vector.
 *
 * @param str The input string to be split.
 * @param del The delimiter character.
 * @return vector<string> A vector containing the resulting substrings.
 */
    static auto splitEntryParams(const string& str, char del) -> vector<string>;
public:
    /**
 * @brief Constructs an Entry object with specific properties.
 *
 * This constructor initializes an Entry object with the provided values for
 * the name, password, category, website, and login properties.
 *
 * @param name The name of the entry.
 * @param password The password of the entry.
 * @param category The category of the entry.
 * @param website The website associated with the entry.
 * @param login The login information for the entry.
 */
    Entry(string name, string password, string category, string website, string login);
    /**
 * @brief Constructs an Entry object from a string representation.
 *
 * This constructor takes a string representation of an entry, where
 * the different properties of the entry (name, password, category, website, and login)
 * are separated by semicolons,like "name;password;category;website;login".
 * It splits the string into individual properties and initializes the corresponding member variables of the Entry object.
 *
 * @param dataString The string representation of the entry.
 * @throws std::invalid_argument if the string argument is not formatted correctly
 */
    explicit Entry(const string& dataString);

    /**
 * @brief Copy constructor for the Entry class.
 *
 * This constructor initializes an Entry object by performing a member-wise copy of another Entry object.
 *
 * @param other The Entry object to be copied.
 * @return void
 */
    Entry(const Entry& other);

    /**
 * @brief Print the entry in tabular format.
 *
 * This method prints the entry as a formatted table
 * with a header containing the names of the entry's fields,
 * and a single row containing their values
 */
    void printAsTable() const;

    /**
 * @brief Prints a range of entries in as a table
 *
 * This function prints a range of Entry objects in a table with 5 columns.
 * The top row contains the names of the fields, each next row contains a single entry object.
 *
 * @param begin An iterator pointing to the beginning of the range.
 * @param end An iterator pointing to the end of the range.
 * @return size_t The number of entries printed.
 */
    static size_t printRangeAlligned(vector<Entry>::iterator begin, vector<Entry>::iterator end);



    /**
 * @brief Overloaded assignment operator for the Entry class.
 *
 * This operator assigns the member variables of the Entry object on the right-hand side to the Entry object on the left-hand side.
 * It performs a member-wise copy of the values.
 *
 * @param other The Entry object to assign from.
 * @return Entry& A reference to the modified Entry object.
 */
    Entry& operator=(const Entry& other);

/**
 * @brief Overloaded stream insertion operator for the Entry class.
 *
 * This operator allows an Entry object to be printed to an output stream.
 * It prints the name, password, category, website, and login of the entry.
 *
 * @param os The output stream.
 * @param entry The Entry object to be printed.
 * @return ostream& A reference to the output stream.
 */
    friend ostream& operator<<(ostream& os, const Entry& entry);

    /**
* @brief Overloaded equality operator for the Entry class.
*
* This operator compares two Entry objects for equality based on their member variables.
*
* @param other The Entry object to compare against.
* @return bool True if the all fields of the Entry objects are equal(case insensitive), false otherwise.
*/
    bool operator==(const Entry& other);



    /**
* @brief Retrieves a concatenated string representation of the entry.
*
* This method returns a string representation of the Entry object, where the
* different properties of the entry (name, password, category, website, and login)
* are concatenated with semicolons as separators.
*
* @return A string representation of the entry with properties concatenated.
*/
    string getConcat();

    /**
 * @brief Getter method for field name.
 *
 * @return const string& name
 */
    [[nodiscard]] const string &getName() const;
    /**
 * @brief Setter method for field name.
 *
 * This method sets field name of the Entry object to the provided value.
 *
 * @param name The new value for field name.
 * @return void
 */
 
    void setName(const string &name);
    /**
 * @brief Getter method for field password.
 *
 * @return const string& password
 */
    [[nodiscard]] const string &getPassword() const;
    /**
* @brief Setter method for field password.
*
* This method sets field password of the Entry object to the provided value.
*
* @param password The new value for field password.
* @return void
*/
    void setPassword(const string &password);
    /**
* @brief Getter method for field category.
*
* @return const string& category
*/
    [[nodiscard]] const string &getCategory() const;
    /**
* @brief Setter method for field category.
*
* This method sets field category of the Entry object to the provided value.
*
* @param category The new value for field category.
* @return void
*/
    void setCategory(const string &website);
    /**
* @brief Getter method for field website.
*
* @return const string& website
*/
    [[nodiscard]] const string &getWebsite() const;
    /**
* @brief Setter method for field website.
*
* This method sets field website of the Entry object to the provided value.
*
* @param website The new value for field website.
* @return void
*/
    void setWebsite(const string &website);
    /**
* @brief Getter method for field login.
*
* @return const string& login
*/
    [[nodiscard]] const string &getLogin() const;
    /**
* @brief Setter method for field login.
*
* This method sets field login of the Entry object to the provided value.
*
* @param login The new value for field login.
* @return void
*/
    void setLogin(const string &login);

    /**
 * @brief Destructor for the Entry class.
 *
 * This is the destructor for the Entry class. It has a default implementation.
 *
 * @return void
 */
    virtual ~Entry();
};

#endif //PASSWORDMANAGER_ENTRY_H