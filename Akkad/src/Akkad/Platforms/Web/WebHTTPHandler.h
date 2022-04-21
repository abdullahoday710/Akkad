#pragma once
#include "Akkad/Net/HTTP/HTTPHandler.h"

namespace Akkad {
	namespace NET {

		class WebHTTPHandler : public HTTPHandler
		{
		public:
			virtual void SendRequest(std::string url, RequestMethod method, std::string requestdata, std::function<void(AsyncHTTPResponse)> callback) override;
			virtual void SendRequest(std::string url, RequestMethod method, std::string requestdata, std::string authToken, std::function<void(AsyncHTTPResponse)> callback) override;
		};

	}
}