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

#ifndef _OFXCONSOLEBASE
#define _OFXCONSOLEBASE

#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <iostream>

//THIS LINE IS THE ONLY THING YOU SHOULD CHANGE IF YOU DON'T HAVE BOOST
//#define USE_BOOST

#include "ofxConsoleFunctor.h"


//different message types
enum consoleTextType{
	CTEXT_COPY,
	CTEXT_MESSAGE,
	CTEXT_COMMANDLINE,
	CTEXT_ERROR,
    CTEXT_WELCOME
};

/*extend this class if you want to add more features
 to consoleText, right now I only added enums to differ
 between various message/string types. this information
 can be used to draw the text for example in different
 colours*/
class consoleText : public std::string{
public:
	consoleTextType textType;
	virtual void render(){};
};

enum consoleType
{
	CTYPE_UCHAR,
	CTYPE_CHAR,
	CTYPE_CHARP,
	CTYPE_UINT,
	CTYPE_INT,
	CTYPE_FLOAT,
	CTYPE_STRING,
	CTYPE_BOOL,
	CTYPE_FUNCTION
};

typedef ofxConsoleFunctor* consoleFunction;

typedef struct {
	// item name
	std::string name;
	
	// item type
	consoleType type;
	consoleTextType textType;
	
	// function callback or pointer to the value
	union
	{   
		consoleFunction function;
		void *variable_pointer;
	};
} consoleItem;

class ofxConsoleBase{
public:
	// constructor & destructor
	ofxConsoleBase();
	virtual ~ofxConsoleBase();
	
	// adds an item to the console
	void addItem(const std::string & strName,
				 void *pointer, consoleType type);
	
	//adds a function to the console (non boost way), this might look confusing because of the template
	//but it's actually pretty straightforward to use. the template is needed to be able
	//to call functions from any object (not predefined).
	//due to compiler issues with templates in .cpp files it's all put in here.
	template <class T>
	void addFunction(const std::string & strName, T * objpointer, void(T::*fctpointer)(const std::vector<std::string> & args)){
		consoleItem it;
		it.name = strName;
		it.type = CTYPE_FUNCTION;
		
		ofxConsoleFunctorNb<T> * eventHolder = new ofxConsoleFunctorNb<T>();
		eventHolder->set(objpointer, fctpointer);
		it.function = (ofxConsoleFunctor*) eventHolder;
		
		itemList.push_back(it);
	}
	
	// sets the command that is to be executed when no commandline match is found (non boost way)
	//this can currently only be done the non boost way :)
	template <class T>
	void setDefaultCommand(T * objpointer, void(T::*fctpointer)(const std::vector<std::string> & args)) { 
		ofxConsoleFunctorNb<T> * eventHolder = new ofxConsoleFunctorNb<T>();
		eventHolder->set(objpointer, fctpointer);
		defaultCommand = (ofxConsoleFunctor*) eventHolder;
	}

#ifdef USE_BOOST
	//addFunction for a boost callback with no parameter
	void addFunction(const std::string & strName, const boost::function<void()> & f){
		consoleItem it;
		it.name = strName;
		it.type = CTYPE_FUNCTION;
		
		ofxConsoleFunctor0 * eventHolder = new ofxConsoleFunctor0(f);
		it.function = (ofxConsoleFunctor*) eventHolder;
		
		itemList.push_back(it);
	}
/*this generates 9 more addFunction templates so you can add void functions with
 up to 9 parameters!*/
#define BOOST_PP_ITERATION_LIMITS ( 1, 9)
	
//link to the file which gets included for each iteration!
#define BOOST_PP_FILENAME_1 "ofxConsoleItemGenerator.h"
#include BOOST_PP_ITERATE()
	
#endif
	
	// deletes an item/function from the console
	void deleteItem(const std::string & strName);
	
	// prints text on the console
	void print(const std::string & strText);
	void print(const std::string & strText, consoleTextType textType);
	
	// sets commandline buffer size
	void setCommandBufferSize(int size);
	
	// clears the console's commandline text buffer
	void clearCommandBuffer() { commandBuffer.clear(); }
	
	// sets text buffer size
	void setTextBufferSize(int size);
	
	// clears the console's text buffer
	void clearTextBuffer() { textBuffer.clear(); }
	
	// changes the commandline text to a previous one
	void historyGo(int where);
	
	// text key hit
	void passKey(char key);
	
	// clears one character
	void passBackspace();
	
	// executes
	void passIntro();
	
	// draw the console
	virtual void render() = 0;
	
protected:
	// parses command line
	void parseCommandLine();
	
	// holds the command line
	std::string commandLine;
	
	// command to be executed by default
	consoleFunction defaultCommand;
	
	// actual line index (in history)
	int lineIndex;
	
	// buffers size
	int commandBufferSize;
	int textBufferSize;
	
	// holds the text buffer
	std::list<consoleText> textBuffer;
	
	// holds the buffer for previously entered commands
	std::vector<consoleText> commandBuffer;
	
	//all registered console elements;
	std::list<consoleItem>  itemList;
	
	//bools
	bool bCommandEcho;
};

#endif
