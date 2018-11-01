#pragma once


template <typename CLASS, typename RET, typename ... ARGS>
template <size_t ... idx>
template <typename FUNC>
kr::JsAny kr::JsMetaLambda<RET(CLASS::*)(ARGS ...)>
	::numunwrap<kr::meta::numlist<idx ...>>
	::call(FUNC fn, const JsArguments & args)
{
	return fn((args[idx].cast<ARGS>())...);
}

template <typename CLASS, typename ... ARGS>
template <size_t ... idx>
template <typename FUNC>
kr::JsAny kr::JsMetaLambda<void(CLASS::*)(ARGS ...)>
	::numunwrap<kr::meta::numlist<idx ...>>
	::call(FUNC fn, const JsArguments & args)
{
	fn((args[idx].cast<ARGS>())...);
	return undefined;
}

template <typename RET, typename ... ARGS>
template <size_t ... idx>
kr::JsAny kr::JsMeta<RET(*)(ARGS ...)>
	::numunwrap<kr::meta::numlist<idx ...>>
	::call(RET(*fn)(ARGS ...), const JsArguments & args)
{
	return fn((args[idx].cast<ARGS>())...);
}

template <typename ... ARGS>
template <size_t ... idx>
kr::JsAny kr::JsMeta<void(*)(ARGS ...)>
	::numunwrap<kr::meta::numlist<idx ...>>
	::call(void(*fn)(ARGS ...), const JsArguments & args)
{
	fn((args[idx].cast<ARGS>())...);
	return undefined;
}

template <typename CLASS, typename RET, typename ... ARGS>
template <size_t ... idx>
kr::JsAny kr::JsMeta<RET(CLASS::*)(ARGS ...)>
	::numunwrap<kr::meta::numlist<idx ...>>
	::call(RET(CLASS::*fn)(ARGS ...), const JsArguments & args)
{
	CLASS * _this = args.getThis().getNativeObject<CLASS>();
	if (_this == nullptr) return undefined;
	return (_this->*fn)((args[idx].cast<ARGS>())...);
}

template <typename CLASS, typename ... ARGS>
template <size_t ... idx>
kr::JsAny kr::JsMeta<void(CLASS::*)(ARGS ...)>
	::numunwrap<kr::meta::numlist<idx ...>>
	::call(void(CLASS::*fn)(ARGS ...), const JsArguments & args)
{
	CLASS * _this = args.getThis().getNativeObject<CLASS>();
	if (_this == nullptr) return undefined;
	(_this->*fn)((args[idx].cast<ARGS>())...);
	return undefined;
}