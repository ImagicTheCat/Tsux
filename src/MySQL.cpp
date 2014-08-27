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
    delete con; if(stmt != NULL)
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
    if(stmt != NULL)
      delete stmt;
    //recreate stmt
    stmt = con->createStatement();
    _use();

    //recreate prepared statements
    std::map<std::string, Query>::iterator it;
    for(it = queries.begin(); it != queries.end(); it++){
      delete it->second.p;
      it->second.p = con->prepareStatement(it->second.string);

      //bind params
      it->second.bindParams();
    }
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

  con = driver->connect(host, user, password);

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
    if(!bind_done)
      bindParams();

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
    if(!bind_done)
      bindParams();

    try{
      return p->execute();
    }catch(sql::SQLException e){
      mysql->handleException(e);
      return p->execute();
    }
  }

  return false;
}

void Query::bindParams(){
  if(p != NULL){
    for(int i = 1; i <= param_size; i++){
      if(bigints.find(i) != bigints.end())
        p->setBigInt(i, bigints.find(i)->second);   
      else if(blobs.find(i) != blobs.end())
        p->setBlob(i, blobs.find(i)->second);   
      else if(booleans.find(i) != booleans.end())
        p->setBoolean(i, booleans.find(i)->second);   
      else if(datetimes.find(i) != datetimes.end())
        p->setDateTime(i, datetimes.find(i)->second);   
      else if(doubles.find(i) != doubles.end())
        p->setDouble(i, doubles.find(i)->second);   
      else if(ints.find(i) != ints.end())
        p->setInt(i, ints.find(i)->second);   
      else if(uints.find(i) != uints.end())
        p->setUInt(i, uints.find(i)->second);   
      else if(ints64.find(i) != ints64.end())
        p->setInt64(i, ints64.find(i)->second);   
      else if(uints64.find(i) != uints64.end())
        p->setUInt64(i, uints64.find(i)->second);   
      else if(nulls.find(i) != nulls.end())
        p->setNull(i, nulls.find(i)->second);   
      else if(strings.find(i) != strings.end())
        p->setString(i, strings.find(i)->second);   
    }

    bind_done = true;
  }
}

//params set
void Query::setBigInt(unsigned int index, const sql::SQLString& v)
{ set(index, v, bigints); }

void Query::setBlob(unsigned int index, std::istream * v)
{ set(index, v, blobs); }

void Query::setBoolean(unsigned int index, bool v)
{ set(index, v, booleans); }

void Query::setDateTime(unsigned int index, const sql::SQLString& v)
{ set(index, v, datetimes); }

void Query::setDouble(unsigned int index, double v)
{ set(index, v, doubles); }

void Query::setInt(unsigned int index, int32_t v)
{ set(index, v, ints); }

void Query::setUInt(unsigned int index, uint32_t v)
{ set(index, v, uints); }

void Query::setInt64(unsigned int index, int64_t v)
{ set(index, v, ints64); }

void Query::setUInt64(unsigned int index, uint64_t v)
{ set(index, v, uints64); }

void Query::setNull(unsigned int index, int v)
{ set(index, v, nulls); }

void Query::setString(unsigned int index, const sql::SQLString& v)
{ set(index, v, strings); }

