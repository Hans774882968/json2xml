#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
#define rep(i,a,b) for(int i = (a);i <= (b);++i)

string tab(int dep){return string(dep,' ');}

//regex_replace不能正确匹配若干反斜杠这种字符串
string myreplace(string s,string old,string cur){
    int pos = 0;string ret;
    while(pos < s.size()){
        int pos1 = s.find(old,pos);
        if(pos1 == string::npos){ret += s.substr(pos);break;}
        else ret += s.substr(pos,pos1 - pos) + cur,pos = pos1 + old.size();
    }
    return ret;
}

//预处理。仍需要保证反斜杠后面的字符为‘"’或‘\’
string clear_spaces(string s){
    string ret = "";
    int pos = 0;
    while(pos < s.size()){
        int tmpp = pos;
        for(;pos < s.size() && s[pos] != '"';++pos);
        ret += myreplace(s.substr(tmpp,pos - tmpp)," ","");
        if(pos >= s.size()) break;
        tmpp = pos;
        int slash_state = 0;
        for(;pos < s.size();++pos){
            if(s[pos] == '\\')
                slash_state = !slash_state;
            else if(pos > tmpp && s[pos] == '"'){
                if(!slash_state) break;
                else slash_state = 0;
            }
        }
        ++pos;
        ret += s.substr(tmpp,pos - tmpp);
    }
    return ret;
}


string clean_str(string s){
    s = myreplace(s,"\\\\","\\");
    s = myreplace(s,"\\\"","\"");
    return s;
}