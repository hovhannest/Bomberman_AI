#pragma once
#include "Log/Log.h"
#include "Log/LogLevel.h"

namespace Bomberman {
    class LoopQuiter {
    public:
		LoopQuiter() : _quitLoop(false) {}
		~LoopQuiter() {};
        
		inline bool shouldQuit() const {
			return _quitLoop;
		}
        
		inline void quitLoop() {
			_quitLoop = true;
			Log::get() << "Quitting" << LogLevel::info;
		}
        
    private:
        bool _quitLoop;
    };
}
