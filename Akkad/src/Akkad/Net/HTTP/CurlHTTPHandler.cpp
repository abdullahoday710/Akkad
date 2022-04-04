#include "CurlHTTPHandler.h"

#include <curl/curl.h>
namespace Akkad {
	namespace NET {
		struct CURLUserData {
			std::string* buffer = 0;
			void* requestData = 0;
			CURL* ezHandle = 0;
			std::function<void(AsyncHTTPResponse)> callback;
		};
		
		size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
		{
			CURLUserData* userdata = (CURLUserData*)userp;
			if (userdata->buffer != nullptr)
			{
				if (buffer != nullptr)
				{
					userdata->buffer->append((char*)buffer, size * nmemb);
				}
			}

			return size * nmemb;
		}

		CurlHTTPHandler::CurlHTTPHandler() {
			curl_global_init(CURL_GLOBAL_ALL);
			m_Handle = curl_multi_init();
		}

		CurlHTTPHandler::CurlHTTPHandler(bool initWinSock)
		{
			curl_global_init(CURL_GLOBAL_SSL);
			m_Handle = curl_multi_init();
		}

		void CurlHTTPHandler::SendRequest(std::string url, RequestMethod method, std::string requestdata, std::function<void(AsyncHTTPResponse)> callback)
		{
			SendRequestImpl(url, method, requestdata, callback);

		}

		void CurlHTTPHandler::SendRequest(std::string url, RequestMethod method, std::string requestdata, std::string authToken, std::function<void(AsyncHTTPResponse)> callback)
		{
			std::string authHeader = "Authorization: Token " + authToken;
			std::vector<std::string> headers;
			headers.push_back(authHeader);

			SendRequestImpl(url, method, requestdata, callback, headers);
		}

		void CurlHTTPHandler::SendRequestImpl(std::string url, RequestMethod method, std::string requestdata, std::function<void(AsyncHTTPResponse)> callback, std::vector<std::string> headers)
		{
			CURLUserData* userData = new CURLUserData();
			userData->buffer = new std::string();
			userData->ezHandle = curl_easy_init();
			userData->callback = callback;
			std::string* buffer = new std::string();

			struct curl_slist* curlheaders = NULL;
			curlheaders = curl_slist_append(curlheaders, "Content-Type: application/json");

			for (auto header : headers)
			{
				if (!header.empty())
				{
					curlheaders = curl_slist_append(curlheaders, header.c_str());
				}
			}

			curl_easy_setopt(userData->ezHandle, CURLOPT_HTTPHEADER, curlheaders);
			curl_easy_setopt(userData->ezHandle, CURLOPT_PRIVATE, userData);
			curl_easy_setopt(userData->ezHandle, CURLOPT_URL, url.c_str());
			curl_easy_setopt(userData->ezHandle, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(userData->ezHandle, CURLOPT_WRITEDATA, userData);

			if (method == RequestMethod::POST)
			{
				char* data = new char[requestdata.size() + 1];
				strcpy(data, requestdata.c_str());

				curl_easy_setopt(userData->ezHandle, CURLOPT_POSTFIELDS, data);
				curl_easy_setopt(userData->ezHandle, CURLOPT_POSTFIELDSIZE, requestdata.size());

				userData->requestData = data;
			}
			curl_multi_add_handle(m_Handle, userData->ezHandle);
			m_RunningHandles++;
		}
		void CurlHTTPHandler::OnUpdate()
		{
			// perform requests each frame....
			CURLMcode code;
			if (m_RunningHandles != 0)
			{

				code = curl_multi_perform(m_Handle, &m_RunningHandles);
			}

			int msgq = 0;
			struct CURLMsg* m;
			m = curl_multi_info_read(m_Handle, &msgq);

			if (m != nullptr)
			{
				if (m->msg == CURLMSG_DONE)
				{
					// cleaning up finished requests....
					CURL* e = m->easy_handle;
					if (m_RunningHandles > 0)
					{
						m_RunningHandles--;
					}
					CURLUserData* userdata;
					curl_easy_getinfo(m->easy_handle, CURLINFO_PRIVATE, &userdata);

					long http_code = 0;
					curl_easy_getinfo(userdata->ezHandle, CURLINFO_RESPONSE_CODE, &http_code);

					AsyncHTTPResponse response;
					response.statusCode = http_code;
					response.responseData = *userdata->buffer;

					userdata->callback(response);

					delete userdata->buffer;
					delete[] userdata->requestData;
					delete userdata;

					curl_multi_remove_handle(m_Handle, e);
					curl_easy_cleanup(e);
					
				}
			}
		}

	}
}

