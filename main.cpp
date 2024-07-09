#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
  return path(data, data + sz);
}

int main(int agrc, char** agrv) {
  string project_name;
  if (agrc == 1) {
    cout << "please, enter the project name"s << endl;
    cin >> project_name;
  } else if (agrc == 2) {
    project_name = static_cast<string>(agrv[1]);
  }
  cout << "project name : " << project_name << endl;

  {
    path project_root_dir = path(project_name);
    error_code err;
    filesystem::remove_all(project_root_dir, err);
    filesystem::create_directory(project_root_dir);
    if (!err) {
      {
        ofstream main_file(project_root_dir / path("main.cpp"));
        if (main_file.is_open()) {
          main_file << "#include <iostream>\n"
                    << "//#include <vector>\n"
                    << "//#include <set>\n"
                    << "//#include <cassert>\n"
                    << "//#include <filesystem>\n"
                    << "using namespace std;\n\n"
                    << "int main(int agrc, char** agrv) {" << "\n"
                    << "}" << endl;
          main_file.close();
        }
        {
          ofstream cmakelists_file(project_root_dir / path("CMakeLists.txt"));
          if (cmakelists_file.is_open()) {
            cmakelists_file << "cmake_minimum_required(VERSION 3.0.0)\n"
                            << "project(" << project_name
                            << " VERSION 0.1.0 LANGUAGES C CXX)\n"
                            << "add_executable(" << project_name
                            << " main.cpp)\n";
            cmakelists_file.close();
          }
        }
      }
    }
  }
}

// TODO : проверить существование
/**
 * if (!filesystem::exists(p)) {
 *    return false;
 * }
 */