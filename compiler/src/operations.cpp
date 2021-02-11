#include "compiler.hpp"


void assign_value(string id)
{   
    Variable *var = memory.at(id);

    if (var->type == iter)
        error("Can't modify loop iterator "+id+"!");

    memory.at(id)->created = true;
    store_to_register("b", var);
}

void write_value(string id)
{
    Variable* var = memory.at(id);

    if(!var->created)
        error("Variable "+id+" has not been initialized!");
        
    set_memory(var);
    write_command("PUT", "a");
}

void read_value(string id)
{
    Variable* var = memory.at(id);
    var->created = true;

    set_memory(var);
    write_command("GET", "a");
}

void add_values(string id1, string id2)
{
    Variable* var1 = memory.at(id1);
    Variable* var2 = memory.at(id2);
    

    assignRegister("b", var1, "c", var2);
    write_command("ADD", "b", "c");
}

void sub_values(string id1, string id2)
 {
    Variable* var1 = memory.at(id1);
    Variable* var2 = memory.at(id2);

    assignRegister("b", var1, "c", var2);
    write_command("SUB", "b", "c");
}

void div_values(string id1, string id2) 
{
    Variable* var1 = memory.at(id1);
    Variable* var2 = memory.at(id2);


    assignRegister("d", var1, "c", var2);
    write_command("RESET", "b");
    write_command("JZERO", "c", "26");
    write_command("RESET", "e");      
    write_command("ADD", "e", "c");
    write_command("RESET", "b");
    write_command("ADD", "b", "e");
    write_command("SUB", "b", "d");
    write_command("JZERO", "b", "2");
    write_command("JUMP", "3");
    write_command("SHL", "e");
    write_command("JUMP", "-5");
    write_command("RESET", "b");
    write_command("RESET", "f"); 
    write_command("ADD", "f", "e");
    write_command("SUB", "f", "d");
    write_command("JZERO", "f", "4");
    write_command("SHL", "b");
    write_command("SHR", "e");
    write_command("JUMP", "5");
    write_command("SHL", "b");
    write_command("INC", "b");
    write_command("SUB", "d", "e");
    write_command("SHR", "e");
    write_command("RESET", "f");
    write_command("ADD", "f", "c");
    write_command("SUB", "f", "e");
    write_command("JZERO", "f", "-14");
}

void mul_values(string id1, string id2)
{
    Variable* var1 = memory.at(id1);
    Variable* var2 = memory.at(id2);

    assignRegister("c", var1, "d", var2);
    write_command("RESET", "b");
    write_command("ADD", "b", "d");
    write_command("SUB", "b", "c");
    write_command("JZERO", "b", "10");
    write_command("RESET","b");
    write_command("JODD", "c", "2");
    write_command("JUMP", "2");
    write_command("ADD", "b", "d");
    write_command("SHL", "d");
    write_command("SHR", "c");
    write_command("JZERO", "c", "2");
    write_command("JUMP", "-6");
    write_command("JUMP", "8");
    write_command("JODD", "d", "2");
    write_command("JUMP", "2");
    write_command("ADD", "b", "c");
    write_command("SHL", "c");
    write_command("SHR", "d");
    write_command("JZERO", "d", "2");
    write_command("JUMP", "-6");
}

void mod_values(string id1, string id2)
{   
    Variable* var1 = memory.at(id1);
    Variable* var2 = memory.at(id2);

    assignRegister("d", var1, "c", var2);
    write_command("JZERO", "c", "28");
    write_command("JZERO", "c", "26");
    write_command("RESET", "e"); 
    write_command("ADD", "e", "c");
    write_command("RESET", "b");
    write_command("ADD", "b", "e");
    write_command("SUB", "b", "d");
    write_command("JZERO", "b", "2");
    write_command("JUMP", "3");
    write_command("SHL", "e");
    write_command("JUMP", "-5");
    write_command("RESET", "b");
    write_command("RESET", "f");
    write_command("ADD", "f", "e");
    write_command("SUB", "f", "d");
    write_command("JZERO", "f", "4");
    write_command("SHL", "b");
    write_command("SHR", "e");
    write_command("JUMP", "5");
    write_command("SHL", "b");
    write_command("INC", "b");
    write_command("SUB", "d", "e");
    write_command("SHR", "e");
    write_command("RESET", "f");
    write_command("ADD", "f", "c");
    write_command("SUB", "f", "e");
    write_command("JZERO", "f", "-13");
    write_command("JUMP", "2");
    write_command("RESET", "d");
    write_command("RESET", "b");
    write_command("ADD", "b", "d");
}

void write_command(string command, string val)
{   
    commands.push_back(command+" "+val);
    program_counter++;
}

void write_command(string command, string val, string val_)
{
    commands.push_back(command+" "+val+" "+val_);
    program_counter++;
}

void error(string message)
{
    cerr<<"LINE "<<yylineno<<": "<<message<<endl;
    exit(1);
}

void start_if()
{   
    replace(commands.at(jumps.top().second), "$bookmark", to_string(program_counter-jumps.top().second));

    assignRegister("f", jumps.top().first);
    jumps.pop();
    string name= "J"+to_string(program_counter);
    declare_variable(name, simple);
    memory.at(name)->created=true;
    store_to_register("f", memory.at(name));
    Variable* var = memory.at(name); 
    jumps.push(make_pair(var, program_counter));
    jumps.top().second++;
    write_command("JZERO", "f", "2");
    write_command("JUMP", "$bookmark");
}

void finish_if()
{
    replace(commands.at(jumps.top().second), "$bookmark", to_string(program_counter-jumps.top().second));

    jumps.pop();
}

void cond_eq(string id, string id2)
{
    Variable* var1 = memory.at(id);
    Variable* var2 = memory.at(id2);

    write_command("RESET","f");
    assignRegister("c", var1, "d", var2);
    write_command("RESET", "e");
    write_command("ADD", "e", "c");
    write_command("SUB", "e", "d");
    write_command("JZERO", "e", "2");
    write_command("JUMP", "5");
    write_command("SUB", "d", "c"); 
    write_command("JZERO", "d", "2");
    write_command("JUMP", "2");
    write_command("INC", "f");
    string name= "J"+to_string(program_counter);
    declare_variable(name, simple);
    memory.at(name)->created=true;
    store_to_register("f", memory.at(name));
    Variable* var = memory.at(name); 
    jumps.push(make_pair(var, program_counter));
    write_command("JZERO", "f", "$bookmark");

    repeat_until = false;
}

void cond_neq(string id, string id2)
{
    Variable* var1 = memory.at(id);
    Variable* var2 = memory.at(id2);
    write_command("RESET","b");
    assignRegister("c", var1, "d", var2);
    write_command("RESET", "e");
    write_command("ADD", "e", "c");
    write_command("SUB", "e", "d");
    write_command("JZERO", "e", "2");
    write_command("INC", "b");
    write_command("SUB", "d", "c"); 
    write_command("JZERO", "d", "2");
    write_command("INC", "b");
    string name= "J"+to_string(program_counter);
    declare_variable(name, simple);
    memory.at(name)->created=true;
    store_to_register("f", memory.at(name));
    Variable* var = memory.at(name); 
    jumps.push(make_pair(var, program_counter));
    write_command("JZERO", "b", "$bookmark");

    repeat_until = false;
}

void cond_lt(string id, string id2)
{
    Variable* var1 = memory.at(id);
    Variable* var2 = memory.at(id2);

    assignRegister("c", var1, "f", var2);
    write_command("SUB", "f", "c");
    string name= "J"+to_string(program_counter);
    declare_variable(name, simple);
    memory.at(name)->created=true;
    store_to_register("f", memory.at(name));
    Variable* var = memory.at(name); 
    jumps.push(make_pair(var, program_counter));
    write_command("JZERO", "f", "$bookmark");

    repeat_until = false;
}

void cond_gt(string id, string id2)
{
    Variable* var1 = memory.at(id);
    Variable* var2 = memory.at(id2);

    assignRegister("f", var1, "c", var2);
    write_command("SUB", "f", "c");
    string name= "J"+to_string(program_counter);
    declare_variable(name, simple);
    memory.at(name)->created=true;
    store_to_register("f", memory.at(name));
    Variable* var = memory.at(name); 
    jumps.push(make_pair(var, program_counter));
    write_command("JZERO", "f", "$bookmark");

    repeat_until = false;
}

void cond_leqt(string id, string id2)
{
    Variable* var1 = memory.at(id);
    Variable* var2 = memory.at(id2);
    
    assignRegister("c", var1, "f", var2);
    write_command("INC", "f");
    write_command("SUB", "f", "c");
    string name= "J"+to_string(program_counter);
    declare_variable(name, simple);
    memory.at(name)->created=true;
    store_to_register("f", memory.at(name));
    Variable* var = memory.at(name); 
    jumps.push(make_pair(var, program_counter));
    write_command("JZERO", "f", "$bookmark");

    repeat_until = false;
}

void cond_geqt(string id, string id2)
{
    Variable* var1 = memory.at(id);
    Variable* var2 = memory.at(id2);

    assignRegister("f", var1, "c", var2);
    write_command("INC", "f");
    write_command("SUB", "f", "c");
    string name= "J"+to_string(program_counter);
    declare_variable(name, simple);
    memory.at(name)->created=true;
    store_to_register("f", memory.at(name));
    Variable* var = memory.at(name); 
    jumps.push(make_pair(var, program_counter));
    write_command("JZERO", "f", "$bookmark");

    repeat_until = false;
}

void replace(string& str, const string& from, const string& to) 
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void start_while()
{   
    loops_stack.push(make_tuple(program_counter, nullptr, nullptr));
}

void finish_while()
{
    write_command("JUMP", to_string(get<0>(loops_stack.top())-program_counter));
    replace(commands.at(jumps.top().second), "$bookmark", to_string(program_counter-jumps.top().second));
    assignRegister("f", jumps.top().first);
    write_command("JZERO", "f", "2");
    write_command("JUMP", to_string(get<0>(loops_stack.top())-program_counter));

    jumps.pop();
    loops_stack.pop();
}

void start_repeat()
{
    loops_stack.push(make_tuple(program_counter, nullptr, nullptr));
}

void finish_repeat()
{
    replace(commands.at(jumps.top().second), "$bookmark", to_string(program_counter-jumps.top().second));
    assignRegister("f", jumps.top().first);
    write_command("JZERO", "f", to_string(2));
    write_command("JUMP", to_string(get<0>(loops_stack.top())-program_counter));

    jumps.pop();
    loops_stack.pop();
}

void start_for(string id, string val1, string val2)
{   
    declare_variable(id, iter);
    memory.at(id)->created = true;
    long long int idx = program_counter;
    declare_variable("C"+to_string(idx), simple);
    memory.at("C"+to_string(idx))->created = true;

    val1=load_num_to_mem(val1);
    val2=load_num_to_mem(val2);

    assignRegister("c", memory.at(val1), "f", memory.at(val2));
    store_to_register("c", memory.at(id));
    store_to_register("f", memory.at("C"+to_string(idx)));
  
    loops_stack.push(make_tuple(program_counter, memory.at(id), memory.at("C"+to_string(idx))));
}

void end_for(int opt)
{   
    if(opt == 1)
    {
        tuple<long long int,  Variable*, Variable*> temp = loops_stack.top();
        assignRegister("b", get<2>(temp), "c", get<1>(temp));

        write_command("RESET", "f");
        write_command("ADD", "f", "b");
        write_command("SUB", "f", "c");
        string name= "J"+to_string(program_counter);
        declare_variable(name, simple);
        memory.at(name)->created=true;
        store_to_register("f", memory.at(name));
        Variable* var = memory.at(name); 
        jumps.push(make_pair(var, program_counter));
        write_command("JZERO", "f", "$bookmark");
        write_command("INC", "c");
        store_to_register("c", get<1>(temp));
        write_command("JUMP", to_string(get<0>(temp)-program_counter));
        replace(commands.at(jumps.top().second), "$bookmark", to_string(program_counter-jumps.top().second));

        jumps.pop();
        loops_stack.pop();
        memory.erase(get<1>(temp)->id);
    }
    else if(opt==2)
    {

        tuple<long long int,  Variable*, Variable*> temp = loops_stack.top();
        assignRegister("b", get<2>(temp), "c", get<1>(temp));

        write_command("RESET", "f");
        write_command("ADD", "f", "c");
        write_command("SUB", "f", "b");
        string name= "J"+to_string(program_counter);
        declare_variable(name, simple);
        memory.at(name)->created=true;
        store_to_register("f", memory.at(name));
        Variable* var = memory.at(name); 
        jumps.push(make_pair(var, program_counter));
        write_command("JZERO", "f", "$bookmark");
        write_command("DEC", "c");
        store_to_register("c", get<1>(temp));
        write_command("JUMP", to_string(get<0>(temp)-program_counter));
        replace(commands.at(jumps.top().second), "$bookmark", to_string(program_counter-jumps.top().second));

        jumps.pop();
        loops_stack.pop();
        memory.erase(get<1>(temp)->id);
    }
}