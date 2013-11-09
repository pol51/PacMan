#ifndef __XVECTOR_H__
#define __XVECTOR_H__

#include <cstdlib>

template<typename T> class XVector
{
  public:
    XVector():
      _blocks(0), _count(0),
      _data(NULL)
    {}

    ~XVector()
    {
      clear();
    }

    void push(const T& item)
    {
      const int Count(_count);
      if (++_count >= _blocks)
      {
        _blocks += 32;
        _data = (T**)realloc(_data, sizeof(T*) * _blocks);
      }
      _data[Count] = new T(item);
    }

    void remove(const int index)
    {
      delete _data[index];
      memmove(_data + index, _data + index + 1, (_count - index) * sizeof(T*));
      --_count;
    }

    void clear()
    {
      for (; --_count >= 0; )
        delete(_data[_count]);
      _blocks = _count = 0;
      free(_data);
      _data = NULL;
    }

    const T &operator[](const int index) const { return *_data[index]; }
    T &operator[](const int index) { return *_data[index]; }

    const T pop() { --_count; T Ret(*_data[_count]); delete _data[_count]; return Ret; }

    const int size() const { return _count; }

    XVector<T> &operator=(const XVector<T> &other)
    {
      clear();
      const int Size(other.size());
      for (int i = 0; i < Size; ++i)
        push(other[i]);
      return *this;
    }

  private:
    int _blocks;
    int _count;
    T**  _data;
};

#endif
