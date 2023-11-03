#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Input.h"
#include "Log.h"

namespace tytas {
	const int DEFAULT_FRAME_COUNT = 600;

	enum RunState {
		STOPPED, //Stopped completely
		PAUSED, //Running, but paused
		PLAYING, //Actively playing (not paused)
	};

	//Having hundreds of INPUT structs for jump_pressed is a waste of space
	//A frame consists of an array of pointers to pre-constructed input structs
	//RunData is an array of these frames,
	//SendInput sends the entire array (frame) of events at once
	typedef std::vector<INPUT> Frame;

	class Run
	{
	public:

		static RunState state;
		static WCHAR pause_menu_item_label[7];

		static bool is_stopped() { return state == RunState::STOPPED; }
		static bool is_running() { return state & (RunState::PLAYING | RunState::PAUSED); }
		static bool is_paused() { return state == RunState::PAUSED; }
		static bool is_playing() { return state == RunState::PLAYING; }

		static int get_current_frame() { return _current_frame; }
		static int get_frame_length() { return _data.size(); }

		static void play();
		static void pause();
		static void resume();
		static void stop();

		//Returns true if successfully added
		static bool add_input_to_frame(int frameIndex, const INPUT& input);
		//Returns true if successfully removed
		static bool remove_input_from_frame(int frameIndex, int inputIndex);
		static void add_new_frame();
		static void add_new_frames(int count);

	private:
		static int _current_frame;
		static std::vector<Frame> _data;
		static std::jthread play_thread;

		static void _execute_frame_events(std::stop_token stoken);
	};
}