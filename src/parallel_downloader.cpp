#include <godot_cpp/classes/json.hpp>

#include "parallel_downloader.h"


void ParallelDownloader::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_max_parallel_request", "value"), &ParallelDownloader::set_max_parallel_request);
	ClassDB::bind_method(D_METHOD("get_max_parallel_request"), &ParallelDownloader::get_max_parallel_request);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_parallel_request"), "set_max_parallel_request", "get_max_parallel_request");

	ClassDB::bind_method(D_METHOD("set_host", "host"), &ParallelDownloader::set_host);
	ClassDB::bind_method(D_METHOD("get_host"), &ParallelDownloader::get_host);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "host"), "set_host", "get_host");

	ClassDB::bind_method(D_METHOD("set_debug_enabled", "enabled"), &ParallelDownloader::set_debug_enabled);
	ClassDB::bind_method(D_METHOD("is_debug_enabled"), &ParallelDownloader::is_debug_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug_enabled"), "set_debug_enabled", "is_debug_enabled");

	ClassDB::bind_method(D_METHOD("download_image", "image_url", "callback", "headers"), &ParallelDownloader::download_image, DEFVAL(PackedStringArray()));
	ClassDB::bind_method(D_METHOD("close_all"), &ParallelDownloader::close_all);
}

ParallelDownloader::ParallelDownloader() {
	set_max_parallel_request(1);
}

void ParallelDownloader::set_host(String p_value) {
	ERR_FAIL_COND_EDMSG(active_requests.is_empty() == false, "Cannot change host while there are active requests.");
	if (host == p_value) {
		return;
	}
	if (p_value.is_empty()) {
		if (debug_enabled) {
			print_line("[Debug] Empty host set. closing all http clients.");
		}
		close_all();
		return;
	}
	
	host = p_value;
	if (debug_enabled) {
		print_line("[Debug] Setting host to: " + host);
	}
	for(Ref<HTTPClient> http_client : http_clients ) {
		http_client->close();
		Error err = http_client->connect_to_host(host);
		if (err != OK) {
			ERR_PRINT_ED("HTTP Client failed to connect to host: " + host + ". Error code: " + itos(err));
		}
	}
	available_http_clients.clear();
}

void ParallelDownloader::_process(double delta) {
	
	for (Ref<HTTPClient> http_client : http_clients )
	{
		HTTPClient::Status last_status = http_client->get_status();
		http_client->poll();
		Error err;
		switch (http_client->get_status())
		{
		case HTTPClient::STATUS_DISCONNECTED:
		case HTTPClient::STATUS_CONNECTION_ERROR:
			if (host.is_empty()) {
				break;
			}
			if (debug_enabled) {
				WARN_PRINT_ED("[Debug] HTTP Client disconnected from host: " + host + ". Attempting to reconnect...");
			}
			http_client->close();
			err = http_client->connect_to_host(host);
			if (err != OK) {
				ERR_PRINT_ED("HTTP Client failed to connect to host: " + host + ". Error code: " + itos(err));
			}
			break;
		case HTTPClient::STATUS_CANT_RESOLVE:
			ERR_PRINT_ED("DNS failure: Can't resolve the hostname for the given URL. Host : " + host);
			break;
		case HTTPClient::STATUS_CANT_CONNECT:
			ERR_PRINT_ED("Error in HTTP connection with host : " + host);
			break;
		case HTTPClient::STATUS_TLS_HANDSHAKE_ERROR:
			ERR_PRINT_ED("TLS Handshake failure with host : " + host);
			break;
		case HTTPClient::STATUS_CONNECTED:
			if (available_http_clients.has(http_client)){
				break;
			}
			available_http_clients.push_back(http_client);
			if (debug_enabled) {
				print_line("[Debug] HTTP Client connected to host: " + host);
			}
			break;
		case HTTPClient::STATUS_BODY:
		
			if (debug_enabled) {
				print_line("[Debug] HTTP Client received response from host: " + host);
			}

			_on_response(http_client, active_requests[http_client]); 
			break;
		case HTTPClient::STATUS_REQUESTING:
			if (debug_enabled) {
				print_line("[Debug] HTTP Client is requesting data from host: " + host);
			}
			break;
		case HTTPClient::STATUS_CONNECTING:
			if (debug_enabled) {
				print_line("[Debug] HTTP Client is connecting to host: " + host);
			}
			break;
		case HTTPClient::STATUS_RESOLVING:
			if (debug_enabled) {
				print_line("[Debug] HTTP Client is resolving host: " + host);
			}
			break;
		default:
			break;
		}
	}

	while (http_clients.size() > max_parallel_request && !available_http_clients.is_empty()) {
		int client_index = available_http_clients.size() - 1;
		Ref<HTTPClient> http_client = available_http_clients[client_index];
		available_http_clients.remove_at(client_index);

		http_clients.erase(http_client);
		if (debug_enabled) {
			print_line("[Debug] Removed excess HTTP Client. Total clients: " + itos(http_clients.size()));
		}
	}
		

	while (!request_queue.is_empty() && available_http_clients.size() > 0) {
		if (debug_enabled) {
			print_line("[Debug] Starting queued download request. Queue size: " + itos(request_queue.size()));
		}
		int client_index = available_http_clients.size() - 1;
		Ref<HTTPClient> http_client = available_http_clients[client_index];
		available_http_clients.remove_at(client_index);

		Request *request = request_queue[0];
		request_queue.remove_at(0);

		_start_request(http_client, request);
	}
	
}

void ParallelDownloader::download_image(String url, Callable p_callback, PackedStringArray headers) {
	

	Request *request = memnew(Request);
	request->url = url;
	request->callback = p_callback;
	request->headers = headers;
	request->type = REQUEST_TYPE_IMAGE;

	_download_request(request);
}
void ParallelDownloader::download_json(String url, Callable p_callback, PackedStringArray headers) {
	

	Request *request = memnew(Request);
	request->url = url;
	request->callback = p_callback;
	request->headers = headers;
	request->type = REQUEST_TYPE_JSON;

	_download_request(request);
}
void ParallelDownloader::download_string(String url, Callable p_callback, PackedStringArray headers) {
	

	Request *request = memnew(Request);
	request->url = url;
	request->callback = p_callback;
	request->headers = headers;
	request->type = REQUEST_TYPE_STRING;

	_download_request(request);
}
void ParallelDownloader::download_data(String url, Callable p_callback, PackedStringArray headers) {
	

	Request *request = memnew(Request);
	request->url = url;
	request->callback = p_callback;
	request->headers = headers;
	request->type = REQUEST_TYPE_DATA;

	_download_request(request);
}

void ParallelDownloader::_download_request(Request *p_request){

	ERR_FAIL_COND_EDMSG(host.is_empty(), "Host is not set. Please set the host before making download requests.");

	if (p_request->url.begins_with("https://") && host.begins_with("http://")) {
		p_request->url = p_request->url.substr(8);
		p_request->url = "http://" + p_request->url;
	} else if (p_request->url.begins_with("http://") && host.begins_with("https://")) {
		p_request->url = p_request->url.substr(7);
		p_request->url = "https://" + p_request->url;
	}

	ERR_FAIL_COND_EDMSG(!p_request->url.begins_with(host), "The requested URL host does not match the configured host. requested URL: " + p_request->url + " Configured host: " + host);
	p_request->url = p_request->url.trim_prefix(host);
	if (debug_enabled) {
		print_line("[Debug] Queued download request for URL: " + p_request->url);
	}
	
	if (available_http_clients.is_empty()) {
		request_queue.push_back(p_request);
	}
	else {
		int client_index = available_http_clients.size() - 1;
		Ref<HTTPClient> http_client = available_http_clients[client_index];
		available_http_clients.remove_at(client_index);
		_start_request(http_client, p_request);
	}
}

void ParallelDownloader::_start_request(Ref<HTTPClient> requester, Request *p_request){
	requester->request(HTTPClient::METHOD_GET, p_request->url, p_request->headers);
	active_requests[requester] = p_request;

	if (debug_enabled) {
		print_line("[Debug] Started download request for URL: " + p_request->url);
	}
}


void ParallelDownloader::_on_response(Ref<HTTPClient> http_client, Request *p_request) {
	int response_code = http_client->get_response_code();
	ERR_FAIL_COND_EDMSG(response_code/100 != 2, "HTTP request failed with response code: " + itos(response_code) + " for URL: " + p_request->url);
	PackedByteArray body;
	while (http_client->get_status() == HTTPClient::STATUS_BODY) {
		PackedByteArray chunk = http_client->read_response_body_chunk();
		body.append_array(chunk);
		http_client->poll();
	}
	if (debug_enabled) {
		print_line("[Debug] Downloaded " + itos(body.size()) + " bytes from URL: " + p_request->url);
	}
	if (p_request->type == REQUEST_TYPE_IMAGE){
		Ref<Image> image = memnew(Image);
		Error error;
		if (p_request->url.ends_with(".png")) {
			error = image->load_png_from_buffer(body);
		} else if (p_request->url.ends_with(".jpg") || p_request->url.ends_with(".jpeg")) {
			error = image->load_jpg_from_buffer(body);
		} else if (p_request->url.ends_with(".webp")) {
			error = image->load_webp_from_buffer(body);
		} else if (p_request->url.ends_with(".bmp")) {
			error = image->load_bmp_from_buffer(body);
		} else if (p_request->url.ends_with(".tga")) {
			error = image->load_tga_from_buffer(body);
		} else if (p_request->url.ends_with(".ktx")) {
			error = image->load_ktx_from_buffer(body);
		} else if (p_request->url.ends_with(".svg")) {
			error = image->load_svg_from_buffer(body);
		} else {
			ERR_FAIL_EDMSG("Unsupported image format for URL: " + p_request->url + ". Supported formats are: png, jpg, jpeg, webp, bmp, tga, ktx, svg. if the format is correct, please specify the correct file extension in the URL. if this is not possible use download data and make the image from data manually.");
		}

		ERR_FAIL_COND_EDMSG(error != OK, "Couldn't load the image from downloaded data.");
		p_request->callback.call(image);
	} else if (p_request->type == REQUEST_TYPE_STRING){
		String body_str = body.get_string_from_utf8();
		p_request->callback.call(body_str);
	} else if (p_request->type == REQUEST_TYPE_JSON){
		String body_str = body.get_string_from_utf8();
		Variant json = JSON::parse_string(body_str);
		p_request->callback.call(json);
	} else if (p_request->type == REQUEST_TYPE_DATA){
		p_request->callback.call(body);
	}
	active_requests.erase(http_client);
	memdelete(p_request);
}
void ParallelDownloader::set_max_parallel_request(int p_value) {
	max_parallel_request = MAX(1, p_value);

	while (http_clients.size() < max_parallel_request){
		Ref<HTTPClient> http_client = memnew(HTTPClient);
		if (!host.is_empty()) {
			if(debug_enabled) {
				print_line("[Debug] HTTP Client connecting to host: " + host);
			}
			Error err = http_client->connect_to_host(host);
			if (err != OK) {
				ERR_PRINT_ED("HTTP Client failed to connect to host: " + host + ". Error code: " + itos(err));
			}
		}
		http_clients.push_back(http_client);
	}
	
};

void ParallelDownloader::cancel_download(String url) {
	if (debug_enabled) {
		print_line("[Debug] Cancelling download for URL: " + url);
	}
	if (request_queue.size() > 0) {
		for (int i = request_queue.size() -1; i >=0 ; i--) {
			Request *request = request_queue[i];
			if (request->url == url) {
				request_queue.remove_at(i);
				if (debug_enabled) {
					print_line("[Debug] Removed queued download request for URL: " + url);
				}
				memdelete(request);
			}
		}
	}
}

void ParallelDownloader::close_all() {
	host = "";

	for(Ref<HTTPClient> http_client : http_clients ) {
		http_client->close();
	}
}
