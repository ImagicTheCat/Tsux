/* 
 * MySQL is a mysqlcppconn wrapper
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_MYSQL
#define H_MYSQL

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class MySQL;

struct Query{
  Query():p(NULL), mysql(NULL), param_size(0), bind_done(true){}
  sql::PreparedStatement *p;
  MySQL* mysql;
  std::string string;

  sql::ResultSet* executeQuery();
  bool execute();
  sql::PreparedStatement& query(){ return *p; }

  //params
  void setBigInt(unsigned int parameterIndex, const sql::SQLString& value);
	void setBlob(unsigned int parameterIndex, std::istream * blob);
	void setBoolean(unsigned int parameterIndex, bool value);
	void setDateTime(unsigned int parameterIndex, const sql::SQLString& value);
	void setDouble(unsigned int parameterIndex, double value);
	void setInt(unsigned int parameterIndex, int32_t value);
	void setUInt(unsigned int parameterIndex, uint32_t value);
	void setInt64(unsigned int parameterIndex, int64_t value);
	void setUInt64(unsigned int parameterIndex, uint64_t value);
	void setNull(unsigned int parameterIndex, int sqlType);
	void setString(unsigned int parameterIndex, const sql::SQLString& value);

  std::map<unsigned int, sql::SQLString> bigints;
  std::map<unsigned int, std::istream*> blobs;
  std::map<unsigned int, bool> booleans;
  std::map<unsigned int, sql::SQLString> datetimes;
  std::map<unsigned int, double> doubles;
  std::map<unsigned int, int32_t> ints;
  std::map<unsigned int, uint32_t> uints;
  std::map<unsigned int, int64_t> ints64;
  std::map<unsigned int, uint64_t> uints64;
  std::map<unsigned int, int> nulls;
  std::map<unsigned int, sql::SQLString> strings;

  //set shortcut
  template<typename T> void set(unsigned int index, const T& v, std::map<unsigned int, T>& map){
    if(index > param_size)
      param_size = index;

    if(bind_done)
      bind_done = false;

    set_unset(index, map, bigints);
    set_unset(index, map, blobs);
    set_unset(index, map, booleans);
    set_unset(index, map, datetimes);
    set_unset(index, map, doubles);
    set_unset(index, map, ints);
    set_unset(index, map, uints);
    set_unset(index, map, ints64);
    set_unset(index, map, uints64);
    set_unset(index, map, nulls);
    set_unset(index, map, strings);

    typename std::map<unsigned int, T>::iterator it = map.find(index);
    if(it != map.end())
      map.erase(it);
    map.insert(std::pair<unsigned int, T>(index, v));
  }

  //set unset shortcut
  template<typename U,typename V> void set_unset(unsigned int index, std::map<unsigned int, U>& current_map, std::map<unsigned int, V>& map){
    typename std::map<unsigned int, V>::iterator it;
    if((void*)&current_map != (void*)&map && (it = map.find(index)) != map.end())
      map.erase(it);
  }

  //bind params to prepared statement
  void bindParams();

  int param_size;
  bool bind_done;
};

class MySQL{
  public:
    MySQL();
    ~MySQL();

    bool connect(const std::string& host, const std::string& user, const std::string& password, bool reconnect = true);
    bool connect();

    void disconnect();
    void clear();
    void bindQueries();

    void handleException(sql::SQLException& e);

    void use(const std::string& _db);

    Query& prepare(const std::string& name, const std::string& query);
    Query& get(const std::string& name);

    bool execute(const std::string& query);
    sql::ResultSet* executeQuery(const std::string& query);
    
  private:
    void _use();

    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    std::map<std::string, Query> queries;

    bool reconnect;

    //if autoreconnect enable
    std::string host, user, password, db;
    Query default_query;
};

#endif
