#include "WebHTTPHandler.h"
#include <emscripten/fetch.h>

#include <iostream>
namespace Akkad {
    namespace NET {

        void downloadSucceeded(emscripten_fetch_t* fetch) {
            // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
          //  printf("%s", fetch->data);
            AsyncHTTPResponse* resp = (AsyncHTTPResponse*)fetch->userData;
            resp->statusCode = fetch->status;
            for (size_t i = 0; i < fetch->numBytes; i++)
            {
                resp->responseData += fetch->data[i];
            }
            resp->isValid = true;
            emscripten_fetch_close(fetch); // Free data associated with the fetch.
        }

        void downloadFailed(emscripten_fetch_t* fetch) {
            printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
            emscripten_fetch_close(fetch); // Also free data on failure.
        }

        void WebHTTPHandler::SendRequest(SharedPtr<AsyncHTTPResponse> respObj, std::string url, RequestMethod method, std::string requestdata = "")
        {
            respObj->Invalidate();
            emscripten_fetch_attr_t attr;
            emscripten_fetch_attr_init(&attr);
            attr.userData = (void*)respObj.get();
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
            attr.onsuccess = downloadSucceeded;
            attr.onerror = downloadFailed;
            if (!requestdata.empty())
            {
                attr.requestData = requestdata.c_str();
            }
            emscripten_fetch(&attr, url.c_str());

        }
    }
}
