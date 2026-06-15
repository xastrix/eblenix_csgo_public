#pragma once

#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <queue>
#include <atomic>
#ifndef CURL_STATICLIB
#define CURL_STATICLIB
#endif
#include <curl.h>

struct http_res_t {
	bool status;
	long response_code;
	std::string body;
};

using http_callback_t = std::function<void(const http_res_t&)>;

struct http_ctx_t {
	CURL* handle;
	std::string response;
	http_callback_t callback;
};

using http_requests_t = std::queue<std::pair<http_callback_t, http_res_t>>;

class c_async_http_mgr {
public:
	void init();
	void undo();

	void add_get_request(const std::string& url, http_callback_t callback);

	void update_thread();
	void poll();

	static std::shared_ptr<c_async_http_mgr> make_shared() {
		return std::shared_ptr<c_async_http_mgr>(new c_async_http_mgr());
	}

private:
	static size_t write_cb(void* c, size_t s, size_t nb, void* up) {
		size_t ts = s * nb;
		
		http_ctx_t* ctx = static_cast<http_ctx_t*>(up);
		ctx->response.append(static_cast<char*>(c), ts);

		return ts;
	}

private:
	CURLM*            m_multi_handle;
	std::thread       m_wthread;
	std::mutex        m_mutex;
	std::atomic<bool> m_running;
	http_requests_t   m_requests;
};

inline std::shared_ptr<c_async_http_mgr> g_http = c_async_http_mgr::make_shared();