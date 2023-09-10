
#include <any>
#ifndef MYSQLCONN

#ifndef MYSQLCONN_EXPORT
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif // !BLOCKQUEUE

#include <assert.h>
#include <mysql/mysql.h>
#include <boost/locale/encoding.hpp>
#include "SqlConnPool.h"

template <>
class API ConnPool<MYSQL> : public SqlConnPool<MYSQL>
{
public:

	static ConnPool<MYSQL>* instance()
	{
		static ConnPool<MYSQL> pool;
		return &pool;
	}

public:

	virtual void init(
		int size,
		std::string host,
		std::string usr,
		std::string pwd,
		std::string db,
		unsigned port = 3306,
		char* unix_socket = nullptr,
		unsigned client_flag = 0
	)
	{
		assert(size > 0);

		try {
			for (int i = 0; i < size; i++) {
				MYSQL* con = nullptr;
				con = mysql_init(nullptr);
				assert (con != nullptr);

				assert(
					mysql_real_connect(
						con,
						host.c_str(),
						usr.c_str(),
						pwd.c_str(),
						db.c_str(),
						port,
						unix_socket,
						client_flag
					) != nullptr
				);

				m_Pool.push(con);
			}
		}
		catch (...) {
            size_t n = m_Pool.size();

			for (int i = 0; i < n; i++) {
				MYSQL* con = nullptr;
				m_Pool.pop(con);
				mysql_close(con);
			}

			throw ("mysql init is fail!");
		}

	}

	virtual bool getConn(MYSQL*& ptr)
	{
		if (!m_Pool.isOpen()) {
			return false;
		}
		return m_Pool.pop(ptr);
	}

	virtual bool getConn(MYSQL*& ptr, int timeout)
	{
		if (!m_Pool.isOpen()) {
			return false;
		}
		return m_Pool.pop(ptr, timeout);
	}

	virtual void release(MYSQL*& ptr)
	{
		if (ptr != nullptr) {

			if (m_Pool.isOpen()) {
				m_Pool.push(ptr);
			}
			else {
				mysql_close(ptr);
				ptr = nullptr;
			}
		}
	}

	virtual void closePool()
	{
		SqlConnPool<MYSQL>::closePool();
		std::deque<MYSQL*> que = m_Pool.residualData();

        size_t n = que.size();

		for (int i = 0; i < n; i++) {
			mysql_close(que[i]);
		}

	}

protected:

	ConnPool() = default;

    virtual ~ConnPool()
    {
        ConnPool<MYSQL>::closePool();
	}

};

template <>
class API RAII<MYSQL> : public ConnPooLRAII<MYSQL>
{
public:

	virtual bool query(const std::string& sql)
	{
		assert(m_Ptr != nullptr);
		std::string str = boost::locale::conv::between(sql, "UTF-8", "GBK");
		return (mysql_real_query(m_Ptr, str.c_str(), (unsigned int)strlen(sql.c_str())) == 0);
	}

	virtual Result result()
	{
		MYSQL_RES* ptr = mysql_store_result(m_Ptr);
		uint64_t n = mysql_num_rows(ptr);
		uint64_t m = mysql_num_fields(ptr);

		Result result(n, Row(m));

        MYSQL_FIELD* field = mysql_fetch_fields(ptr);

		MYSQL_ROW row = nullptr;

		for (int i = 0; i < n; i++) {
            row = mysql_fetch_row(ptr);
			for (int j = 0; j < m; j++) {
                result[i][j] = std::make_pair(field[j].type, boost::locale::conv::between(row[j], "GBK", "UTF-8"));
			}
		}

		mysql_free_result(ptr);
		 
		return result;
	}

	virtual Error error()
	{
		Error err = boost::locale::conv::between(mysql_error(m_Ptr), "GBK", "UTF-8");
		return err;
	}

	virtual bool supportTrans()
	{
        query("show variables like '%storage_engine%'");

		Result res = result();

        size_t n = res.size();
		for (int i = 0; i < n; i++) {
            if (res[i].front().second == "default_storage_engine" &&
                res[i].back().second != "InnoDB") {

				return false;
			}
		}
		return true;
	}

	virtual void transStrat()
	{
		if (isAutoTrans) {
			query("set autocommit=0");
			isAutoTrans = false;
		}
		query("start transaction");
	}

	virtual void transCommit()
	{
		query("commit");
	}

	virtual void transRollback()
	{
		query("rollback");
	}

	virtual void setTransAuto(bool flag)
	{
		if (flag) {
			query("set AUTOCOMMIT=1");
			isAutoTrans = true;
		}
		else {
			query("set AUTOCOMMIT=0");
			isAutoTrans = false;
		}
	}

private:

	bool isAutoTrans = true;

};

using MysqlConnPool = ConnPool<MYSQL>;

using MysqlConnRAII = RAII<MYSQL>;

using MysqlResult = MysqlConnRAII::Result;

using MysqlRow = MysqlConnRAII::Row;

using MysqlError = MysqlConnRAII::Error;

#endif
