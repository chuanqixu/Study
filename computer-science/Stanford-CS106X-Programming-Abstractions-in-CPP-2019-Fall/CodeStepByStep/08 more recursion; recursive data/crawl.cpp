// Notice that this cannot be run on CodeStepByStep
// Because it does not support `filesystem`

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

void crawl(const string &str, const string &indentation = "") {
    auto path = filesystem::absolute(filesystem::path(str));
    string path_str = path.generic_string();
    if (filesystem::is_directory(path)) {
        string dir_name;
        string::size_type dir_name_begin_pos;
        if (path_str[path_str.length() - 1] == '/') {
            dir_name_begin_pos = path_str.rfind('/', path_str.length() - 2) + 1;
            dir_name = path_str.substr(dir_name_begin_pos, path_str.length() - dir_name_begin_pos - 1);
        } else {
            dir_name_begin_pos = path_str.rfind('/') + 1;
            dir_name = path_str.substr(dir_name_begin_pos, path_str.length() - dir_name_begin_pos);
        }
        cout << indentation << dir_name << endl;
        for (const auto &item : filesystem::directory_iterator(path)) {
            crawl(item.path().generic_string(), indentation + "    ");
        }
    } else if (filesystem::is_regular_file(path)) {
        cout << indentation << path_str.substr(path_str.rfind('/') + 1) << endl;
    }
}

int main() {
    crawl("../");

    return 0;
}
