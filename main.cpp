#include <iostream>
using namespace std;

class Token {
    public:
        char kind;
        double value;
        // constractor
        Token(){};
        Token(char, double);
};

Token::Token(char k, double v = 0){
    // if(v == 0) cout << "==== get_token: " << k << endl;
    // else cout << "==== get_token: " << v << endl;
    kind = k;
    value = v;
}


class TokenStream {
    private: 
        bool full;
        Token buffer;

    public:
        TokenStream(){full = false; buffer = 0;}; 
        Token get();
        void put_back(Token);
} ts; // 全局对象ts


Token TokenStream::get() {
    if (full) {
        full = false;
        return buffer;
    }

    char c;
    Token t;
    cin >> c;
    switch(c) {
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
            // trick：若是数字，则以double形式重新读取
            cin.putback(c);
            double d;
            cin >> d;
            return Token('d', d);
        case '+':
        case '-':
        case '*':
        case '/':
            return Token(c);
        default:
            return Token(c);
    }
}

void TokenStream::put_back(Token t) {
    full = true;
    buffer = t;
}


double primary() {
    Token t = ts.get();
    switch(t.kind) {
        case 'd': 
            // cout << "primary: " << t.value << endl; 
            return t.value;
        default: 
            throw runtime_error("错误的表达式！");
    }
}

double term() {
    double left = primary();
    Token t = ts.get();
    switch(t.kind) {
        case '*': return left * term(); break;
        case '/': return left / term(); break;
        default: 
            ts.put_back(t); 
            // cout << "term: " << left << endl; 
            return left;
    }

    // 迭代版本的term，供参考
    // while (true)
	// {
	// 	Token token=TokenStream::get();
	// 	switch (token.kind)
	// 	{
	// 	case '*':
	// 		left*=primary();
	// 		break;
	// 	case '/':
	// 		left/=primary();
	// 		break;
	// 	default:
    //         ts.put_back(t);
	// 		return left;
	// 	}
	// }
}

double expression() {
    double left = term();
    Token t = ts.get();
    switch(t.kind) {
        case '+':
            return left + expression(); break;
        case '-':
            return left - expression(); break;
        default: 
            ts.put_back(t); 
            // cout << "expression: " << left << endl; 
            return left;
    }
}


int main() {
    // cout << ">";
    // 控制结构
    while (cin) {
		try {
            cout << ">";
            double result;
			Token token = ts.get();
			if (token.kind =='q') break;
			else if (token.kind == '?') cout << "=" << result << endl;
			else {
                ts.put_back(token);
				result=expression();
			}
		}
		catch(exception &e) {
			cerr<<"Error:"<<e.what()<<endl;
		}
		catch(...) {
			cerr<<"Unkwon error."<<endl;
		}
	}
    return 0;
}

