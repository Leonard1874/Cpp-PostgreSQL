#include "query_funcs.h"

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name, int mpg, int ppg, int rpg, int apg, double spg, double bpg){
  work W(*C);
  std::string s = "INSERT INTO \"PLAYER\" (\"TEAM_ID\",\"UNIFORM_NUM\",\"FIRST_NAME\",\"LAST_NAME\",\"MPG\",\"PPG\",\"RPG\", \"APG\", \"SPG\", \"BPG\") VALUES(";
  s += W.quote(team_id);
  s += ",";
  s += W.quote(jersey_num);
  s += ",";
  s += W.quote(first_name);
  s += ",";
  s += W.quote(last_name);
  s += ",";
  s += W.quote(mpg);
  s += ",";
  s += W.quote(ppg);
  s += ",";
  s += W.quote(rpg);
  s += ",";
  s += W.quote(apg);
  s += ",";
  s += W.quote(spg);
  s += ",";
  s += W.quote(bpg);
  s += ");";
  W.exec(s.c_str());
  W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses){
  work W(*C);
  std::string s = "INSERT INTO \"TEAM\" (\"NAME\", \"STATE_ID\", \"COLOR_ID\", \"WINS\", \"LOSSES\") VALUES(";
  s += W.quote(name);
  s += ",";
  s += W.quote(state_id);
  s += ",";
  s += W.quote(color_id);
  s += ",";
  s += W.quote(wins);
  s += ",";
  s += W.quote(losses);
  s += ");";
  W.exec(s.c_str());
  W.commit();
}

void add_state(connection *C, string name){
  work W(*C);
  std::string s = "INSERT INTO \"STATE\" (\"NAME\") VALUES(";
  s += W.quote(name);
  s += ");";
  W.exec(s.c_str());
  W.commit();
}


void add_color(connection *C, string name){
  work W(*C);
  std::string s = "INSERT INTO \"COLOR\" (\"NAME\") VALUES(";
  s += W.quote(name);
  s += ");";
  W.exec(s.c_str());
  W.commit();
}

template <typename T>
void appendS(nontransaction& W, std::string& S, const std::string item, int use, T min, T max){
  if(use){
    S += "\"" + item + "\" >= ";
    S += W.quote(min);
    S += " AND \"" + item + "\" <= ";
    S += W.quote(max);
    S += "AND ";
  }
}

void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            ){
  
  nontransaction W(*C);
  std::string S = "SELECT * FROM \"PLAYER\" WHERE ";
  appendS<int>(W,S,"MPG",use_mpg,min_mpg,max_mpg);
  appendS<int>(W,S,"PPG",use_ppg,min_ppg,max_ppg);
  appendS<int>(W,S,"RPG",use_rpg,min_rpg,max_rpg);
  appendS<int>(W,S,"APG",use_apg,min_apg,max_apg);
  appendS<double>(W,S,"SPG",use_spg,min_spg,max_spg);
  appendS<double>(W,S,"BPG",use_bpg,min_bpg,max_bpg);
  S += "\"PLAYER_ID\" >= 0;";
  
  result R(W.exec(S.c_str()));
  std::cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << std::endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<int>() << " ";
    std::cout << c[1].as<int>() << " ";
    std::cout << c[2].as<int>() << " ";
    std::cout << c[3].as<string>() << " ";
    std::cout << c[4].as<string>() << " ";
    std::cout << c[5].as<int>() << " ";
    std::cout << c[6].as<int>() << " ";
    std::cout << c[7].as<int>() << " ";
    std::cout << c[8].as<int>() << " ";
    std::cout << c[9].as<double>() << " ";
    std::cout << c[10].as<double>() << std::endl;
  }
  
}


void query2(connection *C, string team_color){
  nontransaction W(*C);
  std::string s = "SELECT \"TEAM\".\"NAME\" FROM \"TEAM\", \"COLOR\" WHERE \"COLOR\".\"NAME\" = ";
  s += W.quote(team_color);
  s += " AND \"TEAM\".\"COLOR_ID\" = \"COLOR\".\"COLOR_ID\";";
  result R(W.exec(s.c_str()));
  std::cout << "NAME" << std::endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<string>() << std::endl;
  }
}


void query3(connection *C, string team_name){
  nontransaction W(*C);
  std::string s = "SELECT \"FIRST_NAME\", \"LAST_NAME\" FROM \"PLAYER\", \"TEAM\" WHERE \"TEAM\".\"NAME\" = ";
  s += W.quote(team_name);
  s += " AND \"PLAYER\".\"TEAM_ID\" = \"TEAM\".\"TEAM_ID\" ORDER BY \"PPG\" DESC;";
  result R(W.exec(s.c_str()));
  std::cout << "FIRST_NAME LAST_NAME" << std::endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<string>() << " ";
    std::cout << c[1].as<string>() << std::endl;
  }
}


void query4(connection *C, string team_state, string team_color){
  nontransaction W(*C);
  std::string s = "SELECT \"FIRST_NAME\", \"LAST_NAME\", \"UNIFORM_NUM\" FROM \"PLAYER\", \"TEAM\", \"STATE\", \"COLOR\" WHERE \"STATE\".\"NAME\" = ";
  s += W.quote(team_state);
  s += " AND \"COLOR\".\"NAME\" = ";
  s += W.quote(team_color);
  s += " AND \"PLAYER\".\"TEAM_ID\" = \"TEAM\".\"TEAM_ID\" AND \"TEAM\".\"STATE_ID\" = \"STATE\".\"STATE_ID\" AND \"TEAM\".\"COLOR_ID\" = \"COLOR\".\"COLOR_ID\";";
  result R(W.exec(s.c_str()));
  std::cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" << std::endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<string>() << " ";
    std::cout << c[1].as<string>() << " ";
    std::cout << c[2].as<int>() << std::endl;
  }
}


void query5(connection *C, int num_wins){
  nontransaction W(*C);
  std::string s = "SELECT \"FIRST_NAME\", \"LAST_NAME\", \"NAME\", \"WINS\" FROM \"PLAYER\", \"TEAM\" WHERE \"WINS\" > ";
  s += W.quote(num_wins);
  s += " AND \"PLAYER\".\"TEAM_ID\" = \"TEAM\".\"TEAM_ID\";";
  result R(W.exec(s.c_str()));
  std::cout << "FIRST_NAME LAST_NAME NAME WINS" << std::endl;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<string>() << " ";
    std::cout << c[1].as<string>() << " ";
    std::cout << c[2].as<string>() << " ";
    std::cout << c[3].as<int>() << std::endl;
  }
}
