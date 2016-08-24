// http://blog.coldflake.com/posts/C++-delegates-on-steroids/
#ifndef _DELEGATE_H_
#define _DELEGATE_H_

template<typename TReturnType, typename... TParams>
class Delegate
{
	typedef TReturnType(*Type)(void* callee, TParams...);
public:

	Delegate(void* callee, Type function) : m_callee(callee), m_callback(function) { }

	template <class T, TReturnType(T::*TMethod)(TParams...)>
	static Delegate from_function(T* callee)
	{
		Delegate d(callee, &methodCaller<T, TMethod>);
		return d;
	}

	TReturnType operator()(TParams... xs) const
	{
		return (*m_callback)(m_callee, xs...);
	}

private:

	void* m_callee;
	Type m_callback;

	template <class T, TReturnType(T::*TMethod)(TParams...)>
	static TReturnType methodCaller(void* callee, TParams... xs)
	{
		T* p = static_cast<T*>(callee);
		return (p->*TMethod)(xs...);
	}

};

template<typename T, typename return_type, typename... params>
struct DelegateMaker
{
	template<return_type(T::*foo)(params...)>
	static return_type methodCaller(void* o, params... xs)
	{
		return (static_cast<T*>(o)->*foo)(xs...);
	}

	template<return_type(T::*foo)(params...)>
	static Delegate<return_type, params...> Bind(T* o)
	{
		return Delegate<return_type, params...>(o, &DelegateMaker::methodCaller<foo>);
	}
};

template<typename T, typename return_type, typename... params>
DelegateMaker<T, return_type, params... > makeDelegate(return_type(T::*)(params...))
{
	return DelegateMaker<T, return_type, params...>();
}

#define DELEGATE(foo, thisPrt) (makeDelegate(foo).Bind<foo>(thisPrt))

#endif // _DELEGATE_H_
