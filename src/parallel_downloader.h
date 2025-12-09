#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/http_client.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

enum RequestType {
	REQUEST_TYPE_IMAGE,
	REQUEST_TYPE_STRING,
	REQUEST_TYPE_JSON,
	REQUEST_TYPE_DATA
};

struct Request {
	String url;
	Callable callback;
	PackedStringArray headers;
	RequestType type;
};

class ParallelDownloader : public Node {
	GDCLASS(ParallelDownloader, Node)
public :
	ParallelDownloader();
	int max_parallel_request = 1;
	String host;
	bool debug_enabled = false;

	virtual void _process(double delta) override;
	
protected :
	Vector<Ref<HTTPClient>> http_clients;
	Vector<Ref<HTTPClient>> available_http_clients;
	HashMap<Ref<HTTPClient>, Request *> active_requests;
	Vector<Request *> request_queue; 

	static void _bind_methods();
	void _download_request(Request *p_request);
	void _start_request(Ref<HTTPClient> requester, Request *p_request);
	void _on_response(Ref<HTTPClient> http_client, Request *p_request);
	
public:
	void set_max_parallel_request(int p_value);
	int get_max_parallel_request(){ return max_parallel_request;}
	
	void set_host(String p_value);
	String get_host() { return host;}

	void set_debug_enabled(bool p_value) { debug_enabled = p_value; }
	bool is_debug_enabled() { return debug_enabled; }
	
	void download_image(String url, Callable p_callback, PackedStringArray headers = PackedStringArray());
	void download_json(String url, Callable p_callback, PackedStringArray headers = PackedStringArray());
	void download_string(String url, Callable p_callback, PackedStringArray headers = PackedStringArray());
	void download_data(String url, Callable p_callback, PackedStringArray headers = PackedStringArray());

	void cancel_download(String url);
	void close_all();
};