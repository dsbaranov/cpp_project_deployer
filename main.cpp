#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
using filesystem::path;

path operator""_p(const char *data, std::size_t sz) {
  return path(data, data + sz);
}

int main(int agrc, char **agrv) {
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
          main_file << "//#include <iostream>\n"
                    << "//#include \"log_duration.h\"\n"
                    << "//#include <vector>\n"
                    << "//#include <set>\n"
                    << "//#include <cassert>\n"
                    << "//#include <filesystem>\n"
                    << "using namespace std;\n\n"
                    << "int main() {" << "\n"
                    << "}" << endl;
          main_file.close();
        }
      }
      {
        ofstream cmakelists_file(project_root_dir / path("CMakeLists.txt"));
        if (cmakelists_file.is_open()) {
          cmakelists_file << "cmake_minimum_required(VERSION 3.0.0)\n"
                          << "project(" << project_name
                          << " VERSION 0.1.0 LANGUAGES C CXX)\n"
                          << "file(GLOB SOURCES *.h *.cpp)\n"
                          << "add_executable(" << project_name
                          << " main.cpp ${SOURCES})\n"
                          << "target_compile_options(" << project_name
                          << " PRIVATE -Wall -Wextra -Wpedantic -Werror)\n";
          cmakelists_file.close();
        }
      }
      {
        ofstream clang_tidy_file(project_root_dir / path(".clang-tidy"));
        clang_tidy_file << "---\n"
                        << "Checks : "
                           "'*,-llvm-*,-llvmlibc-*,-google-build-using-"
                           "namespace,-modernize-use-trailing-return-type'\n";
        clang_tidy_file.close();
      }
      {
        ofstream log_duration_file(project_root_dir / path("log_duration.h"));
        if (log_duration_file.is_open()) {
          log_duration_file
              << "#pragma once\n"
              << "#include <chrono>\n"
              << "#include <iostream>\n"
              << "#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y\n"
              << "#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)\n"
              << "#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, "
                 "__LINE__)\n"
              << "#define LOG_DURATION(x) LogDuration "
                 "UNIQUE_VAR_NAME_PROFILE(x)\n"
              << "class LogDuration\n"
              << "{\n"
              << "public:\n"
              << "using Clock = std::chrono::steady_clock;\n"
              << "explicit LogDuration(const std::string &id)\n"
              << ": id_(id)\n"
              << "{}\n"
              << "~LogDuration()\n"
              << "{using namespace std::chrono; using namespace "
                 "std::literals;const auto end_time = Clock::now();const auto "
                 "dur = end_time - start_time_;std::cerr << id_ << \" : \"s << "
                 "duration_cast<milliseconds>(dur).count() << \" ms\"s << "
                 "std::endl;}\n"
              << "private:\n"
              << "const std::string id_;\n"
              << "const Clock::time_point start_time_ = Clock::now();\n"
              << "};\n";
          log_duration_file.close();
        };
      }
      {
        ofstream gitignore(project_root_dir / path(".gitignore"));
        if (gitignore.is_open()) {
          gitignore << "build/*\n"
                    << ".vscode\n";
          gitignore.close();
        }
      }
    }
  }
  return 0;
}

// TODO : проверить существование
/**
 * if (!filesystem::exists(p)) {
 *    return false;
 * }
 */