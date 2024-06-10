#include "sentiment.hpp" // contains all headers and relevant function definitions. 

using json = nlohmann::json;
// A few notes: 
// curl_slist is a linked list struct (pointer based)
// It has the following fields: 
// 

// Curl callback function
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
std::string getRedditAccessToken(const std::string& clientId, const std::string& clientSecret, const std::string& username, const std::string& password) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.reddit.com/api/v1/access_token");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        std::cout << "Desired Username: " << username << " Desired Pass: " << password << "\n";
        std::string postFields = "grant_type=password&username=" + username + "&password=" + password;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());

        std::string userCredentials = clientId + ":" + clientSecret;
        std::cout << "User Credentials are as follows: " << userCredentials << "\n";
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "User-Agent: StockSentimentCpp-0.1");


        curl_easy_setopt(curl, CURLOPT_USERPWD, userCredentials.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return readBuffer;
}

// Fetch posts from Reddit WallStreetBets
std::string fetchRedditPosts(const std::string& subreddit, const std::string& accessToken) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::cout << "Fetching Reddit posts!\n";
        std::string url = "https://oauth.reddit.com/r/" + subreddit + "/new";
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        headers = curl_slist_append(headers, "User-Agent: StockSentimentCpp-0.1");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    
    return readBuffer;
}

// Parse and print Reddit posts
void parseAndPrintRedditPosts(const std::string& jsonStr) {
    try {
        // Parse the JSON string
        json jsonData = json::parse(jsonStr);

        // Iterate over each post
        for (const auto& post : jsonData["data"]["children"]) {
            std::string title = post["data"]["title"];
            std::cout << "Post: " << title << std::endl;
        }
    } catch (json::parse_error& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
    }
}
// Function to read secrets from a file
void readSecrets(std::string& clientId, std::string& clientSecret, std::string& username, std::string& password) {
    std::ifstream file("secrets.txt");
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open secrets.txt" << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            if (key == "CLIENT_ID") clientId = value;
            else if (key == "CLIENT_SECRET") clientSecret = value;
            else if (key == "REDDIT_USERNAME") username = value;
            else if (key == "REDDIT_PASSWORD") password = value;
        }
    }
}

int main() {
    std::string clientId, clientSecret, username, password;
    readSecrets(clientId, clientSecret, username, password);

    
    std::string accessTokenJson = getRedditAccessToken(clientId, clientSecret, username, password);
    std::cout << "Access Token Response: " << accessTokenJson << std::endl;

    // Parse JSON response to extract the actual access token
    json accessTokenData = json::parse(accessTokenJson);
    std::string accessToken = accessTokenData["access_token"];

    std::string subreddit = "wallstreetbets";
    std::string postsJson = fetchRedditPosts(subreddit, accessToken);
    parseAndPrintRedditPosts(postsJson);

    return 0;
}