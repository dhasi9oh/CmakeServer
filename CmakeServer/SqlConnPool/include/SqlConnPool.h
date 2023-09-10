// SqlConnPool.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#ifndef SQLCONNPOOL
#define SQLCONNPOOL

#ifndef SQLCONNPOOL_EXPORT
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif // !BLOCKQUEUE

// TODO: 在此处引用程序需要的其他标头。

#include <set>
#include <atomic>
#include <vector>
#include <string>
#include <assert.h>
#include "BlockQueue.h"

// 必须被继承的抽象类 用来统一接口
template <class T>
class API SqlConnPool
{
public:

	virtual void init(
		int size,
		std::string host,
		std::string usr,
		std::string pwd,
		std::string db,
		unsigned port,
		char* unix_socket,
		unsigned client_flag
	) = 0;

	virtual bool getConn(T*&) = 0;

	virtual bool getConn(T*& ptr, int timeout) = 0;

	virtual void release(T*& ptr) = 0;

	virtual void closePool()
	{
		m_Pool.close();
	}

	unsigned int size()
	{
		return m_Pool.size();
	}

protected:

	SqlConnPool() = default;

	virtual ~SqlConnPool()
	{
		closePool();
	}

protected:

	BlockQueue<T*> m_Pool;

};

// 如果使用到没有重写的连接池会报错
template <class DB>
class API ConnPool : public SqlConnPool<DB> 
{
public:

	virtual void init(
		int size,
		std::string host,
		std::string usr,
		std::string pwd,
		std::string db,
		unsigned port,
		std::string unix_socket,
		unsigned client_flag
	) 
	{
		assert(false);
	}

	virtual bool getConn(DB*&) { assert(false); }

	virtual bool getConn(DB*& ptr, int timeout) { assert(false); }

	virtual void release(DB*& ptr) { assert(false); }

protected:

	ConnPool() = default;

	~ConnPool() = default;
};

// raii 机制抽象类，必须要被继承，统一接口
template <class DB>
class API ConnPooLRAII
{
public:

	using Error		= const std::string;

    using Row		= std::deque<std::pair<int,std::string>>;

	using Result	= std::deque<Row>;

	ConnPooLRAII()
	{
		ConnPool<DB>::instance()->getConn(m_Ptr);
	}

    virtual ~ConnPooLRAII()
	{
		ConnPool<DB>::instance()->release(m_Ptr);
	}

	virtual bool query(const std::string& sql) = 0;

	virtual Result result() = 0;

	virtual Error error() = 0;

	virtual bool supportTrans() = 0;

	virtual void transStrat() = 0;

	virtual void transCommit() = 0;

	virtual void transRollback() = 0;

	virtual void setTransAuto(bool flag) = 0;

protected:

	DB* m_Ptr;
	
};

// 使用到没重写的 raii 机制会报错
template <class DB>
class API RAII : public ConnPooLRAII<DB>
{
public:

    using Result = typename ConnPooLRAII<DB>::Result;

    using Error = typename ConnPooLRAII<DB>::Error;

    using Row = typename ConnPooLRAII<DB>::Row;

public:

	virtual bool query(const std::string& sql)
	{
		assert(false);

		return false;
	}

	virtual Result result()
	{
		assert(false);

		return Result();
	}

	virtual Error error()
	{
		assert(false);

		return Error();
	}

	virtual bool supportTrans()
	{
		assert(false);

		return false;
	}

	virtual void transStrat()
	{
		assert(false);
	}

	virtual void transCommit()
	{
		assert(false);
	}

	virtual void transRollback()
	{
		assert(false);
	}

	virtual void setTransAuto(bool flag)
	{
		assert(false);
	}

    virtual ~RAII() = default;
};



#endif
