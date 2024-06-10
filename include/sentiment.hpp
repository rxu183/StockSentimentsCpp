#pragma once // this tells the c++ compiler that things should only be compiled once.
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <nlohmann/json.hpp>

/**
 * @brief Write a callback ?? apparently this is like an event noticer, kind of like Accept in C, except without a "while true" loop.
 * 
 * @param contents 
 * @param size 
 * @param nmemb 
 * @param userp 
 * @return size_t 
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

/**
 * @brief This is a string that fetches tweets with the API.
 * 
 * @param query 
 * @param bearerToken Token for calling the API. Should we publish to github? Surely not, let's keep it in a gitignore.
 * @return std::string Returns a string of tweets based on the query.
 */
std::string fetchRedditPosts(const std::string& subreddit, const std::string& accessToken);

/**
 * @brief Parses the input jsonString, and then prints it to screen.
 * 
 * @param jsonStr This is a string jsonStr that denotes the input.
 */
void parseAndPrintRedditPosts(const std::string& jsonStr);

/**
 * @brief This denotes the string. 
 * 
 * @param text This is a string denoting the input string.
 * @return int Returns the score of the sentiment.
 */
int analyzeSentiment(const std::string& text);

void readSecrets(std::string& clientId, std::string& clientSecret, std::string& username, std::string& password);