/*
 * ini_configuration.h
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

#ifndef INI_CONFIGURATION_INCLUDED
#define INI_CONFIGURATION_INCLUDED

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>



/**
 * \brief The namespace of the INI configuration file parser.
 */
namespace ini {
    /**
     * \brief The base class for all exceptions that are thrown by the INI-parser.
     */
    class ParseException : public std::exception {
    protected:

        /**
         * \brief Constructs a new ParseException instance.
         */
        ParseException() throw();

        /**
         * \brief Constructs a new ParseException instance by copying another one.
         *
         * \param original The exception that is copied.
         */
        ParseException(const ParseException &original) throw();

        /**
         * \brief Destructs a ParseException.
         */
        virtual ~ParseException() throw();

        /**
         * \brief Copies a parse exception.
         *
         * \param original The parse exception that is copied.
         *
         * \return A reference to this instance.
         */
        ParseException &operator=(const ParseException &original) throw();

    public:

        /**
         * \brief Returns a description of the error that occurred.
         *
         * \return A description of the error that occurred.
         */
        virtual const char *what() const throw() = 0;
    };

    /**
     * \brief The exception that is thrown when the parser encounters an unexpected character.
     */
    class UnexpectedCharacter : public ParseException {
    private:

        /**
         * \brief The unexpected character.
         */
        std::istream::int_type character;

        /**
         * \brief The position of the character in the input stream.
         */
        std::istream::pos_type position;

        /**
         * \brief The message that is returned by the what() method.
         */
        std::string message;

    public:

        /**
         * \brief Constructs a new UnexpectedCharacter exception.
         *
         * \param character_init The unexpected character.
         * \param position_init  The position of the character in the input stream.
         */
        UnexpectedCharacter(const std::istream::int_type character_init,
                            const std::istream::pos_type position_init) throw();

        /**
         * \brief Constructs a new UnexpectedCharacter instance by copying another one.
         *
         * \param original The instance that is copied.
         */
        UnexpectedCharacter(const UnexpectedCharacter &original) throw();

        /**
         * \brief Destructs a .
         */
        virtual ~UnexpectedCharacter() throw();

        /**
         * \brief Copies an UnexpectedCharacter.
         *
         * \param original The instance that is copied.
         *
         * \return A reference to this instance.
         */
        UnexpectedCharacter &operator=(const UnexpectedCharacter &original) throw();

        /**
         * \brief Returns a description of the error hat occurred.
         *
         * \return A description of the error hat occurred.
         */
        virtual const char *what() const throw();
    };

    /**
     * \brief The exception that is thrown by the parser when it encounters a section that has the same name as a previously parsed section.
     */
    class DuplicateSection : public ParseException {
    private:

        /**
         * \brief The name of the duplicate section.
         */
        std::string name;

        /**
         * \brief The message that is returned by the what() method.
         */
        std::string message;

    public:

        /**
         * \brief Constructs a new DuplicateSection instance.
         *
         * \param name_init The name of the duplicate section.
         */
        DuplicateSection(const std::string &name_init) throw();

        /**
         * \brief Constructs a new DuplicateSection instance by copying another one.
         *
         * \param original The instance that is copied.
         */
        DuplicateSection(const DuplicateSection &original) throw();

        /**
         * \brief Destructs a DuplicateSection.
         */
        virtual ~DuplicateSection() throw();

        /**
         * \brief Copies a DuplicateSection.
         *
         * \param original The instance that is copied.
         *
         * \return A reference to this instance.
         */
        DuplicateSection &operator=(const DuplicateSection &original) throw();

        /**
         * \brief Returns a description of the error hat occurred.
         *
         * \return A description of the error hat occurred.
         */
        virtual const char *what() const throw();
    };

    /**
     * \brief The exception that is thrown when the parser encounters an entry that has the same key as a previously parsed key in the same section.
     */
    class DuplicateEntry : public ParseException {
    private:

        /**
         * \brief The name of the section that contains the duplicate entry.
         */
        std::string section;

        /**
         * \brief The name of the duplicate entry.
         */
        std::string key;

        /**
         * \brief The message that is returned by the what() method.
         */
        std::string message;

    public:

        /**
         * \brief Constructs a new DuplicateEntry instance.
         *
         * \param section_init The name of the section that contains the duplicate entry.
         * \param key_init     The name of the duplicate entry.
         */
        DuplicateEntry(const std::string &section_init,
                       const std::string &key_init) throw();

        /**
         * \brief Constructs a new DuplicateEntry instance by copying another one.
         *
         * \param original The instance that is copied.
         */
        DuplicateEntry(const DuplicateEntry &original) throw();

        /**
         * \brief Destructs a DuplicateEntry.
         */
        virtual ~DuplicateEntry() throw();

        /**
         * \brief Copies a DuplicateEntry exception.
         *
         * \param original The instance that is copied.
         *
         * \return A reference to this instance.
         */
        DuplicateEntry &operator=(const DuplicateEntry &original) throw();

        /**
         * \brief Returns a description of the error hat occurred.
         *
         * \return A description of the error hat occurred.
         */
        virtual const char *what() const throw();
    };

    /**
     * \brief The execption that is thrown when the value of a nonexistent entry is requested.
     *
     * Note that this exception is only thrown when the value of an Entry is obtained; not when the entry is obtained from a Section.
     */
    class NonexistentEntry : public std::exception {
    private:

        /**
         * \brief The name of the section from which the nonexistent entry is obtained.
         */
        std::string section_name;

        /**
         * \brief The name of the nonexistent entry.
         */
        std::string entry_name;

        /**
         * \brief The message that is returned by the what() method.
         */
        std::string message;

    public:

        /**
         * \brief Constructs a new NonexistentEntry instance.
         *
         * \param section_name_init The name of the section from which the nonexistent entry is obtained.
         * \param entry_name_init   The name of the nonexistent entry.
         */
        NonexistentEntry(const std::string &section_name_init,
                         const std::string &entry_name_init) throw();

        /**
         * \brief Constructs a new NonexistentEntry instance by copying another one.
         *
         * \param original The instance that is copied.
         */
        NonexistentEntry(const NonexistentEntry &original) throw();

        /**
         * \brief Destructs a NonexistentEntry.
         */
        virtual ~NonexistentEntry() throw();

        /**
         * \brief Copies a NonexistentEntry.
         *
         * \param original The instance that is copied.
         *
         * \return A reference to this instance.
         */
        NonexistentEntry &operator=(const NonexistentEntry &original) throw();

        /**
         * \brief Returns a description of the error hat occurred.
         *
         * \return A description of the error hat occurred.
         */
        const char *what() const throw();
    };

    /**
     * \brief The execption that is thrown when the value of an entry in an ini-configuration cannot be converted to the requested value.
     */
    class IncompatibleConversion : public std::exception {
    private:

        /**
         * \brief The name of the section of the entry.
         */
        std::string section_name;

        /**
         * \brief The name of the entry.
         */
        std::string entry_name;

        /**
         * \brief The name of the requested type, e.g. "int".
         */
        std::string type_name;

        /**
         * \brief The message that is returned by the what() method.
         */
        std::string message;

    public:

        /**
         * \brief Constructs a new IncompatibleConversion exception.
         *
         * \param section_name_init The name of the section of the entry.
         * \param entry_name_init   The name of the entry.
         * \param type_name_init    The name of the requested type, e.g. "int".
         */
        IncompatibleConversion(const std::string &section_name_init,
                               const std::string &entry_name_init,
                               const std::string &type_name_init) throw();

        /**
         * \brief Constructs a IncompatibleConversion exception by copying another one.
         *
         * \param original The instance that is copied.
         */
        IncompatibleConversion(const IncompatibleConversion &original) throw();

        /**
         * \brief Destructs a .
         */
        virtual ~IncompatibleConversion() throw();

        /**
         * \brief Copies an IncompatibleConversion.
         *
         * \param original The instance that is copied.
         *
         * \return A reference to this instance.
         */
        IncompatibleConversion &operator=(const IncompatibleConversion &original) throw();

        /**
         * \brief Returns a description of the error hat occurred.
         *
         * \return A description of the error hat occurred.
         */
        virtual const char *what() const throw();
    };


    /**
     * \brief The type that is used to store int tuples (list of ints).
     */
    typedef std::vector<int> IntTuple;

    /**
     * \brief The type that is used to store double tuples (list of doubles).
     */
    typedef std::vector<double> DoubleTuple;

    /**
     * \brief The base class for all classes that contain the value of an entry.
     */
    class Value;

    /**
     * \brief The class that represents an entry in the section of an INI configuration.
     */
    class Entry {
    private:

        /**
         * \brief The name of the section to which this entry belongs.
         */
        std::string section_name;

        /**
         * \brief The name of this entry.
         */
        std::string entry_name;

        /**
         * \brief A pointer to the value of this entry.
         */
        const Value *value_ptr;

    public:

        /**
         * \brief Constructs a new entry given the name of the section it belongs to and its value.
         *
         * \param section_name_init The name of the section to which this entry belongs.
         * \param entry_name_init The name of this entry.
         * \param value_ptr_init A pointer to the value of this entry.
         */
        Entry(const std::string &section_name_init,
              const std::string &entry_name_init,
              const Value *const value_ptr_init);

        /**
         * \brief Constructs an entry by copying another one.
         *
         * \param original The section whose values are copied.
         */
        Entry(const Entry &original);

        /**
         * \brief Destructs an entry.
         */
        ~Entry();

        /**
         * \brief Copies an entry.
         *
         * \param original The section whose values are copied.
         *
         * \return A reference to this entry.
         */
        Entry &operator=(const Entry &original);

        /**
         * \brief Returns the name of the section to which this entry belongs.
         *
         * \return The name of the section to which this entry belongs.
         */
        const std::string &get_section_name() const;

        /**
         * \brief Returns the name of this entry.
         *
         * \return Returns the name of this entry.
         */
        const std::string &get_entry_name() const;

        /**
         * \brief Checks whether this entry exists in the configuration or not.
         *
         * \return \c true if this entry exits, \c false otherwise.
         */
        bool exists() const;

        /**
         * \brief Returns the value as an int.
         *
         * If the entry exists and can be represented as an int, the value is passed to
         * the caller through the parameter and \c true is returned.  If the entry exists
         * but is not representable as an int, an IncompatibleConversion exception is
         * thrown.  If the entry does not exist, \c false is returned and the value of
         * the parameter is not changed.
         *
         * \param ret_val The parameter through which the value is returned.
         *
         * \return \c true if the value exists, \c false otherwise.
         */
        bool as_int_if_exists(int &ret_val) const;

        /**
         * \brief Returns the value as a double.
         *
         * If the entry exists and can be represented as a double, the value is passed to
         * the caller through the parameter and \c true is returned.  If the entry exists
         * but is not representable as a double, an IncompatibleConversion exception is
         * thrown.  If the entry does not exist, \c false is returned and the value of
         * the parameter is not changed.
         *
         * \param ret_val The parameter through which the value is returned.
         *
         * \return \c true if the value exists, \c false otherwise.
         */
        bool as_double_if_exists(double &ret_val) const;

        /**
         * \brief Returns the value as a string.
         *
         * If the entry exists and can be represented as a string, the value is passed to
         * the caller through the parameter and \c true is returned.  If the entry exists
         * but is not representable as a string, an IncompatibleConversion exception is
         * thrown.  If the entry does not exist, \c false is returned and the value of
         * the parameter is not changed.
         *
         * \param ret_val The parameter through which the value is returned.
         *
         * \return \c true if the value exists, \c false otherwise.
         */
        bool as_string_if_exists(std::string &ret_val) const;

        /**
         * \brief Returns the value as a bool.
         *
         * If the entry exists and can be represented as a bool, the value is passed to
         * the caller through the parameter and \c true is returned.  If the entry exists
         * but is not representable as a bool, an IncompatibleConversion exception is
         * thrown.  If the entry does not exist, \c false is returned and the value of
         * the parameter is not changed.
         *
         * \param ret_val The parameter through which the value is returned.
         *
         * \return \c true if the value exists, \c false otherwise.
         */
        bool as_bool_if_exists(bool &ret_val) const;

        /**
         * \brief Returns the value as an int tuple.
         *
         * If the entry exists and can be represented as an int tuple, the value is
         * passed to the caller through the parameter and \c true is returned.  If the
         * value exists but is not representable as an int tuple, an
         * IncompatibleConversion exception is thrown.  If the entry does not exist, \c
         * false is returned and the value of the parameter is not changed.
         *
         * \param ret_val The parameter through which the value is returned.
         *
         * \return \c true if the value exists, \c false otherwise.
         */
        bool as_int_tuple_if_exists(IntTuple &ret_val) const;

        /**
         * \brief Returns the value as a double tuple.
         *
         * If the entry exists and can be represented as a double tuple, the value is
         * passed to the caller through the parameter and \c true is returned.  If the
         * value exists but is not representable as a double tuple, an
         * IncompatibleConversion exception is thrown.  If the entry does not exist, \c
         * false is returned and the value of the parameter is not changed.
         *
         * \param ret_val The parameter through which the value is returned.
         *
         * \return \c true if the value exists, \c false otherwise.
         */
        bool as_double_tuple_if_exists(DoubleTuple &ret_val) const;

        /**
         * \brief Returns the value as an int.
         *
         * If the entry exists and can be represented as an int, it is returned.  If the
         * value is not representable as an int, an IncompatibleConversion exception is
         * thrown.  If the entry does not exist, a NonexistentEntry exception is thrown.
         *
         * \return The value as an int.
         */
        int as_int_or_die() const;

        /**
         * \brief Returns the value as a double.
         *
         * If the entry exists and can be represented as a double, it is returned.  If
         * the value is not representable as a double, an IncompatibleConversion
         * exception is thrown.  If the entry does not exist, a NonexistentEntry
         * exception is thrown.
         *
         * \return The value as a double.
         */
        double as_double_or_die() const;

        /**
         * \brief Returns the value as a string.
         *
         * If the entry exists and can be represented as a string, it is returned.  If
         * the value is not representable as a string, an IncompatibleConversion
         * exception is thrown.  If the entry does not exist, a NonexistentEntry
         * exception is thrown.
         *
         * \return The value as a string.
         */
        std::string as_string_or_die() const;

        /**
         * \brief Returns the value as a bool.
         *
         * If the entry exists and can be represented as a bool, it is returned.  If the
         * value is not representable as a bool, an IncompatibleConversion exception is
         * thrown.  If the entry does not exist, a NonexistentEntry exception is thrown.
         *
         * \return The value as a bool.
         */
        bool as_bool_or_die() const;

        /**
         * \brief Returns the value as an int tuple.
         *
         * If the entry exists and can be represented as an int tuple, it is returned.
         * If the value is not representable as an int tuple, an IncompatibleConversion
         * exception is thrown.  If the entry does not exist, a NonexistentEntry
         * exception is thrown.
         *
         * \return The value as an int tuple.
         */
        IntTuple as_int_tuple_or_die() const;

        /**
         * \brief Returns the value as a double tuple.
         *
         * If the entry exists and can be represented as a double tuple, it is returned.
         * If the value is not representable as a double tuple, an IncompatibleConversion
         * exception is thrown.  If the entry does not exist, a NonexistentEntry
         * exception is thrown.
         *
         * \return The value as a double tuple.
         */
        DoubleTuple as_double_tuple_or_die() const;

        /**
         * \brief Returns the value as an int.
         *
         * If the entry exists and can be represented as an int, it is returned.  If the
         * value is not representable as an int, an IncompatibleConversion exception is
         * thrown.  If the entry does not exist, a default value is returned.
         *
         * \param def_val The default value that is returned if the value does not exist.
         *
         * \return The value as an int or the default value if the value does not exist.
         */
        int as_int_or_default(const int def_val) const;

        /**
         * \brief Returns the value as a double.
         *
         * If the entry exists and can be represented as a double, it is returned.  If
         * the value is not representable as a double, an IncompatibleConversion
         * exception is thrown.  If the entry does not exist, a default value is
         * returned.
         *
         * \param def_val The default value that is returned if the value does not exist.
         *
         * \return The value as a double or the default value if the value does not exist.
         */
        double as_double_or_default(const double def_val) const;

        /**
         * \brief Returns the value as a string.
         *
         * If the entry exists and can be represented as a string, it is returned.  If
         * the value is not representable as a string, an IncompatibleConversion
         * exception is thrown.  If the entry does not exist, a default value is
         * returned.
         *
         * \param def_val The default value that is returned if the value does not exist.
         *
         * \return The value as a string or the default value if the value does not exist.
         */
        std::string as_string_or_default(const std::string &def_val) const;

        /**
         * \brief Returns the value as a bool.
         *
         * If the entry exists and can be represented as a bool, it is returned.  If the
         * value is not representable as a bool, an IncompatibleConversion exception is
         * thrown.  If the entry does not exist, a default value is returned.
         *
         * \param def_val The default value that is returned if the value does not exist.
         *
         * \return The value as a bool or the default value if the value does not exist.
         */
        bool as_bool_or_default(const bool def_val) const;

        /**
         * \brief Returns the value as an int tuple.
         *
         * If the entry exists and can be represented as an int tuple, it is returned.
         * If the value is not representable as an int tuple, an IncompatibleConversion
         * exception is thrown.  If the entry does not exist, a default value is
         * returned.
         *
         * \param def_val The default value that is returned if the value does not exist.
         *
         * \return The value as an int tuple or the default value if the value does not exist.
         */
        IntTuple as_int_tuple_or_default(const IntTuple &def_val) const;

        /**
         * \brief Returns the value as a double tuple.
         *
         * If the entry exists and can be represented as a double tuple, it is returned.
         * If the value is not representable as a double tuple, an IncompatibleConversion
         * exception is thrown.  If the entry does not exist, a default value is
         * returned.
         *
         * \param def_val The default value that is returned if the value does not exist.
         *
         * \return The value as a double tuple or the default value if the value does not exist.
         */
        DoubleTuple as_double_tuple_or_default(const DoubleTuple &def_val) const;

        /**
         * \brief An alias for as_int_or_die.
         *
         * This conversion operator allows a Value to be converted to an int when it is
         * assigned to an int variable.
         *
         * \return The int value of the Value.
         */
        operator int() const;

        /**
         * \brief An alias for as_double_or_die.
         *
         * This conversion operator allows a Value to be converted to a double when it is
         * assigned to a double variable.
         *
         * \return The double value of the Value.
         */
        operator double() const;

        /**
         * \brief An alias for as_string_or_die.
         *
         * This conversion operator allows a Value to be converted to a string when it is
         * assigned to a string variable.
         *
         * \return The string value of the Value.
         */
        operator std::string() const;

        /**
         * \brief An alias for as_bool_or_die.
         *
         * This conversion operator allows a Value to be converted to a bool when it is
         * assigned to a bool variable.
         *
         * \return The bool value of the Value.
         */
        operator bool() const;

        /**
         * \brief An alias for as_int_tuple_or_die.
         *
         * This conversion operator allows a Value to be converted to an int tuple when
         * it is assigned to an int tuple variable.
         *
         * \return The int tuple value of the Value.
         */
        operator IntTuple() const;

        /**
         * \brief An alias for as_int_or_die.
         *
         * This conversion operator allows a Value to be converted to a double tuple when
         * it is assigned to a double tuple variable.
         *
         * \return The double tuple value of the Value.
         */
        operator DoubleTuple() const;

        /**
         * \brief An alias for as_int_or_default.
         *
         * \param def_val The value that is returned if the requested value does not exist.
         *
         * \return The requested value as an int or def_val if the value does not exist.
         */
        int operator||(const int def_val) const;

        /**
         * \brief An alias for as_double_or_default.
         *
         * \param def_val The value that is returned if the requested value does not exist.
         *
         * \return The requested value as a double or def_val if the value does not exist.
         */
        double operator||(const double def_val) const;

        /**
         * \brief An alias for as_string_or_default.
         *
         * \param def_val The value that is returned if the requested value does not exist.
         *
         * \return The requested value as a string or def_val if the value does not exist.
         */
        std::string operator||(const std::string &def_val) const;

        /**
         * \brief An alias for as_bool_or_default.
         *
         * \param def_val The value that is returned if the requested value does not exist.
         *
         * \return The requested value as a bool or def_val if the value does not exist.
         */
        bool operator||(const bool def_val) const;

        /**
         * \brief An alias for as_int_tuple_or_default.
         *
         * \param def_val The value that is returned if the requested value does not exist.
         *
         * \return The requested value as an int tuple or def_val if the value does not exist.
         */
        IntTuple operator||(const IntTuple &def_val) const;

        /**
         * \brief An alias for as_double_tuple_or_default.
         *
         * \param def_val The value that is returned if the requested value does not exist.
         *
         * \return The requested value as a double tuple or def_val if the value does not exist.
         */
        DoubleTuple operator||(const DoubleTuple &def_val) const;
    };

    /**
     * \brief The type of the map in which the values are stored.
     */
    typedef std::map<std::string, Value *> ValueMap;

    /**
     * \brief The type of the iterator for iterating over a ValueMap.
     */
    typedef ValueMap::iterator ValueIter;

    /**
     * \brief The type of the iterator for iterating over a constant ValueMap.
     */
    typedef ValueMap::const_iterator ConstValueIter;

    /**
     * \brief The type that is used to represent sections that are stored in the configuration file.
     */
    class Section {
    private:

        /**
         * \brief The name of this section.
         */
        std::string section_name;

        /**
         * \brief A pointer to the map that stores the entries of the section.
         */
        const ValueMap *values;

    public:

        /**
         * \brief Creates a new section.
         *
         * \param section_name_init The name of the section.
         * \param values_init An iterator to the map that stores the entries of the section.
         */
        Section(const std::string &section_name_init,
                const ValueMap *const values_init);

        /**
         * \brief Creates a new section by copying another one.
         *
         * \param original The section that is copied.
         */
        Section(const Section &original);

        /**
         * \brief Destructs a section.
         */
        ~Section();

        /**
         * \brief Copies another section.
         *
         * \param original The section that is copied.
         *
         * \return A reference to this section.
         */
        Section &operator=(const Section &original);

        /**
         * \brief Looks up a entry in the section given its key and returns it.
         *
         * \param key The entry corresponding to the key.
         *
         * \return The entry corresponding to the key or an empty entry if the requested entry does not exist.
         */
        Entry operator[](const std::string &key) const;
    };

    /**
     * \brief The type in which a configuration file is stored.
     */
    class Configuration {
    private:

        /**
         * \brief The type of the map in which sections are stored.
         */
        typedef std::map<std::string, ValueMap> SectionMap;

        /**
         * \brief The iterator for iterating over a SectionMap.
         */
        typedef SectionMap::iterator SectionIter;

        /**
         * \brief The iterator for iterating over a constant SectionMap.
         */
        typedef SectionMap::const_iterator ConstSectionIter;

        /**
         * \brief Maps the names of the sections on the entries in them.
         */
        SectionMap sections;

        /**
         * \brief Deletes the entries in a section.
         */
        static void delete_section(const SectionMap::value_type &section);

        /**
         * \brief Constructs an INI configuration by copying another one.
         *
         * This copy-assignment operator is made private in order to avoid copying.
         * INI configurations should not be copied, they should be passed by reference
         * instead.
         *
         * \param original The INI configuration that is copied.
         */
        Configuration(const Configuration &original);

        /**
         * \brief Copies an INI configuration.
         *
         * This copy-assignment operator is made private in order to avoid copying.
         * INI configurations should not be copied, they should be passed by reference
         * instead.
         *
         * \param original The INI configuration that is copied.
         *
         * \return A reference to this INI configuration.
         */
        Configuration &operator=(const Configuration &original);

    public:

        /**
         * \brief Constructs a new (empty) configuration.
         */
        Configuration();

        /**
         * \brief Constructs a new Configuration by parsing the content from a stream.
         *
         * \param input_stream The stream from which the content is parsed.
         */
        Configuration(std::istream &input_stream);

        /**
         * \brief Destructs a Configuration and frees all entries stored in it.
         */
        ~Configuration();

        /**
         * \brief Retrieves a section from the configuration file given its key.
         *
         * If the requested section does not exist, a section containing no values is
         * returned.
         *
         * \param key The name of the requested section.
         *
         * \return A reference to the requested section.
         */
        Section operator[](const std::string &key) const;

        /**
         * \brief Reads a configuration file from a stream.
         *
         * \param input_stream The input stream from which the configuration is read.
         */
        void parse(std::istream &input_stream);

        /**
         * \brief Formats the contents of the Configuration to text and prints it to an output stream.
         *
         * \param output_stream The output stream to which the output is written.
         */
        void print(std::ostream &output_stream) const;
    };

    /**
     * \brief Convenience operator for reading configurations from an input stream.
     *
     * This operator reads the configuration from the input stream using the
     * Configuration::parse method.
     *
     * \param input_stream  The input stream from which the configuration is read.
     * \param configuration The Configuration object in which the parsed configuration is stored.
     *
     * \return A reference to the input stream.
     */
    std::istream &operator>>(std::istream &input_stream,
                             Configuration &configuration);

    /**
     * \brief Convenience operator for writing configurations to an output stream.
     *
     * This operator prints the configuration to the output stream using the
     * Configuration::print method.
     *
     * \param output_stream The output stream to which the configuration is written.
     * \param configuration The Configuration object in which the parsed configuration is stored.
     *
     * \return A reference to the output stream.
     */
    std::ostream &operator<<(std::ostream &output_stream,
                             const Configuration &configuration);
}


#endif // INI_CONFIGURATION_INCLUDED
