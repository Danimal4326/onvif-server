/* --------------------------------------------------------------------------
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
 server.cpp
 
 onvif server
----------------------------------------------------------------------------- */

#include <sstream>
#include <thread>

#include "DeviceBinding.nsmap"
#include "soapDeviceBindingService.h"
#include "soapDeviceIOBindingService.h"

#include "soapMediaBindingService.h"
#include "soapImagingBindingService.h"

#include "soapReceiverBindingService.h"

#include "soapRecordingBindingService.h"
#include "soapReplayBindingService.h"
#include "soapSearchBindingService.h"

#include "soapPTZBindingService.h"

#include "soapDisplayBindingService.h"

#include "soapEventBindingService.h"
#include "soapPullPointSubscriptionBindingService.h"
#include "soapNotificationProducerBindingService.h"
#include "soapSubscriptionManagerBindingService.h"

// ws-discovery
#include "wsseapi.h"
#include "wsd-server.h"

#include "onvif_impl.h"

#include <yaml-cpp/yaml.h>

int http_get(struct soap *soap)
{
	int retCode = 404;
	FILE *fd = fopen(soap->path + 1, "rb"); 
	if (fd != NULL)
	{
		if (!soap_tag_cmp(soap->path, "*.html"))
			soap->http_content = "text/html"; 
		if (!soap_tag_cmp(soap->path, "*.wsdl"))
			soap->http_content = "text/xml"; 
		soap_response(soap, SOAP_FILE);
		for (;;)
		{
			size_t r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
			if ( (r == 0) || (soap_send_raw(soap, soap->tmpbuf, r)) )
				break;
		}
		fclose(fd);
		soap_end_send(soap);
		retCode = SOAP_OK;
	}
	return retCode;
} 

#define FOREACH_SERVICE(APPLY,soap) \
		APPLY(DeviceBindingService,soap)    \
		APPLY(DeviceIOBindingService,soap)  \
		\
		APPLY(MediaBindingService,soap)     \
		APPLY(ImagingBindingService,soap)   \
                \
		APPLY(PTZBindingService,soap)   \
                \
		APPLY(RecordingBindingService,soap) \
		APPLY(ReplayBindingService,soap)    \
		APPLY(SearchBindingService,soap)    \
		\
		APPLY(ReceiverBindingService,soap)  \
		\
		APPLY(DisplayBindingService,soap)   \
		\
		APPLY(EventBindingService,soap)                 \
		APPLY(PullPointSubscriptionBindingService,soap) \
		APPLY(NotificationProducerBindingService,soap)  \
		APPLY(SubscriptionManagerBindingService,soap)  \
/**/

#define DECLARE_SERVICE(service,soap) service service ## _inst(soap);

#define DISPATCH_SERVICE(service,soap) \
				else if (service ## _inst.dispatch() != SOAP_NO_METHOD) {\
					soap_send_fault(soap); \
					soap_stream_fault(soap, std::cerr); \
				}


int main(int argc, char* argv[])
{		
	std::string username;
	int httpport = 8080;
	ServiceContext deviceCtx;
	std::string config_path;

	// int c = 0;
	// while ((c = getopt (argc, argv, "h" "u:p:" "H:" "c:")) != -1)
	// {
	// 	switch (c)
	// 	{
	// 		case 'H':	httpport  = atoi(optarg); break;
	// 		case 'u':	username  = optarg; break;
	// 		case 'p':	deviceCtx.m_userList[username] = User(optarg,tt__UserLevel__Administrator); break;
	// 		case 'c':   config_path = optarg; break;
	// 		case 'h':
	// 			std::cout << argv[0] << " [-H http port] [-R rtsp port] [-u username] [-p password]" << std::endl;
	// 			exit(0);
	// 		break;
	// 	}
	// }

	int c = 0;
	while ((c = getopt(argc, argv, "h" "c:")) != -1)
	{
		switch(c)
		{
			case 'c':
				config_path = optarg;
				break;
			case 'h':
				std::cout << argv[0] << " [-c config]" << std::endl;
				exit(0);
		}
	}

	// check if path is provided
	if(config_path.length() == 0)
	{
		std::cerr << "Path to a config file for RTSP streams is required." << std::endl;
		exit(0);
	}

	// open yaml file
	YAML::Node config = YAML::LoadFile(config_path);
	YAML::Node server = config["server"];
	if(!server || server.Type() != YAML::NodeType::Map)
	{
		// bad bad bad
		exit(0);
	}

	std::cout << "Username:  " << server["username"].as<std::string>() << std::endl;
	std::cout << "Password:  " << server["password"].as<std::string>() << std::endl;
	deviceCtx.m_userList[server["username"].as<std::string>()] = User(server["password"].as<std::string>(), tt__UserLevel__Administrator);
	
	std::cout << "HTTP Port: " << server["http_port"].as<std::int16_t>() << std::endl;
	deviceCtx.m_port = server["http_port"].as<std::int16_t>();
	std::cout << "# Devices: " << server["devices"].size() << std::endl;

	for(auto device : server["devices"])
	{
		std::string name = device["name"].as<std::string>();
		std::uint16_t port = device["rtsp_port"].as<std::int16_t>();
		std::string uri = device["rtsp_uri"].as<std::string>();

		std::cout << "- Name: " << name << std::endl;
		std::cout << "  Port: " << port << std::endl;
		std::cout << "  URI: "  << uri << std::endl;
		deviceCtx.m_devices[name] = (Device(name, port, uri));
	}

	const time_t timestamp = time(NULL);
	struct tm * localtm = localtime(&timestamp);
	deviceCtx.m_timezone     = localtm->tm_zone;
	deviceCtx.m_isdst        = localtm->tm_isdst;

	// start WS server
	struct soap *soap = soap_new();
	soap->user = (void*)&deviceCtx;
	soap->fget = http_get; 
	soap->bind_flags |= SO_REUSEADDR;
	{			
		FOREACH_SERVICE(DECLARE_SERVICE,soap)

		if (!soap_valid_socket(soap_bind(soap, NULL, deviceCtx.m_port, 100))) 
		{
			soap_stream_fault(soap, std::cerr);
		}
		else
		{
			// start WS-Discovery
			std::ostringstream os;
			os << "http://" << deviceCtx.getLocalIp() << ":" << deviceCtx.m_port << "/onvif/device_service";
			std::string url(os.str());
			std::cout << "Published URL:" << url << std::endl;

			wsdconf conf(url.c_str()
							,"\"http://www.onvif.org/ver10/network/wsdl\":NetworkVideoTransmitter"
							, deviceCtx.getScopesString().c_str()
							, soap_wsa_rand_uuid(soap));

			std::thread wsdd( [&conf] { 
				wsd_server(conf); 
			});

			// SOAP services
			while (soap_accept(soap) != SOAP_OK) 
			{
				if (soap_begin_serve(soap))
				{
					soap_stream_fault(soap, std::cerr);
				}
				FOREACH_SERVICE(DISPATCH_SERVICE,soap)
				else 
				{
					std::cout << "Unknown service" << std::endl;				
				}				
			}

			conf.m_stop = true;
			wsdd.join();
		}
	}
	
	soap_destroy(soap);
	soap_end(soap);
	soap_free(soap); 			
	
	return 0;
}
