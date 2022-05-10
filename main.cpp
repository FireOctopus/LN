#include<iostream>
#include<fstream>
#include<map>
#include<stack>
#include"LN.h"

using namespace std;

void dump(ofstream &out,stack <LN*> &s)
{
	while (!s.empty()) {
		out << s.top()->toString() << endl;
		s.pop();
	}
}

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("Usage: \"%s\" <Input file> <Output file>", argv[0]);
	}
	ifstream in(argv[1]);
	if (!in.is_open()) {
		printf("ERROR: INPUT FILE \"%s\" NOT FOUND", argv[1]);
		return 1;
	}
	map<string, short int> operators = { {"+", 2}, {"-", 2}, {"*", 2}, {"/", 2}, {"%", 2}, {"~", 1}, {"_", 1},
										 {"<", 2}, {"<=", 2}, {">", 2}, {">=", 2}, {"==", 2}, {"!=", 2} };
	stack<LN*> expr;
	stack<LN*> results;
	ofstream out(argv[2]);
	if (!out.is_open()) {
		printf("ERROR: CANNOT CREATE OUTPUT FILE \"%s\"", argv[2]);
		return 1;
	}
	try {
		while (!in.eof()) {
			string token;
			in >> token;
			if (operators.count(token) == 0)
				expr.push(new LN(token));
			else {
				LN t = LN();
				if (operators[token] == 1) {
					LN op = *expr.top();
					expr.pop();
					if (token == "_") {
						t = -op;
					} else {
						t = op.sqrt();
					}
				} else {
					LN b = *expr.top();
					expr.pop();
					LN a = *expr.top();
					expr.pop();
					switch (token[0]) {
					case '+':
						t = a + b;
						break;
					case '-':
						t = a - b;
						break;
					case '*':
						t = a * b;
						break;
					case '/':
						t = a / b;
						break;
					case '%':
						t = a % b;
						break;
					case '<':
						t = (token.length() == 2) ? (a <= b ? *(new LN(1LL)) : *(new LN())) :
							(a < b ? *(new LN(1LL)) : *(new LN()));
						break;
					case '>':
						t = (token.length() == 2) ? (a >= b ? *(new LN(1LL)) : *(new LN())) :
							(a > b ? *(new LN(1LL)) : *(new LN()));
						break;
					case '=':
						t = (a == b ? *(new LN(1LL)) : *(new LN()));
						break;
					case '!':
						t = (a != b ? *(new LN(1LL)) : *(new LN()));
					}
				}
				expr.push(new LN(t));
				results.push(new LN(t));
			}
		}
	} catch (bad_alloc &e) {
		printf("ERROR: Memory allocation failed");
		dump(out, results);
		return 2;
	} catch (int castError) {
		printf("Cast_Execption: Cannot cast to long long");
		return castError;
	}
	in.close();
	dump(out, results);
	out.close();
	return 0;
}
