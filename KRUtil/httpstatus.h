#pragma once

#include <KR3/main.h>

namespace kr
{
	enum class HttpStatus:uint
	{
		Undefined = 0,
		Ok = 200,

		MovePermanently = 301,
		Found = 302,
		NotModified = 304,

		BadRequest = 400,
		Forbidden = 403,
		NotFound = 404,
		LengthRequired = 411,

		ServerInternalError = 500,
		MethodNotImplemented = 501,
		HTTPVersionNotSupported = 505,
		CustomError = 555,
	};
}