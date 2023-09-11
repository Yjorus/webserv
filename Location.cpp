Location::Location()
{
	this->_path = "";
	this->_root = "";
	this->_listing = false;
	this->_index = "";
	this->_
}

void	Location::setPath(std::string path)
{
	this->_path = path;
}

void	Location::setRoot(std::string path)
{
	if (checkFile(path) != 2)
		throw std::invalid_argument("root in location is not a directory");
	this->_path = path;
}