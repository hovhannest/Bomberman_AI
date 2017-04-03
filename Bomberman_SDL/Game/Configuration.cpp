
#include "Configuration.h"
#include "..\tinyxml2\tinyxml2.h"
#include "Constants.h"
#include "..\Core\Log\Log.h"
#include "..\Core\Log\LoggerFactory.h"

using namespace std;
using namespace tinyxml2;
using namespace Bomberman::Constants;

namespace Bomberman {
	Configuration::Configuration() : _loaded(false) {
		defaults();
	}

	Configuration::Configuration(string fileName) : _loaded(false) {
		load(fileName);
	}

	bool Configuration::load(string fileName) {
		XMLDocument file(true, COLLAPSE_WHITESPACE);
		bool result = false;

		if (_loaded) {
			Log::get() << "Overwriting configuration file \"" << _fileName << "\"." << LogLevel::warning;
		}

		if (file.LoadFile(fileName.c_str()) == XML_SUCCESS) {
			XMLElement *root = file.RootElement();

			loadViewport(root->FirstChildElement(CFG_VIEWPORT.c_str()));
			loadLoggers(root->FirstChildElement(CFG_LOGGERS.c_str()));
			loadStartMap(root->FirstChildElement(CFG_START_MAP.c_str()));

			Log::get() << "Using configuration file \"" << fileName << "\"." << LogLevel::info;

			result = true;

			_fileName = fileName;
		}
		else {
			Log::get() << "Configuration file \"" << fileName << "\" not found, using defaults." << LogLevel::warning;

			defaults();
		}

		_loaded = true;

		return result;
	}

	bool Configuration::loaded() const {
		return _loaded;
	}

	string Configuration::viewportTitle() const {
		if (!loaded()) {
			Log::get() << "Using unloaded configuration!" << LogLevel::fatal;
		}

		return _viewportTitle;
	}

	int Configuration::viewportWidth() const {
		if (!loaded()) {
			Log::get() << "Using unloaded configuration!" << LogLevel::fatal;
		}

		return _viewportWidth;
	}

	int Configuration::viewportHeight() const {
		if (!loaded()) {
			Log::get() << "Using unloaded configuration!" << LogLevel::fatal;
		}

		return _viewportHeight;
	}

	string Configuration::startMap() const {
		return _startMap;
	}

	void Configuration::defaults() {
		_fileName = CFG_NULLFILE;
		_startMap.clear();

		loadViewport(nullptr);
		loadLoggers(nullptr);
	}

	void Configuration::loadViewport(void *ptr) {
		XMLElement *node = (XMLElement *)ptr;

		if (node != nullptr) {
			XMLElement *viewportTitle = node->FirstChildElement(CFG_TITLE.c_str());
			if (viewportTitle != nullptr) {
				_viewportTitle = viewportTitle->GetText();
			}
			else {
				Log::get() << "No title for viewport in configuration file." << LogLevel::info;
			}

			XMLElement *viewportWidth = node->FirstChildElement(CFG_WIDTH.c_str());
			if (viewportWidth != nullptr) {
				string value = viewportWidth->GetText();

				try {
					_viewportWidth = stoi(value);
				}
				catch (...) {
					_viewportWidth = 0;
				}
			}
			else {
				Log::get() << "No width for viewport in configuration file, using default." << LogLevel::info;
			}

			XMLElement *viewportHeight = node->FirstChildElement(CFG_HEIGHT.c_str());
			if (viewportHeight != nullptr) {
				string value = viewportHeight->GetText();

				try {
					_viewportHeight = stoi(value);
				}
				catch (...) {
					_viewportHeight = 0;
				}
			}
			else {
				Log::get() << "No height for viewport in configuration file, using default." << LogLevel::info;
			}
		}
		else {
			_viewportWidth = 600;
			_viewportHeight = 460;

			return;
		}

		if (_viewportWidth < 1) {
			Log::get() << "Invalid value for viewport width in configuration file, using default." << LogLevel::warning;

			_viewportWidth = 600;
		}

		if (_viewportHeight < 1) {
			Log::get() << "Invalid value for viewport height in configuration file, using default." << LogLevel::warning;

			_viewportWidth = 460;
		}
	}

	void Configuration::loadLoggers(void *ptr) {
		XMLElement *node = (XMLElement *)ptr;

		if (node == nullptr) return;

		for (XMLElement *loggerName = node->FirstChildElement(CFG_LOGGER.c_str()); loggerName != nullptr; loggerName = loggerName->FirstChildElement(CFG_LOGGER.c_str())) {
			auto logger = LoggerFactory::get().getLogger(loggerName->GetText());
			Log::get().addLogger(logger);
		}
	}

	void Configuration::loadStartMap(void *ptr) {
		XMLElement *node = (XMLElement *)ptr;

		if (node == nullptr) return;

		_startMap = node->GetText();
	}
}
