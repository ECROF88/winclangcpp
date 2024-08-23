
#include <exception>
#include <iostream>
#include <string>

// std::string indent;
// std::string code;
struct IdentGuard {
  explicit IdentGuard(std::string &indent_) : indent(indent_)
  {
    old_indent = indent;
    indent += "  ";
  }
  IdentGuard(IdentGuard &&) = delete;
  ~IdentGuard()
  {
    std::cout << "析构函数" << '\n';
    indent = old_indent;
  }
  std::string &indent;
  std::string old_indent;
};
struct Codegen {
  std::string code;
  std::string indent;
  void emit_variable(const std::string &name)
  {
    ;
    code += indent + "int " + name + ";\n";
  }
  void emit(const std::string &name) { code += indent + name + "\n"; }
  void gen()
  {
    emit("int main(){");
    {
      IdentGuard guard(indent);
      emit_variable("i");
      //   throw std::runtime_error("something wrong");
      emit_variable("j");
    }
    emit("}");
    emit_variable("g");
  }
};

int main()
{
  Codegen cg;
  try {
    cg.gen();
  } catch (std::exception const &e) {
  }
  std::cout << cg.code << std::endl;
}