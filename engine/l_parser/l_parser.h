/*
 * l_parser.h
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

#ifndef L_PARSER_INCLUDED
#define L_PARSER_INCLUDED

#include <map>
#include <string>
#include <set>
#include <exception>



/**
 * \brief The namespace used by the LParser
 */
namespace LParser {
	/**
	* \brief The exception thrown when an invalid file is read
	*/
	class ParserException : public std::exception {
	private:
		/**
		 * \brief The message explaining what went wrong
		 */
		std::string message;

	public:

		/**
		 * \brief Constructor
		 *
		 * \param msg	String explaining what went wrong
		 * \param line	The line in the file at which the parser failed
		 * \param pos	The position on the line at which the parser failed
		 */
		ParserException(std::string const &msg, unsigned int line, unsigned int pos);

		/**
		 * \brief Copy Constructor
		 *
		 * \param original	The exception to be copied
		 */
		ParserException(const ParserException &original);

		/**
		 * \brief Destructor
		 */
		virtual ~ParserException() throw();

		/**
		 * \brief Assignment operator
		 *
		 * \param original	The original exception to be assigned to this one
		 */
		ParserException &operator=(const ParserException &original);

		/**
					 * \brief Returns a description of the error hat occurred.
					 *
					 * \return A description of the error hat occurred.
					 */
		virtual const char *what() const throw();
	};

	/**
	 * \brief This is the Base Class used by LParser2D and LParser3D
	 */
	class LSystem {
	protected:

		/**
		 * \brief Constructor: creates an empty LSystem
		 */
		LSystem();

		/**
		 * \brief Copy-constructor: creates a new L-System from an existing L-System
		 *
		 * \param system	The L-System to be copied
		 */
		LSystem(LSystem const &system);

		/**
		 * \brief Destructor
		 */
		virtual ~LSystem();

		/**
		 *\brief Assignment operator
		 *
		 * \param system	the L-System to be copied
		 */
		LSystem &operator=(LSystem const &system);

	public:
		/**
		 * \brief returns the Alphabet of the L-System
		 *
		 * \return 	a const reference to the vector containing the alphabet
		 */
		std::set<char> const &get_alphabet() const;

		/**
		 * \brief Draw function. Returns true if a line needs to be drawn for this character
		 *
		 * \param c	the character of the alphabet
		 *
		 * \return	whether a line needs to be drawn for the character
		 */
		bool draw(char c) const;

		/**
		 * \brief Replacement function. Returns the replacement string for a given character of the Alphabet
		 *
		 * \param c 	the character of the alphabet
		 *
		 * \return	replacement string
		 */
		const std::string get_replacement(char c) const;

		/**
		 * \brief Returns the angle of the L-System.
		 *
		 * \returns 	the angle used by the LSystem
		 */
		double get_angle() const;

		/**
		 * \brief Returns the initiator string of the L-System
		 *
		 * \return 	the inititor string of the L-System
		 */
		std::string const &get_initiator() const;

		/**
		 * \brief Retrurns the number of times a symbol must be replaced by it's replacement string
		 *
		 * \return 	the number of replacements;
		 */
		unsigned int get_nr_iterations() const;

	protected:
		/**
		 * \brief the alphabet of the l-system
		 */
		std::set<char> alphabet;

		/**
		 * \brief the draw function mapping of the l-system
		 */
		std::map<char, bool> drawfunction;

		/**
		 * \brief the initiator stringof the l-system
		 */
		std::string initiator;

		/**
		 * \brief the angle of the l-system
		 */
		double angle;

		/**
		 * \brief the replacement rules of the l-system
		 */
		std::map<char, std::string> replacementrules;

		/**
		 * \brief the number of replacements of the l-system
		 */
		unsigned int nrIterations;
	};


	class LSystem2D;

	/**
	 * \brief Writes an LSystem2D to an output stream.
	 *
	 * \param out		The outputstream to write the LSystem2D to
	 * \param system	The L-System to be written
	 *
	 * \return		The outputstream the L-System was written to
	 */
	std::ostream &operator<<(std::ostream &out, LSystem2D const &system);

	/**
	 * \brief Reads an LSystem2D from an output stream.
	 *
	 * \param in		The input stream to read the LSystem2D from
	 * \param system	The L-System object in which the parsed LSystem is to be stored
	 *
	 * \return		The input stream from which the L-System was read
	 */
	std::istream &operator>>(std::istream &in, LSystem2D &system);

	/**
	 * \brief This class represents a 2D-LSystem
	*/
	class LSystem2D : public LSystem {
	public:
		/**
		 * \brief Constructor
		 * */
		LSystem2D();

		/**
		 * \brief Copy Constructor
		 *
		 * \param system	The L-System to be copied
		 */
		LSystem2D(LSystem2D const &system);

		/**
		 * \brief Constructor: reads the LSystem from an input stream
		 *
		 * \param in		The input stream from which the L-System is to be read
		 */
		LSystem2D(std::istream &in);

		/*
		 * \brief Destructor
		 */
		virtual ~LSystem2D();

		/**
		 * \brief Assignment operator. Assigns another LSystem to this object
		 *
		 * \param system	The L-System to be assigned to this object
		 */
		LSystem2D &operator=(LSystem2D const &system);

		/**
		 * \brief Returns the starting angle of the 2D L-System
		 *
		 * \return 	the starting angle of the L-System
		 * */
		double get_starting_angle() const;

	protected:
		friend std::istream &operator>>(std::istream &in, LSystem2D &system);

		/**
		 * \brief the starting angle of the 2D-LSystem
		*/
		double startingAngle;
	};

	class LSystem3D;

	/**
	 * \brief Writes an LSystem3D to an output stream.
	 *
	 * \param out		The outputstream to write the LSystem2D to
	 * \param system	The L-System to be written
	 *
	 * \return		The outputstream the L-System was written to
	 */
	std::ostream &operator<<(std::ostream &out, LSystem3D const &system);

	/**
	 * \brief Reads an LSystem3D from an output stream.
	 *
	 * \param in		The input stream to read the LSystem2D from
	 * \param system	The L-System object in which the parsed LSystem is to be stored
	 *
	 * \return		The input stream from which the L-System was read
	 */
	std::istream &operator>>(std::istream &in, LSystem3D &system);

	/**
	 * \brief This class represents a 3D-LSystem
	*/
	class LSystem3D : public LSystem {
	public:
		/**
		 * \brief Constructor
		 * */
		LSystem3D();

		/**
		 * \brief Copy Constructor
		 *
		 * \param system	The L-System to be assigned to this object
		 */
		LSystem3D(LSystem3D const &system);

		/**
		 * \brief Constructor: reads the LSystem from an input stream
		 *
		 * \param in		The input stream from which the LSystem is to be read
		 */
		LSystem3D(std::istream &in);

		/**
		 * \brief Destructor
		 */
		virtual ~LSystem3D();

		/**
		 * \brief Assignment operator
		 *
		 * \param system	The L-System to be assigned to this object
		 */
		LSystem3D &operator=(LSystem3D const &system);

	private:
		friend std::istream &operator>>(std::istream &in, LSystem3D &system);
	};

}


#endif //L_PARSER_INCLUDED
