/* 
 * MySQL is a mysqlcppconn wrapper
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#include "MySQL.hpp"

MySQL::MySQL():
  con(NULL), stmt(NULL)
{
  driver = get_driver_instance();
}

MySQL::~MySQL(){
  clear();
}

void MySQL::clear(){
  disconnect();

  if(con != NULL)
    delete con;
  if(stmt != NULL)
    delete stmt;

  con = NULL;
  stmt = NULL;

  std::map<std::string, Query>::iterator it;
  for(it = queries.begin(); it != queries.end(); it++)
    delete it->second.p;
  
  queries.clear();
}

void MySQL::disconnect(){
  if(con != NULL && !con->isClosed()){
    con->close();
  }
}

void MySQL::bindQueries(){
  if(con != NULL){
    if(stmt == NULL)
      stmt = con->createStatement();
    _use();
  }
}

bool MySQL::connect(const std::string& _host, const std::string& _user, const std::string& _password, bool _reconnect){
  reconnect = _reconnect;

  host = _host;
  user = _user;
  password = _password;

  return connect();
}

bool MySQL::connect(){
  if(con != NULL){
    disconnect();
    delete con;
  }

  std::cout << "connect" << std::endl;
  con = driver->connect(host, user, password);

  std::cout << "bind queries" << std::endl;
  bindQueries();

  return !con->isClosed();
}

sql::ResultSet* MySQL::executeQuery(const std::string& query){
  if(stmt != NULL){
    try{
      return stmt->executeQuery(query);
    }catch(sql::SQLException e){
      handleException(e);
      return stmt->executeQuery(query);
    }
  }
  
  return NULL;
}


bool MySQL::execute(const std::string& query){
  if(stmt != NULL){
    try{
      std::cout << "try: " << query << std::endl;
      return stmt->execute(query);
    }catch(sql::SQLException e){
      handleException(e);
      return stmt->execute(query);
    }
  }
  
  return false;
}

void MySQL::use(const std::string& _db){
  db = _db;
  _use();
}

void MySQL::_use(){
  if(con != NULL && db != ""){
    execute("USE "+db);
  }
}

Query& MySQL::prepare(const std::string& name, const std::string& query){
  if(con != NULL){
    std::map<std::string, Query>::iterator it = queries.find(name);
    if(it != queries.end()){
      delete it->second.p;
      queries.erase(it);
    }

    Query q;
    q.string = query;
    q.p = con->prepareStatement(query);
    q.mysql = this;
    
    queries.insert(std::pair<std::string, Query>(name, q));
  }

  return get(name);
}


Query& MySQL::get(const std::string& name){
  std::map<std::string, Query>::iterator it = queries.find(name);
  if(it != queries.end())
    return it->second;
  else
    return default_query;
}


void MySQL::handleException(sql::SQLException& e){
  std::cout << "[tsux] exception: " << e.what() << std::endl;
  connect();
}

/* QUERY */
sql::ResultSet* Query::executeQuery(){
  if(p != NULL){
    try{
      return p->executeQuery();
    }catch(sql::SQLException e){
      mysql->handleException(e);
      return p->executeQuery();
    }
  }

  return NULL;
}

bool Query::execute(){
  if(p != NULL){
    try{
      return p->execute();
    }catch(sql::SQLException e){
      mysql->handleException(e);
      return p->execute();
    }
  }

  return false;
}

