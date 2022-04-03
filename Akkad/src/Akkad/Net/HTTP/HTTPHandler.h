#pragma once
#include <Akkad/core.h>

#include <string>

#include <functional>

namespace Akkad {
	namespace NET {
		enum class RequestMethod {GET, POST};
		// TODO : Support more request methods and add support for content types....
		struct AsyncHTTPResponse
		{
			bool isValid = false;
			int statusCode = -1;
			std::string responseData;
			
			void Invalidate()
			{
				isValid = false;
				statusCode = -1;
				responseData = "";
			}
		};

		class HTTPHandler {
		public:
			virtual void SendRequest(std::string url, RequestMethod method, std::string requestdata, std::function<void(AsyncHTTPResponse)> callback) = 0;
			virtual void SendRequest(std::string url, RequestMethod method, std::string requestdata, std::string authToken, std::function<void(AsyncHTTPResponse)> callback) = 0;
			virtual void OnUpdate() {};
		};
	}
}