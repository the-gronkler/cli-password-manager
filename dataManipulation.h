#ifndef PASSWORDMANAGER_DATAMANIPULATION_H
#define PASSWORDMANAGER_DATAMANIPULATION_H

#include <string>
#include <vector>
#include <chrono>
#include <array>


/**
 * @brief Checks if all characters in a string are digits.
 *
 * @param s The input string to check.
 * @return `true` if all characters in the string are digits, `false` otherwise.
 */
bool isDigits(std::string const& s);

/**
 * @brief Splits a string into substrings based on a delimiter character.
 *
 * @param str The input string to be split.
 * @param del The delimiter character.
 * @return A vector containing the substrings obtained after splitting the input string.
 */
std::vector<std::string> split(const std::string& str, char del);

/**
* @brief Compares two strings for equality in a case-insensitive manner.
*
* @param s1 The first string to compare.
* @param s2 The second string to compare.
* @return `true` if the strings are equal, `false` otherwise.
*/
bool strEquals(const std::string& s1, const std::string& s2);

/**
 * @brief Encrypts a string by adding a key to each character.
 *
 * Function takes an input string `data` and a key string `key` and performs encryption
 * by adding the corresponding key character to each character in the input string.
 * If the key string is shorter than the input string, it wraps around to the beginning of the key
 *
 * @param data The input string to encrypt.
 * @param key The key used for encryption.
 * @return The encrypted string.
 */
std::string encrypt(std::string const& data, std::string const& key);

/**
 * @brief Decrypts a string by subtracting a key from each character.
 *
 * Function takes an input string `data` and a key string `key` and performs decryption
 * by subtracting the corresponding key character from each character in the input string.
 * If the key string is shorter than the input string, it wraps around to the beginning of the key.
 *
 * @param data The input string to decrypt.
 * @param key The key used for decryption.
 * @return The decrypted string.
 */
std::string decrypt(std::string const& data, std::string const& key);

/**
 * @brief Converts the given string to lowercase.
 *
 * @param s The input string.
 * @return The string converted to lowercase.
 */
std::string toLowerCase(std::string s);

/**
* @brief Checks if the larger of the two strings starts with the smaller string.
*
* @param s1 The first string.
* @param s2 The second string.
* @return True if the beginning of the strings matches, false otherwise.
*/
bool stringBeginMatches(const std::string& s1, const std::string& s2);

/**
 * @brief Checks a password for security issues and returns a string describing them.
 * If no issues are found function returns empty string.
 *
 * @param password The password to check.
 * @return A string describing any issues with the password.
 */
std::string getPasswordIssues(const std::string& password);

/**
 * @brief Generates a random password with the specified characteristics.
 *
 * @param size The length of the password.
 * @param includeUppercase Whether to include uppercase letters in the password.
 * @param includeSpecial Whether to include special characters in the password.
 * @return The generated password.
 */
std::string generatePassword(int size, bool includeUppercase, bool includeSpecial);



#endif //PASSWORDMANAGER_DATAMANIPULATION_H
