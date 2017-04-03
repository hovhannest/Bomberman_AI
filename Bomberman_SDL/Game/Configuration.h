#pragma once

#include <string>

namespace Bomberman {
	class Configuration {
	public:
		Configuration();
		Configuration(std::string fileName);

		bool load(std::string fileName);
		bool loaded() const;

		std::string viewportTitle() const;

		int viewportWidth() const;
		int viewportHeight() const;

		std::string startMap() const;

	private:
		void defaults();
		void loadViewport(void *);
		void loadLoggers(void *);
		void loadStartMap(void *);

		bool _loaded;
		std::string _fileName;
		std::string _viewportTitle;
		std::string _startMap;
		int _viewportWidth;
		int _viewportHeight;
	};
}