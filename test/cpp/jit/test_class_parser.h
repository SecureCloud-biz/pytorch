#pragma once

#include <test/cpp/jit/test_base.h>
#include <torch/csrc/jit/script/parser.h>

namespace torch {
namespace jit {
namespace script {
const auto testSource = R"JIT(
  class FooTest:
    def __init__(self, x):
      self.x = x

    def get_x(self):
      return self.x

    an_attribute : Tensor
)JIT";

void testClassParser() {
  Parser p(std::make_shared<Source>(testSource));
  std::vector<Def> definitions;
  std::vector<Resolver> resolvers;

  const auto classDef = ClassDef(p.parseClassLike());
  p.lexer().expect(TK_EOF);

  ASSERT_EQ(classDef.name().name(), "FooTest");
  ASSERT_EQ(classDef.body().size(), 3);
  ASSERT_EQ(Def(classDef.body()[0]).name().name(), "__init__");
  ASSERT_EQ(Def(classDef.body()[1]).name().name(), "get_x");
  ASSERT_EQ(Var(Assign(classDef.body()[2]).lhs()).name().name(), "an_attribute");
  ASSERT_FALSE(Assign(classDef.body()[2]).rhs().present());
  ASSERT_FALSE(Assign(classDef.body()[2]).rhs().present());
}
} // namespace script
} // namespace jit
} // namespace torch
