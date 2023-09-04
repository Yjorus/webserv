bool	Server::checkHost(std::string ip)
{
	size_t	a,b = 0;
	int d;
	char e;
	std::vector<std::string>	split;
	while (true)
	{
		b = ip.find(".", a);
		if (b == std::string::npos)
			break;
		std::string hold = ip.substr(a, b - a);
		split.push_back(hold);
		if (b + 1 >= ip.size())
			break;
		a = b + 1;
	}
	if (split.size() != 4)
		return (true);
	for (a = 0; a < 4; a++)
	{
		d = -1;
		std::stringstream ss(split[a]);
		ss >> d >> e;
		if (d < 0 || d > 255 || e)
			return (true);
	}
	return (false);
}