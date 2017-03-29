#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template<typename T>
class Singleton 
{
public:
	static T* getInstance();
	void drop();
	
protected:
	static T* _instance;
public:
	Singleton();
};
template <typename T>
T* Singleton<T>::getInstance()
{
	if (!_instance)
		_instance = new T;
	return _instance;
}
template<typename T>
void Singleton<T>::drop()
{
	if (_instance)
		delete _instance;
}
template<typename T>
Singleton<T>::Singleton()
{

}
template<typename T> T* Singleton<T>::_instance = 0;
#endif