/*
 * ini_configuration.cc
 * Copyright (C) 2011  Bart Sas
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

#include "ini_configuration.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <exception>
#include <functional>
#include <iomanip>
#include <memory>
#include <sstream>


namespace ini {
    ParseException::ParseException() throw()
            : std::exception() {
        // Does nothing...
    }

    ParseException::ParseException(const ParseException &original) throw()
            : std::exception(original) {
        // Does nothing...
    }

    ParseException::~ParseException() throw() {
        // Does nothing...
    }

    ParseException &ParseException::operator=(const ParseException &original) throw() {
        std::exception::operator=(original);
        return *this;
    }


    UnexpectedCharacter::UnexpectedCharacter(const std::istream::int_type character_init,
                                             const std::istream::pos_type position_init) throw()
            : ParseException(), character(character_init), position(position_init), message("") {
        std::ostringstream stream;

        if (character == std::istream::traits_type::eof()) {
            stream << "end-of-file";
        } else {
            stream << "character \'";

            if (std::isprint(character)) {
                stream << static_cast<char>(character);
            } else {
                stream << "\\"
                       << std::oct
                       << std::setfill('0')
                       << std::setw(3)
                       << static_cast<int>(character);
            }

            stream << "\'";
        }

        stream << " not expected at position " << position;
        message = stream.str();
    }

    UnexpectedCharacter::UnexpectedCharacter(const UnexpectedCharacter &original) throw()
            : ParseException(), character(original.character), position(original.position), message(original.message) {
        // Does nothing...
    }

    UnexpectedCharacter::~UnexpectedCharacter() throw() {
        // Does nothing...
    }

    UnexpectedCharacter &UnexpectedCharacter::operator=(const UnexpectedCharacter &original) throw() {
        std::exception::operator=(original);

        character = original.character;
        position = original.position;
        message = original.message;

        return *this;
    }

    const char *UnexpectedCharacter::what() const throw() {
        return message.c_str();
    }


    DuplicateSection::DuplicateSection(const std::string &name_init) throw()
            : ParseException(), name(name_init), message("") {
        message = "section named \'"
                  + name
                  + "\' encoutered twice in configuration file";
    }

    DuplicateSection::DuplicateSection(const DuplicateSection &original) throw()
            : ParseException(), name(original.name), message(original.message) {
        // Does nothing...
    }

    DuplicateSection::~DuplicateSection() throw() {
        // Does nothing...
    }

    DuplicateSection &DuplicateSection::operator=(const DuplicateSection &original) throw() {
        std::exception::operator=(original);

        name = original.name;
        message = original.message;

        return *this;
    }

    const char *DuplicateSection::what() const throw() {
        return message.c_str();
    }


    DuplicateEntry::DuplicateEntry(const std::string &section_init,
                                   const std::string &key_init) throw()
            : ParseException(), section(section_init), key(key_init), message("") {
        message = "entry named \'"
                  + key
                  + "\' ecountered twice in section \'"
                  + section
                  + "\'";
    }

    DuplicateEntry::DuplicateEntry(const DuplicateEntry &original) throw()
            : ParseException(), section(original.section), key(original.key), message(original.message) {
        // Does nothing...
    }

    DuplicateEntry::~DuplicateEntry() throw() {
        // Does nothing...
    }

    DuplicateEntry &DuplicateEntry::operator=(const DuplicateEntry &original) throw() {
        std::exception::operator=(original);

        section = original.section;
        key = original.key;
        message = original.message;

        return *this;
    }

    const char *DuplicateEntry::what() const throw() {
        return message.c_str();
    }


    NonexistentEntry::NonexistentEntry(const std::string &section_name_init,
                                       const std::string &entry_name_init) throw()
            : std::exception(), section_name(section_name_init), entry_name(entry_name_init), message("") {
        message = "entry with name \'"
                  + entry_name
                  + "\' does not exist in section \'"
                  + section_name
                  + "\'";
    }

    NonexistentEntry::NonexistentEntry(const NonexistentEntry &original) throw()
            : std::exception(original), message(original.message) {
        // Does nothing...
    }

    NonexistentEntry::~NonexistentEntry() throw() {
        // Does nothing...
    }

    NonexistentEntry &NonexistentEntry::operator=(const NonexistentEntry &original) throw() {
        std::exception::operator=(original);
        message = original.message;
        return *this;
    }

    const char *NonexistentEntry::what() const throw() {
        return message.c_str();
    }


    IncompatibleConversion::IncompatibleConversion(const std::string &section_name_init,
                                                   const std::string &entry_name_init,
                                                   const std::string &type_name_init) throw()
            : std::exception(), section_name(section_name_init), entry_name(entry_name_init), type_name(type_name_init),
              message("") {
        message = "cannot convert value of \'"
                  + entry_name
                  + "\' in section \'"
                  + section_name
                  + "\' to "
                  + type_name;
    }

    IncompatibleConversion::IncompatibleConversion(const IncompatibleConversion &original) throw()
            : std::exception(original), section_name(original.section_name), entry_name(original.entry_name),
              type_name(original.type_name), message(original.message) {
        // Does nothing...
    }

    IncompatibleConversion::~IncompatibleConversion() throw() {
        // Does nothing...
    }

    IncompatibleConversion &IncompatibleConversion::operator=(const IncompatibleConversion &original) throw() {
        std::exception::operator=(original);

        section_name = original.section_name;
        entry_name = original.entry_name;
        type_name = original.type_name;
        message = original.message;

        return *this;
    }

    const char *IncompatibleConversion::what() const throw() {
        return message.c_str();
    }


    class Value {
    protected:

        Value();

    public:

        virtual ~Value();

        virtual bool exists() const = 0;

        virtual bool as_int_if_exists(const std::string &section_name,
                                      const std::string &entry_name,
                                      int &ret_val) const = 0;

        virtual bool as_double_if_exists(const std::string &section_name,
                                         const std::string &entry_name,
                                         double &ret_val) const = 0;

        virtual bool as_string_if_exists(const std::string &section_name,
                                         const std::string &entry_name,
                                         std::string &ret_val) const = 0;

        virtual bool as_bool_if_exists(const std::string &section_name,
                                       const std::string &entry_name,
                                       bool &ret_val) const = 0;

        virtual bool as_int_tuple_if_exists(const std::string &section_name,
                                            const std::string &entry_name,
                                            IntTuple &ret_val) const = 0;

        virtual bool as_double_tuple_if_exists(const std::string &section_name,
                                               const std::string &entry_name,
                                               DoubleTuple &ret_val) const = 0;

        virtual void print(std::ostream &output_stream) const = 0;
    };

    Value::Value() {
        // Does nothing...
    }

    Value::~Value() {
        // Does nothing...
    }


    namespace {
        class ValueBase : public Value {
        protected:

            ValueBase();

        public:

            virtual ~ValueBase();

            virtual bool exists() const;

            virtual bool as_int_if_exists(const std::string &section_name,
                                          const std::string &entry_name,
                                          int &ret_val) const;

            virtual bool as_double_if_exists(const std::string &section_name,
                                             const std::string &entry_name,
                                             double &ret_val) const;

            virtual bool as_string_if_exists(const std::string &section_name,
                                             const std::string &entry_name,
                                             std::string &ret_val) const;

            virtual bool as_bool_if_exists(const std::string &section_name,
                                           const std::string &entry_name,
                                           bool &ret_val) const;

            virtual bool as_int_tuple_if_exists(const std::string &section_name,
                                                const std::string &entry_name,
                                                IntTuple &ret_val) const;

            virtual bool as_double_tuple_if_exists(const std::string &section_name,
                                                   const std::string &entry_name,
                                                   DoubleTuple &ret_val) const;
        };

        ValueBase::ValueBase() {
            // Does nothing...
        }

        ValueBase::~ValueBase() {
            // Does nothing...
        }

        bool ValueBase::exists() const {
            return true;
        }

        bool ValueBase::as_int_if_exists(const std::string &section_name,
                                         const std::string &entry_name,
                                         int &/*ret_val*/) const {
            throw IncompatibleConversion(section_name, entry_name, "int");
        }

        bool ValueBase::as_double_if_exists(const std::string &section_name,
                                            const std::string &entry_name,
                                            double &/*ret_val*/) const {
            throw IncompatibleConversion(section_name, entry_name, "double");
        }

        bool ValueBase::as_string_if_exists(const std::string &section_name,
                                            const std::string &entry_name,
                                            std::string &/*ret_val*/) const {
            throw IncompatibleConversion(section_name, entry_name, "string");
        }

        bool ValueBase::as_bool_if_exists(const std::string &section_name,
                                          const std::string &entry_name,
                                          bool &/*ret_val*/) const {
            throw IncompatibleConversion(section_name, entry_name, "bool");
        }

        bool ValueBase::as_int_tuple_if_exists(const std::string &section_name,
                                               const std::string &entry_name,
                                               IntTuple &/*ret_val*/) const {
            throw IncompatibleConversion(section_name, entry_name, "int tuple");
        }

        bool ValueBase::as_double_tuple_if_exists(const std::string &section_name,
                                                  const std::string &entry_name,
                                                  DoubleTuple &/*ret_val*/) const {
            throw IncompatibleConversion(section_name, entry_name, "double tuple");
        }


        class IntValue : public ValueBase {
        private:

            const int value;

        public:

            IntValue(const int value_init);

            virtual ~IntValue();

            virtual bool as_int_if_exists(const std::string &section_name,
                                          const std::string &entry_name,
                                          int &ret_val) const;

            virtual bool as_double_if_exists(const std::string &section_name,
                                             const std::string &entry_name,
                                             double &ret_val) const;

            virtual void print(std::ostream &output_stream) const;
        };

        IntValue::IntValue(const int value_init)
                : ValueBase(), value(value_init) {
            // Does nothing...
        }

        IntValue::~IntValue() {
            // Does nothing...
        }

        bool IntValue::as_int_if_exists(const std::string &/*section_name*/,
                                        const std::string &/*entry_name*/,
                                        int &ret_val) const {
            ret_val = value;
            return true;
        }

        bool IntValue::as_double_if_exists(const std::string &/*section_name*/,
                                           const std::string &/*entry_name*/,
                                           double &ret_val) const {
            ret_val = static_cast<double>(value);
            return true;
        }

        void IntValue::print(std::ostream &output_stream) const {
            output_stream << value;
        }


        class DoubleValue : public ValueBase {
        private:

            const double value;

        public:

            DoubleValue(const double value_init);

            virtual ~DoubleValue();

            virtual bool as_double_if_exists(const std::string &section_name,
                                             const std::string &entry_name,
                                             double &ret_val) const;

            virtual void print(std::ostream &output_stream) const;
        };

        DoubleValue::DoubleValue(const double value_init)
                : ValueBase(), value(value_init) {
            // Does nothing...
        }

        DoubleValue::~DoubleValue() {
            // Does nothing...
        }

        bool DoubleValue::as_double_if_exists(const std::string &/*section_name*/,
                                              const std::string &/*entry_name*/,
                                              double &ret_val) const {
            ret_val = value;
            return true;
        }

        void DoubleValue::print(std::ostream &output_stream) const {
            output_stream << value;
        }


        class StringValue : public ValueBase {
        private:

            const std::string value;

        public:

            StringValue(const std::string &value_init);

            virtual ~StringValue();

            virtual bool as_string_if_exists(const std::string &section_name,
                                             const std::string &entry_name,
                                             std::string &ret_val) const;

            virtual void print(std::ostream &output_stream) const;
        };

        StringValue::StringValue(const std::string &value_init)
                : ValueBase(), value(value_init) {
            // Does nothing...
        }

        StringValue::~StringValue() {
            // Does nothing...
        }

        bool StringValue::as_string_if_exists(const std::string &/*section_name*/,
                                              const std::string &/*entry_name*/,
                                              std::string &ret_val) const {
            ret_val = value;
            return true;
        }

        void StringValue::print(std::ostream &output_stream) const {
            const char quote = value.find('\'') == std::string::npos ? '\'' : '\"';
            output_stream << quote << value << quote;
        }


        class BoolValue : public ValueBase {
        private:

            const bool value;

        public:

            BoolValue(const bool value_init);

            virtual ~BoolValue();

            virtual bool as_bool_if_exists(const std::string &section_name,
                                           const std::string &entry_name,
                                           bool &ret_val) const;

            virtual void print(std::ostream &output_stream) const;
        };

        BoolValue::BoolValue(const bool value_init)
                : ValueBase(), value(value_init) {
            // Does nothing...
        }

        BoolValue::~BoolValue() {
            // Does nothing...
        }

        bool BoolValue::as_bool_if_exists(const std::string &/*section_name*/,
                                          const std::string &/*entry_name*/,
                                          bool &ret_val) const {
            ret_val = value;
            return true;
        }

        void BoolValue::print(std::ostream &output_stream) const {
            output_stream << (value ? "true" : "false");
        }


        class TupleValue : public ValueBase {
        private:

            typedef std::vector<Value *> ElementList;
            typedef ElementList::iterator ElementIter;
            typedef ElementList::const_iterator ConstElementIter;

            const ElementList elements;

            static void delete_element(Value *const element);

        public:

            TupleValue(const std::vector<Value *> &elements_init);

            virtual ~TupleValue();

            virtual bool as_int_tuple_if_exists(const std::string &section_name,
                                                const std::string &entry_name,
                                                IntTuple &ret_val) const;

            virtual bool as_double_tuple_if_exists(const std::string &section_name,
                                                   const std::string &entry_name,
                                                   DoubleTuple &ret_val) const;

            virtual void print(std::ostream &output_stream) const;
        };

        TupleValue::TupleValue(const std::vector<Value *> &elements_init)
                : ValueBase(), elements(elements_init) {
            // Does nothing...
        }

        TupleValue::~TupleValue() {
            std::for_each(elements.begin(), elements.end(), delete_element);
        }

        void TupleValue::delete_element(Value *const element) {
            delete element;
        }

        bool TupleValue::as_int_tuple_if_exists(const std::string &section_name,
                                                const std::string &entry_name,
                                                IntTuple &ret_val) const {
            ret_val.clear();
            ret_val.reserve(elements.size());

            const ConstElementIter first_element = elements.begin();
            const ConstElementIter last_element = elements.end();

            for (ConstElementIter i = first_element; i != last_element; ++i) {
                int element_value;
                const bool exists = (*i)->as_int_if_exists(section_name, entry_name, element_value);
                assert(exists);
                ret_val.push_back(element_value);
            }

            return true;
        }

        bool TupleValue::as_double_tuple_if_exists(const std::string &section_name,
                                                   const std::string &entry_name,
                                                   DoubleTuple &ret_val) const {
            ret_val.clear();
            ret_val.reserve(elements.size());

            const ConstElementIter first_element = elements.begin();
            const ConstElementIter last_element = elements.end();

            for (ConstElementIter i = first_element; i != last_element; ++i) {
                double element_value;
                const bool exists = (*i)->as_double_if_exists(section_name, entry_name, element_value);
                assert(exists);
                ret_val.push_back(element_value);
            }

            return true;
        }

        void TupleValue::print(std::ostream &output_stream) const {
            output_stream << "(";

            const ConstElementIter first_element = elements.begin();
            const ConstElementIter last_element = elements.end();

            for (ConstElementIter i = first_element; i != last_element; ++i) {
                if (i != first_element) {
                    output_stream << ", ";
                }

                (*i)->print(output_stream);
            }

            output_stream << ")";
        }


        class EmptyValue : public Value {
        public:

            EmptyValue();

            virtual ~EmptyValue();

            virtual bool exists() const;

            virtual bool as_int_if_exists(const std::string &section_name,
                                          const std::string &entry_name,
                                          int &ret_val) const;

            virtual bool as_double_if_exists(const std::string &section_name,
                                             const std::string &entry_name,
                                             double &ret_val) const;

            virtual bool as_string_if_exists(const std::string &section_name,
                                             const std::string &entry_name,
                                             std::string &ret_val) const;

            virtual bool as_bool_if_exists(const std::string &section_name,
                                           const std::string &entry_name,
                                           bool &ret_val) const;

            virtual bool as_int_tuple_if_exists(const std::string &section_name,
                                                const std::string &entry_name,
                                                IntTuple &ret_val) const;

            virtual bool as_double_tuple_if_exists(const std::string &section_name,
                                                   const std::string &entry_name,
                                                   DoubleTuple &ret_val) const;

            virtual void print(std::ostream &output_stream) const;
        };

        EmptyValue::EmptyValue() {
            // Does nothing...
        }

        EmptyValue::~EmptyValue() {
            // Does nothing...
        }

        bool EmptyValue::exists() const {
            return false;
        }

        bool EmptyValue::as_int_if_exists(const std::string &/*section_name*/,
                                          const std::string &/*entry_name*/,
                                          int &/*ret_val*/) const {
            return false;
        }

        bool EmptyValue::as_double_if_exists(const std::string &/*section_name*/,
                                             const std::string &/*entry_name*/,
                                             double &/*ret_val*/) const {
            return false;
        }

        bool EmptyValue::as_string_if_exists(const std::string &/*section_name*/,
                                             const std::string &/*entry_name*/,
                                             std::string &/*ret_val*/) const {
            return false;
        }

        bool EmptyValue::as_bool_if_exists(const std::string &/*section_name*/,
                                           const std::string &/*entry_name*/,
                                           bool &/*ret_val*/) const {
            return false;
        }

        bool EmptyValue::as_int_tuple_if_exists(const std::string &/*section_name*/,
                                                const std::string &/*entry_name*/,
                                                IntTuple &/*ret_val*/) const {
            return false;
        }

        bool EmptyValue::as_double_tuple_if_exists(const std::string &/*section_name*/,
                                                   const std::string &/*entry_name*/,
                                                   DoubleTuple &/*ret_val*/) const {
            return false;
        }

        void EmptyValue::print(std::ostream &/*output_stream*/) const {
            assert(false);
        }

        // The value that is returned when a value that does not exist is requested.
        const EmptyValue nonexistent_value;


        void delete_value(Value *const value) {
            delete value;
        }

        void delete_entry(const ValueMap::value_type &entry) {
            delete entry.second;
        }


        bool is_eof_or_newline(std::istream::int_type chr) {
            return chr == std::istream::traits_type::eof()
                   || chr == '\n'
                   || chr == '\r';
        }

        bool is_eol(std::istream::int_type chr) {
            return is_eof_or_newline(chr)
                   || chr == ';';
        }

        bool is_hspace(std::istream::int_type chr) {
            return chr == '\t' || chr == ' ';
        }

        bool is_quote(std::istream::int_type chr) {
            return chr == '\'' || chr == '\"';
        }

        void skip_wspace(std::istream &input_stream) {
            for (;;) {
                while (std::isspace(input_stream.peek())) {
                    input_stream.get();
                }

                if (input_stream.peek() != ';') {
                    break;
                }

                while (!is_eof_or_newline(input_stream.peek())) {
                    input_stream.get();
                }
            }
        }

        void skip_hspace(std::istream &input_stream) {
            while (is_hspace(input_stream.peek())) {
                input_stream.get();
            }
        }

        void assert_chars(std::istream &input_stream,
                          const char *const chars) {
            for (const char *i = chars; *i != '\0'; ++i) {
                if (input_stream.peek() != *i) {
                    throw UnexpectedCharacter(input_stream.peek(), input_stream.tellg());
                }

                input_stream.get();
            }
        }

        std::string read_key(std::istream &input_stream) {
            skip_hspace(input_stream);
            std::istream::pos_type pos = input_stream.tellg();
            std::istream::int_type chr = input_stream.get();

            // The first character of a key has to be a letter.
            if (!std::isalnum(chr)) {
                throw UnexpectedCharacter(chr, pos);
            }

            std::string key = "";

            // Read all letters and digits the key consists of.
            while (std::isalnum(chr)) {
                key += static_cast<char>(chr);
                chr = input_stream.get();
            }

            input_stream.putback(chr);
            return key;
        }

        Value *read_number(std::istream &input_stream) {
            std::istream::int_type chr = input_stream.get();
            int sign = +1;

            switch (chr) {
                case '-':
                    sign = -1;
                case '+':
                    chr = input_stream.get();
            }

            int int_val = 0;
            double double_val = 0;

            // Read the value of the number as both an int and a double.
            while (std::isdigit(chr)) {
                int_val = int_val * 10 + chr - '0';
                double_val = double_val * 10 + chr - '0';
                chr = input_stream.get();
            }

            // If there is no radix point the number is considered to be an int.
            if (chr != '.') {
                input_stream.putback(chr);
                return new IntValue(sign * int_val);
            }

            chr = input_stream.get();
            double denom = 1;

            // Read the fractional part.
            while (std::isdigit(chr)) {
                double_val = double_val * 10 + chr - '0';
                denom *= 10;
                chr = input_stream.get();
            }

            input_stream.putback(chr);
            return new DoubleValue(sign * double_val / denom);
        }

        Value *read_string(std::istream &input_stream) {
            const std::istream::int_type quote = input_stream.get();
            assert(is_quote(quote));
            std::istream::pos_type pos = input_stream.tellg();
            std::istream::int_type chr = input_stream.get();
            std::string value = "";

            while (chr != quote) {
                // EOFs and newlines cannot occur in a string.
                if (is_eof_or_newline(chr)) {
                    throw UnexpectedCharacter(chr, pos);
                }

                value += static_cast<char>(chr);
                pos = input_stream.tellg();
                chr = input_stream.get();
            }

            return new StringValue(value);
        }

        Value *read_tuple(std::istream &input_stream) {
            assert(input_stream.peek() == '(');
            input_stream.get();
            skip_wspace(input_stream);

            // Check whether the tuple is the empty tuple.
            if (input_stream.peek() == ')') {
                return new TupleValue(std::vector<Value *>());
            }

            std::vector<Value *> elements;

            try {
                for (;;) {
                    elements.push_back(read_number(input_stream));
                    skip_wspace(input_stream);
                    const std::istream::pos_type pos = input_stream.tellg();
                    const std::istream::int_type chr = input_stream.get();

                    if (chr == ')') {
                        break;
                    }

                    if (chr != ',') {
                        throw UnexpectedCharacter(chr, pos);
                    }

                    skip_wspace(input_stream);
                }
            }
            catch (...) {
                // Free the already parsed elements.
                std::for_each(elements.begin(), elements.end(), delete_value);
                throw;
            }

            return new TupleValue(elements);
        }

        bool is_ci_equal(const std::string &lhs,
                         const std::string &rhs) {
            if (lhs.length() != rhs.length()) {
                return false;
            }

            std::string::const_iterator i = lhs.begin();
            std::string::const_iterator j = rhs.begin();
            const std::string::const_iterator last = lhs.end();

            while (i != last) {
                if (std::tolower(*i++) != std::tolower(*j++)) {
                    return false;
                }
            }

            return true;
        }

        Value *read_raw(std::istream &input_stream) {
            std::istream::int_type chr = input_stream.get();
            std::string value = "";
            std::string::size_type last = 0;

            while (!is_eol(chr)) {
                value += static_cast<char>(chr);

                if (!std::isspace(chr)) {
                    last = value.length();
                }

                chr = input_stream.get();
            }

            input_stream.putback(chr);
            value.erase(last);

            if (is_ci_equal(value, "true")) {
                return new BoolValue(true);
            } else if (is_ci_equal(value, "false")) {
                return new BoolValue(false);
            }

            return new StringValue(value);
        }

        Value *read_value(std::istream &input_stream) {
            const std::istream::int_type chr = input_stream.peek();

            if (std::isdigit(chr) || chr == '+' || chr == '-') {
                return read_number(input_stream);
            } else if (is_quote(chr)) {
                return read_string(input_stream);
            } else if (chr == '(') {
                return read_tuple(input_stream);
            } else if (is_eol(chr)) {
                return new StringValue("");
            }

            return read_raw(input_stream);
        }

        ValueMap read_entries(const std::string &name,
                              std::istream &input_stream) {
            ValueMap values;

            try {
                while (input_stream.peek() != std::istream::traits_type::eof()
                       && input_stream.peek() != '[') {
                    const std::string key = read_key(input_stream);
                    skip_hspace(input_stream);
                    assert_chars(input_stream, "=");
                    skip_hspace(input_stream);
                    std::unique_ptr<Value> value(read_value(input_stream));

                    if (!values.insert(std::make_pair(key, value.get())).second) {
                        throw DuplicateEntry(name, key);
                    }

                    value.release();
                    skip_wspace(input_stream);
                }
            }
            catch (...) {
                std::for_each(values.begin(), values.end(), delete_entry);
                throw; // Re-throw the exception.
            }

            return values;
        }
    }


    Entry::Entry(const std::string &section_name_init,
                 const std::string &entry_name_init,
                 const Value *const value_ptr_init)
            : section_name(section_name_init), entry_name(entry_name_init), value_ptr(value_ptr_init) {
        // Does nothing...
    }

    Entry::Entry(const Entry &original)
            : section_name(original.section_name), entry_name(original.entry_name), value_ptr(original.value_ptr) {
        // Does nothing...
    }

    Entry::~Entry() {
        // Does nothing...
    }

    Entry &Entry::operator=(const Entry &original) {
        section_name = original.section_name;
        entry_name = original.entry_name;
        value_ptr = original.value_ptr;

        return *this;
    }

    const std::string &Entry::get_section_name() const {
        return section_name;
    }

    const std::string &Entry::get_entry_name() const {
        return entry_name;
    }

    bool Entry::exists() const {
        return value_ptr->exists();
    }

    bool Entry::as_int_if_exists(int &ret_val) const {
        return value_ptr->as_int_if_exists(section_name, entry_name, ret_val);
    }

    bool Entry::as_double_if_exists(double &ret_val) const {
        return value_ptr->as_double_if_exists(section_name, entry_name, ret_val);
    }

    bool Entry::as_string_if_exists(std::string &ret_val) const {
        return value_ptr->as_string_if_exists(section_name, entry_name, ret_val);
    }

    bool Entry::as_bool_if_exists(bool &ret_val) const {
        return value_ptr->as_bool_if_exists(section_name, entry_name, ret_val);
    }

    bool Entry::as_int_tuple_if_exists(IntTuple &ret_val) const {
        return value_ptr->as_int_tuple_if_exists(section_name, entry_name, ret_val);
    }

    bool Entry::as_double_tuple_if_exists(DoubleTuple &ret_val) const {
        return value_ptr->as_double_tuple_if_exists(section_name, entry_name, ret_val);
    }

    int Entry::as_int_or_die() const {
        int value;

        if (as_int_if_exists(value)) {
            return value;
        }

        throw NonexistentEntry(section_name, entry_name);
    }

    double Entry::as_double_or_die() const {
        double value;

        if (as_double_if_exists(value)) {
            return value;
        }

        throw NonexistentEntry(section_name, entry_name);
    }

    std::string Entry::as_string_or_die() const {
        std::string value;

        if (as_string_if_exists(value)) {
            return value;
        }

        throw NonexistentEntry(section_name, entry_name);
    }

    bool Entry::as_bool_or_die() const {
        bool value;

        if (as_bool_if_exists(value)) {
            return value;
        }

        throw NonexistentEntry(section_name, entry_name);
    }

    IntTuple Entry::as_int_tuple_or_die() const {
        IntTuple value;

        if (as_int_tuple_if_exists(value)) {
            return value;
        }

        throw NonexistentEntry(section_name, entry_name);
    }

    DoubleTuple Entry::as_double_tuple_or_die() const {
        DoubleTuple value;

        if (as_double_tuple_if_exists(value)) {
            return value;
        }

        throw NonexistentEntry(section_name, entry_name);
    }

    int Entry::as_int_or_default(const int def_val) const {
        int value;

        if (as_int_if_exists(value)) {
            return value;
        }

        return def_val;
    }

    double Entry::as_double_or_default(const double def_val) const {
        double value;

        if (as_double_if_exists(value)) {
            return value;
        }

        return def_val;
    }

    std::string Entry::as_string_or_default(const std::string &def_val) const {
        std::string value;

        if (as_string_if_exists(value)) {
            return value;
        }

        return def_val;
    }

    bool Entry::as_bool_or_default(const bool def_val) const {
        bool value;

        if (as_bool_if_exists(value)) {
            return value;
        }

        return def_val;
    }

    IntTuple Entry::as_int_tuple_or_default(const IntTuple &def_val) const {
        IntTuple value;

        if (as_int_tuple_if_exists(value)) {
            return value;
        }

        return def_val;
    }

    DoubleTuple Entry::as_double_tuple_or_default(const DoubleTuple &def_val) const {
        DoubleTuple value;

        if (as_double_tuple_if_exists(value)) {
            return value;
        }

        return def_val;
    }

    Entry::operator int() const {
        return as_int_or_die();
    }

    Entry::operator double() const {
        return as_double_or_die();
    }

    Entry::operator std::string() const {
        return as_string_or_die();
    }

    Entry::operator bool() const {
        return as_bool_or_die();
    }

    Entry::operator IntTuple() const {
        return as_int_tuple_or_die();
    }

    Entry::operator DoubleTuple() const {
        return as_double_tuple_or_die();
    }

    int Entry::operator||(const int def_val) const {
        return as_int_or_default(def_val);
    }

    double Entry::operator||(const double def_val) const {
        return as_double_or_default(def_val);
    }

    std::string Entry::operator||(const std::string &def_val) const {
        return as_string_or_default(def_val);
    }

    bool Entry::operator||(const bool def_val) const {
        return as_bool_or_default(def_val);
    }

    IntTuple Entry::operator||(const IntTuple &def_val) const {
        return as_int_tuple_or_default(def_val);
    }

    DoubleTuple Entry::operator||(const DoubleTuple &def_val) const {
        return as_double_tuple_or_default(def_val);
    }


    Section::Section(const std::string &section_name_init,
                     const ValueMap *const values_init)
            : section_name(section_name_init), values(values_init) {
        // Does nothing...
    }

    Section::Section(const Section &original)
            : section_name(original.section_name), values(original.values) {
        // Does nothing...
    }

    Section::~Section() {
        // Does nothing...
    }

    Entry Section::operator[](const std::string &key) const {
        const ConstValueIter value = values->find(key);

        if (value == values->end()) {
            return Entry(section_name, key, &nonexistent_value);
        }

        return Entry(section_name, key, &*value->second);
    }

    namespace {
        /*
         * The section that is returned when the requested section does not exists.  This
         * section does not contain any values so all the values that are retrieved from
         * this section will be EmptyValues.
         */
        const ValueMap empty_section;
    }


    Configuration::Configuration()
            : sections() {
        // Does nothing...
    }

    Configuration::Configuration(std::istream &input_stream)
            : sections() {
        parse(input_stream);
    }

    Configuration::Configuration(const Configuration &) {
        // This copy constructor should not be used.
        std::terminate();
    }

    Configuration::~Configuration() {
        // Delete all sections in this configuration.
        std::for_each(sections.begin(), sections.end(), delete_section);
    }

    Configuration &Configuration::operator=(const Configuration &) {
        // This copy-assignment operator should not be used.
        std::terminate();
        return *this;
    }

    void Configuration::delete_section(const SectionMap::value_type &section) {
        std::for_each(section.second.begin(), section.second.end(), delete_entry);
    }

    Section Configuration::operator[](const std::string &name) const {
        const ConstSectionIter section = sections.find(name);

        // Return a section containing no values if the
        // section does not exist.
        if (section == sections.end()) {
            return Section(name, &empty_section);
        }

        return Section(name, &section->second);
    }

    void Configuration::parse(std::istream &input_stream) {
        skip_wspace(input_stream);

        while (input_stream.peek() != std::istream::traits_type::eof()) {
            assert_chars(input_stream, "[");
            skip_hspace(input_stream);
            const std::string name = read_key(input_stream);
            skip_hspace(input_stream);
            assert_chars(input_stream, "]");
            skip_wspace(input_stream);
            const ValueMap section = read_entries(name, input_stream);

            if (!sections.insert(std::make_pair(name, section)).second) {
                throw DuplicateSection(name);
            }
        }
    }

    void Configuration::print(std::ostream &output_stream) const {
        const ConstSectionIter first_section = sections.begin();
        const ConstSectionIter last_section = sections.end();

        for (ConstSectionIter i = first_section; i != last_section; ++i) {
            // Print a blank line between sections.
            if (i != first_section) {
                output_stream << std::endl;
            }

            /* Print the header of the section. */
            output_stream << "[" << i->first << "]" << std::endl;

            const ConstValueIter first_entry = i->second.begin();
            const ConstValueIter last_entry = i->second.end();

            // Print the entries in the section.
            for (ConstValueIter j = first_entry; j != last_entry; ++j) {
                output_stream << j->first << " = ";
                j->second->print(output_stream);
                output_stream << std::endl;
            }
        }
    }

    std::istream &operator>>(std::istream &input_stream,
                             Configuration &configuration) {
        configuration.parse(input_stream);
        return input_stream;
    }

    std::ostream &operator<<(std::ostream &output_stream,
                             const Configuration &configuration) {
        configuration.print(output_stream);
        return output_stream;
    }
}
