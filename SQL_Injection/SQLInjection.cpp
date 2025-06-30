// SQLInjection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <locale>
#include <tuple>
#include <vector>
#include <regex>
#include "sqlite3.h"

// DO NOT CHANGE
typedef std::tuple<std::string, std::string, std::string> user_record;
const std::string str_where = " where ";

// DO NOT CHANGE
static int callback(void* possible_vector, int argc, char** argv, char** azColName)
{
  if (possible_vector == NULL)
  { // no vector passed in, so just display the results
    for (int i = 0; i < argc; i++) 
    {
      std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
  }
  else
  {
    std::vector< user_record >* rows =
      static_cast<std::vector< user_record > *>(possible_vector);

    rows->push_back(std::make_tuple(argv[0], argv[1], argv[2]));
  }
  return 0;
}

// DO NOT CHANGE
bool initialize_database(sqlite3* db)
{
  char* error_message = NULL;
  std::string sql = "CREATE TABLE USERS(" \
    "ID INT PRIMARY KEY     NOT NULL," \
    "NAME           TEXT    NOT NULL," \
    "PASSWORD       TEXT    NOT NULL);";

  int result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
  if (result != SQLITE_OK)
  {
    std::cout << "[ERROR] Failed to create USERS table. ERROR = " << error_message << std::endl;
    sqlite3_free(error_message);
    return false;
  }
  std::cout << "[INFO] USERS table created." << std::endl;

  // insert some dummy data
  sql = "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (1, 'Fred', 'Flinstone');" \
    "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (2, 'Barney', 'Rubble');" \
    "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (3, 'Wilma', 'Flinstone');" \
    "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (4, 'Betty', 'Rubble');";

  result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
  if (result != SQLITE_OK)
  {
    std::cout << "Data failed to insert to USERS table. ERROR = " << error_message << std::endl;
    sqlite3_free(error_message);
    return false;
  }

  return true;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {

    // Create a vector to store the tokens
    std::vector<std::string> tokens;
    // Find the first occurrence of the delimiter
    size_t start = 0;

    // Find the position of the delimiter in the string
    size_t end = str.find(delimiter);

    // Loop until the delimiter is not found
    while (end != std::string::npos) {

        // Extract the token from the string
        tokens.push_back(str.substr(start, end - start));

        // Move the start position to the next character after the delimiter
        start = end + delimiter.length();

        // Find the next occurrence of the delimiter
        end = str.find(delimiter, start);
    }

    // Add the last token after the last delimiter
    tokens.push_back(str.substr(start, end));

    // If the last token is empty, remove it
    if (tokens.back().empty()) {
        tokens.pop_back();
    }

    // Return the vector of tokens
    return tokens;
}

bool run_query(sqlite3* db, const std::string& sql, std::vector< user_record >& records)
{
    // TODO: Fix this method to fail and display an error if there is a suspected SQL Injection
    //  NOTE: You cannot just flag 1=1 as an error, since 2=2 will work just as well. You need
    //  something more generic

    // Check SQL query with Regex patterns.
    // This regex pattern will check for always true statements like 1=1 or 2=2 or 'a'='a' It should handle edge cases
    // such as spaces between the equal sign and the values, mismatched quotes types, etc.
    std::regex pattern(R"((["']?)([^'"\s]+)(["']?)\s*=\s*\1\2\3)", std::regex_constants::icase);
    std::smatch match;
    if (std::regex_search(sql, match, pattern))
    {
        std::cout << std::endl << "[ERROR] SQL Injection detected in query: " << std::endl;
        std::cout << "\tFailed due to potential always true statement.";
        std::cout << std::endl << "\t" << sql;
        std::cout << std::endl << "[INFO] Query not executed." << std::endl << std::endl;
        return false;
    }

    // Check for SQL keywords in the query
    std::string lower_sql = sql;
    // Convert to lowercase for case-insensitive comparison
    std::transform(lower_sql.begin(), lower_sql.end(), lower_sql.begin(), ::tolower);
    // Targets the most common SQL keywords that could be used in an injection attack
    // drop, delete, insert, update, select, where
    if (lower_sql.find("drop") != std::string::npos || lower_sql.find("delete") != std::string::npos ||
        lower_sql.find("insert") != std::string::npos || lower_sql.find("update") != std::string::npos)
    {
        std::cout << std::endl << "[ERROR] SQL Injection detected in query: " << std::endl;
        std::cout << "\tFailed due to potentially dangerous SQL keywords. Not allowed in application prepared queries.";
        std::cout << std::endl << "\t" << sql;
        std::cout << std::endl << "[INFO] Query not executed." << std::endl << std::endl;
        return false;
    }

    // clear any prior results
    records.clear();

    char* error_message;
    if(sqlite3_exec(db, sql.c_str(), callback, &records, &error_message) != SQLITE_OK)
    {
        std::cout << std::endl << "[ERROR] Data failed to be queried from USERS table. ERROR = " << std::endl << "\t" << error_message << std::endl << std::endl;
        sqlite3_free(error_message);
        return false;
    }

    return true;
}

// DO NOT CHANGE
bool run_query_injection(sqlite3* db, const std::string& sql, std::vector< user_record >& records)
{
  std::string injectedSQL(sql);
  std::string localCopy(sql);

  // we work on the local copy because of the const
  std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);
  if(localCopy.find_last_of(str_where) >= 0)
  { // this sql has a where clause
    if(localCopy.back() == ';')
    { // smart SQL developer terminated with a semicolon - we can fix that!
      injectedSQL.pop_back();
    }

    switch (rand() % 4)
    {
    case 1:
      injectedSQL.append(" or 2=2;");
      break;
    case 2:
      injectedSQL.append(" or 'hi'='hi';");
      break;
    case 3:
      injectedSQL.append(" or 'hack'='hack';");
      break;
    case 0:
    default:
      injectedSQL.append(" or 1=1;");
      break;
    }
  }
  
  return run_query(db, injectedSQL, records);
}


// DO NOT CHANGE
void dump_results(const std::string& sql, const std::vector< user_record >& records)
{
  std::cout << std::endl << "SQL: " << sql << " ==> " << records.size() << " records found." << std::endl;

  for (auto record : records)
  {
    std::cout << "User: " << std::get<1>(record) << " [UID=" << std::get<0>(record) << " PWD=" << std::get<2>(record) << "]" << std::endl;
  }
}

// DO NOT CHANGE
void run_queries(sqlite3* db)
{
  char* error_message = NULL;

  std::vector< user_record > records;

  // query all
  std::string sql = "SELECT * from USERS";
  if (!run_query(db, sql, records)) return;
  dump_results(sql, records);

  //  query 1
  sql = "SELECT ID, NAME, PASSWORD FROM USERS WHERE NAME='Fred'";
  if (!run_query(db, sql, records)) return;
  dump_results(sql, records);

  //  run query 1 with injection 5 times
  for (auto i = 0; i < 5; ++i)
  {
    if (!run_query_injection(db, sql, records)) continue;
    dump_results(sql, records);
  }

}

// You can change main by adding stuff to it, but all the existing code must remain, and be in the
//  order called, and with none of this existing code placed into conditional statements
int main()
{
  // initialize random seed:
  srand(time(nullptr));

  int return_code = 0;
  std::cout << std::endl << "[] Welcome to the SQL Injection Example []" << std::endl;
  std::cout << "==========================================" << std::endl << std::endl;

  // the database handle
  sqlite3* db = NULL;
  char* error_message = NULL;

  // open the database connection
  int result = sqlite3_open(":memory:", &db);

  if(result != SQLITE_OK)
  {
    std::cout << "[INFO] Failed to connect to the database and terminating. ERROR=" << sqlite3_errmsg(db) << std::endl;
    return -1;
  }

  std::cout << "[INFO] Connected to the database." << std::endl;

  // initialize our database
  if(!initialize_database(db))
  {
    std::cout << "[INFO] Database Initialization Failed. Terminating." << std::endl;
    return_code = -1;
  }
  else
  {
    run_queries(db);
  }

  // close the connection if opened
  if(db != NULL)
  {
    sqlite3_close(db);
  }

  return return_code;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
