class	RunServer
{
	public:
		RunServer();
		~RunServer();
		RunServer(RunServer const &copy);
		RunServer	&operator=(RunServer const &assign);

		void	setupServers(std::vector<Server> servers);
		void	connectClient(Server &server);
		void	removeClient(int a);
		void	serverLoop();
		void	addToSet(int a, fd_set &set);
		void	removeFromSet(int a, fd_set &set);
		void	setupSets();
		void	readRequest(int a, Client &client);
};