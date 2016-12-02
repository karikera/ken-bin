#pragma once

namespace kr
{
	// �ε��� ������
	// �������� ������ �ռ��Ͽ� �ε����� ������
	class IndexMaker
	{
	private:
		int index;
		int maximum;

	public:
		// Ư�� ���� �ε����� ���� ��, ��Ģ�� �����Ѵ�.
		// ���ø� Ư��ȭ�� �̿��Ͽ�, ��Ģ�� ������ �� �ִ�.
		template <typename T> inline int make(T value);

		inline IndexMaker()
		{
			index = 0;
			maximum = 1;
		}
		template <typename T>
		inline IndexMaker(T value)
		{
			index = make(value);
			maximum = StateConst<T>::count;
		}

		// �� �ռ�, ����
		template <typename T>
		inline IndexMaker& operator |=(T value)
		{
			typedef StateConst<T> C;
			index += make(value) * maximum;
			maximum *= C::count;
			return *this;
		}

		// �� �ռ�
		template <typename T>
		inline const IndexMaker operator |(T value) const
		{
			IndexMaker out = *this;
			out |= value;
			return out;
		}

		//// XXX: ���ڸ� ������ ���۵�����, ������� �ʴ´�
		//template <typename T>
		//inline friend const IndexMaker operator |(T value, const IndexMaker im)
		//{
		//	im |= value;
		//	return im;
		//}

		// ���ڷ� �����´�
		inline operator int() const
		{
			return index;
		}
	};

	template <typename T> inline int IndexMaker::make(T value)
	{
		typedef StateConst<T> C;
		int res = value - C::offset;
		assert(res < C::count);
		return res;
	}

}