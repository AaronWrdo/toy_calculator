#include <iostream>
#include <vector>
using namespace std;
double expression();


// token.type == 'n' 表示token为数字
// token.type == 'v' 表示为字符串
// token.type == 'o' 为保留字
const char NUMBER = 'n';
const char STRING = 's';
const char RESERVED = 'r';


// 定义"var"为保留字，不能作为变量名
const string V = "var";


// Token类
class Token {
public:
    char type;
    double value;
    string name;
    Token(){};
    Token(char c):type(c){}; // 符号
    Token(char c, double d):type(c), value(d){}; // 数字
    Token(char c, string s): type(c), name(s){}; // 字符串
};


// Token输入流类，用于读取和存放token
class TokenStream {
private:
    bool full;
    Token buffer;
public:
    TokenStream(){full = false; buffer = 0;};
    Token get();
    void put_back(Token);
} ts; // 全局对象ts


// 读取token，起到词法分析的作用
Token TokenStream::get() {
    if (full) {
        full = false;
        return buffer;
    }

    char c;
    Token t;
    cin >> c;
    switch(c) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
        case '?':
        case ';':
            // 处理读入符号的情况
            return Token(c);
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            // 处理读入数字的情况
            // trick：第一位读到数字，则将其放回stream，以double形式重新读取
            cin.putback(c);
            double d;
            cin >> d;
            return Token(NUMBER, d);
        default:
            // 处理读入 string 的情况
            // 注意此处不能和数字用同一种思路处理，必须逐个读入char并判断
            if(isalpha(c)) { // 合法的 name 必须以字母开始
                string s;
                s += c;

                // 剩余字符为字母或数字
                // 使用 cin.get() 以避免跳过空格、tab等
                while(cin.get(c) && (isalpha(c) || isdigit(c))) s += c;

                // 处理保留字
                if(s == V) return Token(RESERVED, s); 

                cin.putback(c); // 将多读的一个char放回
                return Token(STRING, s);
            }
            throw runtime_error("不符合规则的字符！");
    }
}

// 将token临时放入缓冲区
void TokenStream::put_back(Token t) {
    full = true;
    buffer = t;
}


// 变量类
class Variable {
public:
    string name;
    double value;
    Variable(){}
    Variable(string n, double d):name(n), value(d){}
};
vector<Variable> var_table; // 存放所有定义的变量

void print_var_table() {
    int i, j = var_table.size();
    cout << "===== 定义的变量如下：" << endl;
    for (i = 0; i < j; i++) {
        cout << var_table[i].name << " = " << var_table[i].value << endl;
    }
}

// 在 var_table 中查找变量，若有则返回变量的值
double get_var(string name) {
    for(int i = 0; i < var_table.size(); i++) {
        if (name == var_table[i].name)
            return var_table[i].value;
    }
    throw runtime_error("找不到名为"+name+"的变量！");
}

// 设置变量
void set_var(string name, double value) {
    // 先判断变量是否存在，如果已经存在，就覆盖
    for(int i = 0; i < var_table.size(); i++) {
        if (name == var_table[i].name) {
            var_table[i].value = value;
            return;
        }
    }
    var_table.push_back(Variable(name, value));
}


double primary() {
    Token t = ts.get();
    Token t2; // 提前声明 t2 和 d，注意 switch-case 中不能定义对象
    double d;
    switch(t.type) {
        case NUMBER:
            return t.value;
        case STRING: // 处理变量
            return get_var(t.name);
        case '+':
            return primary();
        case '-':
            return -primary();
        case '(':
            d = expression();
            t2 = ts.get();
            if(t2.type != ')') throw runtime_error("缺少反括号。");
            return d;
        default:
            throw runtime_error("不符合规则的表达式！");
    }
}


double term() {
    double left = primary();
    Token t = ts.get();
    double divider;
    switch(t.type) {
        case '*': return left * term(); break;
        case '/':
            // 除法计算，注意处理除数为0的情况
            divider = term();
            if(divider == 0) throw runtime_error("除数不能为零。");
            return left / divider;
        case '%':
            // 取余运算
            divider = term();
            if(divider == 0) throw runtime_error("除数不能为零。");
            return left - (divider * int(left / divider));
        default:
            ts.put_back(t);
            return left;
    }
}

double expression() {
    double left = term();
    Token t = ts.get();
    switch(t.type) {
        case '+':
            return left + expression();
        case '-':
            return left - expression();
        default:
            ts.put_back(t);
            return left;
    }
}


double declaration() {
    Token t = ts.get();
    Token t1;
    if(t.name == V) {
        t1 = ts.get();
        if(t1.type == RESERVED) throw runtime_error("不能使用保留字作为变量名！");
        string var_name = t1.name;

        t1 = ts.get();
        if (t1.type != '=') throw runtime_error("赋值语句缺少‘=’！");

        double right = expression();
        set_var(var_name, right);

        t1 = ts.get();
        if (t1.type != ';') throw runtime_error("赋值语句缺少‘;’！");

        cout << "===== 变量表更新。" << endl;
        print_var_table(); // 打印var_table
        return right;
    }
    ts.put_back(t);
    return expression();
}


int main() {
    cout << "================= 简易计算器 ===============\n"
         << "输入要计算的表达式后，输入'?'显示结果。\n"
         << "支持加(+) 减(-) 乘(*) 除(/) 取余(%) 运算。\n"
         << "支持浮点数运算。\n"
         << "支持自定义变量。\n"
         << "输入'q'退出计算器。\n";

    // 控制结构
    // todo:优化控制结构
    // todo:出结果和提示的格式优化
    while(cin) {
        try {
            cout << "> ";
            double result;
            Token token = ts.get();
            if (token.name == "q") break;
            else if (token.type == '?') cout << "=" << result << endl;
            else {
                ts.put_back(token);
                result = declaration();
            }
        }
        catch(exception &e) {
            cerr << "Error:" << e.what() << endl;
            while(cin) { // 若表达式出错，则不输出该表达式结果
                Token token = ts.get();
                if(token.type == '?') break; // 以‘?’作为表达式的结尾
            }
        }
        catch(...) {
            cerr << "Unkwon error." << endl;
        }
    }
    return 0;
}
