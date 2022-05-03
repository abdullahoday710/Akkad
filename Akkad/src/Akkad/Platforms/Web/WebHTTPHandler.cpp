#include "WebHTTPHandler.h"
#include <emscripten/fetch.h>

#include <iostream>
#include "Akkad/Logging.h"
namespace Akkad {
    namespace NET {

        struct EmFetchUserData
        {
            void* requestData;
            void* token;
            std::function<void(AsyncHTTPResponse)> callback;
        };

        void FetchCB(emscripten_fetch_t* fetch) {
            EmFetchUserData* userdata = (EmFetchUserData*)fetch->userData;
            AsyncHTTPResponse response;
            response.statusCode = fetch->status;
            for (size_t i = 0; i < fetch->numBytes; i++)
            {
                response.responseData += fetch->data[i];
            }
            response.isValid = true;
            userdata->callback(response); // call the user provided lambda or function...
            emscripten_fetch_close(fetch); // Free data associated with the fetch.
            free(userdata->requestData);
            free(userdata);
        }

        void WebHTTPHandler::SendRequest(std::string url, RequestMethod method, std::string requestdata, std::function<void(AsyncHTTPResponse)> callback)
        {
            emscripten_fetch_attr_t attr;
            emscripten_fetch_attr_init(&attr);

            EmFetchUserData* emUserData = new EmFetchUserData();


            const char* headers[] = { "Content-Type", "application/json", NULL };
            attr.requestHeaders = headers;

            switch (method)
            {
            case Akkad::NET::RequestMethod::GET:
                strcpy(attr.requestMethod, "GET");
                break;
            case Akkad::NET::RequestMethod::POST:
                strcpy(attr.requestMethod, "POST");
                break;
            default:
                break;
            }

            attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
            attr.onsuccess = FetchCB;
            attr.onerror = FetchCB;

            if (!requestdata.empty())
            {
                // Fuck emscripten....
                const char* data = requestdata.c_str();
                attr.requestData = (const char*)malloc(requestdata.size() * sizeof(char));
                strcpy((char*)attr.requestData, data);
                attr.requestDataSize = strlen(attr.requestData);

                emUserData->requestData = (void*)attr.requestData;
            }
            emUserData->callback = callback;
            attr.userData = (void*)emUserData;
            emscripten_fetch(&attr, url.c_str());
        }
        void WebHTTPHandler::SendRequest(std::string url, RequestMethod method, std::string requestdata, std::string authToken, std::function<void(AsyncHTTPResponse)> callback)
        {
            emscripten_fetch_attr_t attr;
            emscripten_fetch_attr_init(&attr);

            EmFetchUserData* emUserData = new EmFetchUserData();

            std::string token = "Token ";
            token += authToken;
            const char* headers[] = { "Content-Type", "application/json", "Authorization", "", NULL };
            
            strcpy((char*)headers[3], token.c_str());
            attr.requestHeaders = headers;

            switch (method)
            {
            case Akkad::NET::RequestMethod::GET:
                strcpy(attr.requestMethod, "GET");
                break;
            case Akkad::NET::RequestMethod::POST:
                strcpy(attr.requestMethod, "POST");
                break;
            default:
                break;
            }

            attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
            attr.onsuccess = FetchCB;
            attr.onerror = FetchCB;

            if (!requestdata.empty())
            {
                // Fuck emscripten....
                const char* data = requestdata.c_str();
                attr.requestData = (const char*)malloc(requestdata.size() * sizeof(char));
                strcpy((char*)attr.requestData, data);
                attr.requestDataSize = strlen(attr.requestData);

                emUserData->requestData = (void*)attr.requestData;
            }
            emUserData->callback = callback;
            attr.userData = (void*)emUserData;
            emscripten_fetch(&attr, url.c_str());
        }
    }
}
