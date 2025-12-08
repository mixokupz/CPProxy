#include <iostream>
#include "proxy/Proxy.h"
#include <atomic>
#include <csignal>

static proxy::Proxy* g_proxy_instance = nullptr;
static std::atomic<bool> g_shutdown_forced {false};

void sigint_handler(int sig){
	if(g_shutdown_forced.exchange(true)){
		exit(1);
	}
	if(g_proxy_instance){
		g_proxy_instance->request_shutdown();
	}
	alarm(15);
}
void sigalrm_handler(int sig){
	PLOGI << "Graceful shutdown time out => Forcing exit";
	exit(1);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "USAGE: " << argv[0] << " <PORT>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    std::cout << "Proxy is ready!" << std::endl << "listaning at " << port << std::endl;

    proxy::Proxy proxy(port);
    g_proxy_instance = &proxy;

    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalrm_handler);
    try{
    	proxy.run();
    } catch(const std::system_error& e){
	    if(!g_proxy_instance->is_shutting_down()){
	    	PLOGI << "Fatal error:" << e.what();
		return 1;
	    }
    
    }
    proxy.wait_for_threads_shutdown();
    return 0;
}
