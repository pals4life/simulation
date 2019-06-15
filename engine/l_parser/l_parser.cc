/*
 * l_parser.cc
 * Copyright (C) 2011  Daniel van den Akker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "l_parser.h"

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <random>
#include <sstream>

namespace {
	//helper class to enable / certain exceptions
	//and automatically disable them at the end of the scope
	class enable_exceptions {
	private:
		std::ios &ios;
		std::ios::iostate state;
	public:
		enable_exceptions(std::ios &an_ios, std::ios::iostate exceptions) :
				ios(an_ios) {
			state = ios.exceptions();
			ios.exceptions(exceptions);
		}

		~enable_exceptions() {
			ios.exceptions(state);
		}
	};

	class stream_parser {
	private:
		std::istream &in;
		enable_exceptions ex;
		unsigned int line;
		unsigned int col;
		bool endOfLineRead;
	public:
		stream_parser(std::istream &stream) :
				in(stream), ex(stream, std::istream::failbit | std::istream::badbit), line(1), col(0),
				endOfLineRead(false) {
			in.clear();
		}

		virtual ~stream_parser() {
		}

		std::istream::int_type getChar() {
			std::istream::int_type c = in.get();
			if (endOfLineRead) {
				line++;
				col = 0;
				endOfLineRead = false;
			} else
				col++;
			//goto next line when next char is read
			if (c == '\n') {
				endOfLineRead = true;
			}

			return c;
		}

		std::istream::int_type peekChar() {
			return in.peek();
		}

		void skip_comments_and_whitespace() {
			if (in.eof())
				return;
			for (; !in.eof();) {
				while ((!in.eof()) && isspace(peekChar()))
					getChar();
				if (in.eof())
					break;
				if (peekChar() == '#') {
					while ((!in.eof()) && peekChar() != '\n')
						getChar();
				} else
					break;
			}
		}

		void assertChars(std::string const &chars) {
			for (char const *i = chars.c_str(); *i != 0x0; i++) {
				if (getChar() != *i)
					throw LParser::ParserException(std::string("Did not find expected string\"") + chars + "\"", line,
												   col);
			}
		}

		std::string readQuotedString() {
			if (((char) getChar()) != '"')
				throw LParser::ParserException("Did not find expected string char:'\"'", line, col);
			std::string value("");
			for (std::istream::int_type c = getChar(); c != '"'; c = getChar())
				value += c;
			return value;
		}

		int readInt() {
			std::istream::int_type c = getChar();
			//int sign = +1;
			if (c == '-') {
				//sign = -1;
				c = getChar();
			}
			if (!std::isdigit(c))
				throw LParser::ParserException("Did not find expected digit when expected", line, col);
			int value = 0;
			while (std::isdigit(c) && (!in.eof())) {
				value = value * 10 + c - '0';
				try {
					c = peekChar();
				} catch (std::ios::failure &f) {
					if (in.eof())
						break;
					else
						throw f;
				}

				if (std::isdigit(c))
					getChar();
			}
			return value;
		}

		double readDouble() {
			int whole = readInt();
			if (peekChar() != '.') {
				return whole;
			}
			getChar();
			std::istream::int_type c = getChar();
			if (!std::isdigit(c))
				throw LParser::ParserException("Did not find expected digit when expected", line, col);
			int part = 0;
			int denom = 1;
			while (std::isdigit(c) && (!in.eof())) {
				part = part * 10 + c - '0';
				denom *= 10;
				try {
					c = peekChar();
				} catch (std::ios::failure &f) {
					if (in.eof())
						break;
					else
						throw f;
				}
				if (std::isdigit(c))
					getChar();
			}
			return (double) whole + ((double) part / (double) denom);
		}

		int getLine() {
			return line;
		}

		int getCol() {
			return col;
		}

	};

	void print_system(std::ostream &out, LParser::LSystem const &system) {

		if (system.get_alphabet().size() == 0) {
			out << "Alphabet = {}" << std::endl;
			out << "Draw = {}" << std::endl;
			out << "Rules = {}" << std::endl;
		} else if (system.get_alphabet().size() == 1) {
			//case with 1 element is special
			out << "Alphabet = {" << *system.get_alphabet().begin() << "}" << std::endl;
			out << "Draw = {" << *system.get_alphabet().begin() << " -> "
				<< (system.draw(*system.get_alphabet().begin()) ? 1 : 0) << "}" << std::endl;
			out << "Rules = {" << *system.get_alphabet().begin() << " -> \""
				<< system.get_replacement(*system.get_alphabet().begin()) << "\"}" << std::endl;
		} else {
			out << "Alphabet = {" << *system.get_alphabet().begin();
			for (std::set<char>::const_iterator i = ++system.get_alphabet().begin();
				 i != system.get_alphabet().end(); i++) {
				out << ", " << *i;
			}
			out << "}" << std::endl;

			out << "Draw = {" << *system.get_alphabet().begin() << "-> "
				<< (system.draw(*system.get_alphabet().begin()) ? 1 : 0) << "";
			for (std::set<char>::const_iterator i = ++system.get_alphabet().begin();
				 i != system.get_alphabet().end(); i++) {
				out << "," << std::endl << "\t" << *i << " -> " << (system.draw(*i) ? 1 : 0);
			}
			out << std::endl << "}" << std::endl;

			out << "Rules = {" << *system.get_alphabet().begin() << "-> \""
				<< system.get_replacement(*system.get_alphabet().begin()) << "\"";
			for (std::set<char>::const_iterator i = ++system.get_alphabet().begin();
				 i != system.get_alphabet().end(); i++) {
				out << "," << std::endl << "\t" << *i << " -> \"" << system.get_replacement(*i) << "\"";
			}
			out << std::endl << "}" << std::endl;
		}
		out << "Initiator = \"" << system.get_initiator() << "\"" << std::endl;
		out << "Angle = " << system.get_angle() << std::endl;
	}

	void parse_alphabet(std::set<char> &alphabet, stream_parser &parser) {
		parser.skip_comments_and_whitespace();
		parser.assertChars("Alphabet");
		parser.skip_comments_and_whitespace();
		parser.assertChars("=");
		parser.skip_comments_and_whitespace();
		parser.assertChars("{");
		alphabet.clear();
		char c = parser.getChar();
		while (true) {
			if (!std::isalpha(c))
				throw LParser::ParserException("Invalid Alphabet character", parser.getLine(), parser.getCol());
			if (alphabet.find(c) != alphabet.end())
				throw LParser::ParserException(std::string("Double entry '") + c + "' in alphabet specification",
											   parser.getLine(), parser.getCol());
			alphabet.insert(c);
			parser.skip_comments_and_whitespace();
			c = parser.getChar();
			if (c == '}')
				break;
			else if (c != ',')
				throw LParser::ParserException("Expected ','", parser.getLine(), parser.getCol());

			parser.skip_comments_and_whitespace();
			c = parser.getChar();
		}
	}

	void parse_draw(std::set<char> const &alphabet, std::map<char, bool> &draw, stream_parser &parser) {
		parser.skip_comments_and_whitespace();
		parser.assertChars("Draw");
		parser.skip_comments_and_whitespace();
		parser.assertChars("=");
		parser.skip_comments_and_whitespace();
		parser.assertChars("{");
		parser.skip_comments_and_whitespace();
		draw.clear();
		char c = parser.getChar();
		while (true) {
			if (!std::isalpha(c))
				throw LParser::ParserException("Invalid Alphabet character", parser.getLine(), parser.getCol());
			if (alphabet.find(c) == alphabet.end())
				throw LParser::ParserException("Draw function specified for char which is not part of the alphabet.",
											   parser.getLine(), parser.getCol());
			if (draw.find(c) != draw.end())
				throw LParser::ParserException(std::string("Double entry '") + c + "' in draw specification",
											   parser.getLine(), parser.getCol());
			char alphabet_char = c;
			parser.skip_comments_and_whitespace();
			parser.assertChars("->");
			parser.skip_comments_and_whitespace();
			int value = parser.readInt();
			if (value != 0 && value != 1)
				throw LParser::ParserException(std::string("Invalid draw specification for entry '") + alphabet_char +
											   "' in draw specification", parser.getLine(), parser.getCol());
			draw[alphabet_char] = (bool) value;
			parser.skip_comments_and_whitespace();
			c = parser.getChar();
			if (c == '}')
				break;
			else if (c != ',')
				throw LParser::ParserException("Expected ','", parser.getLine(), parser.getCol());
			parser.skip_comments_and_whitespace();
			c = parser.getChar();
		}
	}

	bool isValidRule(std::set<char> const &alphabet, std::string const &rule, bool parse2D) {
		int num_parenthesis = 0;
		for (std::string::const_iterator i = rule.begin(); i != rule.end(); i++) {
			switch (*i) {
				case '+':
				case '-':
					break;
				case '^':
				case '&':
				case '\\':
				case '/':
				case '|':
					//only valid if parsing a 3D system
					if (parse2D)
						return false;
					break;
				case '(':
					num_parenthesis++;
					break;
				case ')':
					if (num_parenthesis <= 0)
						return false;
					num_parenthesis--;
					break;
				default:
					if (alphabet.find(*i) == alphabet.end()) {
						if (std::isdigit(*i) or *i == '.' or *i == ';' or *i == ' ') {
							break;
						}
						return false;
					}
			}
		}
		return num_parenthesis == 0;
	}

	void parse_rules(std::set<char> const &alphabet, std::map<char, std::string> &rules, stream_parser &parser,
					 bool parse2D) {
		parser.skip_comments_and_whitespace();
		parser.assertChars("Rules");
		parser.skip_comments_and_whitespace();
		parser.assertChars("=");
		parser.skip_comments_and_whitespace();
		parser.assertChars("{");
		parser.skip_comments_and_whitespace();
		rules.clear();
		char c = parser.getChar();
		while (true) {
			if (!std::isalpha(c))
				throw LParser::ParserException("Invalid Alphabet character", parser.getLine(), parser.getCol());
			if (alphabet.find(c) == alphabet.end())
				throw LParser::ParserException(std::string("Replacement rule specified for char '") + c +
											   "' which is not part of the alphabet. ", parser.getLine(),
											   parser.getCol());
			if (rules.find(c) != rules.end())
				throw LParser::ParserException(std::string("Double entry '") + c + "' in rules specification ",
											   parser.getLine(), parser.getCol());
			char alphabet_char = c;
			parser.skip_comments_and_whitespace();
			parser.assertChars("->");
			parser.skip_comments_and_whitespace();
			std::string rule = parser.readQuotedString();
			if (!isValidRule(alphabet, rule, parse2D))
				throw LParser::ParserException(std::string("Invalid rule specification for entry '") + alphabet_char +
											   "' in rule specification", parser.getLine(), parser.getCol());
			rules[alphabet_char] = rule;
			parser.skip_comments_and_whitespace();
			c = parser.getChar();
			if (c == '}')
				break;
			else if (c != ',')
				throw LParser::ParserException("Expected ','", parser.getLine(), parser.getCol());
			parser.skip_comments_and_whitespace();
			c = parser.getChar();
		}
	}

	std::string parse_initiator(std::set<char> const &alphabet, stream_parser &parser, bool parse2D) {
		std::string initiator;
		parser.skip_comments_and_whitespace();
		parser.assertChars("Initiator");
		parser.skip_comments_and_whitespace();
		parser.assertChars("=");
		parser.skip_comments_and_whitespace();
		initiator = parser.readQuotedString();
		if (!isValidRule(alphabet, initiator, parse2D))
			throw LParser::ParserException("Invalid initiator rule", parser.getLine(), parser.getCol());
		return initiator;
	}

	double parse_angle(stream_parser &parser, std::string expectedString) {
		double angle;
		parser.skip_comments_and_whitespace();
		parser.assertChars(expectedString);
		parser.skip_comments_and_whitespace();
		parser.assertChars("=");
		parser.skip_comments_and_whitespace();
		angle = parser.readDouble();
		return angle;
	}

	int parse_iterations(stream_parser &parser) {
		int nrIterations;
		parser.skip_comments_and_whitespace();
		parser.assertChars("Iterations");
		parser.skip_comments_and_whitespace();
		parser.assertChars("=");
		parser.skip_comments_and_whitespace();
		nrIterations = parser.readInt();
		if (nrIterations < 0)
			throw LParser::ParserException("Number of iterations should not be negative", parser.getLine(),
										   parser.getCol());
		return nrIterations;
	}
}

LParser::ParserException::ParserException(std::string const &msg, unsigned int line, unsigned int pos) :
		std::exception(), message() {
	std::stringstream s;
	s << "Parser error at line: " << line << " pos: " << pos << " " << msg;
	message = s.str();
}

LParser::ParserException::ParserException(const LParser::ParserException &original)
		: std::exception(original), message(original.message) {
}

LParser::ParserException::~ParserException() throw() {
}

LParser::ParserException &LParser::ParserException::operator=(const LParser::ParserException &original) {
	message = original.message;
	return *this;
}

const char *LParser::ParserException::what() const throw() {
	return message.c_str();
}

LParser::LSystem::LSystem() :
		alphabet(), drawfunction(), initiator(""), angle(0.0), replacementrules(), nrIterations(0) {
}

LParser::LSystem::LSystem(LSystem const &system) :
		alphabet(system.alphabet), drawfunction(system.drawfunction), initiator(system.initiator), angle(system.angle),
		replacementrules(system.replacementrules), nrIterations(system.nrIterations) {
}

LParser::LSystem::~LSystem() {
}

LParser::LSystem &LParser::LSystem::operator=(LParser::LSystem const &system) {
	alphabet.clear();
	alphabet.insert(system.alphabet.begin(), system.alphabet.end());
	drawfunction.clear();
	drawfunction.insert(system.drawfunction.begin(), system.drawfunction.end());
	replacementrules.clear();
	replacementrules.insert(system.replacementrules.begin(), system.replacementrules.end());
	angle = system.angle;
	nrIterations = system.nrIterations;
	return *this;
}

std::set<char> const &LParser::LSystem::get_alphabet() const {
	return alphabet;
}

bool LParser::LSystem::draw(char c) const {
	assert(get_alphabet().find(c) != get_alphabet().end());
	return drawfunction.find(c)->second;
}

const std::string LParser::LSystem::get_replacement(char c) const { //stochastische l_systemen
	assert(get_alphabet().find(c) != get_alphabet().end());
	std::string in = replacementrules.find(c)->second;
	if (std::isdigit(in[0])) {
		std::stringstream ss(in);
		std::string line;
		std::map<double, std::string> rules;
		double sum = 0;
		while (std::getline(ss, line, ';')) {
			const auto p = line.find(' ');
			sum += 100 * std::stod(line.substr(0, p));
			rules[sum] = line.substr(p + 1, line.size() - 1);
		}
		assert(sum == 100);
		std::uniform_real_distribution<double> dist(0, 100);
		std::default_random_engine re(std::random_device{}());
		double result = dist(re);
		for (const auto &rule: rules) {
			if (result <= rule.first) {
				return rule.second;
			}
		}
	}
	return in;
}

double LParser::LSystem::get_angle() const {
	return angle;
}

std::string const &LParser::LSystem::get_initiator() const {
	return initiator;
}

unsigned int LParser::LSystem::get_nr_iterations() const {
	return nrIterations;
}

LParser::LSystem2D::LSystem2D() :
		LSystem(), startingAngle(0.0) {
}

LParser::LSystem2D::LSystem2D(LSystem2D const &system) :
		LSystem(system), startingAngle(system.startingAngle) {
}

LParser::LSystem2D::LSystem2D(std::istream &in) :
		LSystem(), startingAngle(0.0) {
	in >> *this;
}

LParser::LSystem2D::~LSystem2D() {
}

LParser::LSystem2D &LParser::LSystem2D::operator=(LParser::LSystem2D const &system) {
	LParser::LSystem::operator=(system);
	startingAngle = system.startingAngle;
	return *this;
}

double LParser::LSystem2D::get_starting_angle() const {
	return startingAngle;
}

std::ostream &LParser::operator<<(std::ostream &out, LParser::LSystem2D const &system) {
	print_system(out, system);
	out << "StartingAngle = " << system.get_starting_angle() << std::endl;
	out << "Iterations = " << system.get_nr_iterations() << std::endl;
	return out;
}

std::istream &LParser::operator>>(std::istream &in, LParser::LSystem2D &system) {
	stream_parser parser(in);
	parse_alphabet(system.alphabet, parser);
	parse_draw(system.alphabet, system.drawfunction, parser);
	parse_rules(system.alphabet, system.replacementrules, parser, true);
	system.initiator = parse_initiator(system.alphabet, parser, true);
	system.angle = parse_angle(parser, "Angle");
	system.startingAngle = parse_angle(parser, "StartingAngle");
	system.nrIterations = parse_iterations(parser);

	return in;
}

std::ostream &LParser::operator<<(std::ostream &out, LParser::LSystem3D const &system) {
	print_system(out, system);
	out << "Iterations = " << system.get_nr_iterations() << std::endl;
	return out;
}

std::istream &LParser::operator>>(std::istream &in, LParser::LSystem3D &system) {
	stream_parser parser(in);
	parse_alphabet(system.alphabet, parser);
	parse_draw(system.alphabet, system.drawfunction, parser);
	parse_rules(system.alphabet, system.replacementrules, parser, false);
	system.initiator = parse_initiator(system.alphabet, parser, false);
	system.angle = parse_angle(parser, "Angle");

	system.nrIterations = parse_iterations(parser);

	return in;
}

LParser::LSystem3D::LSystem3D() :
		LSystem() {
}

LParser::LSystem3D::LSystem3D(LParser::LSystem3D const &system) :
		LSystem(system) {
}

LParser::LSystem3D::LSystem3D(std::istream &in) :
		LSystem() {
	in >> *this;
}

LParser::LSystem3D::~LSystem3D() {
}

LParser::LSystem3D &LParser::LSystem3D::operator=(LParser::LSystem3D const &system) {
	LParser::LSystem::operator=(system);
	return *this;
}
