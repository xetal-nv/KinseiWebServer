#include "HTTPlibs/client_http.hpp"
#include "HTTPlibs/server_http.hpp"

// Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// Added for the default_resource example
#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>


//Client Kinsei
#include "Kinsei/KinseiDeviceUtility.h" //Utilities


using namespace std;
// Added for the json-example:
using namespace boost::property_tree;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;






int main() {

  KinseiDeviceUtility kinsei_device_utility("192.168.2.172",2005);

  if(kinsei_device_utility.IsConnected()==false){
    cout<<"Device not connected"<<endl;
    exit(-1);
  }

  if(kinsei_device_utility.StartThread() == false){
    cout<<"Error initializing the kinsei device utility thread"<<endl;
    exit(-1);
  }
  // HTTP-server at port 8080 using 1 thread
  // Unless you do more heavy non-threaded processing in the resources,
  // 1 thread is usually faster than several threads
  HttpServer server;
  server.config.port = 8080;

  cout<<"HttpServer running"<<endl;



  server.resource["^/sendconfig$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    try {
        ptree pt;
        read_json(request->content, pt);

        //auto name=pt.get<string>("firstName")+" "+pt.get<string>("lastName");
        auto name="TODO:Read json post and apply it to the configuration file";
        response->write(name);
    }
    catch(const exception &e) {
        response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
    }
  };

  // Read the TrackingServer Configuration
  server.resource["^/readconfig$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    cout << "Request from " << request->remote_endpoint_address() << ":" << request->remote_endpoint_port() << endl;
    response->write("TODO:Needs to read the config file and send it back in json format");
  };

  //Request
  server.resource["^/kinseiRoomRequest$"]["GET"] = [&kinsei_device_utility](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    string test;
    if(kinsei_device_utility.IsThreadRunning() == false) test.assign("");
    else kinsei_device_utility.GetJsonString(test);

    //cout << "Request from " << request->remote_endpoint_address() << ":" << request->remote_endpoint_port() << endl;

    //cout << "Query Fields="<<endl;
    auto query_fields = request->parse_query_string();
    for(auto &field : query_fields){
      if(field.first.compare("callback") ==0) test=field.second+"("+test+")";
      //cout << field.first << ": " << field.second << endl;

    }

    response->write(test);
  };





  // Default GET-example. If no other matches, this anonymous function will be called.
  // Will respond with content in the web/-directory, and its subdirectories.
  // Default file: index.html
  // Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
  server.default_resource["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    try {
      auto web_root_path = boost::filesystem::canonical("web");
      auto path = boost::filesystem::canonical(web_root_path / request->path);
      // Check if path is within web_root_path
      if(distance(web_root_path.begin(), web_root_path.end()) > distance(path.begin(), path.end()) ||
         !equal(web_root_path.begin(), web_root_path.end(), path.begin()))
        throw invalid_argument("path must be within root path");
      if(boost::filesystem::is_directory(path))
        path /= "index.html";

      SimpleWeb::CaseInsensitiveMultimap header;

      // Uncomment the following line to enable Cache-Control
      // header.emplace("Cache-Control", "max-age=86400");

      auto ifs = make_shared<ifstream>();
      ifs->open(path.string(), ifstream::in | ios::binary | ios::ate);

      if(*ifs) {
        auto length = ifs->tellg();
        ifs->seekg(0, ios::beg);

        header.emplace("Content-Length", to_string(length));
        response->write(header);

        // Trick to define a recursive function within this scope (for example purposes)
        class FileServer {
        public:
          static void read_and_send(const shared_ptr<HttpServer::Response> &response, const shared_ptr<ifstream> &ifs) {
            // Read and send 128 KB at a time
            static vector<char> buffer(131072); // Safe when server is running on one thread
            streamsize read_length;
            if((read_length = ifs->read(&buffer[0], static_cast<streamsize>(buffer.size())).gcount()) > 0) {
              response->write(&buffer[0], read_length);
              if(read_length == static_cast<streamsize>(buffer.size())) {
                response->send([response, ifs](const SimpleWeb::error_code &ec) {
                  if(!ec)
                    read_and_send(response, ifs);
                  else
                    cerr << "Connection interrupted" << endl;
                });
              }
            }
          }
        };
        FileServer::read_and_send(response, ifs);
      }
      else
        throw invalid_argument("could not read file");
    }
    catch(const exception &e) {
      response->write(SimpleWeb::StatusCode::client_error_bad_request, "Could not open path " + request->path + ": " + e.what());
    }
  };

  server.on_error = [](shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
    // Handle errors here
  };

  thread server_thread([&server]() {
    // Start server
    server.start();
  });



  server_thread.join();
}
