#pragma once 

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>
#include <climits>

using namespace std;

enum Type {simple, array_, iter, const_};

class Variable
{
    public:
        string id;
        bool created = false;
        int adress;
        int start;
        int length;
        Type type;

        Variable(string id, int adress, int length, Type type)
        {
            this->id = id;
            this->adress = adress;
            this->length = length;
            this->type = type;
        }
};


extern int yylineno;
extern vector<vector<string>> meta_array;
extern vector<string> commands;
extern map <string , Variable*> memory; 
extern long long int memory_offset;      //indicates how many memory cells are occupied
extern long long int program_counter;
extern map<string, long long int> registers;
extern stack<pair<Variable*, Variable*>> arr_queue;
extern stack<pair<Variable*, long long int >> jumps;
extern stack<tuple<long long int, Variable*, Variable*>> loops_stack;
extern bool repeat_until;

void declare_variable(string id, Type type);
void declare_variable(string id, string start, string end);
void set_up_memory();
string load_num_to_mem(string num);
void load_val_to_reg(string reg, long long int num);
void set_memory(Variable*);
string set_reg_to_val(string id);
void get_composed_id(string tab_id, string v_id);
void get_composed_num_id(string tab_id, string v_id);
void push_jump_loop();

void store_to_register(string reg, Variable* addr);
void set_register(string reg, long long int val);
void assignRegister(string reg, Variable* var);
void assignRegister(string reg1, Variable* var1, string reg2, Variable* var2);
void loadRegister(string reg, Variable* var);
void expire_registers();
void check_if_exists(string id);

void assign_value(string id);
void write_value(string id);
void read_value(string id);
void add_values(string id1, string id2);
void sub_values(string id1, string id2);
void div_values(string id1, string id2);
void mul_values(string id1, string id2);
void mod_values(string id1, string id2);

void start_while();
void finish_while();
void start_repeat();
void finish_repeat();
void start_for(string id, string val1, string val2);
void end_for(int opt);
void start_if();
void finish_if();
void cond_eq(string id, string id2);
void cond_neq(string id, string id2);
void cond_lt(string id, string id2);
void cond_gt(string id, string id2);
void cond_leqt(string id, string id2);
void cond_geqt(string id, string id2);

void write_command(string command, string val);
void write_command(string command, string val, string val_);
void error(string message);

void replace(string& str, const string& from, const string& to);