#include "sentiment.hpp" // contains all headers and relevant function definitions. 

using json = nlohmann::json;
// Curl callback function
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Fetch tweets from Twitter API
std::string fetchTweets(const std::string& query, const std::string& bearerToken)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://api.twitter.com/2/tweets/search/recent?query=" + query;
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + bearerToken).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    
    return readBuffer;
}

// Parse and print tweets with sentiment analysis
void parseAndPrintTweets(const std::string& jsonStr) {
    try {
        // Parse the JSON string
        json jsonData = json::parse(jsonStr);

        // Iterate over each tweet
        for (const auto& tweet : jsonData["data"]) {
            std::string text = tweet["text"];
            int sentimentScore = analyzeSentiment(text);
            std::cout << "Tweet: " << text << "\nSentiment Score: " << sentimentScore << std::endl;
        }
    } catch (json::parse_error& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
    }
}


// Basic sentiment analysis function
std::map<std::string, int> sentimentDictionary = {
    {"good", 1}, {"great", 2}, {"excellent", 3},
    {"bad", -1}, {"terrible", -2}, {"awful", -3}
};

int analyzeSentiment(const std::string& text)
{
    int sentimentScore = 0;
    std::string word;
    std::istringstream stream(text);
    while (stream >> word) {
        // Convert to lowercase
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        // Remove punctuation
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());

        if (sentimentDictionary.find(word) != sentimentDictionary.end()) {
            sentimentScore += sentimentDictionary[word];
        }
    }
    return sentimentScore;
}

int main()
{
    std::string bearerToken = "your_bearer_token";
    std::string query = "OpenAI";
    std::string tweetsJson = fetchTweets(query, bearerToken);

    parseAndPrintTweets(tweetsJson);

    return 0;
}
