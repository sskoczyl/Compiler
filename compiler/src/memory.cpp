#include "compiler.hpp"

vector<vector<string>> meta_array;
vector<string> commands;
map <string , Variable*> memory; 
stack<pair<Variable*, Variable*>> arr_queue;
stack<pair<Variable*, long long int>> jumps;
stack<tuple<long long int, Variable*, Variable*>> loops_stack;
long long int memory_offset=0;
long long int program_counter=0;
bool repeat_until = false;

string load_num_to_mem(string num)
{   
    if(memory.find(num) == memory.end())
    {   
        Type new_type = const_;
        Variable* new_variable = new Variable(num, memory_offset, 1, new_type);
        memory_offset++;
        memory.insert(make_pair(num, new_variable));

        write_command("RESET", "b");
        load_val_to_reg("b", stoi(num));
        assign_value(num);
    }

    return num;
}

void load_val_to_reg(string reg, long long int num) {
    if (num < 10) {
        for (long long int i = 0; i < num; ++i) {
            write_command("INC", reg);
        }
    } 
    else 
    {
        load_val_to_reg(reg, num >> 1);
        write_command("SHL", reg);
        if (num % 2 == 1) {
            write_command("INC", reg);
        }
    }
}

void declare_variable(string id, Type type)
{   
    if(memory.find(id) != memory.end())
        error("Variable "+id+" has already been declared!");
    else
    {   
        Type new_type = type;
        Variable* new_variable = new Variable(id, memory_offset, 1, new_type);
        memory_offset++;
        memory.insert(make_pair(id, new_variable));
    }   
}

void declare_variable(string id, string start, string end)
{
    if(memory.find(id) != memory.end())
        error("Variable "+id+" has already been declared!");
    else if(stoi(end) < stoi(start))
        error("Wrong "+id+" array bounds!\n");
    else
    {   
        Type type = array_;
        long long int len = abs(stoll(end)-stoll(start)) + 2;
        Variable *new_var = new Variable(id, memory_offset, len, type);
        new_var->start = stoll(end);
        memory_offset += len + 1;
        memory.insert(make_pair(id, new_var));
    }
}

void store_to_register(string reg, Variable* val)
{
    set_memory(val);
    write_command("STORE", reg, "a");
}

void set_register(string reg, long long int num)
{
    write_command("RESET", reg);
    load_val_to_reg(reg, num);
}

string set_reg_to_val(string id)
{
    Variable* v = memory.at(id);

    if(!v->created && v->type != array_)
        error("Variable "+id+" has not been initialized!");

    assignRegister("b", v);

    return id;
}

void assignRegister(string reg, Variable* var) 
{
    if (var->type == const_)
       set_register(reg, stoll(var->id));
    else 
        loadRegister(reg, var);
}

void assignRegister(string reg1, Variable* var1, string reg2, Variable* var2)
{
    if (var1->type == array_ && var2->type == array_) 
    {
        assignRegister(reg2, var1);
        assignRegister(reg1, var2);
    } else {
        assignRegister(reg1, var1);
        assignRegister(reg2, var2);
    }
}

void loadRegister(string reg, Variable* var) 
{
    set_memory(var);
    write_command("LOAD", reg, "a");
}

void set_memory(Variable* var) 
{
    if (var->type == array_) 
    {
        assignRegister("f", arr_queue.top().second);
        set_register("a", arr_queue.top().first->adress);
        write_command("ADD", "a", "f");
        arr_queue.pop();
    } else {
        set_register("a", var->adress);
    }
}

void get_composed_id(string tab_id, string v_id)
{
    if(memory.find(v_id) == memory.end())
        error("Variable "+v_id+" has not been initialized");

    Variable* tab = memory.at(tab_id);
    Variable* idx = memory.at(v_id);

    if(tab->type!=array_)
        error("Variable "+tab_id+" is not an array");
    
    if(!idx->created)
        error("Variable "+v_id+" has not been initialized!");

    arr_queue.push(make_pair(tab, idx));
}

void get_composed_num_id(string tab_id, string v_id)
{
    Variable* tab = memory.at(tab_id);

    if(memory.find(v_id) == memory.end())
        v_id = load_num_to_mem(v_id);


    Variable* idx = memory.at(v_id);

    if(tab->type!=array_)
        error("Variable "+tab_id+" is not an array");
    
    if(!idx->created)
        error("Variable "+v_id+" has not been initialized!");

    arr_queue.push(make_pair(tab, idx));
}

void push_jump_loop()
{
    string name= "J"+to_string(program_counter);
    declare_variable(name, simple);
    memory.at(name)->created=true;
    store_to_register("a", memory.at(name));
    Variable* var = memory.at(name); 
    jumps.push(make_pair(var, program_counter));
}

void check_if_exists(string id)
{
    if(memory.find(id)==memory.end())
        error("Variable "+id+" has not been declared!");

    if(memory.at(id)->type==array_)
        error("Variable "+id+" is array type. Cannot be used like that!");
}