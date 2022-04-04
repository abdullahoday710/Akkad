#pragma once
#include "HTTPHandler.h"

#include <vector>
namespace Akkad {
	namespace NET {

		class CurlHTTPHandler : public HTTPHandler
		{
		public:
			CurlHTTPHandler();
			CurlHTTPHandler(bool initWinSock);

			virtual void SendRequest(std::string url, RequestMethod method, std::string requestdata, std::function<void(AsyncHTTPResponse)> callback) override;
			virtual void SendRequest(std::string url, RequestMethod method, std::string requestdata, std::string authToken, std::function<void(AsyncHTTPResponse)> callback) override;
			virtual void OnUpdate() override;
		private:
			void SendRequestImpl(std::string url, RequestMethod method, std::string requestdata, std::function<void(AsyncHTTPResponse)> callback, std::vector<std::string> headers = {});
			void* m_Handle;
			int m_RunningHandles = 0;
		};
	}

}