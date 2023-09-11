Location::Location()
{
	this->_path = "";
	this->_root = "";
	this->_listing = false;
	this->_index = "";
	this->client_body_size = 0;
}

void	Location::setPath(std::string path)
{
	this->_path = path;
}

void	Location::setRoot(std::string path)
{
	if (checkFile(path) != 2)
		throw std::invalid_argument("root in location is not a directory");
	this->_root = path;
}

void	Location::setIndex(std::string path)
{
	this->_index = path;
}

void	Location::setListing(std::string option)
{
	if (option != "on" && option != "off")
		throw std::invalid_argument("listing must be on or off");
	else
		this->_listing = (option == "on");
}
