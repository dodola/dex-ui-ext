# define n BOOST_PP_ITERATION()
template <BOOST_PP_ENUM_PARAMS(n, class T)> 
void addFunction(const std::string & strName, const boost::function< void(BOOST_PP_ENUM_PARAMS(n, T)) > & f) {
	consoleItem it;
	it.name = strName;
	it.type = CTYPE_FUNCTION;
	BOOST_PP_REPEAT(1, CONSTRUCTORNAME, n)<BOOST_PP_ENUM_PARAMS(n, T)> * holder = 
	new BOOST_PP_REPEAT(1, CONSTRUCTORNAME, n)<BOOST_PP_ENUM_PARAMS(n, T)>(f);
	it.function = (ofxConsoleFunctor*)(holder);
	itemList.push_back(it);
};
#undef n