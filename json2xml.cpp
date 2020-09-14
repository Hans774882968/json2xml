#include <bits/stdc++.h>
#include "utils.cpp"
using namespace std;
typedef long long LL;
#define rep(i,a,b) for(int i = (a);i <= (b);++i)
const int SZ = 100 + 5;

int n;
string s,cur;char token;int idx;

string parse_val(string ky,int dep,int tag_attr);
string parse_obj(string nam,int dep,int tag_attr);

template<typename Type>inline void read(Type &xx){
    Type f = 1;char ch;xx = 0;
    for(ch = getchar();ch < '0' || ch > '9';ch = getchar()) if(ch == '-') f = -1;
    for(;ch >= '0' && ch <= '9';ch = getchar()) xx = xx * 10 + ch - '0';
    xx *= f;
}

void advance(){
    /*思想：归纳基础是可以正确来到第1个引号。那么我们只要保证反斜杠是匹配的，就可以保证第1次正确地来到和上述引号匹配的引号。这个算法可以保证第1个key正确读取，于是由归纳法，当前总是能正确判定是否在字符串内部。
    最后应停在匹配引号的后一个位置，比如":"、","和"}"。*/
    if(s[idx] == '"' && s[idx - 1] != '\\'){
        int pos = idx,slash_state = 0;
        for(;;++pos){
            if(s[pos] == '\\')
                slash_state = !slash_state;
            else if(pos > idx && s[pos] == '"'){
                if(!slash_state) break;
                else slash_state = 0;
            }
        }
        cur = s.substr(idx + 1,pos - idx - 1);
        idx = pos + 1;
    }
    else token = s[idx++];
}

string parse_str(){
    advance();
    return cur;
}

string parse_array(string ky,int dep,int tag_attr){
    advance();
    string res = parse_val(ky,dep,tag_attr);
    advance();
    while(token == ','){
        res += (tag_attr ? "," : "") + parse_val(ky,dep,tag_attr);
        advance();
    }
    return res;
}

string parse_val(string ky,int dep,int tag_attr){
    if(!tag_attr){
        if(s[idx] == '{'){
            return parse_obj(ky,dep + 4,tag_attr);
        }
        else if(s[idx] == '['){
            return parse_array(ky,dep,tag_attr);
        }
        string val = clean_str(parse_str());
        string res = tab(dep + 4) + "<" + ky + ">" + val + "</" + ky + ">\n";
        return res;
    }
    if(s[idx] == '{'){
        parse_obj(ky,dep + 4,tag_attr);
        return "[object Object]";
    }
    else if(s[idx] == '['){
        return parse_array(ky,dep,tag_attr);
    }
    return clean_str(parse_str());
}

string parse_key(){
    return parse_str();
}

//key必须预先clean，放进map后再clean比较麻烦；
//val可以预先clean，但是对象不应该被clean，因为会产生未预料结果。比如(假设已转义)"\\\\"->"\\"->"\"。
string parse_obj(string nam = "o",int dep = 0,int tag_attr = 0){
    string hd = tab(dep) + "<" + nam,body(""),tl = tab(dep) + "</" + nam + ">\n";
    advance();//assert(token == '{');
    string ky = clean_str(parse_key());
    advance();//assert(token == ':');
    string val = parse_val(ky,dep,ky[0] == '-');
    if(ky[0] == '-') hd += " " + ky.substr(1) + "=\"" + val + "\"";
    else body += val;
    advance();
    while(token == ','){
        ky = clean_str(parse_key());
        advance();//assert(token == ':');
        string val = parse_val(ky,dep,ky[0] == '-');
        if(ky[0] == '-') hd += " " + ky.substr(1) + "=\"" + val + "\"";
        else body += val;
        advance();
    }
    //assert(token == '}');
    if(body.size() > 0) return hd + ">\n" + body + tl;
    return hd + " />\n";
}

int main(int argc, char** argv) {
    while(1){
        read(n);
        if(n <= 0) break;
        idx = 0;s = "";
        rep(_,1,n){
            char s0[SZ];fgets(s0,SZ,stdin);s0[strlen(s0) - 1] = '\0';
            s += s0;
        }
        s = clear_spaces(s);
        string res = parse_obj();
        ofstream outf("output.xml",ios::out);
        outf << res;
        outf.close();
        cout << "Written successfully!\n";
    }
    return 0;
}