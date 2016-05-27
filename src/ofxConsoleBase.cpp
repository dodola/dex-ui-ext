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

#include "ofxConsoleBase.h"

ofxConsoleBase::ofxConsoleBase(){
	lineIndex = 0;
	defaultCommand = NULL;
	commandBufferSize = 20;
	textBufferSize = 50;
	
	//init bools
	bCommandEcho = true;
}

ofxConsoleBase::~ofxConsoleBase()
{
	commandBuffer.clear();
	textBuffer.clear();
}

void ofxConsoleBase::print(const std::string & strTxt)
{
	// push it
	textBuffer.push_back((consoleText&)strTxt);
	textBuffer.back().textType = CTEXT_COPY;
	
	// if we are out of bounds
	if(textBuffer.size() > textBufferSize)
		textBuffer.pop_front();
}

void ofxConsoleBase::print(const std::string & strTxt, consoleTextType textType)
{
	// push it
	textBuffer.push_back((consoleText&)strTxt);
	textBuffer.back().textType = textType;
	
	// if we are out of bounds
	if(textBuffer.size() > textBufferSize)
		textBuffer.pop_front();
}

void ofxConsoleBase::parseCommandLine()
{
	std::ostringstream out;
	std::string::size_type index = 0;
	std::vector<std::string> arguments;
	std::list<consoleItem>::const_iterator iter;
	bool bCommandFound = false;
	
	// add to text buffer
	if(bCommandEcho)
	{
		print(commandLine);
	}
	
	// add to commandline buffer
	commandBuffer.push_back((consoleText&)commandLine);
	if(commandBuffer.size() > commandBufferSize) commandBuffer.erase(commandBuffer.begin());
	
	// tokenize
	std::stringstream commandStream(commandLine); 
	std::string temp;
	while(getline(commandStream, temp, ' '))
		arguments.push_back(temp);
	
	// execute
	for(iter = itemList.begin(); iter != itemList.end(); ++iter)
	{
		if(iter->name == arguments[0] || "/"+iter->name == arguments[0])
		{
			bCommandFound = true;
			switch(iter->type)
			{
				case CTYPE_UCHAR:
					if(arguments.size() > 2)
					{
						
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((unsigned char *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
					}
					else if(arguments.size() == 2)
					{
						*((unsigned char *)(*iter).variable_pointer) = (unsigned char) atoi(arguments[1].c_str());
					}
					break;
					
				case CTYPE_CHAR:
					if(arguments.size() > 2)
					{
						
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((char *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					else if(arguments.size() == 2)
					{
						*((char *)(*iter).variable_pointer) = (char) atoi(arguments[1].c_str());
						
					}
					break;
					
				case CTYPE_UINT:
					if(arguments.size() > 2)
					{
						
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((unsigned int *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					if(arguments.size() == 2)
					{
						*((unsigned int *)(*iter).variable_pointer) = (unsigned int) atoi(arguments[1].c_str());
						
					}
					break;
					
				case CTYPE_INT:
					if(arguments.size() > 2)
					{
						print("Too many arguments for < CTYPE_INT >", CTEXT_ERROR);
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((int *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					if(arguments.size() == 2)
					{
						*((int *)(*iter).variable_pointer) = (int) atoi(arguments[1].c_str());
						
					}
					break;
				case CTYPE_BOOL:
					if(arguments.size() > 2)
					{
						print("Too many arguments for < CTYPE_BOOL >", CTEXT_ERROR);
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((bool *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					if(arguments.size() == 2)
					{
						*((bool *)(*iter).variable_pointer) = (bool) atoi(arguments[1].c_str());
					}
					break;
				case CTYPE_FLOAT:
					if(arguments.size() > 2)
					{
						print("Too many arguments for < CTYPE_FLOAT >", CTEXT_ERROR);
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((float *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					if(arguments.size() == 2)
					{
						*((float *)(*iter).variable_pointer) = (float)atof(arguments[1].c_str());
					}
					break;
					
				case CTYPE_STRING:
					if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << (std::string *)(*iter).variable_pointer;
						print(out.str());
						
					}
					else if(arguments.size() > 1)
					{
						// reset variable_pointeriable
						*((std::string *)(*iter).variable_pointer) = "";
						
						// add new string
						for(int i = 0; i < arguments.size(); ++i)
						{
							*((std::string *)(*iter).variable_pointer) += arguments[i];
						}
						
					}
					break;
					
				case CTYPE_FUNCTION:
					
					ofxConsoleFProps * fProps = (*iter).function->callBack(arguments);
					
					if(fProps->err.size()){
					    std::string args;
						for(int i=0; i<fProps->numArgs; i++){
							args+=fProps->arguments[i];
							if(i<fProps->numArgs-1){
								args+=", ";
							}
						}
						for(int i=0; i<fProps->err.size(); i++){
							print("< "+(*iter).name+"("+args+") > "+fProps->err[i], CTEXT_ERROR);
						}
						
						fProps->err.clear();
					}
					
					break;
////					
//				default:
//					if(defaultCommand!=NULL) defaultCommand->callBack(arguments);
//					break;
			}
		} 
	}
	
	if(!bCommandFound) {
		if(defaultCommand!=NULL) defaultCommand->callBack(arguments);
	}
}

void ofxConsoleBase::passKey(char key)
{
	// if we can print the char
	if(key > 31) commandLine += key;
}

void ofxConsoleBase::passBackspace()
{
	if(commandLine.length() > 0)
	{
		commandLine.erase(commandLine.end()-1);
	}
}

void ofxConsoleBase::passIntro()
{
	if(commandLine.length() > 0)
	{
		parseCommandLine();
		commandLine.clear();
		lineIndex=commandBuffer.size();
	}
}

void ofxConsoleBase::addItem(const std::string & strName, void *pointer, consoleType type){
	consoleItem it;
	
	// fill item properties
	it.name = strName;
	it.type = type;
	
	// address
	if(type != CTYPE_FUNCTION)
	{
		it.variable_pointer = pointer;
	}
	
	// add item
	itemList.push_back(it);
}

void ofxConsoleBase::deleteItem(const std::string & strName)
{
	std::list<consoleItem>::iterator iter;
	
	// find item
	for (iter = itemList.begin(); iter != itemList.end(); ++iter)
	{
		if (iter->name == strName)
		{
			itemList.erase(iter);
			break;
		}
	}
}

void ofxConsoleBase::setTextBufferSize(int size)
{
	textBufferSize = size;
	
	// remove any extra line
	if(textBuffer.size() > textBufferSize)
	{
		std::list<consoleText>::iterator it;
		it = textBuffer.end();
		advance(it, -textBufferSize);
		textBuffer.erase(textBuffer.begin(), it);
	}
}

void ofxConsoleBase::setCommandBufferSize(int size)
{
	commandBufferSize = size;
	
	// remove any extra line
	if(commandBuffer.size() > commandBufferSize)
	{
		std::vector<consoleText>::iterator it;
		//advance(it, -commandBufferSize);
		it = commandBuffer.end() - commandBufferSize;
		commandBuffer.erase(commandBuffer.begin(), it);
	}
}

void ofxConsoleBase::historyGo(int where)
{
	// increment line index
	lineIndex += where;
	
	// error correct line index
	if(lineIndex < 0)
	{
		lineIndex = 0;
	}
	else if(lineIndex >= commandBuffer.size())
	{
		lineIndex = commandBuffer.size() - 1;
	}
	
	// set command line
	if(!commandBuffer.empty())
	{
		commandLine = commandBuffer[lineIndex];
	}
}
