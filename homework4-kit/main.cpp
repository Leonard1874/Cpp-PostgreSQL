#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "exerciser.h"

using namespace std;
using namespace pqxx;

vector<string> parseLine(string& line){
  stringstream ss(line);
  string temp;
  vector<string> res;
  while(getline(ss,temp,' ')){
    res.push_back(temp);
  }
  return res;
}

vector<string> getLine(const char* fileName){
  string line;
  ifstream myfile(fileName);
  vector<string> gotLine;
  if (myfile.good()){
    while (getline(myfile, line)){
      gotLine.push_back(line);
    }
    myfile.close();
  }
  else{
    throw exception();
  }
  return gotLine;
}

int main (int argc, char *argv[]) {

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  try{
    /*drop all tables*/
    string dropsql;
    work W0(*C);
    dropsql = "DROP TABLE IF EXISTS \"PLAYER\";";
    W0.exec(dropsql);
    W0.commit();

    work W1(*C);
    dropsql = "DROP TABLE IF EXISTS \"TEAM\";";
    W1.exec(dropsql);
    W1.commit();

    work W2(*C);
    dropsql = "DROP TABLE IF EXISTS \"STATE\";";
    W2.exec(dropsql);
    W2.commit();

    work W3(*C);
    dropsql = "DROP TABLE IF EXISTS \"COLOR\";";
    W3.exec(dropsql);
    W3.commit();
    
    /*create tables*/
    work W4(*C);
    string createsql = "CREATE TABLE \"COLOR\"("  \
      "\"COLOR_ID\"       SERIAL   NOT NULL," \
      "\"NAME\"           TEXT     NOT NULL," \
      "PRIMARY KEY (\"COLOR_ID\"));";
    W4.exec(createsql);
    W4.commit();

    work W5(*C);
    createsql = "CREATE TABLE \"STATE\"("  \
      "\"STATE_ID\"       SERIAL   NOT NULL," \
      "\"NAME\"           TEXT     NOT NULL," \
      "PRIMARY KEY (\"STATE_ID\"));";
    W5.exec(createsql);
    W5.commit();

    work W6(*C);
    createsql = "CREATE TABLE \"TEAM\"("  \
      "\"TEAM_ID\"      SERIAL   NOT NULL," \
      "\"NAME\"           TEXT     NOT NULL," \
      "\"STATE_ID\"       INT      NOT NULL," \
      "\"COLOR_ID\"       INT      NOT NULL," \
      "\"WINS\"           INT      NOT NULL," \
      "\"LOSSES\"          INT      NOT NULL," \
      "PRIMARY KEY (\"TEAM_ID\"),"\
      "FOREIGN KEY (\"STATE_ID\") REFERENCES \"STATE\"(\"STATE_ID\"),"\
      "FOREIGN KEY (\"COLOR_ID\") REFERENCES \"COLOR\"(\"COLOR_ID\"));";
    W6.exec(createsql);
    W6.commit();

    work W7(*C);
    createsql = "CREATE TABLE \"PLAYER\"("  \
      "\"PLAYER_ID\"      SERIAL   NOT NULL," \
      "\"TEAM_ID\"        INT      NOT NULL," \
      "\"UNIFORM_NUM\"    INT      NOT NULL," \
      "\"FIRST_NAME\"     TEXT     NOT NULL," \
      "\"LAST_NAME\"      TEXT     NOT NULL," \
      "\"MPG\"            INT      NOT NULL," \
      "\"PPG\"            INT      NOT NULL," \
      "\"RPG\"            INT      NOT NULL," \
      "\"APG\"            INT      NOT NULL," \
      "\"SPG\"   DOUBLE PRECISION  NOT NULL," \
      "\"BPG\"   DOUBLE PRECISION  NOT NULL," \
      "PRIMARY KEY (\"PLAYER_ID\"),"\
      "FOREIGN KEY (\"TEAM_ID\") REFERENCES \"TEAM\"(\"TEAM_ID\"));";
    W7.exec(createsql);
    W7.commit();

    /*load table*/
    vector<string> gotLine;
    vector<string> parsedLine;
    gotLine = getLine("color.txt");
    for(size_t i = 0; i < gotLine.size(); i++){
      parsedLine = parseLine(gotLine[i]);
      add_color(C, parsedLine[1]);
    }
    
    gotLine = getLine("state.txt");
    for(size_t i = 0; i < gotLine.size(); i++){
      parsedLine = parseLine(gotLine[i]);
      add_state(C, parsedLine[1]);
    }
    
    gotLine = getLine("team.txt");
    for(size_t i = 0; i < gotLine.size(); i++){
      parsedLine = parseLine(gotLine[i]);
      add_team(C, parsedLine[1], atoi(parsedLine[2].c_str()), atoi(parsedLine[3].c_str()), atoi(parsedLine[4].c_str()), atoi(parsedLine[5].c_str()));
    }

    gotLine = getLine("player.txt");
    for(size_t i = 0; i < gotLine.size(); i++){
      parsedLine = parseLine(gotLine[i]);
      add_player(C, atoi(parsedLine[1].c_str()), atoi(parsedLine[2].c_str()), parsedLine[3], parsedLine[4], atoi(parsedLine[5].c_str()), atoi(parsedLine[6].c_str()), atoi(parsedLine[7].c_str()), atoi(parsedLine[8].c_str()),atof(parsedLine[9].c_str()), atof(parsedLine[10].c_str()));
    }
  
  }
  catch(const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }

  exercise(C);

  //Close database connection
  C->disconnect();

  return 0;
}


