#include "shell/lexer/token.hpp"
#include "shell/logger.hpp"

namespace shell {

Token::Token(Token::Type type, Variant &&variant) : type(type), variant(std::move(variant)) {
}

void Token::print() const {
	LOG_DEBUG("%\n", toString());
}

static const map<Token::Type, string> token_type_strings = {{Token::Type::Word, "Word"},
                                                            {Token::Type::IoNumber, "IoNumber"},
                                                            {Token::Type::Newline, "Newline"},
                                                            {Token::Type::Semicolon, "Semicolon"},
                                                            {Token::Type::And, "And"},
                                                            {Token::Type::Pipe, "Pipe"},
                                                            {Token::Type::PipeAnd, "PipeAnd"},
                                                            {Token::Type::ParenthesisOpen, "ParenthesisOpen"},
                                                            {Token::Type::ParenthesisClose, "ParenthesisClose"},
                                                            {Token::Type::AndIf, "AndIf"},
                                                            {Token::Type::OrIf, "OrIf"},
                                                            {Token::Type::DoubleSemicolon, "DoubleSemicolon"},
                                                            {Token::Type::Less, "Less"},
                                                            {Token::Type::DoubleLess, "DoubleLess"},
                                                            {Token::Type::Great, "Great"},
                                                            {Token::Type::DoubleGreat, "DoubleGreat"},
                                                            {Token::Type::LessAnd, "LessAnd"},
                                                            {Token::Type::GreatAnd, "GreatAnd"},
                                                            {Token::Type::LessGreat, "LessGreat"},
                                                            {Token::Type::DoubleLessDash, "DoubleLessDash"},
                                                            {Token::Type::Clobber, "Clobber"}};

string Token::toString() const {
	const string &type_string = token_type_strings.at(type);
	switch (type) {
		case Token::Type::Word:
			return type_string + "(" + get<WordToken>().toString() + ")";
		/* operators */
		case Token::Type::Semicolon:
		case Token::Type::And:
		case Token::Type::Pipe:
		case Token::Type::PipeAnd:
		case Token::Type::ParenthesisOpen:
		case Token::Type::ParenthesisClose:
		case Token::Type::AndIf:
		case Token::Type::OrIf:
		case Token::Type::DoubleSemicolon:
		case Token::Type::Less:
		case Token::Type::Great:
		case Token::Type::DoubleLess:
		case Token::Type::DoubleGreat:
		case Token::Type::LessAnd:
		case Token::Type::GreatAnd:
		case Token::Type::LessGreat:
		case Token::Type::DoubleLessDash:
		case Token::Type::Clobber:
			return type_string + "(" + getOperatorString(type) + ")";
		case Token::Type::Newline:
			return type_string;
		case Token::Type::IoNumber:
			return type_string + "(" + get<IoNumber>().toString();
		default:
			throw std::runtime_error("token not supported");
	}
}

Token::Type Token::getType() const {
	return type;
}

string WordToken::toString() const {
	return value;
}

string IoNumber::toString() const {
	return value;
}
const vector<pair<string, Token::Type>> Token::operator_types = {{";", Type::Semicolon},
                                                                 {"&", Type::And},
                                                                 {"|", Type::Pipe},
                                                                 {"|&", Type::PipeAnd},
                                                                 {"(", Type::ParenthesisOpen},
                                                                 {")", Type::ParenthesisClose},
                                                                 {"&&", Type::AndIf},
                                                                 {"||", Type::OrIf},
                                                                 {";;", Type::DoubleSemicolon},
                                                                 {"<", Type::Less},
                                                                 {"<<", Type::DoubleLess},
                                                                 {">", Type::Great},
                                                                 {">>", Type::DoubleGreat},
                                                                 {"<&", Type::LessAnd},
                                                                 {">&", Type::GreatAnd},
                                                                 {"<>", Type::LessGreat},
                                                                 {"<<-", Type::DoubleLessDash},
                                                                 {">|", Type::Clobber}};

int Token::prefixOperatorMatches(const string &s) {
	int count = 0;
	for (auto &p : operator_types) {
		if (p.first.rfind(s, 0) == 0) {
			count++;
		}
	}
	return count;
}

optional<Token::Type> Token::exactOperatorType(const string &s) {
	for (auto &p : operator_types) {
		if (p.first == s) {
			return p.second;
		}
	}
	return nullopt;
}

const string &Token::getOperatorString(Token::Type type) {
	for (auto &p : operator_types) {
		if (p.second == type) {
			return p.first;
		}
	}
	throw std::runtime_error("operator token type not recognized");
}

} // namespace shell
