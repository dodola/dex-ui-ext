# define n BOOST_PP_ITERATION()
template<BOOST_PP_ENUM_PARAMS(n, class T)>
BOOST_PP_REPEAT(1, CLASSNAME, n)
public:

BOOST_PP_REPEAT(1, CONSTRUCTORNAME, n)(const boost::function< void(BOOST_PP_ENUM_PARAMS(n, T)) > & f){
	cb = f;
	properties.numArgs = n;
	size_t len;
	int s;
	char * p;
	
	BOOST_PP_REPEAT(n, TEMPNAME, ~);
}

boost::function<void(BOOST_PP_ENUM_PARAMS(n, T))> cb;

virtual ofxConsoleFProps * callBack(const std::vector<std::string> & args){
	if(args.size()!=properties.numArgs+1){
        properties.err.push_back("wrong number of arguments!");
		return &properties;
	}
	BOOST_PP_REPEAT(n, PARAMS, T)
	if(!properties.err.size()) {
		cb(BOOST_PP_ENUM_PARAMS(n, arg));
		return &properties;
	} else {
		return &properties;
	}
}
};
#undef n