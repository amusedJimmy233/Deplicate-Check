#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <cstdio>

using namespace std;

//从文件读入到string里
string read_file_to_string(const char *filename) {
    ifstream ifile(filename);
    //将文件读入到ostringstream对象buf中
    ostringstream buf;
    char ch;
    while (buf && ifile.get(ch)) {
        buf.put(ch);
    }

    //返回与流对象buf关联的字符串
    return buf.str();
}

//删除标点符号
string delete_signal_sign(string target, const string& sign){
    size_t start = target.find(sign);
    while(start!=std::string::npos){
        //target.replace(start,1," ");
        target.erase(start,1);
        start = target.find(sign);
    }
    return target;
}

string delete_comments(string target)
{
    //单行注释
    size_t start = target.find("//");
    while (start != std::string::npos) {
        int cnt = 0;
        size_t i = start;
        while (target[i] != '\n') {
            i++;
            cnt++;
        }
        target.erase(start, cnt);
        start = target.find("//");
    }

    //多行注释
    start = target.find("/*");
    while (start != std::string::npos) {
        size_t end = target.find("*/");
        target.erase(start, end - start + 2);
        start = target.find("/*");
    }

    return target;
}

string delete_elements(string file1) {
    //删除注释
    file1 = delete_comments(file1);

    //删除单个标点符号
    //file1=delete_signal_sign(file1,"(");
    //file1=delete_signal_sign(file1,")");
    file1=delete_signal_sign(file1,"{");
    file1=delete_signal_sign(file1,"}");
    //file1=delete_signal_sign(file1,";");
    file1=delete_signal_sign(file1,",");

    return file1;
}

string modify_file(string original_file,vector<string> &file2) {
    //去除注释
    string file1 = delete_elements(std::move(original_file));

    istringstream iss(file1);
    string modified_file;
    string word;
    while (iss >> word) {
        modified_file += word;
        //modified_file += " ";
        file2.push_back(word);
    }
    return modified_file;
}

int calculate_duplicate_count(const vector<string>& word_list, const string& modified_file,int n)
{
    string target;
    queue<string>qu;
    int duplicate_count = 0;
    cout<<"重复内容："<<endl;
    for (const auto& word:word_list){
        target+=word;
        qu.push(word);
        if(qu.size()==n){
            int flag = 0;
            // 寻找
            if(modified_file.find(target)!=std::string::npos) {
                flag = 1;
                duplicate_count += (int)qu.size();
            }
            //输出
            while(!qu.empty()){
                if (flag) cout<<qu.front()<<" ";
                qu.pop();
            }
            if(flag) cout<<endl;
            target="";
        }
    }
    return duplicate_count;
}

int main() {
    const char *f1 = "../ori.txt";
    const char *f2 = "../own.txt";
    string original_file1 = read_file_to_string(f1);
    string original_file2 = read_file_to_string(f2);

    vector<string>file1_list;
    vector<string>file2_list;

    string modified_file1 = modify_file(original_file1,file1_list);
    string modified_file2 = modify_file(original_file2,file2_list);

    cout<<"请输入阈值"<<endl;
    int n;
    cin>>n;

    int duplicate_count = calculate_duplicate_count(file1_list,modified_file2,n);

    double duplicate_rate = (double)duplicate_count/(double)file1_list.size();
    cout<<"总数量："<<file1_list.size()<<" 重复数量："<<duplicate_count<<endl;
    printf("重复率：%.2f% \n",duplicate_rate*100);

    return 0;
}