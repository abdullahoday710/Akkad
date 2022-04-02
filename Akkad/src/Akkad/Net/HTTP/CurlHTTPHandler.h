#pragma once
#include "HTTPHandler.h"

namespace Akkad {
	namespace NET {

		class CurlHTTPHandler : public HTTPHandler
		{
		public:
			CurlHTTPHandler();

			virtual void SendRequest(std::string url, RequestMethod method, std::string requestdata, std::function<void(AsyncHTTPResponse)> callback) override;
			virtual void OnUpdate() override;
		private:
			void* m_Handle;
			int m_RunningHandles = 0;
		};
	}

}