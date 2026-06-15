#include "http.h"

void c_async_http_mgr::init()
{
	curl_global_init(CURL_GLOBAL_ALL);
	m_multi_handle = curl_multi_init();

	m_running = true;
	m_wthread = std::thread(&c_async_http_mgr::update_thread, this);
}

void c_async_http_mgr::undo()
{
	if (!m_running)
		return;

	m_running = false;

	if (m_multi_handle)
		curl_multi_wakeup(m_multi_handle);

	if (m_wthread.joinable())
		m_wthread.join();

	if (m_multi_handle) {
		curl_multi_cleanup(m_multi_handle);
		m_multi_handle = nullptr;
	}

	curl_global_cleanup();
}

void c_async_http_mgr::add_get_request(const std::string& url, http_callback_t callback)
{
	if (!m_running)
		return;

	CURL* eh = curl_easy_init();
	if (!eh)
		return;

	http_ctx_t* ctx = new http_ctx_t();

	ctx->handle = eh;
	ctx->callback = callback;

	curl_easy_setopt(eh, CURLOPT_URL, url.c_str());
	curl_easy_setopt(eh, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36 Valve Steam Client");
	curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(eh, CURLOPT_WRITEDATA, ctx);
	curl_easy_setopt(eh, CURLOPT_PRIVATE, ctx);
	curl_easy_setopt(eh, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(eh, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(eh, CURLOPT_TIMEOUT, 5L);

	std::lock_guard<std::mutex> lock(m_mutex);
	curl_multi_add_handle(m_multi_handle, eh);
}

void c_async_http_mgr::update_thread()
{
	int still_running = 0;

	while (m_running)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		curl_multi_perform(m_multi_handle, &still_running);

		CURLMsg* msg  = nullptr;
		int      msgq = 0;

		while ((msg = curl_multi_info_read(m_multi_handle, &msgq)))
		{
			if (msg->msg == CURLMSG_DONE)
			{
				CURL* eh = msg->easy_handle;

				http_ctx_t* ctx = nullptr;
				curl_easy_getinfo(eh, CURLINFO_PRIVATE, &ctx);

				http_res_t resp;
				resp.status = (msg->data.result == CURLE_OK);
				resp.response_code = -1;

				if (resp.status) {
					curl_easy_getinfo(eh, CURLINFO_RESPONSE_CODE, &resp.response_code);
					resp.body = ctx->response;
				}

				m_requests.push({ ctx->callback, resp });

				curl_multi_remove_handle(m_multi_handle, eh);
				curl_easy_cleanup(eh);

				delete ctx;
			}
		}

		lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void c_async_http_mgr::poll()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	while (!m_requests.empty())
	{
		auto& [callback, resp] = m_requests.front();

		callback(resp);
		m_requests.pop();
	}
}