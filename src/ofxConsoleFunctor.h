/*
 Text Console for c++/OpenFrameworks 
 Copyright (c) 2009 Matthias Dörfelt <info@mokafolio.de>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*Unless you are a boost/c++ pro and can improve things you should not change anything
 of these files. If you want to have a custom console using OF, simply use ofxConsole.h
 as your base class and build your own thing ontop of it. If you wan't to use it within
 any other c++ environment use ofxConsoleBase.h as your base class.*/

#ifndef _OFXCONSOLEFUNCTOR
#define _OFXCONSOLEFUNCTOR

#include <vector>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;

/*struct to save function properties (argument types ect.).
 this gets returned to the console when a callback 
 invokes so the console can print out errors*/
struct ofxConsoleFProps{
	//number of arguments
	int numArgs;
	
	//argument type names (for detailed error messages)
	std::vector<std::string> arguments;
	
	//holds the errors of the last callback, empty if there are none
	std::vector<std::string> err;
};

//Functor base class
class ofxConsoleFunctor{
public:
	ofxConsoleFunctor(){};
	
	//object to store function properties
	ofxConsoleFProps properties;
	
	//virtual callback() function for polyphormism
	virtual ofxConsoleFProps * callBack(const std::vector<std::string> & args) = 0;
};

/* If you don't want to use boost you can use this simple callback system similar
 to the one OF uses anyways.- The main drawback of this is that you can only bind 
 functions to the console, which have ofxConsoleEventArgs as an argument, so that
 you would have to write wrapper functions most of the time! */

//ofxConsoleFunctorNb (no Boost) is the defaul callback class if no boost is used
template<class T>
class ofxConsoleFunctorNb : public ofxConsoleFunctor{
public:
	ofxConsoleFunctorNb(){};
	
	void set(T * _objectPointer, void(T::*_functionPointer)(const std::vector<std::string> &)){
		objectPointer = _objectPointer;
		functionPointer = _functionPointer;
	}
	
	T * objectPointer;
	void(T::*functionPointer)(const std::vector<std::string> &);
	
	virtual ofxConsoleFProps * callBack(const std::vector<std::string> & args){
		(*objectPointer.*functionPointer)(args);
		return &properties;
	}
};

/*if you have boost and want to benefit from it define this in
 ofxConsoleBase.h
 remember to add boost to your header seach paths in your
 Project settings!!
 You should use boost if you want to add alot of functions
 to ofxConsole!
 */
#ifdef USE_BOOST
#include <boost/preprocessor.hpp>
#include <boost/function.hpp>;
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

//Macros to generate c++ classes for various ofxConsoleFunctor templates that take
//different numbers of arguments!! (Using boost::preprocessor)
#define CLASSNAME(z, n, text) class ofxConsoleFunctor ## text : public ofxConsoleFunctor{
#define CONSTRUCTORNAME(z, n, text) ofxConsoleFunctor ## text
#define TEMPNAME(z, n, text) p=abi::__cxa_demangle(typeid(T ## n).name(), 0, &len, &s); properties.arguments.push_back(p);
#define PARAMS(z, n, text) text ## n arg ## n = castToType<text ## n>(args[n+1], properties.err);

#ifndef BOOST_PP_IS_ITERATING

//Functor for functions with no arguments
class ofxConsoleFunctor0 : public ofxConsoleFunctor{
public:
	ofxConsoleFunctor0(const boost::function< void() > & f){
		cb = f;
		properties.numArgs = 0;
	};
	
	boost::function<void()> cb;
	virtual ofxConsoleFProps * callBack(const std::vector<std::string> & args){
		if(args.size()!=properties.numArgs+1){
			properties.err.push_back("wrong number of arguments!");
			return &properties;
		} else {
			cb();
			return &properties;
		}
	};
};

//template that tries to convert the commandline string to any type
template < class T > 
T castToType(const std::string & str, std::vector<std::string> & err){
	//decode typenames for error message
	size_t len;
	int s;
	char * p=abi::__cxa_demangle(typeid(T).name(), 0, &len, &s); 
	/*size_t lentwo;
	 int stwo;
	 char * ptwo=abi::__cxa_demangle(typeid(str).name(), 0, &lentwo, &stwo);*/
	
	try
	{
		return boost::lexical_cast<T>(str);
	}
	catch( const boost::bad_lexical_cast & )
	{
		err.push_back("Can't convert commandline argument < "+str+" > to type "+p+"!");
	}
};

//how many arguments do we need for our templates(9 is the maximum, so you can register functions with up to 9 
//arguments to the console!!
#define BOOST_PP_ITERATION_LIMITS ( 1, 9)

//link to the file which gets included for each iteration!
#define BOOST_PP_FILENAME_1 "ofxConsoleFunctorGenerator.h"
#include BOOST_PP_ITERATE()
#endif
#endif
#endif
