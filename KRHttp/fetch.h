#pragma once

#include <KR3/mt/thread.h>
#include <KRMessage/promise.h>
#include <KRMessage/pump.h>
#include "common/httpstatus.h"


struct curl_slist;
struct curl_httppost;

namespace kr
{
	class HttpRequest
	{
	public:
		HttpRequest(const char * url) noexcept;
		~HttpRequest() noexcept;

		void setRequestHeader(const char * line) noexcept;
		void setPostFields(AText data) noexcept;
		Promise<AText>* fetchAsText() noexcept;
		Promise<void>* fetchAsFile(AText16 filename) noexcept;
		AText fetchAsTextSync() throws(CurlError, HttpException);
		void fetchAsFileSync(pcstr16 filename) throws(CurlError, HttpException, Error);
		HttpStatus getStatusCode() noexcept;

	private:
		void * m_curl;
		curl_slist * m_headers;
		AText m_postdata;
	};
	Promise<AText>* fetchAsText(const char * url) noexcept;
	Promise<void>* fetchAsFile(const char * url, AText16 filename) noexcept;
}
