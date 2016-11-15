#include <google/protobuf/compiler/command_line_interface.h>
#include <protoc-c/c_generator.h>

#define PACKAGE_STRING "1.2.1"

int main(int argc, char* argv[]) {
  google::protobuf::compiler::CommandLineInterface cli;

  // Support generation of Foo code.
  google::protobuf::compiler::c::CGenerator c_generator;
  cli.RegisterGenerator("--c_out", &c_generator,
  "Generate C/H files.");

  // Add version info generated by automake
  cli.SetVersionInfo(PACKAGE_STRING);
  
  return cli.Run(argc, argv);
}