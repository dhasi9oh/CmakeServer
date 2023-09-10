// BlockQueue.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。
#ifndef BLOCKQUEUE
#define BLOCKQUEUE


#ifndef BLOCKQUEUE_EXPORT
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif // !BLOCKQUEUE


#include <deque>
#include <mutex>
#include <chrono>
#include <assert.h>
#include <condition_variable>

template <class T>
class API BlockQueue
{
public:

	BlockQueue(int max_size = 10000)
	{
		assert(max_size > 0);
		m_MaxSize = max_size;
		is_Open = true;
	}
	
	~BlockQueue()
	{
		close();
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lock(m_Mtx);

		return m_Deque.empty();
	}

	bool full()
	{
		std::lock_guard<std::mutex> lock(m_Mtx);

		return m_Deque.size() >= m_MaxSize;
	}

	void close()
	{
		{
			std::lock_guard<std::mutex> lock(m_Mtx);
			is_Open = false;
		}

		m_PopCon.notify_all();
		m_PushCon.notify_all();
	}

	bool isOpen()
	{
		std::lock_guard<std::mutex> lock(m_Mtx);

		return is_Open;
	}


	int size()
	{
		std::lock_guard<std::mutex> lock(m_Mtx);

		return m_Deque.size();
	}

	T front()
	{
		std::lock_guard<std::mutex> lock(m_Mtx);

		return m_Deque.front();
	}

	T back()
	{
		std::lock_guard<std::mutex> lock(m_Mtx);

		return m_Deque.back();
	}

	// 放入一个元素并通知一个调用了pop函数并堵塞的线程(无时间限制)
	bool push(const T& value)
	{
		std::unique_lock<std::mutex> lock(m_Mtx);

		while (m_Deque.size() >= m_MaxSize) {
			m_PushCon.wait(lock);

			if (!is_Open) {
				return false;
			}
		}

		m_Deque.push_back(value);
		m_PopCon.notify_one();

		return true;
	}

	// 取出一个元素并通知一个调用了push函数并堵塞的线程(无时间限制)
	bool pop(T& value)
	{
		std::unique_lock<std::mutex> lock(m_Mtx);

		while (m_Deque.size() <= 0) {
			m_PopCon.wait(lock);

			if (!is_Open) {
				return false;
			}
		}

		value = std::move(m_Deque.front());
		m_Deque.pop_front();

		m_PushCon.notify_one();

		return true;
	}

	// 取出一个元素并通知一个调用了push函数并堵塞的线程(有时间限制)
	bool pop(T& value, int timeout)
	{
		std::unique_lock<std::mutex> lock(m_Mtx);

		while (m_Deque.empty()) {
			if (m_PopCon.wait_for(lock, std::chrono::seconds(timeout)) == std::cv_status::timeout) {
				return false;
			}

			if (!is_Open) {
				return false;
			}
		}

		value = std::move(m_Deque.front());
		m_Deque.pop_front();

		m_PushCon.notify_one();

		return true;
	}

	void clear()
	{
		std::lock_guard<std::mutex> lock(m_Mtx);
		m_Deque.clear();
	}

	// 堵塞队列关闭后还剩余的全部元素
	std::deque<T> residualData()
	{
		std::lock_guard<std::mutex> lock(m_Mtx);
		return std::deque<T>(std::move(m_Deque));
	}

private:

	int m_MaxSize;

	bool is_Open;

	std::mutex m_Mtx;

	std::deque<T> m_Deque;

	std::condition_variable m_PushCon;

	std::condition_variable m_PopCon;
};

// TODO: 在此处引用程序需要的其他标头。
// BlockQueue.cpp: 定义应用程序的入口点。
//

#endif // !
