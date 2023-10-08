#include "shell/grammar/rule.hpp"

namespace shell {

Rule::Option::Option(Rule::NonTerminal &&opt) : type(Rule::Option::Type::NonTerminal), option(std::move(opt)) {
}
Rule::Option::Option(Rule::Terminal &&opt) : type(Rule::Option::Type::Terminal), option(std::move(opt)) {
}

} // namespace shell
