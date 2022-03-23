#pragma once
#include <Akkad/core.h>

#include <string>

namespace Akkad {
	namespace NET {
		enum class RequestMethod {GET, POST};

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
			virtual void SendRequest(SharedPtr<AsyncHTTPResponse> respObj, std::string url, RequestMethod method, std::string requestdata = "") = 0;

		};
	}
}