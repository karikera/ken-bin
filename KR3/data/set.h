#pragma once

#include <string>
#include <unordered_set>

#include <KR3/main.h>


namespace kr
{
	template <typename TK, typename TV>
	class SetWrapper :public std::unordered_set<TK>
	{
	private:
		using ComponentTK = typename TK::Component;
	public:
		using super = std::unordered_set<TK>;
		using typename super::iterator;
		using typename super::const_iterator;
		using typename super::size_type;
		using super::begin;
		using super::end;
		using super::size;

		std::pair<iterator, bool> insert(const TV & value)
		{
			return super::insert(value.template cast<ComponentTK>());
		}
		size_type erase(const TV &key)
		{
			return super::erase(key.template cast<ComponentTK>());
		}
		iterator erase(iterator iter)
		{
			return super::erase(iter);
		}
		iterator find(const TV& key)
		{
			return super::find(key.template cast<ComponentTK>());
		}
		const_iterator find(const TV& key) const
		{
			return super::find(key.template cast<ComponentTK>());
		}
		bool has(const TV& key) const noexcept
		{
			return find(key) != end();
		}
	};

	template <typename T> using Set = meta::if_t<IsBuffer<T>::value, SetWrapper<ABuffer, T>, std::unordered_set<T>>;
	template <typename T> using ReferenceSet = meta::if_t<IsBuffer<T>::value, SetWrapper<Buffer, T>, std::unordered_set<T>>;

}
