#include "Run.h"

namespace tytas {
	RunState Run::state = RunState::STOPPED;
	WCHAR Run::pause_menu_item_label[] = L"Pause";
	std::jthread Run::play_thread;

	const WCHAR PAUSE_LABEL[] = L"Pause";
	const WCHAR RESUME_LABEL[] = L"Resume";

	int Run::_current_frame = 0;
	std::vector<Frame> Run::_data {DEFAULT_FRAME_COUNT};

	inline void set_pause_label() {
		wcscpy_s(Run::pause_menu_item_label, PAUSE_LABEL);
	}

	inline void set_resume_label() {
		wcscpy_s(Run::pause_menu_item_label, RESUME_LABEL);
	}


	void Run::play() {
		state = RunState::PLAYING;
		play_thread = std::jthread([](std::stop_token stoken) {
			_execute_frame_events(stoken);
		});
		set_pause_label();
	}

	void Run::pause() {
		if (is_paused()) {
			resume();
			return;
		}
		state = RunState::PAUSED;
		set_resume_label();
	}

	void Run::resume() {
		//TODO Resume the execution of the TAS
		state = RunState::PLAYING;
		set_pause_label();
	}

	void Run::stop() {
		play_thread.request_stop();
		play_thread.join();
		_current_frame = 0;
		state = RunState::STOPPED;
		set_pause_label();
	}

	bool Run::add_input_to_frame(int frameIndex, const INPUT& input) {
		if (frameIndex >= _data.size()) {
			// Frame index out of bounds
			return false;
		}

		// Input is copied into the frame (input vector)
		_data[frameIndex].push_back(input);
		return true;
	}

	bool Run::remove_input_from_frame(int frameIndex, int inputIndex) {
		if (frameIndex >= _data.size()) {
			// Frame index out of bounds
			return false;
		}
		if (inputIndex >= _data[frameIndex].size()) {
			// Input index out of bounds
			return false;
		}
		Frame* f = &_data[frameIndex];
		f->erase(f->begin() + inputIndex);
		return true;
	}

	void Run::add_new_frame() {
		_data.emplace_back();
	}

	void Run::add_new_frames(int n) {
		_data.resize(_data.size() + n);
	}

	void Run::_execute_frame_events(std::stop_token stoken) {
		using namespace std::chrono_literals;
		while (true) {
			if (stoken.stop_requested()) {
				return;
			}

			if (_current_frame >= _data.size()) {
				Log::WriteLine("Run has finished.");
				stop();
				return;
			}

			LOGFORMATLINE(L"Executing frame {}", _current_frame);

			Frame* frame = &_data[_current_frame];
			if (frame->size() > 0) {
				Log::WriteLine("Sending inputs.");
				SendInput(frame->size(), frame->data(), sizeof(INPUT));
			}

			_current_frame++;
			std::this_thread::sleep_for(16.666666666666667ms);
		}
	}
}