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
  Query():p(NULL), mysql(NULL){}
  sql::PreparedStatement *p;
  MySQL* mysql;
  std::string string;

  sql::ResultSet* executeQuery();
  bool execute();
  sql::PreparedStatement& query(){ return *p; }
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
