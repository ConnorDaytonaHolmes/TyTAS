#include <coroutine>
#include <windows.h>

struct promise;
struct coroutine : std::coroutine_handle<promise> {
	using promise_type = ::promise;
	std::coroutine_handle<::promise> inputs_submitted;
};

struct promise {
	coroutine get_return_object() { return { .inputs_submitted = coroutine::from_promise(*this) }; }
	std::suspend_always initial_suspend() noexcept { return {}; }
	std::suspend_always final_suspend() noexcept { return {}; }
	void return_void() {}
	void unhandled_exception() {}
};