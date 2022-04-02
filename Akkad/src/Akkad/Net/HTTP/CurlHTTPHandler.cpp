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

			double downloadedSize = 0;
			curl_easy_getinfo(userdata->ezHandle, CURLINFO_SIZE_DOWNLOAD, &downloadedSize);

			size_t totalSize = size * nmemb;
			if (downloadedSize == totalSize)
			{
				long http_code = 0;
				curl_easy_getinfo(userdata->ezHandle, CURLINFO_RESPONSE_CODE, &http_code);

				AsyncHTTPResponse response;
				response.statusCode = http_code;
				response.responseData = *userdata->buffer;

				userdata->callback(response);

				delete userdata->buffer;
				delete[] userdata->requestData;
				delete userdata;

			}
			return size * nmemb;
		}

		CurlHTTPHandler::CurlHTTPHandler() {
			curl_global_init(CURL_GLOBAL_ALL);
			m_Handle = curl_multi_init();
		}

		void CurlHTTPHandler::SendRequest(std::string url, RequestMethod method, std::string requestdata, std::function<void(AsyncHTTPResponse)> callback)
		{

			CURLUserData* userData = new CURLUserData();
			userData->buffer = new std::string();
			userData->ezHandle = curl_easy_init();
			userData->callback = callback;
			std::string* buffer = new std::string();

			struct curl_slist* headers = NULL;
			headers = curl_slist_append(headers, "Content-Type: application/json");

			curl_easy_setopt(userData->ezHandle, CURLOPT_HTTPHEADER, headers);
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
					m_RunningHandles--;
					curl_multi_remove_handle(m_Handle, e);
					curl_easy_cleanup(e);
				}
			}
		}
	}
}

