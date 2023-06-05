#include "Neo4JHttpRequest.h"

#include <cstdlib>

#include "BioLog.h"
#include "MsgImport.h"
#include <curl/curl.h>
#include "FileUtils.h"

using namespace Log;

static size_t writeCallback(char* contents, size_t size, size_t nmemb,
                            std::string* userp) {

    userp->append(contents, nmemb);
    return size * nmemb;
}

Neo4JHttpRequest::Neo4JHttpRequest(Neo4JHttpRequest::RequestProps&& props)
    : _url(props.host + ":" + std::to_string(props.port) +
           "/db/data/transaction/commit"),
      _username(props.user),
      _password(props.password)
{
}

Neo4JHttpRequest::~Neo4JHttpRequest() {
}

bool Neo4JHttpRequest::exec() {
    _data = "";
    CURL* curl = curl_easy_init();
    if (!curl) {
        // Could not initialize curl
        return false;
    }

    _jsonRequest =
        "{\"statements\": [{\"statement\": \"" + _statement + "\"}]}";

    std::string userPwd = _username + ":" + _password;

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, _jsonRequest.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.81.0");
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    BioLog::log(msg::INFO_SEND_NEO4J_HTTP_REQUEST());
    CURLcode res = curl_easy_perform(curl);

    if (res != 0) {
        BioLog::log(msg::ERROR_NEO4J_HTTP_REQUEST() << _statement);
        return false;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return true;
}

bool Neo4JHttpRequest::writeToFile(const std::filesystem::path& path) const {
    return FileUtils::writeFile(path, _data);
}


