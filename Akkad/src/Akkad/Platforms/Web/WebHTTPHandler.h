#pragma once
#include "Akkad/Net/HTTP/HTTPHandler.h"

namespace Akkad {
	namespace NET {

		class WebHTTPHandler : public HTTPHandler
		{
		public:
			virtual void SendRequest(SharedPtr<AsyncHTTPResponse> respObj, std::string url, RequestMethod method, std::string requestdata) override;
		};

	}
}